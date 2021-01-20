//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTIONARGUMENT_H
#define MODLOADER_FUNCTIONARGUMENT_H

#include <string>
#include <nlohmann/json.hpp>

namespace GoMint {

    struct FunctionArgument {

        std::string m_type;
        std::string m_name;

    };

    void to_json(nlohmann::json& j, const FunctionArgument& p);
    void from_json(const nlohmann::json& j, FunctionArgument& p);

}

#endif //MODLOADER_FUNCTIONARGUMENT_H
