//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOLMEMBERFUNCTIONDECL_H
#define MODLOADER_SYMBOLMEMBERFUNCTIONDECL_H

#include "SymbolFunctionDecl.h"
#include "SymbolMemberFunctionInheritance.h"

namespace GoMint {

    struct SymbolMemberFunctionDecl : public SymbolFunctionDecl {

    public:

        SymbolMemberFunctionInheritance m_inheritance = SymbolMemberFunctionInheritance::Invalid;
        std::intptr_t m_thisdelta = 0;

    };

    void to_json(nlohmann::json& j, const GoMint::SymbolMemberFunctionDecl& p);
    void from_json(const nlohmann::json& j, GoMint::SymbolMemberFunctionDecl& p);

}

#endif //MODLOADER_SYMBOLMEMBERFUNCTIONDECL_H
