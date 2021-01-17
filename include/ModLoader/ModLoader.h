//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MODLOADER_H
#define MODLOADER_MODLOADER_H

#include <memory>

namespace GoMint {

    class ModLoaderEntry;

    /**
     * @author Ciel DeVille
     * @version 1.0.0
     * @stability experimental
     */
    class ModLoader {

        friend class ModLoaderEntry;
    public:

        /**
         * @return The main instance of the mod loader for the running process.
         */
        static ModLoader* getInstance();


    private:

        /** The mod loader singleton instance */
        static std::unique_ptr<ModLoader> k_modLoader;

        ModLoader();

        bool initialize();

    };

}

#endif //MODLOADER_MODLOADER_H
