//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/ModLoader.h>

#include <SymExtract/Symbols.h>

#include <cstdio>

namespace GoMint {

    std::unique_ptr<ModLoader> ModLoader::k_modLoader;

    ModLoader::ModLoader() {

    }

    ModLoader* ModLoader::getInstance() {
        return k_modLoader.get();
    }

    bool ModLoader::initialize() {
        if (!Symbols::loadSymbols()) {
            return false;
        }

        int value = Symbols::getIncredibleValue();
        printf("The incredible value is: %d\n", value);
        return true;
    }

}