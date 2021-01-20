//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "FunctionArgument.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const FunctionArgument& p) {
        j["type"] = p.m_type;
        j["name"] = p.m_name;
    }

    void from_json(const nlohmann::json& j, FunctionArgument& p) {
        j.at("type").get_to(p.m_type);
        j.at("name").get_to(p.m_name);
    }

}