//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "ModLoaderEntry.h"

#include <ModLoader/ModLoader.h>

namespace GoMint {

    bool ModLoaderEntry::initialize() {
        ModLoader::k_modLoader = std::unique_ptr<ModLoader>(new ModLoader());
        return ModLoader::k_modLoader->initialize();
    }

}