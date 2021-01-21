//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MOD_H
#define MODLOADER_MOD_H

#include <ModLoader/ModLoader.h>
#include <ModLoader/Logger.h>
#include <ModLoader/SharedLibrary.h>

#include <array>

namespace GoMint {

    class ModLoader;

    typedef std::array<std::uint8_t, 16> ModUUID;

    /**
     * Interface to access mod implementations.
     *
     * This class handles all the setup required to activate modifications from shared libraries
     * and to properly initialize and manage them at runtime.
     */
    class Mod {

    public:

        /**
         * Attempts to activate the mod given the loader requesting it and its shared library file.
         * Performs basic initialization of the modification so at least its metadata is available
         * after it was loaded from disk.
         *
         * @param loader The loader requesting the mod
         * @param file The file of the mod's shared library
         * @return The mod if loading succeeded or nullptr otherwise
         */
        static std::unique_ptr<Mod> loadModFromLibrary(ModLoader* loader, const std::string& file);

        /**
         * Desstructs the mod and releases its shared library handle.
         */
        ~Mod();

        /** @return The mod's unique identifier */
        const ModUUID& getUUID() const;
        /** @return The mod's version */
        const SemanticVersion& getVersion() const;
        /** @return The mod loader version this mod expects to be run on */
        const SemanticVersion& getModLoaderVersion() const;
        /** @return The mod's long name, if provided */
        const std::string& getLongName() const;
        /** @return The mod's short name, if provided */
        const std::string& getShortName() const;
        /** @return The mod's author, if provided */
        const std::string& getAuthor() const;
        /** @return The mod's description, if provided */
        const std::string& getDescription() const;


        /**
         * Performs basic initialization of the mod and loads all metadata.
         *
         * @param loader The loader requesting the mod's initialization
         * @return Whether or not initialization succeeded
         */
        bool initialize(ModLoader* loader);

        /**
         * Attempts to active the mod by invoking its activation handler. Only if the mod's activation
         * handler returns ModActivationResult::Success will the mod be considered activated.
         *
         * @param loader The loader requesting the mod's activation
         * @return The result the mod's activation handler returned
         */
        ModActivationResult activate(ModLoader* loader);

        /**
         * Deactivates the mod by invoking its deactivation handler.
         *
         * @param loader THe loader requesting the mod's deactivation
         */
        void deactivate(ModLoader* loader);


    private:

        typedef std::uint32_t       (MOD_CALL* GetModInfoVersionFunc)();
        typedef void                (MOD_CALL* GetModInfoFunc)(ModInformation*);
        typedef ModActivationResult (MOD_CALL* OnModActivateFunc)(IModLoader*, ILogger*);
        typedef void                (MOD_CALL* OnModDeactivateFunc)(IModLoader*);

        Mod(SharedLibraryPtr library,
            GetModInfoVersionFunc getModInfoVersion,
            GetModInfoFunc getModInfo,
            OnModActivateFunc onModActivate,
            OnModDeactivateFunc onModDeactivate);

        bool             m_initialized;
        SharedLibraryPtr m_library;
        LoggerPtr        m_logger;

        // Metadata
        ModUUID         m_uuid;
        SemanticVersion m_version;
        SemanticVersion m_modLoaderVersion;
        std::string     m_longName;
        std::string     m_shortName;
        std::string     m_author;
        std::string     m_description;

        GetModInfoVersionFunc m_getModInfoVersion;
        GetModInfoFunc        m_getModInfo;
        OnModActivateFunc     m_onModActivate;
        OnModDeactivateFunc   m_onModDeactivate;

        bool m_loaded;

    };

    using ModPtr = std::unique_ptr<Mod>;

}

#endif //MODLOADER_MOD_H
