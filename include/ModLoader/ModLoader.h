//
// This header file contains all API types exported by the ModLoader.
//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MODLOADER_H
#define MODLOADER_MODLOADER_H

#include <ModLoader/Config.h>

#include <cstdint>
#include <memory>
#include <string>

namespace GoMint {

    /**
     * Enumeration of allowed result values during mod loading.
     */
    enum class ModLoadResult : unsigned int {

        Success = 0,
        Failed

    };

    /**
     * Descriptor structure which holds a semantic version number.
     */
    struct SemanticVersion {
        std::uint32_t m_major;
        std::uint32_t m_minor;
        std::uint32_t m_patch;
    };

    /**
     * Descriptor structure used to provide meta-data about a mod.
     */
    struct ModInformation {
        static constexpr std::uint32_t STRUCTURE_VERSION      = 1;
        static constexpr std::size_t   MAX_LONGNAME_LENGTH    = 255;
        static constexpr std::size_t   MAX_SHORTNAME_LENGTH   = 15;
        static constexpr std::size_t   MAX_AUTHOR_LENGTH      = 255;
        static constexpr std::size_t   MAX_DESCRIPTION_LENGTH = 255;

        std::uint32_t   m_structureVersion;
        std::uint8_t    m_uuid[16];
        SemanticVersion m_version;
        SemanticVersion m_modLoaderVersion;
        char            m_longName[MAX_LONGNAME_LENGTH + 1];
        char            m_shortName[MAX_SHORTNAME_LENGTH + 1];
        char            m_author[MAX_AUTHOR_LENGTH + 1];
        char            m_description[MAX_DESCRIPTION_LENGTH + 1];
    };

    /**
     * Enumeration of possible log levels.
     */
    enum class LogLevel : unsigned int {

        Trace = 0,
        Debug,
        Info,
        Warning,
        Error,
        Critical,
        Off,
        Invalid

    };

    /**
     * Logger class which mods should use to produce log output.
     */
    class ILogger {

    public:

        virtual void log(LogLevel level, const std::string& msg) = 0;
        virtual void trace(const std::string& msg) = 0;
        virtual void debug(const std::string& msg) = 0;
        virtual void info(const std::string& msg) = 0;
        virtual void warning(const std::string& msg) = 0;
        virtual void error(const std::string& msg) = 0;
        virtual void critical(const std::string& msg) = 0;

    };


    /**
     * Base interface for accessing the mod loader's capabilities.
     * One can acquire an instance through the
     */
    class IModLoader {

    public:

        /** @return The installed modloader's major version */
        virtual int getMajorVersion() = 0;
        /** @return The installed modloader's minor version */
        virtual int getMinorVersion() = 0;
        /** @return The installed modloader's patch version */
        virtual int getPatchVersion() = 0;

    };

}

extern "C" {

/**
 * If available, this method will retrieve an instance of the IModLoader interface.
 *
 * The returned interface need not be released.
 *
 * @return The interface if available or NULL otherwise.
 */
MODLOADER_API GoMint::IModLoader* MODLOADER_CALL get_mod_loader_instance();

}

#endif //MODLOADER_MODLOADER_H
