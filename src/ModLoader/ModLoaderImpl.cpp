//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/ModLoaderImpl.h>
#include <ModLoader/Filesystem.h>
#include <ModLoader/Wrappers/BlockTypeRegistry.h>

#if defined(MODLOADER_DEBUG)

#   include <ModLoader/Debugger.h>

#endif

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cstdio>

namespace GoMint {

    //
    // Implementation
    //
    ModLoader ModLoader::k_instance;
    const char* ModLoader::k_logPattern = "[%Y-%m-%d %H:%M:%S.%e] [%=15n] [%^%=8l%$] %v";
#if defined(MODLOADER_DEBUG)
    spdlog::level::level_enum ModLoader::k_logLevel = spdlog::level::debug;
#else
    spdlog::level::level_enum ModLoader::k_logLevel = spdlog::level::warn;
#endif
    std::string ModLoader::k_modDirectory = "mods";

    ModLoader::ModLoader() noexcept
            : m_staticallyInitialized{false},
              m_dynamicallyInitialized{false},
              m_logger{nullptr},
              m_dedicatedServer{nullptr},
              hook_DedicatedServer_start_callback{nullptr} {

    }

    bool ModLoader::initializeStatically() {
        if (k_instance.m_staticallyInitialized) {
            return true;
        }
        if (!SymExtract::loadSymbols() || !k_instance.installHooks()) {
            return false;
        }
        k_instance.m_staticallyInitialized = true;
        return true;
    }

    bool ModLoader::initialize(SymExtract::DedicatedServer* server, const std::string& sessionId) {
        checkForDebuggingOptions();

        if (m_dynamicallyInitialized) {
            return true;
        }
        printf("Initializating ModLoader...\n");

        if (!Filesystem::isDirectory(k_modDirectory) &&
            !Filesystem::createDirectory(k_modDirectory)) {
            printf("Could not create '%s' directory\n", k_modDirectory.c_str());
            return false;
        }

        if (!initializeLogging()) {
            printf("Could not setup logging\n");
            return false;
        }

        m_logger->info("Searching for mods in '{}'", k_modDirectory);
        if (!searchForMods()) {
            return false;
        }

        m_dedicatedServer        = server;
        m_dynamicallyInitialized = true;

        m_logger->info("Initialization complete");
        return true;
    }

    spdlog::logger* ModLoader::getLogger() const {
        return m_logger.get();
    }

    LoggerPtr ModLoader::createLogger(const std::string& name) {
        auto logger = std::make_shared<spdlog::logger>(name, std::begin(m_loggingSinks), std::end(m_loggingSinks));
        prepareLogger(logger.get());
        return std::make_unique<Logger>(logger);
    }

    bool ModLoader::installHooks() {
        bool success = true;
        success &= m_hooks.registerMemberHook(SymExtract::DedicatedServer::k_start,
                                              &ModLoader::hook_DedicatedServer_start,
                                              hook_DedicatedServer_start_callback);
        success &= m_hooks.registerMemberHook(SymExtract::ServerInstanceEventCoordinator::k_sendServerInitializeEnd,
                                              &ModLoader::hook_ServerInstanceEventCoordinator_sendServerInitializeEnd,
                                              hook_ServerInstanceEventCoordinator_sendServerInitializeEnd_callback);
        return success && m_hooks.install();
    }

