//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "TypeDecl.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::TypeDecl& p) {
        j["name"] = p.m_name;
        j["type"] = p.m_type;
        j["memfuncs"] = p.m_memberFunctions;
    }

    void from_json(const nlohmann::json& j, GoMint::TypeDecl& p) {
        j.at("name").get_to(p.m_name);
        j.at("type").get_to(p.m_type);
        j.at("memfuncs").get_to(p.m_memberFunctions);
    }

}
