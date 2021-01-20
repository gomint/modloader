//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTIONSYMBOL_H
#define MODLOADER_FUNCTIONSYMBOL_H

#include "Symbol.h"
#include "FunctionArgument.h"

namespace GoMint {

    struct FunctionSymbol : public Symbol {

    public:

        FunctionSymbol() : Symbol(SymbolType::Function) {}
        ~FunctionSymbol() override = default;

        std::string m_returnType;
        std::vector<FunctionArgument> m_arguments;

        void generatePointerTypeDeclaration(FileWriter& writer) override;
        void generateVariableCast(FileWriter& writer, const std::string& expression) override;
        void generateFunctionDeclaration(FileWriter& writer, const std::string& name);
        void generateFunctionDefinition(FileWriter& writer, const std::string& clazz, const std::string& name);

    };

    void to_json(nlohmann::json& j, const GoMint::FunctionSymbol& p);
    void from_json(const nlohmann::json& j, GoMint::FunctionSymbol& p);

}

#endif //MODLOADER_FUNCTIONSYMBOL_H
