//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_TYPE_H
#define MODLOADER_TYPE_H

#include "Function.h"
#include "MemberFunction.h"

#include <vector>

namespace GoMint {

    class Schema;

    /**
     * Declaration of a type which may have members.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    struct Type {

        std::string m_name;

        std::vector<Function> m_functions;
        std::vector<MemberFunction> m_memberFunctions;

        bool resolveSymbols(Schema* schema);

    };

    void to_json(nlohmann::json& j, const GoMint::Type& p);
    void from_json(const nlohmann::json& j, GoMint::Type& p);

}

#endif //MODLOADER_TYPE_H
