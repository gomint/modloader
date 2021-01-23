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

        // Load symbols and their definitions
        if (j.contains("symbols")) {
            auto& sj = j.at("symbols");
            if (!sj.is_object()) {
                return false;
            }

            for (auto& el : sj.items()) {
                if (!el.value().is_string()) return false;

                if (m_symbolsByName.find(el.key()) != m_symbolsByName.end()) {
                    continue;
                }

                auto& ref = m_symbols.emplace_back(std::make_unique<Symbol>(el.key(), el.value().get<std::string>()));
                m_symbolsByName.emplace(ref->getName(), ref.get());
                m_symbolsByLookup.emplace(ref->getLookup(), ref.get());
            }
        }

        // Load platform specific sizes of standard types
        if (j.contains("typesizes")) {
            auto& tj = j.at("typesizes");
            if (!tj.is_object()) {
                return false;
            }

            for (auto& el : tj.items()) {
                if (!el.value().is_number_unsigned()) return false;

                const auto& name = el.key();
                auto size = el.value().get<std::uint64_t>();

                auto& ref = m_externTypes.emplace_back(std::make_unique<ExternType>(name, size));
                m_typeDescriptors.emplace(name, ref.get());
            }
        }

        // Load actual schema
        if (!loadFile(schema)) {
            return false;
        }

        return true;
    }

    bool Schema::loadFile(const std::filesystem::path& path) {
        SchemaFile file(this);
        bool       isOutputFile = false;
        if (!file.import(path, isOutputFile)) {
            return false;
        }

        if (isOutputFile) {
            m_files.emplace_back(std::move(file));
        }
        return true;
    }

    Symbol* Schema::findSymbolByName(const std::string& name) {
        auto it = m_symbolsByName.find(name);
        return (it == m_symbolsByName.end()) ? nullptr : it->second;
    }

    Symbol* Schema::findSymbolByLookup(const std::string& lookup) {
        auto it = m_symbolsByLookup.find(lookup);
        return (it == m_symbolsByLookup.end()) ? nullptr : it->second;
    }

    TypeDescriptor* Schema::findTypeDescriptorByName(const std::string& name) {
        auto it = m_typeDescriptors.find(name);
        return (it == m_typeDescriptors.end()) ? nullptr : it->second;
    }

    std::optional<std::reference_wrapper<const Constant>> Schema::findConstantByName(const std::string& name) {
        auto it = m_constants.find(name);
        return (it == m_constants.end()) ?
            std::optional<std::reference_wrapper<const Constant>>() :
            std::make_optional(std::reference_wrapper<const Constant>(it->second));
    }

    bool Schema::validate(bool verbose) {
        bool valid = true;
        for (auto& symbol : m_symbols) {
            if (!symbol->hasAddressOffset()) {
                if (verbose) printf("%s: missing\n", symbol->getName().c_str());
                valid = false;
            } else {
                if (verbose)
                    printf("%s: %p\n", symbol->getName().c_str(), reinterpret_cast<void*>(symbol->getAddressOffset()));
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

    void Schema::addType(Type* type) {
        m_types.push_back(type);
        m_typeDescriptors.emplace(type->getName(), type);
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
            writer.newline() << "class " << type->getName() << ";";
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

        writer.newline();
        generateBaseAddressLoader(writer);
        writer.newline();
        writer.newline() << "bool loadSymbols() {";
        writer.increaseIndent();

        writer.newline() << "std::uintptr_t baseAddress = getBaseAddress();";
        writer.newline() << "if (baseAddress == 0) return false;";
        for (auto& schemaFile : m_files) {
            if (!schemaFile.generateFunctionPointerLoaders(writer, "baseAddress")) {
                return false;
            }
        }
        writer.newline() << "return true;";
        writer.decreaseIndent().newline() << "}";

        writer.decreaseIndent().newline() << "} }";

        return true;
    }

}