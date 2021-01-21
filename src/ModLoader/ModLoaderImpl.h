//
// This file is named in a way that differs from the usually strict naming scheme followed everywhere else
// since it would otherwise collide with the ModLoader.h public header file.
//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MODLOADERIMPL_H
#define MODLOADER_MODLOADERIMPL_H

#include <ModLoader/ModLoader.h>
#include <ModLoader/Mod.h>
#include <ModLoader/HookDispatcher.h>
#include <SymExtract/Symbols.h>
#include <SymExtract/Types/DedicatedServer.h>

#include <spdlog/spdlog.h>

namespace GoMint {

    /**
     * Base class of the entire ModLoader.
     */
    class ModLoader : public IModLoader {

        friend IModLoader* ::get_mod_loader_instance();
    public:

        /**
         * Invoked once per process by the platform-dependent entry point. Should initialize the ModLoader.
         *
         * @return Whether or not initialization succeeded
         */
        static bool initializeStatically();

        bool initialize(SymExtract::DedicatedServer* server, const std::string& sessionId);
        LoggerPtr createLogger(const std::string& name);

        //
        // Public API
        //
        int getMajorVersion() override;
        int getMinorVersion() override;
        int getPatchVersion() override;

    private:
        //
        // Hooks
        //
        SymExtract::DedicatedServer_start_funcptr hook_DedicatedServer_start_callback;
        void hook_DedicatedServer_start(const std::string& sessionId);

        HookDispatcher m_hooks;

    private:

        static ModLoader k_instance;
        static const char* k_logPattern;
        static spdlog::level::level_enum k_logLevel;

        //
        // Fields
        //
        bool m_staticallyInitialized;
        bool m_dynamicallyInitialized;
        std::vector<spdlog::sink_ptr> m_loggingSinks;
        std::unique_ptr<spdlog::logger> m_logger;
        std::vector<ModPtr>             m_loadedMods;

        SymExtract::DedicatedServer* m_dedicatedServer;


        ModLoader() noexcept;

        bool installHooks();
        bool initializeLogging();
        void prepareLogger(spdlog::logger* logger);
        bool searchForMods();

    };

}

#endif //MODLOADER_MODLOADERIMPL_H
