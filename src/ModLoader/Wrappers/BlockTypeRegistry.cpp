//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <ModLoader/Wrappers/BlockTypeRegistry.h>
#include <ModLoader/Wrappers/BlockLegacy.h>
#include <SymExtract/Types/BlockTypeRegistry.h>

namespace GoMint {

    void BlockTypeRegistry::forEachBlock(std::function<bool (const IBlockLegacy&)> callback) {
        SymExtract::BlockTypeRegistry::forEachBlock(
                [&](const SymExtract::BlockLegacy& block) -> bool {
                    BlockLegacy wrapper(const_cast<SymExtract::BlockLegacy*>(&block));
                    return callback(wrapper);
                }
        );
    }
}
