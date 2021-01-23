//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "Schema.h"

#include <fstream>

namespace GoMint {

    bool Schema::loadFrom(const std::filesystem::path& schema, const std::filesystem::path& platform) {
        std::ifstream in(platform, std::ios::in | std::ios::binary);
        if (!in) {
            return false;
        }

        nlohmann::json j;
        in >> j;
        in.close();

        if (j.contains("typesizes")) {
            auto& tj = j.at("typesizes");
            if (!tj.is_object()) {
                return false;
            }

            for (auto& el : tj.items()) {
                if (!el.value().is_number_unsigned()) return false;

                const auto& name = el.key();
                auto size = el.value().get<std::uint64_t>();

                m_typeSizeDeclarations.emplace_back(std::make_unique<TypeSizeDeclaration>(name, size));
                m_typeSizeDescriptors.emplace(name, m_typeSizeDeclarations.back().get());
            }
        }

        if (!loadFile(schema)) {
            return false;
        }

        if (j.contains("symbolnames")) {
            auto& sj = j.at("symbolnames");
            if (!sj.is_object()) {
                return false;
            }

            for (auto& el : sj.items()) {
                if (!el.value().is_string()) return false;

                const auto& name = el.key();
                auto finder = m_symbolsByName.find(name);
                if (finder == m_symbolsByName.end()) {
                    continue;
                }

                finder->second->m_symbolName = el.value().get<std::string>();
                m_symbolsByLookup.insert(std::make_pair(finder->second->m_symbolName, finder->second));
            }
        }

        return true;
    }

    bool Schema::loadFile(const std::filesystem::path& path) {
        SchemaFile file(this);
        bool isOutputFile = false;
        if (!file.import(path, isOutputFile)) {
            return false;
        }

        if (isOutputFile) {
            m_files.emplace_back(std::move(file));
        }
        return true;
    }

    Symbol * Schema::findSymbolByName(const std::string& name) {
        auto it = m_symbolsByName.find(name);
        return (it == m_symbolsByName.end()) ? nullptr : it->second;
    }

    Symbol* Schema::findSymbolByLookup(const std::string& lookup) {
        auto it = m_symbolsByLookup.find(lookup);
        return (it == m_symbolsByLookup.end()) ? nullptr : it->second;
    }

    TypeSizeDescriptor * Schema::findTypeSizeByName(const std::string& name) {
        auto it = m_typeSizeDescriptors.find(name);
        return (it == m_typeSizeDescriptors.end()) ? nullptr : it->second;
    }

    bool Schema::validate(bool verbose) {
        bool valid = true;
        for (auto& symbol : m_symbols) {
            if (symbol->m_addressOffset == 0) {
                if (verbose) printf("%s: missing\n", symbol->m_name.c_str());
                valid = false;
            } else {
                if (verbose) printf("%s: %p\n", symbol->m_name.c_str(), reinterpret_cast<void*>(symbol->m_addressOffset));
            }
        }
        return valid;
    }

    bool Schema::generate(const std::filesystem::path& path, const std::string& includePrefix) {
        std::filesystem::path headerPath = path / "include" / includePrefix;
        std::filesystem::path sourcePath = path / "src" / includePrefix;

        for (auto& file : m_files) {
            if (!file.generate(headerPath, sourcePath, includePrefix)) {
                return false;
            }
        }

        return generateHeader(headerPath / "Symbols.h", includePrefix) &&
               generateSource(sourcePath / "Symbols.cpp", includePrefix);
    }

    void Schema::addSymbol(Symbol* symbol) {
        m_symbols.push_back(symbol);
        m_symbolsByName.insert(std::make_pair(symbol->m_name, symbol));
    }

    void Schema::addType(Type* type) {
        m_types.push_back(type);
        m_typeSizeDescriptors.emplace(type->getName(), type);
    }

    void Schema::addInclude(const std::string& include) {
        m_requiredIncludes.insert(include);
    }

    bool Schema::generateHeader(const std::filesystem::path& file, const std::string& includePrefix) {
        std::filesystem::create_directories(file.parent_path());
        FileWriter writer(file);
        if (!writer.isOpen()) {
            return false;
        }

        writer.newline() << "#ifndef __SYMEXTRACT_SYMBOLS__";
        writer.newline() << "#define __SYMEXTRACT_SYMBOLS__";

        for (auto& include : m_requiredIncludes) {
            writer.newline() << "#include " << include;
        }
        writer.newline() << "#include <" << includePrefix << "Helpers.h>";
        writer.newline();
        writer.newline() << "namespace GoMint { namespace SymExtract {";
        writer.increaseIndent();

        for (auto& type : m_types) {
            writer.newline() << "class " << type->m_name << ";";
        }
        writer.newline();
        writer.newline() << "bool loadSymbols();";

        writer.decreaseIndent().newline() << "} }";
        writer.newline() << "#endif";

        return true;
    }

    bool Schema::generateSource(const std::filesystem::path& file, const std::string& includePrefix) {
        std::filesystem::create_directories(file.parent_path());
        FileWriter writer(file);
        if (!writer.isOpen()) {
            return false;
        }

        writer.newline() << "#include <cstdint>";
        writer.newline() << "#include <" << includePrefix << "Symbols.h>";
        for (auto& schemaFile : m_files) {
            writer.newline() << "#include <" << includePrefix << schemaFile.m_filename << ".h>";
        }
        generateNativeIncludes(writer);
        writer.newline();
        writer.newline() << "namespace GoMint { namespace SymExtract {";
        writer.increaseIndent();

        for (auto& symbol : m_symbols) {
            writer.newline() << symbol->m_pointerType << " " << symbol->m_variableName << " = nullptr;";
        }
        writer.newline();
        generateBaseAddressLoader(writer);
        writer.newline();
        writer.newline() << "bool loadSymbols() {";
        writer.increaseIndent();

        writer.newline() << "std::uintptr_t baseAddress = getBaseAddress();";
        writer.newline() << "if (baseAddress == 0) return false;";
        for (auto& symbol : m_symbols) {
            writer.newline() << symbol->m_variableName << " = ";
            symbol->generateVariableCast(writer, "(baseAddress + " + std::to_string(symbol->m_addressOffset) + "ULL)");
            writer.cont() << ";";
        }
        writer.newline() << "return true;";
        writer.decreaseIndent().newline() << "}";

        writer.decreaseIndent().newline() << "} }";

        return true;
    }

}