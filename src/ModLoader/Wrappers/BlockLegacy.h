//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_BLOCKLEGACY_H
#define MODLOADER_BLOCKLEGACY_H

#include <ModLoader/ModLoader.h>
#include <SymExtract/Types/BlockLegacy.h>

namespace GoMint {

    class BlockLegacy : public IBlockLegacy {

    public:

        BlockLegacy(SymExtract::BlockLegacy* self);

        const std::string& getIdentifier() const override;
        const std::string& getNamespace() const override;
        float getExplosionResistance() const override;

    private:

        SymExtract::BlockLegacy* m_self;

    };

}

#endif //MODLOADER_BLOCKLEGACY_H
