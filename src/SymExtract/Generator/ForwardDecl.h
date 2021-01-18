//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FORWARDDECL_H
#define MODLOADER_FORWARDDECL_H

#include <nlohmann/json.hpp>

#include <string>

namespace GoMint {

    struct ForwardDecl {

        std::string m_name;
        std::string m_type;

    };

    void to_json(nlohmann::json& j, const GoMint::ForwardDecl& p);
    void from_json(const nlohmann::json& j, GoMint::ForwardDecl& p);

}

#endif //MODLOADER_FORWARDDECL_H
