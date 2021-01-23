//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "BlockLegacy.h"

namespace GoMint {

    BlockLegacy::BlockLegacy(SymExtract::BlockLegacy* self)
        : m_self{self} {

    }

    const std::string& BlockLegacy::getIdentifier() const {
        return m_self->m_hashedIdentifier.m_string;
    }
    const std::string& BlockLegacy::getNamespace() const {
        return m_self->m_namespace;
    }

    float BlockLegacy::getExplosionResistance() const {
        return m_self->getExplosionResistance(nullptr);
    }

}

