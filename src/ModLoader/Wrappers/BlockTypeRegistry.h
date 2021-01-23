//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_BLOCKTYPEREGISTRY_H
#define MODLOADER_BLOCKTYPEREGISTRY_H

#include <ModLoader/ModLoader.h>

namespace GoMint {

    class BlockTypeRegistry : public IBlockTypeRegistry {

    public:

        void forEachBlock(std::function<bool (const IBlockLegacy&)> callback) override;

    };

}

#endif //MODLOADER_BLOCKTYPEREGISTRY_H
