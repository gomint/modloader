//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_GENERATOR_H
#define MODLOADER_GENERATOR_H

#include "../Schema/Schema.h"
#include "../Schema/MemberFunctionSymbol.h"

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

        bool run(const Schema& definitions,
                 const std::string& headerFile,
                 const std::string& sourceFile);

    private:

        static constexpr int k_maxIndentLevel = 15;

        void resetIndent();
        void increaseIndent();
        void decreaseIndent();

        bool generateHeader(const Schema& definitions, const std::string& headerFile);
        void generateHeaderIncludes(const Schema& definitionTable, std::ofstream& out);
        void generateHeaderForwardDeclarations(const Schema& definitions, std::ofstream& out);
        void generateHeaderTypes(const Schema& definitions, std::ofstream& out);
        void generateHeaderMemberFunction(const Schema& definitionTable, std::ofstream& out,
                                          const Type& type, const MemberFunction& memfunc,
                                          const MemberFunctionSymbol& symbol);
        void generateHeaderSymbols(const Schema& definitions, std::ofstream& out);

        bool generateSource(const Schema& definitions, const std::string& headerFile, const std::string& sourceFile);
        void generateSourceTypes(const Schema& definitions, std::ofstream& out);
        void generateSourceLoadSymbols(const Schema& definitionTable, std::ofstream& out);

        void generateMemberFunctionInvocable(std::ofstream& out, const MemberFunctionSymbol& symbol) const;
        void generateFunctionPointerDecl(std::ofstream& out, const FunctionSymbol& function, const std::string& varname);
        void generateFunctionArgumentList(std::ofstream& out, const FunctionSymbol& function, bool named = false);


        char m_indent[k_maxIndentLevel + 1];

        int m_indentLevel;
    };

}

#endif //MODLOADER_GENERATOR_H
