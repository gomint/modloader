//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "SymbolDecl.h"

#include "SymbolMemberFunctionDecl.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::SymbolDecl& p) {
        j["name"] = p.m_name;
        j["type"] = p.m_type;
        j["ptrname"] = p.m_pointerName;
        j["varname"] = p.m_variableName;
    }

    void from_json(const nlohmann::json& j, GoMint::SymbolDecl& p) {
        j.at("name").get_to(p.m_name);
        j.at("type").get_to(p.m_type);
        j.at("ptrname").get_to(p.m_pointerName);
        j.at("varname").get_to(p.m_variableName);
    }

}