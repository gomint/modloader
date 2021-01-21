//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MOD_H
#define MODLOADER_MOD_H

#include <ModLoader/ModLoader.h>
#include <ModLoader/Logger.h>

#if defined(MODLOADER_WINDOWS)

#   include <ModLoader/Windows/SharedLibrary.h>

#else
#   error "No implementation for SharedLibrary provided"
#endif

namespace GoMint {

    class ModLoader;

    class Mod {

    public:

        static std::unique_ptr<Mod> loadModFromLibrary(ModLoader* loader, const std::string& file);

        ~Mod();

        const std::string& getLongName() const;
        const std::string& getShortName() const;
        const std::string& getAuthor() const;
        const std::string& getDescription() const;

        ModLoadResult load(IModLoader* loader);
        void unload(IModLoader* loader);


    private:

        typedef void          (* GetModInfoFunc)(ModInformation*);
        typedef ModLoadResult (* OnModLoadFunc)(IModLoader*, ILogger*);
        typedef void          (* OnModUnloadFunc)(IModLoader*);

        Mod(SharedLibraryPtr library,
            GetModInfoFunc getModInfo,
            OnModLoadFunc onModLoad,
            OnModUnloadFunc onModUnloadFunc);

        void initialize(ModLoader* loader);

        SharedLibraryPtr m_library;
        LoggerPtr        m_logger;

        std::string m_longName;
        std::string m_shortName;
        std::string m_author;
        std::string m_description;

        GetModInfoFunc  m_getModInfo;
        OnModLoadFunc   m_onModLoad;
        OnModUnloadFunc m_onModUnload;

        bool m_loaded;

    };

    using ModPtr = std::unique_ptr<Mod>;

}

#endif //MODLOADER_MOD_H
