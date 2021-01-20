//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "Symbol.h"

#include "MemberFunctionSymbol.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::Symbol& p) {
        j["name"] = p.m_name;
        j["type"] = p.m_type;
        j["ptrtype"] = p.m_pointerType;
        j["varname"] = p.m_variableName;
    }

    void from_json(const nlohmann::json& j, GoMint::Symbol& p) {
        j.at("name").get_to(p.m_name);
        j.at("type").get_to(p.m_type);
        j.at("ptrtype").get_to(p.m_pointerType);
        j.at("varname").get_to(p.m_variableName);
    }

}