    bool ModLoader::initializeLogging() {
        m_loggingSinks.clear();

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#if defined(MODLOADER_WINDOWS)
        consoleSink->set_color(spdlog::level::debug, consoleSink->GREEN);
        consoleSink->set_color(spdlog::level::info, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
#else
        consoleSink->set_color(spdlog::level::debug, consoleSink->green);
        consoleSink->set_color(spdlog::level::info, consoleSink->cyan);
#endif

        auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("mods/common.log", 8 * 1024 * 1024, 3);
        // Push empty line into log files
        {
            spdlog::logger helperLogger = spdlog::logger("__util", fileSink);
            helperLogger.set_pattern("%v");
            helperLogger.critical("");
        }

        m_loggingSinks.push_back(consoleSink);
        m_loggingSinks.push_back(fileSink);

        m_logger = std::make_unique<spdlog::logger>("ModLoader", std::begin(m_loggingSinks), std::end(m_loggingSinks));
        auto defaultLogger = std::make_shared<spdlog::logger>("Unknown", std::begin(m_loggingSinks),
                                                              std::end(m_loggingSinks));

        prepareLogger(m_logger.get());
        prepareLogger(defaultLogger.get());

        spdlog::set_default_logger(defaultLogger);
        return true;
    }

    void ModLoader::prepareLogger(spdlog::logger* logger) {
        logger->set_pattern(k_logPattern);
        logger->set_level(k_logLevel);
    }

    bool ModLoader::searchForMods() {
        std::vector<std::string> dllFiles = Filesystem::listSharedLibraries(k_modDirectory);
        m_logger->info("Examining {} potential mods", dllFiles.size());
        for (auto& dll : dllFiles) {
            auto mod = Mod::loadModFromLibrary(this, k_modDirectory + "/" + dll);
            if (mod == nullptr) {
                continue;
            }

            if (!mod->initialize(this)) {
                continue;
            }

            m_logger->info(
                    "Loaded mod '{}' v{}.{}.{} by {}",
                    mod->getLongName(),
                    mod->getVersion().m_major,
                    mod->getVersion().m_minor,
                    mod->getVersion().m_patch,
                    mod->getAuthor()
            );
            if (mod->activate(this) == ModActivationResult::Success) {
                m_logger->info("Activated mod '{}'", mod->getLongName());
                m_loadedMods.emplace_back(std::move(mod));
            }
        }

        return true;
    }

    void ModLoader::checkForDebuggingOptions() {
#if defined(MODLOADER_DEBUG)
        Debugger::waitForDebugger();
        Debugger::breakpoint();
#endif
    }


    //
    // Events
    //
    void ModLoader::onDedicatedServerStart(SymExtract::DedicatedServer* server, const std::string& sessionID) {

    }

    void ModLoader::onServerInitializationComplete(void* server) {
#if defined(MODLOADER_DEBUG)
        Debugger::breakpoint();
#endif
        BlockTypeRegistry registry;
        registry.forEachBlock([&](const IBlockLegacy& block) -> bool {
            m_logger->info("Detected block: {}, explosion resistance={}", block.getIdentifier(), block.getExplosionResistance());
            return true;
        });
    }



    //
    // Hooks
    //

    // =============================================================================== //
    // WARNING
    //
    // The following member functions are subject of _EXTREME_ stretching of C++'s
    // low-level implementation details. NEVER rely the this pointer to actually
    // point to a low-level instance! Most likely, it is not!
    // =============================================================================== //

    void ModLoader::hook_DedicatedServer_start(const std::string& sessionId) {
        // Warning: this is DedicatedServer NOT ModLoader!
        auto     * server = reinterpret_cast<SymExtract::DedicatedServer*>(this);
        ModLoader& loader = ModLoader::k_instance;
        if (!loader.initialize(server, sessionId)) {
            return; // Will simply quit the server
        }
        loader.onDedicatedServerStart(server, sessionId);
        (reinterpret_cast<SymExtract::SingleInvocable*>(this)->*loader.hook_DedicatedServer_start_callback)(sessionId);
    }

    void ModLoader::hook_ServerInstanceEventCoordinator_sendServerInitializeEnd(void* server) {
        // Warning: this is ServerInstanceEventCoordinator NOT ModLoader!
        ModLoader& loader = ModLoader::k_instance;
        loader.onServerInitializationComplete(server);
        (reinterpret_cast<SymExtract::SingleInvocable*>(this)->*
         loader.hook_ServerInstanceEventCoordinator_sendServerInitializeEnd_callback)(server);
    }

    //
    // Public API
    //
    SemanticVersion ModLoader::getVersion() const {
        return MODLOADER_VERSION;
    }

    IBlockTypeRegistry* ModLoader::getBlockTypeRegistry() {
        // TODO: Once symbols are acquired
        return nullptr;
    }

}

extern "C" {
    MODLOADER_API GoMint::IModLoader* MODLOADER_CALL get_mod_loader_instance() {
        GoMint::ModLoader& loader = GoMint::ModLoader::k_instance;
        if (loader.m_staticallyInitialized && loader.m_dynamicallyInitialized) {
            return &loader;
        }
        return nullptr;
    }
}