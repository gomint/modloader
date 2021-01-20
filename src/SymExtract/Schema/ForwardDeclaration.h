//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FORWARDDECLARATION_H
#define MODLOADER_FORWARDDECLARATION_H

#include <nlohmann/json.hpp>

#include <string>

namespace GoMint {

    struct ForwardDeclaration {

        std::string m_name;
        std::string m_type;

    };

    void to_json(nlohmann::json& j, const GoMint::ForwardDeclaration& p);
    void from_json(const nlohmann::json& j, GoMint::ForwardDeclaration& p);

}

#endif //MODLOADER_FORWARDDECLARATION_H
