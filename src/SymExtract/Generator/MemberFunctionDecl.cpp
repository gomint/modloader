//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "MemberFunctionDecl.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::MemberFunctionDecl& p) {
        j["name"] = p.m_name;
        j["symbol"] = p.m_symbol;
    }

    void from_json(const nlohmann::json& j, GoMint::MemberFunctionDecl& p) {
        j.at("name").get_to(p.m_name);
        j.at("symbol").get_to(p.m_symbol);
    }

}