//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "Type.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::Type& p) {
        j["name"] = p.m_name;
        j["funcs"] = p.m_functions;
        j["memfuncs"] = p.m_memberFunctions;
    }

    void from_json(const nlohmann::json& j, GoMint::Type& p) {
        j.at("name").get_to(p.m_name);

        if (j.contains("funcs")) {
            j.at("funcs").get_to(p.m_functions);
        } else {
            p.m_functions.clear();
        }

        if (j.contains("memfuncs")) {
            j.at("memfuncs").get_to(p.m_memberFunctions);
        } else {
            p.m_memberFunctions.clear();
        }
    }

    bool Type::resolveSymbols(Schema* schema) {
        for (auto& func : m_functions) {
            if (!func.resolveSymbols(schema)) {
                return false;
            }
        }

        for (auto& memfunc : m_memberFunctions) {
            if (!memfunc.resolveSymbol(schema)) {
                return false;
            }
        }

        return true;
    }
}
