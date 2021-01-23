//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "SchemaFile.h"
#include "Schema.h"

#include "FixedFunctionResolver.h"
#include "VirtualFunctionResolver.h"

#include "../FileWriter.h"

#include <algorithm>
#include <cstring>
#include <fstream>

namespace GoMint {

    SchemaFile::SchemaFile(Schema* schema)
            : m_schema{schema},
              m_accessibility{AccessibilityLevel::Public} {

    }


    bool SchemaFile::import(const std::filesystem::path& file, bool& isOutput) {
        isOutput = false;

        std::ifstream in(file, std::ios::in | std::ios::binary);
        if (!in) {
            return false;
        }

        nlohmann::json j;
        in >> j;
        in.close();

        if (j.contains("import")) {
            std::vector<std::string> imports;
            j.at("import").get_to(imports);

            for (const auto& import : imports) {
                if (!m_schema->loadFile(file.parent_path() / import)) {
                    return false;
                }
            }

            // Do not allow includes from file which generate output
            return true;
        }


        // This schema file does produce output
        if (!importFileMeta(j)) {
            return false;
        }

        if (j.contains("type")) {
            isOutput = true;
            if (!importType(j.at("type"))) {
                return false;
            }
        }

        return true;
    }

    bool SchemaFile::importFileMeta(const nlohmann::json& j) {
        //
        // Obligatory
        //
        j.at("filename").get_to(m_filename);

        //
        // Optional
        //
        if (j.contains("accessibility")) {
            j.at("accessibility").get_to(m_accessibility);
        } else {
            m_accessibility = AccessibilityLevel::Public;
        }

        if (j.contains("includes")) {
            j.at("includes").get_to(m_includes);
            for (auto& include : m_includes) {
                if (!include.empty() && include[0] == '<') {
                    m_schema->addInclude(include);
                }
            }
        } else {
            m_includes.clear();
        }
        return true;
    }

    bool SchemaFile::importType(const nlohmann::json& j) {
        //
        // Obligatory
        //
        std::string name;

        j.at("name").get_to(name);

        m_type = std::make_unique<Type>(name);


        //
        // Optional
        //
        if (j.contains("size")) j.at("size").get_to(m_type->m_size);
        if (j.contains("alignment")) j.at("alignment").get_to(m_type->m_alignment);
        if (j.contains("inheritance")) j.at("inheritance").get_to(m_type->m_inheritance);

        m_type->m_fields.clear();
        if (j.contains("layout")) {
            auto& layout = j.at("layout");
            if (layout.is_array()) {
                for (const auto & it : layout) {
                    if (!importField(it)) {
                        return false;
                    }
                }
            }
        }

        m_type->m_functions.clear();
        if (j.contains("funcs")) {
            auto& funcs = j.at("funcs");
            if (funcs.is_array()) {
                for (const auto& it : funcs) {
                    if (!importFunction(it, false)) {
                        return false;
                    }
                }
            }
        }

        m_type->m_memberFunctions.clear();
        if (j.contains("memfuncs")) {
            auto& funcs = j.at("memfuncs");
            if (funcs.is_array()) {
                for (const auto& it : funcs) {
                    if (!importFunction(it, true)) {
                        return false;
                    }
                }
            }
        }

        m_schema->addType(m_type.get());
        return true;
    }

    bool SchemaFile::importField(const nlohmann::json& j) {
        if (!j.is_object()) return false;

        //
        // Obligatory
        //
        std::string typeName;
        std::string name;

        j.at("type").get_to(typeName);
        j.at("name").get_to(name);

        TypeDescriptor* type = m_schema->findTypeDescriptorByName(typeName);
        if (type == nullptr) return false;

        //
        // Optional
        //
        std::uint64_t offset = Field::UNKNOWN_OFFSET;

        if (j.contains("offset")) j.at("offset").get_to(offset);

        m_type->m_fields.emplace_back(type, name, offset);
        return true;
    }

