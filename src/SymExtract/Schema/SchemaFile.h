//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SCHEMAFILE_H
#define MODLOADER_SCHEMAFILE_H

#include "../FileWriter.h"
#include "AccessibilityLevel.h"
#include "Symbol.h"
#include "Type.h"

#include <filesystem>
#include <string>

namespace GoMint {

    class Schema;

    class SchemaFile {

        friend class Schema;
    public:

        bool generate(const std::filesystem::path& headerPath,
                      const std::filesystem::path& sourcePath,
                      const std::string& includePrefix);

    private:

        Schema* m_schema;

        AccessibilityLevel m_accessibility;
        std::string m_filename;
        std::vector<std::string> m_includes;
        std::unique_ptr<Type> m_type;
        std::vector<std::unique_ptr<Symbol>> m_symbols;

        explicit SchemaFile(Schema* schema);

        bool import(const std::filesystem::path& path, bool& isOutput);

        bool importSymbols(const nlohmann::json& j);
        bool importSymbol(const nlohmann::json& j);
        bool importType(const nlohmann::json& j);

        bool createDirectories(const std::filesystem::path& file);
        bool generateHeader(const std::filesystem::path& file, const std::string& includePrefix);
        bool generateSource(const std::filesystem::path& file, const std::string& includePrefix);

        // Header Generation
        std::string generateIncludeGuardName();
        void generateIncludeGuardBegin(FileWriter& writer);
        void generateIncludeGuardEnd(FileWriter& writer);
        void generateIncludeList(FileWriter& writer, const std::string& includePrefix);
        void generateNamespaceBegin(FileWriter& writer);
        void generateNamespaceEnd(FileWriter& writer);
        void generateSymbolDeclarations(FileWriter& writer);
        void generateSymbolTypeDeclaration(FileWriter& writer, Symbol* symbol);
        void generateSymbolVariableDeclaration(FileWriter& writer, Symbol* symbol);
        void generateTypeDeclaration(FileWriter& writer);

        void generateIncludeHeader(FileWriter& writer, const std::string& includePrefix);
        void generateTypeDefinition(FileWriter& writer);
        void generateArgumentList(FileWriter& writer, const std::vector<FunctionArgument>& args);

    };

}

#endif //MODLOADER_SCHEMAFILE_H
