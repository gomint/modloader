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

        bool generateFunctionPointerLoaders(FileWriter& writer, const std::string& baseaddrVar);

    private:

        Schema* m_schema;

        // File Metadata
        AccessibilityLevel                   m_accessibility;
        std::string                          m_filename;
        std::vector<std::string>             m_includes;

        // Type Definition
        std::unique_ptr<Type>                m_type;

        explicit SchemaFile(Schema* schema);

        bool import(const std::filesystem::path& path, bool& isOutput);

        bool importFileMeta(const nlohmann::json& j);
        bool importType(const nlohmann::json& j);
        bool importField(const nlohmann::json& j);
        bool importFunction(const nlohmann::json& j, bool isMember);

        bool createDirectories(const std::filesystem::path& file);
        bool generateHeader(const std::filesystem::path& file, const std::string& includePrefix);
        bool generateSource(const std::filesystem::path& file, const std::string& includePrefix);

        bool generateFunctionPointerLoader(FileWriter& writer, const std::string& baseaddrVar, Function& function, bool isMember);

        // Header Generation
        std::string generateIncludeGuardName();
        void generateIncludeGuardBegin(FileWriter& writer);
        void generateIncludeGuardEnd(FileWriter& writer);
        void generateIncludeList(FileWriter& writer, const std::string& includePrefix);
        void generateNamespaceBegin(FileWriter& writer);
        void generateNamespaceEnd(FileWriter& writer);
        bool generateTypeDeclaration(FileWriter& writer);
        bool generateTypeLayout(FileWriter& writer);
        bool generateTypeFunctionDeclarations(FileWriter& writer, std::vector<Function>& functions, bool isMember);
        bool generateTypeFunctionPointerDeclaration(FileWriter& writer, Function& function, bool isMember);
        bool generateTypeFunctionDeclaration(FileWriter& writer, Function& function, bool isMember);
        bool getPointerContextInsert(std::string& out, bool isMember);
        void getPointerNames(std::string& outTypename, std::string& outVarname, Function& function);

        void generateIncludeHeader(FileWriter& writer, const std::string& includePrefix);
        bool generateTypeDefinition(FileWriter& writer);
        bool generateTypeFunctionDefinitions(FileWriter& writer, std::vector<Function>& functions, bool isMember);
        bool generateTypeFunctionPointerDefinitions(FileWriter& writer, Function& function, bool isMember);
        bool generateTypeFunctionDefinition(FileWriter& writer, Function& function, bool isMember);
        void generateArgumentList(FileWriter& writer, const std::vector<FunctionArgument>& args, bool types);

    };

}

#endif //MODLOADER_SCHEMAFILE_H
