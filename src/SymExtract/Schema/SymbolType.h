//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOLTYPE_H
#define MODLOADER_SYMBOLTYPE_H

#include <nlohmann/json.hpp>

namespace GoMint {

    /**
     * Enumeration of supported symbol types
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    enum class SymbolType : unsigned int {

        /** Regular function (global, static) */
        Function = 0,
        /** Member function (bound to instance) */
        MemberFunction,

        Invalid

    };

    NLOHMANN_JSON_SERIALIZE_ENUM(SymbolType, {
        { SymbolType::Function, "func" },
        { SymbolType::MemberFunction, "memfunc" },
        { SymbolType::Invalid, nullptr },
    })

}

#endif //MODLOADER_SYMBOLTYPE_H
