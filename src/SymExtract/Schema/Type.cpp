//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "Type.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::Type& p) {
        j["name"]                                                    = p.m_name;
        if (p.m_size != Type::UNKNOWN_SIZE) j["size"]                = p.m_size;
        if (p.m_alignment != Type::UNKNOWN_ALIGNMENT) j["alignment"] = p.m_alignment;
        if (!p.m_members.empty()) j["layout"]                        = p.m_members;
        if (!p.m_functions.empty()) j["funcs"]                       = p.m_functions;
        if (!p.m_memberFunctions.empty()) j["memfuncs"]              = p.m_memberFunctions;
    }

    void from_json(const nlohmann::json& j, GoMint::Type& p) {
        j.at("name").get_to(p.m_name);

        p.m_alignment = Type::UNKNOWN_ALIGNMENT;

        if (j.contains("layout")) {
            j.at("size").get_to(p.m_size);
            j.at("layout").get_to(p.m_members);
        } else {
            p.m_size = Type::UNKNOWN_SIZE;
            p.m_members.clear();
        }

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
        for (auto& member : m_members) {
            if (!member.resolveSize(schema)) {
                return false;
            }
        }

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

    const std::string& Type::getName() {
        return m_name;
    }

    std::uint64_t Type::getSize() {
        return m_size;
    }

}