    bool SchemaFile::importFunction(const nlohmann::json& j, bool isMember) {
        FunctionSignature signature;
        FunctionResolution resolution;
        FunctionResolverPtr resolver;

        //
        // Obligatory
        //
        j.at("name").get_to(signature.m_name);
        j.at("ret").get_to(signature.m_returnType);
        j.at("resolution").get_to(resolution);

        if (resolution == FunctionResolution::Invalid ||
            (resolution == FunctionResolution::Virtual && !isMember)) {
            return false;
        }

        switch (resolution) {
            case FunctionResolution::Fixed: {
                FixedFunctionResolver::AddressOffsetReference address;

                auto& addressJ = j.at("address");
                if (addressJ.is_string()) {
                    Symbol* symbol = m_schema->findSymbolByName(addressJ.get<std::string>());
                    if (symbol == nullptr) return false;
                    address = symbol;
                } else if (addressJ.is_number_unsigned()) {
                    address = addressJ.get<std::uint64_t>();
                } else {
                    return false;
                }

                resolver = FunctionResolverPtr(new FixedFunctionResolver(address));
            } break;

            case FunctionResolution::Virtual: {
                std::uint64_t vindex;
                j.at("vindex").get_to(vindex);
                resolver = FunctionResolverPtr(new VirtualFunctionResolver(vindex));
            } break;

            default:
                return false;
        }

        //
        // Optional
        //
        signature.m_arguments.clear();
        if (j.contains("args")) {
            auto& args = j.at("args");
            if (!args.is_array()) return false;
            for (const auto& arg : args) {
                std::string type;
                std::string name;

                arg.at("type").get_to(type);
                arg.at("name").get_to(name);

                signature.m_arguments.emplace_back(type, name);
            }
        }

        if (isMember) {
            m_type->m_memberFunctions.emplace_back(signature, std::move(resolver));
        } else {
            m_type->m_functions.emplace_back(signature, std::move(resolver));
        }
        return true;
    }

    bool SchemaFile::generate(const std::filesystem::path& headerPath,
                              const std::filesystem::path& sourcePath,
                              const std::string& includePrefix) {
        std::filesystem::path headerFile = headerPath / (m_filename + ".h");
        std::filesystem::path sourceFile = sourcePath / (m_filename + ".cpp");

        if (!createDirectories(headerFile) || !createDirectories(sourceFile)) {
            return false;
        }

        return generateHeader(headerFile, includePrefix) && generateSource(sourceFile, includePrefix);
    }

    bool SchemaFile::generateFunctionPointerLoaders(FileWriter& writer, const std::string& baseaddrVar) {
        if (m_type == nullptr) return true;

        for (auto& func : m_type->m_functions) {
            if (!generateFunctionPointerLoader(writer, baseaddrVar, func, false)) return false;
        }

        for (auto& func : m_type->m_memberFunctions) {
            if (!generateFunctionPointerLoader(writer, baseaddrVar,  func, true)) return false;
        }
        return true;
    }

    bool SchemaFile::generateFunctionPointerLoader(FileWriter& writer, const std::string& baseaddrVar, Function& function, bool isMember) {
        auto* resolver = function.getResolver();
        if (!resolver->hasFunctionPointer()) return true;

        std::string pointerTypename;
        std::string pointerVarname;
        getPointerNames(pointerTypename, pointerVarname, function);

        return resolver->generateFunctionPointerLoader(writer, baseaddrVar, m_type.get(), &function, pointerTypename, pointerVarname, isMember);
    }

    bool SchemaFile::createDirectories(const std::filesystem::path& file) {
        std::error_code err;
        std::filesystem::create_directories(file.parent_path(), err);
        return !err;
    }

    bool SchemaFile::generateHeader(const std::filesystem::path& file, const std::string& includePrefix) {
        FileWriter writer(file);
        if (!writer.isOpen()) {
            return false;
        }

        generateIncludeGuardBegin(writer);
        writer.newline();
        generateIncludeList(writer, includePrefix);
        writer.newline();
        generateNamespaceBegin(writer);
        writer.newline();
        if (!generateTypeDeclaration(writer)) {
            return false;
        }
        writer.newline();
        generateNamespaceEnd(writer);
        writer.newline();
        generateIncludeGuardEnd(writer);

        return true;
    }

