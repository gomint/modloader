//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/Mod.h>
#include <ModLoader/ModLoaderImpl.h>
#include <ModLoader/Filesystem.h>

#include <cstring>


namespace GoMint {

    std::unique_ptr<Mod> Mod::loadModFromLibrary(ModLoader* loader, const std::string& file) {
        std::string absolute = Filesystem::getAbsolutePath(file);
        if (absolute.empty()) {
            return nullptr;
        }

        SharedLibraryPtr library = SharedLibrary::load(absolute);
        if (library == nullptr) {
            return nullptr;
        }

        void* getModInfoVersion = library->getProcedureAddress("get_mod_information_version");
        void* getModInfo        = library->getProcedureAddress("get_mod_information");
        void* onModActivate     = library->getProcedureAddress("on_mod_activate");
        void* onModDeactivate   = library->getProcedureAddress("on_mod_deactivate");

        if (getModInfoVersion == nullptr ||
            getModInfo == nullptr ||
            onModActivate == nullptr ||
            onModDeactivate == nullptr) {
            return nullptr;
        }

        std::unique_ptr<Mod> mod(new Mod(std::move(library),
                                         reinterpret_cast<GetModInfoVersionFunc>(getModInfoVersion),
                                         reinterpret_cast<GetModInfoFunc>(getModInfo),
                                         reinterpret_cast<OnModActivateFunc>(onModActivate),
                                         reinterpret_cast<OnModDeactivateFunc>(onModDeactivate)));
        return mod;
    }

    Mod::Mod(SharedLibraryPtr library,
             GetModInfoVersionFunc getModInfoVersion,
             GetModInfoFunc getModInfo,
             OnModActivateFunc onModActivate,
             OnModDeactivateFunc onModDeactivate)
            : m_initialized{false},
              m_library{std::move(library)},
              m_uuid{0},
              m_version{0},
              m_modLoaderVersion{0},
              m_getModInfoVersion{getModInfoVersion},
              m_getModInfo{getModInfo},
              m_onModActivate{onModActivate},
              m_onModDeactivate{onModDeactivate},
              m_loaded{false} {

    }

    Mod::~Mod() = default;

    const ModUUID& Mod::getUUID() const {
        return m_uuid;
    }

    const SemanticVersion& Mod::getVersion() const {
        return m_version;
    }

    const SemanticVersion& Mod::getModLoaderVersion() const {
        return m_modLoaderVersion;
    }

    const std::string& Mod::getLongName() const {
        return m_longName;
    }

    const std::string& Mod::getShortName() const {
        return m_shortName;
    }

    const std::string& Mod::getAuthor() const {
        return m_author;
    }

    const std::string& Mod::getDescription() const {
        return m_description;
    }

    bool Mod::initialize(ModLoader* loader) {
        std::uint32_t infoVersion = m_getModInfoVersion();
        if (infoVersion > ModInformation::STRUCTURE_VERSION) {
            /*
             * Mod might try to write to invalid memory regions if we attempt to retrieve it's information
             * using an outdated version of the ModInformation structure => incompatible.
             */
            loader->getLogger()->warn(
                    "A mod is requiring a newer version of ModLoader (ModInformation structure version: {})",
                    infoVersion);
            return false;
        }

        // Load raw information
        ModInformation info{0};
        m_getModInfo(&info);

        // Be safe! Convert all known information about this mod
        std::memcpy(&m_uuid[0], info.m_uuid, 16);
        m_version          = info.m_version;
        m_modLoaderVersion = info.m_modLoaderVersion;

        std::size_t longNameLength    = strnlen_s(info.m_longName, ModInformation::MAX_LONGNAME_LENGTH + 1);
        std::size_t shortNameLength   = strnlen_s(info.m_shortName, ModInformation::MAX_SHORTNAME_LENGTH + 1);
        std::size_t authorLength      = strnlen_s(info.m_author, ModInformation::MAX_AUTHOR_LENGTH + 1);
        std::size_t descriptionLength = strnlen_s(info.m_description, ModInformation::MAX_DESCRIPTION_LENGTH + 1);

        m_longName    = (longNameLength == ModInformation::MAX_LONGNAME_LENGTH + 1) ? "" : std::string(info.m_longName,
                                                                                                       longNameLength);
        m_shortName   = (shortNameLength == ModInformation::MAX_SHORTNAME_LENGTH + 1) ? "" : std::string(
                info.m_shortName, shortNameLength);
        m_author      = (authorLength == ModInformation::MAX_AUTHOR_LENGTH + 1) ? "" : std::string(info.m_author,
                                                                                                   authorLength);
        m_description = (descriptionLength == ModInformation::MAX_DESCRIPTION_LENGTH + 1) ? "" : std::string(
                info.m_description, descriptionLength);

        // Perform version compatibility checks:
        SemanticVersion loaderVersion = loader->getVersion();
        if (!loaderVersion.isCompatible(m_modLoaderVersion)) {
            if (m_modLoaderVersion > loaderVersion) {
                loader->getLogger()->warn(
                        "Outdated loader: '{}' requires version '{}.{}.{}",
                        m_longName,
                        m_modLoaderVersion.m_major,
                        m_modLoaderVersion.m_minor,
                        m_modLoaderVersion.m_patch
                );
            } else {
                loader->getLogger()->warn(
                        "Outdated mod: '{}' requires old loader version '{}.{}.{}",
                        m_longName,
                        m_modLoaderVersion.m_major,
                        m_modLoaderVersion.m_minor,
                        m_modLoaderVersion.m_patch
                );
            }
            return false;
        }

        // Initialize logger
        m_logger = m_shortName.empty() ? std::make_unique<Logger>(spdlog::default_logger()) : loader->createLogger(
                m_shortName);

        m_initialized = true;
        return true;
    }

    ModActivationResult Mod::activate(ModLoader* loader) {
        if (!m_initialized && !initialize(loader)) return ModActivationResult::Failed;
        if (m_loaded) return ModActivationResult::Success;

        ModActivationResult result = m_onModActivate(loader, m_logger.get());
        if (result == ModActivationResult::Success) {
            m_loaded = true;
        } else {
            m_loaded = false;
        }
        return result;
    }

    void Mod::deactivate(ModLoader* loader) {
        if (!m_loaded) return;

        m_onModDeactivate(loader);
        m_loaded = false;
    }

}
