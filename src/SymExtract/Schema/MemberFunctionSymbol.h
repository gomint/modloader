//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MEMBERFUNCTIONSYMBOL_H
#define MODLOADER_MEMBERFUNCTIONSYMBOL_H

#include "FunctionSymbol.h"
#include "TypeInheritance.h"

namespace GoMint {

    struct MemberFunctionSymbol : public Symbol {

    public:

        MemberFunctionSymbol() : Symbol(SymbolType::MemberFunction) {}
        ~MemberFunctionSymbol() override = default;

        std::string m_returnType;
        std::vector<FunctionArgument> m_arguments;
        TypeInheritance m_inheritance = TypeInheritance::Invalid;
        std::intptr_t m_thisdelta = 0;

        void generatePointerTypeDeclaration(FileWriter& writer) override;
        void generateVariableCast(FileWriter& writer, const std::string& expression) override;
        void generateMemberFunctionDeclaration(FileWriter& writer, const std::string& name);
        void generateMemberFunctionDefinition(FileWriter& writer, const std::string& clazz, const std::string& name);

    };

    void to_json(nlohmann::json& j, const GoMint::MemberFunctionSymbol& p);
    void from_json(const nlohmann::json& j, GoMint::MemberFunctionSymbol& p);

}

#endif //MODLOADER_MEMBERFUNCTIONSYMBOL_H
