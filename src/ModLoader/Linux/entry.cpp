//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <cstdlib>

#include "../ModLoaderEntry.h"

__attribute__((constructor))
static void modloader_entry() {
    if (!GoMint::ModLoaderEntry::initialize()) {
        exit(1);
    }
}