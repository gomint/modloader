//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOLMEMBERFUNCTIONINHERITANCE_H
#define MODLOADER_SYMBOLMEMBERFUNCTIONINHERITANCE_H

#include <nlohmann/json.hpp>

namespace GoMint {

    enum class SymbolMemberFunctionInheritance : unsigned int {

        /** Invocation of the member function should follow single-inheritance invocation */
        Single = 0,
        /** Invocation of the member function should follow multi-inheritance invocation */
        Multi,

        Invalid

    };

    NLOHMANN_JSON_SERIALIZE_ENUM(SymbolMemberFunctionInheritance, {
        { SymbolMemberFunctionInheritance::Single, "single" },
        { SymbolMemberFunctionInheritance::Multi, "multi" },
        { SymbolMemberFunctionInheritance::Invalid, nullptr },
    })

}

#endif //MODLOADER_SYMBOLMEMBERFUNCTIONINHERITANCE_H
