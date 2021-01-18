//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_GENERATOR_H
#define MODLOADER_GENERATOR_H

#include "DefinitionTable.h"
#include "SymbolMemberFunctionDecl.h"

#include <fstream>

namespace GoMint {

    /**
     * Generates header and source file with glue code exporting extracted symbols.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    class Generator {

    public:

        Generator();

        bool run(const DefinitionTable& definitions,
                 const std::string& headerFile,
                 const std::string& sourceFile);

    private:

        static constexpr int k_maxIndentLevel = 15;

        void resetIndent();
        void increaseIndent();
        void decreaseIndent();

        bool generateHeader(const DefinitionTable& definitions, const std::string& headerFile);
        void generateHeaderForwardDeclarations(const DefinitionTable& definitions, std::ofstream& out);
        void generateHeaderTypes(const DefinitionTable& definitions, std::ofstream& out);
        void generateHeaderMemberFunction(const DefinitionTable& definitionTable, std::ofstream& out,
                                          const TypeDecl& type, const MemberFunctionDecl& memfunc,
                                          const SymbolMemberFunctionDecl& symbol);
        void generateHeaderSymbols(const DefinitionTable& definitions, std::ofstream& out);

        bool generateSource(const DefinitionTable& definitions, const std::string& headerFile, const std::string& sourceFile);
        void generateSourceTypes(const DefinitionTable& definitions, std::ofstream& out);
        void generateSourceLoadSymbols(const DefinitionTable& definitionTable, std::ofstream& out);

        void generateMemberFunctionInvocable(std::ofstream& out, const SymbolMemberFunctionDecl& symbol) const;
        void generateFunctionPointerDecl(std::ofstream& out, const SymbolFunctionDecl& function, const std::string& varname);
        void generateFunctionArgumentList(std::ofstream& out, const SymbolFunctionDecl& function, bool named = false);


        char m_indent[k_maxIndentLevel + 1];

        int m_indentLevel;
    };

}

#endif //MODLOADER_GENERATOR_H
