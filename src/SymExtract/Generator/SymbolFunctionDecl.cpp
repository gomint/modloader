//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "SymbolFunctionDecl.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::SymbolFunctionDecl& p) {
        j["ret"] = p.m_returnType;
        j["args"] = p.m_argumentTypes;
        to_json(j, static_cast<const SymbolDecl&>(p));
    }

    void from_json(const nlohmann::json& j, GoMint::SymbolFunctionDecl& p) {
        j.at("ret").get_to(p.m_returnType);
        j.at("args").get_to(p.m_argumentTypes);
        from_json(j, static_cast<SymbolDecl&>(p));
    }

}