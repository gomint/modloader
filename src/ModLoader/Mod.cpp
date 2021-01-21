//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/Mod.h>
#include <ModLoader/ModLoaderImpl.h>

#include <cstring>

#if defined(MODLOADER_WINDOWS)
#   include <ModLoader/Windows/Filesystem.h>
#else
#   error "No implementation for Filesystem provided"
#endif

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

        void* getModInfoFunc = library->getProcedureAddress("get_mod_information");
        void* onModLoadFunc = library->getProcedureAddress("on_mod_load");
        void* onModUnloadFunc = library->getProcedureAddress("on_mod_unload");

        if (getModInfoFunc == nullptr || onModLoadFunc == nullptr || onModUnloadFunc == nullptr) {
            return nullptr;
        }

        std::unique_ptr<Mod> mod(new Mod(std::move(library),
                                         reinterpret_cast<GetModInfoFunc>(getModInfoFunc),
                                         reinterpret_cast<OnModLoadFunc>(onModLoadFunc),
                                         reinterpret_cast<OnModUnloadFunc>(onModUnloadFunc)));
        mod->initialize(loader);
        return mod;
    }

    Mod::Mod(SharedLibraryPtr library,
             GetModInfoFunc getModInfo,
             OnModLoadFunc onModLoad,
             OnModUnloadFunc onModUnload)
             : m_library{std::move(library)},
               m_getModInfo{getModInfo},
               m_onModLoad{onModLoad},
               m_onModUnload{onModUnload},
               m_loaded{false} {

    }

    Mod::~Mod() = default;

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

    void Mod::initialize(ModLoader* loader) {
        ModInformation info{0};
        m_getModInfo(&info);

        // Be safe!
        std::size_t longNameLength = strnlen_s(info.m_longName, ModInformation::MAX_LONGNAME_LENGTH + 1);
        std::size_t shortNameLength = strnlen_s(info.m_shortName, ModInformation::MAX_SHORTNAME_LENGTH + 1);
        std::size_t authorLength = strnlen_s(info.m_author, ModInformation::MAX_AUTHOR_LENGTH + 1);
        std::size_t descriptionLength = strnlen_s(info.m_description, ModInformation::MAX_DESCRIPTION_LENGTH + 1);

        m_longName = (longNameLength == ModInformation::MAX_LONGNAME_LENGTH + 1) ? "" : std::string(info.m_longName, longNameLength);
        m_shortName = (shortNameLength == ModInformation::MAX_SHORTNAME_LENGTH + 1) ? "" : std::string(info.m_shortName, shortNameLength);
        m_author = (authorLength == ModInformation::MAX_AUTHOR_LENGTH + 1) ? "" : std::string(info.m_author, authorLength);
        m_description = (descriptionLength == ModInformation::MAX_DESCRIPTION_LENGTH + 1) ? "" : std::string(info.m_description, descriptionLength);

        m_logger = m_shortName.empty() ? std::make_unique<Logger>(spdlog::default_logger()) : loader->createLogger(m_shortName);
    }

    ModLoadResult Mod::load(IModLoader* loader) {
        if (m_loaded) return ModLoadResult::Success;

        ModLoadResult result = m_onModLoad(loader, m_logger.get());
        if (result == ModLoadResult::Success) {
            m_loaded = true;
        } else {
            m_loaded = false;
        }
        return result;
    }

    void Mod::unload(IModLoader* loader) {
        if (!m_loaded) return;

        m_onModUnload(loader);
        m_loaded = false;
    }

}
