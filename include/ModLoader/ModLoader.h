//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MODLOADER_H
#define MODLOADER_MODLOADER_H

#include <SymExtract/Symbols.h>

#include <funchook.h>
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

        /*
         * Hooks
         */
        typedef void (*DedicatedServer_Start)(void* instance, const std::string& sessionId);

        static void hook_DedicatedServer_start(void* instance, const std::string& sessionId);

        DedicatedServer_Start m_DedicatedServer_Start;

        /*
         * Fields
         */
        funchook_t* m_hooks;
        SymExtract::SymbolTable m_symbolTable;
        SymExtract::DedicatedServer* m_dedicatedServer;

        /*
         * Methods
         */
        ModLoader();

        bool initialize();
        bool installHooks();

    };

}

#endif //MODLOADER_MODLOADER_H
