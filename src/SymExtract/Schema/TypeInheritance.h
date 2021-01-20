//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_TYPEINHERITANCE_H
#define MODLOADER_TYPEINHERITANCE_H

#include <nlohmann/json.hpp>

namespace GoMint {

    enum class TypeInheritance : unsigned int {

        /** Invocation of the member function should follow single-inheritance invocation */
        Single = 0,
        /** Invocation of the member function should follow multi-inheritance invocation */
        Multi,

        Invalid

    };

    NLOHMANN_JSON_SERIALIZE_ENUM(TypeInheritance, {
        { TypeInheritance::Single, "single" },
        { TypeInheritance::Multi, "multi" },
        { TypeInheritance::Invalid, nullptr },
    })

}

#endif //MODLOADER_TYPEINHERITANCE_H
