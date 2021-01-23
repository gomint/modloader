//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTIONRESOLUTION_H
#define MODLOADER_FUNCTIONRESOLUTION_H

#include <nlohmann/json.hpp>

namespace GoMint {

    /**
     * Types of function resolution.
     */
    enum class FunctionResolution : unsigned int {

        /**
         * In this function resolution scheme, a function's address offset is static and does
         * not need to be determined at runtime. It can therefore be substituted by a simple
         * symbol.
         *
         * Applicable to: functions, member functions
         */
        Fixed = 0,

        /**
         * In this function resolution scheme, a function's address is stored in a vtable
         * for any given index. How exactly vtable resolution is carried out depends on the
         * type of inheritance employed by the type that contains function.
         *
         * Applicable to: member functions
         */
        Virtual,

        /**
         * Denotes an unknown or invalid type of function resolution
         */
        Invalid

    };

    NLOHMANN_JSON_SERIALIZE_ENUM(FunctionResolution, {
        { FunctionResolution::Fixed, "fixed" },
        { FunctionResolution::Virtual, "virtual" },
        { FunctionResolution::Invalid, nullptr }
    });

}

#endif //MODLOADER_FUNCTIONRESOLUTION_H
