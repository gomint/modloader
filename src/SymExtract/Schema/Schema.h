//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SCHEMA_H
#define MODLOADER_SCHEMA_H

#include "Constant.h"
#include "ExternType.h"
#include "ForwardDeclaration.h"
#include "SchemaFile.h"
#include "Symbol.h"
#include "Type.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_set>

namespace GoMint {

    /**
     * Table containing all definitions loaded from a symbol table file.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    class Schema {

        friend class SchemaFile;

    public:

        bool loadFrom(const std::filesystem::path& schema, const std::filesystem::path& platform);

        Symbol* findSymbolByName(const std::string& name);
        Symbol* findSymbolByLookup(const std::string& lookup);
        TypeDescriptor* findTypeDescriptorByName(const std::string& name);
        std::optional<std::reference_wrapper<const Constant>> findConstantByName(const std::string& name);

        bool validate(bool verbose = true);

        bool generate(const std::filesystem::path& path, const std::string& includePrefix);

    private:

        std::vector<SchemaFile> m_files;

        // Platform specifics
        std::vector<std::unique_ptr<Symbol>>      m_symbols;
        std::unordered_map<std::string, Symbol*>  m_symbolsByName;
        std::unordered_map<std::string, Symbol*>  m_symbolsByLookup;
        std::vector<std::unique_ptr<ExternType>>  m_externTypes;
        std::unordered_map<std::string, Constant> m_constants;

        // Schema Generation
        std::vector<Type*>                               m_types;
        std::unordered_map<std::string, TypeDescriptor*> m_typeDescriptors;
        std::unordered_set<std::string>                  m_requiredIncludes;


        bool loadFile(const std::filesystem::path& path);
        void addType(Type* type);
        void addInclude(const std::string& include);

        bool generateHeader(const std::filesystem::path& file, const std::string& includePrefix);
        bool generateSource(const std::filesystem::path& file, const std::string& includePrefix);

        void generateNativeIncludes(FileWriter& writer);
        void generateBaseAddressLoader(FileWriter& writer);

    };

    void to_json(nlohmann::json& j, const GoMint::Schema& p);
    void from_json(const nlohmann::json& j, GoMint::Schema& p);

}

#endif //MODLOADER_SCHEMA_H
