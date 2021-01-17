//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MODLOADERENTRY_H
#define MODLOADER_MODLOADERENTRY_H

namespace GoMint {

    /**
     * Helper class to allow platform-dependant mechanisms to initialize the mod loader singleton.
     */
    class ModLoaderEntry {

    public:

        static bool initialize();

    };

}

#endif //MODLOADER_MODLOADERENTRY_H
