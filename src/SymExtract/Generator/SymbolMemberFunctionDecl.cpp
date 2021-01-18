//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "SymbolMemberFunctionDecl.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::SymbolMemberFunctionDecl& p) {
        j["inheritance"] = p.m_inheritance;
        if (p.m_inheritance == SymbolMemberFunctionInheritance::Multi) {
            j["thisdelta"] = p.m_thisdelta;
        }
        to_json(j, static_cast<const SymbolFunctionDecl&>(p));
    }

    void from_json(const nlohmann::json& j, GoMint::SymbolMemberFunctionDecl& p) {
        j.at("inheritance").get_to(p.m_inheritance);
        if (p.m_inheritance == SymbolMemberFunctionInheritance::Multi) {
            j.at("thisdelta").get_to(p.m_thisdelta);
        }
        from_json(j, static_cast<SymbolFunctionDecl&>(p));
    }

}