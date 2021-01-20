//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "ForwardDeclaration.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::ForwardDeclaration& p) {
        j["name"] = p.m_name;
        j["type"] = p.m_type;
    }

    void from_json(const nlohmann::json& j, GoMint::ForwardDeclaration& p) {
        j.at("name").get_to(p.m_name);
        j.at("type").get_to(p.m_type);
    }

}