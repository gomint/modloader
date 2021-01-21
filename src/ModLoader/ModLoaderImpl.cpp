//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/ModLoaderImpl.h>
#include <SymExtract/Types/DedicatedServer.h>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if defined(MODLOADER_WINDOWS)
#   include <ModLoader/Windows/Filesystem.h>
#endif

#include <cstdio>

namespace GoMint {

    //
    // Implementation
    //
    ModLoader ModLoader::k_instance;
    const char* ModLoader::k_logPattern = "[%Y-%m-%d %H:%M:%S.%e] [%-13n] [%^%-8l%$] %v";
#if defined(MODLOADER_DEBUG)
    spdlog::level::level_enum ModLoader::k_logLevel = spdlog::level::debug;
#else
    spdlog::level::level_enum ModLoader::k_logLevel = spdlog::level::warn;
#endif

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
        if (m_dynamicallyInitialized) {
            return true;
        }
        printf("Initializating ModLoader...\n");
        if (!Filesystem::isDirectory("mods") && !Filesystem::createDirectory("mods")) {
            return false;
        }
        if (!initializeLogging()) {
            return false;
        }

        m_logger->info("Searching for mods in 'mods/'");
        if (!searchForMods()) {
            return false;
        }

        m_dedicatedServer = server;
        m_dynamicallyInitialized = true;

        m_logger->info("Initialization complete");
        return true;
    }

    LoggerPtr ModLoader::createLogger(const std::string& name) {
        auto logger = std::make_shared<spdlog::logger>(name, std::begin(m_loggingSinks), std::end(m_loggingSinks));
        prepareLogger(logger.get());
        return std::make_unique<Logger>(logger);
    }

    bool ModLoader::installHooks() {
        bool success = true;
        success &= m_hooks.registerMemberHook(SymExtract::g_DedicatedServer_start,
                                              &ModLoader::hook_DedicatedServer_start,
                                              hook_DedicatedServer_start_callback);
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
        auto defaultLogger = std::make_shared<spdlog::logger>("Unknown", std::begin(m_loggingSinks), std::end(m_loggingSinks));

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
        if (!Filesystem::isDirectory("mods") && !Filesystem::createDirectory("mods")) {
            return false;
        }

        std::vector<std::string> dllFiles = Filesystem::listDLLsInDirectory("mods");
        m_logger->info("Examining {} potential mods", dllFiles.size());
        for (auto& dll : dllFiles) {
            auto mod = Mod::loadModFromLibrary(this, "mods/" + dll);
            if (mod == nullptr) {
                continue;
            }

            m_logger->info("Detected mod '{}'", mod->getLongName());
            if (mod->load(this) == ModLoadResult::Success) {
                m_logger->info("Loaded mod '{}'", mod->getLongName());
                m_loadedMods.emplace_back(std::move(mod));
            }
        }

        return true;
    }

    //
    // Hooks
    //
    void ModLoader::hook_DedicatedServer_start(const std::string& sessionId) {
        // Warning: this is DedicatedServer NOT ModLoader!
        ModLoader& loader = ModLoader::k_instance;
        if (!loader.initialize(reinterpret_cast<SymExtract::DedicatedServer*>(this), sessionId)) {
            return; // Will simply quit the server
        }
        (reinterpret_cast<SymExtract::SingleInvocable*>(this)->*loader.hook_DedicatedServer_start_callback)(sessionId);
    }

    //
    // Public API
    //
    int ModLoader::getMajorVersion() {
        return MODLOADER_VERSION_MAJOR;
    }

    int ModLoader::getMinorVersion() {
        return MODLOADER_VERSION_MINOR;
    }

    int ModLoader::getPatchVersion() {
        return MODLOADER_VERSION_PATCH;
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