//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MEMBERFUNCTIONDECL_H
#define MODLOADER_MEMBERFUNCTIONDECL_H

#include <nlohmann/json.hpp>

#include <string>

namespace GoMint {

    struct MemberFunctionDecl {

        std::string m_name;
        std::string m_symbol;

    };

    void to_json(nlohmann::json& j, const GoMint::MemberFunctionDecl& p);
    void from_json(const nlohmann::json& j, GoMint::MemberFunctionDecl& p);

}

#endif //MODLOADER_MEMBERFUNCTIONDECL_H