    bool SchemaFile::generateSource(const std::filesystem::path& file, const std::string& includePrefix) {
        FileWriter writer(file);
        if (!writer.isOpen()) {
            return false;
        }

        generateIncludeHeader(writer, includePrefix);
        writer.newline();
        generateNamespaceBegin(writer);
        writer.newline();
        if (!generateTypeDefinition(writer)) {
            return false;
        }
        writer.newline();
        generateNamespaceEnd(writer);
        return true;
    }

    std::string SchemaFile::generateIncludeGuardName() {
        std::string name = m_filename;
        std::replace(name.begin(), name.end(), '/', '_');
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });
        return name;
    }

    void SchemaFile::generateIncludeGuardBegin(FileWriter& writer) {
        std::string guard = generateIncludeGuardName();
        writer.newline() << "#ifndef __SYMEXTRACT_" << guard << "__";
        writer.newline() << "#define __SYMEXTRACT_" << guard << "__";
    }

    void SchemaFile::generateIncludeGuardEnd(FileWriter& writer) {
        writer.newline() << "#endif";
    }

    void SchemaFile::generateIncludeList(FileWriter& writer, const std::string& includePrefix) {
        writer.newline() << "#include <" << includePrefix << "Symbols.h>";
        for (auto& include : m_includes) {
            if (!include.empty() && include[0] == '@') {
                writer.newline() << "#include <" << includePrefix << include.substr(1) << ".h>";
            }
        }
    }

    void SchemaFile::generateNamespaceBegin(FileWriter& writer) {
        writer.newline() << "namespace GoMint { namespace SymExtract {";
        writer.increaseIndent();
    }

    void SchemaFile::generateNamespaceEnd(FileWriter& writer) {
        writer.decreaseIndent().newline() << "} }";
    }

    bool SchemaFile::generateTypeDeclaration(FileWriter& writer) {
        if (m_type == nullptr) return true;

        writer.newline() << "class " << m_type->m_name << " {";
        writer.newline();
        writer.newline() << "public:";
        writer.increaseIndent();

        if (!m_type->m_fields.empty()) {
            if (!generateTypeLayout(writer)) {
                return false;
            }
        }

        if (!m_type->m_functions.empty()) {
            if (!generateTypeFunctionDeclarations(writer, m_type->m_functions, false)) {
                return false;
            }
        }

        if (!m_type->m_memberFunctions.empty()) {
            if (!generateTypeFunctionDeclarations(writer, m_type->m_memberFunctions, true)) {
                return false;
            }
        }

        writer.decreaseIndent().newline() << "};";
        return true;
    }

    bool SchemaFile::generateTypeLayout(FileWriter& writer) {
        if (!m_type->hasSize()) {
            return false;
        }

        std::uint64_t currentOffset = 0;
        std::size_t   paddingCount  = 0;

        std::sort(m_type->m_fields.begin(), m_type->m_fields.end(), [&](const Field& a, const Field& b) -> bool {
            return a.getOffset() < b.getOffset();
        });

        for (auto& field : m_type->m_fields) {
            if (field.hasOffset() && field.getOffset() > currentOffset) {
                // Generate padding
                std::uint64_t diff = field.getOffset() - currentOffset;
                writer.newline() << "std::uint8_t m_unknown" << (paddingCount++) << "[" << diff << "];";
                currentOffset += diff;
            }

            writer.newline() << field.getType()->getName() << " " << field.getName() << ";";
            currentOffset += field.getType()->getSize();
        }

        if (m_type->m_size > currentOffset) {
            writer.newline() << "std::uint8_t m_unknown" << (paddingCount++) << "["
                             << (m_type->m_size - currentOffset) << "];";
        }

        writer.newline();
        return true;
    }

    bool SchemaFile::generateTypeFunctionDeclarations(FileWriter& writer, std::vector<Function>& functions, bool isMember) {
        for (auto& function : functions) {
            if (!generateTypeFunctionPointerDeclaration(writer, function, isMember)) return false;
            if (!generateTypeFunctionDeclaration(writer, function, isMember)) return false;
            writer.newline();
        }
        return true;
    }

    bool SchemaFile::generateTypeFunctionPointerDeclaration(FileWriter& writer, Function& function, bool isMember) {
        auto& sig = function.getSignature();

        std::string pointerContextInsert;
        std::string pointerTypename;
        std::string pointerVarname;

        if (!getPointerContextInsert(pointerContextInsert, isMember)) return false;
        getPointerNames(pointerTypename, pointerVarname, function);

        // Pointer Type Declaration
        writer.newline() << "using " << pointerTypename << " = " << sig.m_returnType << "(" << pointerContextInsert << "*)(";
        generateArgumentList(writer, sig.m_arguments, true);
        writer.cont() << ");";

        // Pointer Declaration
        if (function.getResolver()->hasFunctionPointer()) {
            writer.newline() << "static " << pointerTypename << " " << pointerVarname << ";";
        }
        return true;
    }

    bool SchemaFile::generateTypeFunctionDeclaration(FileWriter& writer, Function& function, bool isMember) {
        writer.newline();
        if (!isMember) {
            writer.cont() << "static ";
        }

        auto& sig = function.getSignature();

        writer.cont() << sig.m_returnType << " " << sig.m_name << "(";
        generateArgumentList(writer, sig.m_arguments, true);
        writer.cont() << ");";
        return true;
    }

    bool SchemaFile::getPointerContextInsert(std::string& out, bool isMember) {
        if (isMember) {
            switch (m_type->m_inheritance) {
                case TypeInheritance::Single:
                    out = "SingleInvocable::";
                    break;

                case TypeInheritance::Multi:
                    out = "MultiInvocable::";
                    break;

                default:
                    return false;
            }
        } else {
            out = "";
        }
        return true;
    }

    void SchemaFile::getPointerNames(std::string& outTypename, std::string& outVarname, Function& function) {
        const auto& name = function.getSignature().m_name;
        outTypename = name + "_funcptr";
        outVarname = "k_" + name;
    }


    void SchemaFile::generateIncludeHeader(FileWriter& writer, const std::string& includePrefix) {
        writer.newline() << "#include <" << includePrefix << m_filename << ".h>";
    }

    bool SchemaFile::generateTypeDefinition(FileWriter& writer) {
        if (m_type == nullptr) return true;

        if (!generateTypeFunctionDefinitions(writer, m_type->m_functions, false)) return false;
        writer.newline();
        if (!generateTypeFunctionDefinitions(writer, m_type->m_memberFunctions, true)) return false;

        return true;
    }

    bool SchemaFile::generateTypeFunctionDefinitions(FileWriter& writer, std::vector<Function>& functions, bool isMember) {
        for (auto& func : functions) {
            if (func.getResolver()->hasFunctionPointer()) {
                if (!generateTypeFunctionPointerDefinitions(writer, func, isMember)) return false;
            }
            if (!generateTypeFunctionDefinition(writer, func, isMember)) return false;
        }
        return true;
    }

    bool SchemaFile::generateTypeFunctionPointerDefinitions(FileWriter& writer, Function& function, bool isMember) {
        std::string pointerTypename;
        std::string pointerVarname;
        getPointerNames(pointerTypename, pointerVarname, function);

        writer.newline() << m_type->getName() << "::" << pointerTypename << " " << m_type->m_name << "::" << pointerVarname << " = nullptr;";
        return true;
    }

    bool SchemaFile::generateTypeFunctionDefinition(FileWriter& writer, Function& function, bool isMember) {
        auto& sig = function.getSignature();

        std::string pointerTypename;
        std::string pointerVarname;
        getPointerNames(pointerTypename, pointerVarname, function);

        writer.newline() << sig.m_returnType << " " << m_type->m_name << "::" << sig.m_name << "(";
        generateArgumentList(writer, sig.m_arguments, true);
        writer.cont() << ") {";
        writer.increaseIndent();

        if (!function.getResolver()->generateFunctionInvocation(writer, m_type.get(), &function, pointerTypename, pointerVarname, isMember)) {
            return false;
        }

        writer.decreaseIndent().newline() << "}";
        return true;
    }

    void SchemaFile::generateArgumentList(FileWriter& writer, const std::vector<FunctionArgument>& args, bool types) {
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (i != 0) {
                writer.cont() << ", ";
            }
            if (types) {
                writer.cont() << args[i].m_type << " ";
            }
            writer.cont() << args[i].m_name;
        }
    }

}