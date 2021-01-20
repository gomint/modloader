//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_ACCESSIBILITYLEVEL_H
#define MODLOADER_ACCESSIBILITYLEVEL_H

#include <nlohmann/json.hpp>

namespace GoMint {

    enum class AccessibilityLevel : unsigned int {

        Public = 0,

        Invalid

    };

    NLOHMANN_JSON_SERIALIZE_ENUM(AccessibilityLevel, {
        { AccessibilityLevel::Public, "public" },
        { AccessibilityLevel::Invalid, nullptr },
    })

}

#endif //MODLOADER_ACCESSIBILITYLEVEL_H
