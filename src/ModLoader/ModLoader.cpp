//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/ModLoader.h>

#include <SymExtract/Types/DedicatedServer.h>

namespace GoMint {

    std::unique_ptr<ModLoader> ModLoader::k_modLoader;

    ModLoader::ModLoader() {

    }

    ModLoader* ModLoader::getInstance() {
        return k_modLoader.get();
    }

    bool ModLoader::initialize() {
        if (!SymExtract::loadSymbols()) {
            printf("[ModLoader] Failed to load native symbols\n");
            return false;
        }
        printf("[ModLoader] Loaded native symbols\n");

        if (!installHooks()) {
            printf("[ModLoader] Failed to install hooks\n");
        }
        printf("[ModLoader] Installed hooks\n");

        printf("[ModLoader] Initialization complete\n");
        return true;
    }

    bool ModLoader::installHooks() {
        m_hooks = funchook_create();

        int result;

        m_DedicatedServer_Start = reinterpret_cast<DedicatedServer_Start>(SymExtract::retrieveFunctionAddress(SymExtract::g_DedicatedServer_start));
        result = funchook_prepare(m_hooks, (void**) &m_DedicatedServer_Start, (void*) &hook_DedicatedServer_start);
        if (result != 0) {
            printf("[ModLoader] Failed to prepare hook into DedicatedServer::start()\n");
            return false;
        }

        result = funchook_install(m_hooks, 0);
        if (result != 0) {
            printf("[ModLoader] Failed to install hooks\n");
            return false;
        }

        return true;
    }


    /*
     * Hooks
     */
    void ModLoader::hook_DedicatedServer_start(void* instance, const std::string& sessionId) {
        ModLoader* modLoader = ModLoader::getInstance();
        modLoader->m_dedicatedServer = reinterpret_cast<SymExtract::DedicatedServer*>(instance);

        printf("[ModLoader] Hook triggered: DedicatedServer::start()\n");
        printf("[ModLoader] ModLoader::m_dedicatedServer = %p\n", modLoader->m_dedicatedServer);
        printf("[ModLoader] The real session ID was: %s\n", sessionId.c_str());

        modLoader->m_DedicatedServer_Start(instance, sessionId);
    }

}