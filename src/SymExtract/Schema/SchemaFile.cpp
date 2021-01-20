//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "SchemaFile.h"
#include "Schema.h"

#include "FunctionSymbol.h"
#include "MemberFunctionSymbol.h"

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

        if (j.contains("accessibility")) {
            j.at("accessibility").get_to(m_accessibility);
        } else {
            m_accessibility = AccessibilityLevel::Public;
        }

        j.at("filename").get_to(m_filename);

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

        if (j.contains("symbols")) {
            isOutput = true;
            if (!importSymbols(j.at("symbols"))) {
                return false;
            }
        }

        if (j.contains("type")) {
            isOutput = true;
            if (!importType(j.at("type"))) {
                return false;
            }
        }

        return true;
    }

    bool SchemaFile::importType(const nlohmann::json& j) {
        m_type = std::make_unique<Type>();
        from_json(j, *m_type);

        if (!m_type->resolveSymbols(m_schema)) {
            printf("Unresolved symbols in type '%s'\n", m_type->m_name.c_str());
            return false;
        }

        m_schema->addType(m_type.get());
        return true;
    }

    bool SchemaFile::importSymbols(const nlohmann::json& j) {
        if (!j.is_array()) return false;

        for (auto& symbol : j) {
            if (!importSymbol(symbol)) {
                return false;
            }
        }

        return true;
    }

    bool SchemaFile::importSymbol(const nlohmann::json& j) {
        if (!j.is_object() || !j.contains("type")) return false;

        SymbolType type;
        j.at("type").get_to(type);

        std::unique_ptr<Symbol>* symbol = nullptr;
        switch (type) {
            case SymbolType::Function: {
                auto* func = new FunctionSymbol();
                from_json(j, *func);
                symbol = &(m_symbols.emplace_back(std::unique_ptr<Symbol>(func)));
            }
                break;

            case SymbolType::MemberFunction: {
                auto* memfunc = new MemberFunctionSymbol();
                from_json(j, *memfunc);
                symbol = &(m_symbols.emplace_back(std::unique_ptr<Symbol>(memfunc)));
            }
                break;

            default:
                return false;
        }

        if (symbol != nullptr) {
            m_schema->addSymbol(symbol->get());
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
        generateSymbolDeclarations(writer);
        writer.newline();
        generateTypeDeclaration(writer);
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
        generateTypeDefinition(writer);
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
                writer.newline() << "#include <" << includePrefix << include << ">";
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

    void SchemaFile::generateSymbolDeclarations(FileWriter& writer) {
        writer.newline() << "//";
        writer.newline() << "// Symbol Types";
        writer.newline() << "//";
        for (auto& symbol : m_symbols) {
            generateSymbolTypeDeclaration(writer, symbol.get());
        }

        writer.newline();

        writer.newline() << "//";
        writer.newline() << "// Symbol List";
        writer.newline() << "//";
        for (auto& symbol : m_symbols) {
            generateSymbolVariableDeclaration(writer, symbol.get());
        }
    }

    void SchemaFile::generateSymbolTypeDeclaration(FileWriter& writer, Symbol* symbol) {
        writer.newline() << "using " << symbol->m_pointerType << " = ";
        symbol->generatePointerTypeDeclaration(writer);
        writer.cont() << ";";
    }

    void SchemaFile::generateSymbolVariableDeclaration(FileWriter& writer, Symbol* symbol) {
        writer.newline() << "extern " << symbol->m_pointerType << " " << symbol->m_variableName << ";";
    }

    void SchemaFile::generateTypeDeclaration(FileWriter& writer) {
        if (m_type == nullptr) return;

        writer.newline() << "class " << m_type->m_name << " {";
        writer.newline();
        writer.newline() << "public:";
        writer.increaseIndent();

        for (auto& func : m_type->m_functions) {
            writer.newline();
            func.m_symbol->generateFunctionDeclaration(writer, func.m_name);
            writer.cont() << ";";
        }

        writer.newline();

        for (auto& memfunc : m_type->m_memberFunctions) {
            writer.newline();
            memfunc.m_symbol->generateMemberFunctionDeclaration(writer, memfunc.m_name);
            writer.cont() << ";";
        }

        writer.decreaseIndent().newline() << "}";
    }


    void SchemaFile::generateIncludeHeader(FileWriter& writer, const std::string& includePrefix) {
        writer.newline() << "#include <" << includePrefix << m_filename << ".h>";
    }

    void SchemaFile::generateTypeDefinition(FileWriter& writer) {
        if (m_type == nullptr) return;

        for (auto& func : m_type->m_functions) {
            writer.newline();
            func.m_symbol->generateFunctionDefinition(writer, m_type->m_name, func.m_name);
            writer.cont() << " {";
            writer.increaseIndent();

            if (func.m_symbol->m_returnType != "void") {
                writer.newline() << "return ";
            } else {
                writer.newline();
            }
            writer.newline() << func.m_symbol->m_variableName << "(";
            generateArgumentList(writer, func.m_symbol->m_arguments);
            writer.cont() << ");";

            writer.decreaseIndent().newline() << "}";
            writer.newline();
        }

        writer.newline();

        for (auto& func : m_type->m_memberFunctions) {
            writer.newline();
            func.m_symbol->generateMemberFunctionDefinition(writer, m_type->m_name, func.m_name);
            writer.cont() << " {";
            writer.increaseIndent();

            writer.newline() << "auto* self = reinterpret_cast<";
            switch(func.m_symbol->m_inheritance) {
                case TypeInheritance::Single:
                    writer.cont() << "SingleInvocable";
                    break;
                case TypeInheritance::Multi:
                    writer.cont() << "MultiInvocable";
                    break;
                default:
                    writer.cont() << "void";
                    break;
            }
            writer.cont() << "*>(this);";

            if (func.m_symbol->m_returnType != "void") {
                writer.newline() << "return ";
            } else {
                writer.newline();
            }

            writer.cont() << "(self->*" << func.m_symbol->m_variableName << ")(";
            generateArgumentList(writer, func.m_symbol->m_arguments);
            writer.cont() << ");";

            writer.decreaseIndent().newline() << "};";
            writer.newline();
        }
    }

    void SchemaFile::generateArgumentList(FileWriter& writer, const std::vector<FunctionArgument>& args) {
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (i != 0) {
                writer.cont() << ", ";
            }
            writer.cont() << args[i].m_name;
        }
    }

}