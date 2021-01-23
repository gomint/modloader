//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_CONSTANT_H
#define MODLOADER_CONSTANT_H

#include <cstdint>
#include <variant>

namespace GoMint {

    /**
     * A variant type able to hold any constants defined in a schema.
     */
    using Constant = std::variant<std::uint64_t>;

}

#endif //MODLOADER_CONSTANT_H
