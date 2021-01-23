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
#include <SymExtract/Types/ServerInstanceEventCoordinator.h>

#include <spdlog/spdlog.h>

namespace GoMint {

    /**
     * Base class of the entire ModLoader.
     */
    class ModLoader : public IModLoader {

        friend IModLoader* ::get_mod_loader_instance();
    public:

        /**
         * Invoked once per process by the platform-dependent entry point. Should initialize the static components
         * of the ModLoader. Since this function will be invoked from the DllMain function on Windows it is very
         * limited on what it can actually do without leading to deadlocks. Basically, it is only supposed to provide
         * the minimum amount of setup required to hook into the server's startup routine to be invoked again at a
         * later time from outside of DllMain.
         *
         * @return Whether or not initialization succeeded
         */
        static bool initializeStatically();

        /**
         * Invoked once the server is starting up. This initialization routine will be invoked from outside
         * DllMain which allows it to do all further setup required.
         *
         * @param server The DedicatedServer instance the mod loader has hijacked
         * @param sessionId The session ID passed to DedicatedServer::start as a parameter.
         * @return
         */
        bool initialize(SymExtract::DedicatedServer* server, const std::string& sessionId);

        /** @return The mod loader's own logger instance */
        spdlog::logger* getLogger() const;
        /**
         * Creates a new logger with the specified name and binds it to all configured logging sinks.
         *
         * @param name The name for the logger (should not exceed 15 characters)
         * @return The newly created logger
         */
        LoggerPtr createLogger(const std::string& name);

        //
        // Public API
        //
        SemanticVersion getVersion() const override;
        IBlockTypeRegistry* getBlockTypeRegistry() override;

    private:
        //
        // Events
        //
        void onDedicatedServerStart(SymExtract::DedicatedServer* server, const std::string& sessionID);
        void onServerInitializationComplete(void* server);

    private:
        //
        // Hooks
        //
        SymExtract::DedicatedServer_start_funcptr hook_DedicatedServer_start_callback;
        void hook_DedicatedServer_start(const std::string& sessionId);

        SymExtract::ServerInstanceEventCoordinator_sendServerInitializeEnd_funcptr hook_ServerInstanceEventCoordinator_sendServerInitializeEnd_callback;
        void hook_ServerInstanceEventCoordinator_sendServerInitializeEnd(void* server);

        HookDispatcher m_hooks;

    private:

        static ModLoader k_instance;
        static const char* k_logPattern;
        static spdlog::level::level_enum k_logLevel;
        static std::string k_modDirectory;

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

        /** Static Initialization: installs required hooks */
        bool installHooks();
        /** Dynamic Initializtion: initializes logging system */
        bool initializeLogging();
        /** Dynamic Initialization: configures the logger and binds it to available logging sinks */
        void prepareLogger(spdlog::logger* logger);
        /** Dynamic Initialization: searches for mods */
        bool searchForMods();

        void checkForDebuggingOptions();

    };

}

#endif //MODLOADER_MODLOADERIMPL_H
