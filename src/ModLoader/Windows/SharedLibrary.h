//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SHAREDLIBRARY_H
#define MODLOADER_SHAREDLIBRARY_H

#include <memory>
#include <string>

namespace GoMint {

    class SharedLibrary {

    public:

        static std::unique_ptr<SharedLibrary> load(const std::string& file);

        ~SharedLibrary();
        void* getProcedureAddress(const std::string& name);

    private:

        SharedLibrary(void* module);

        void* m_module;

    };

    using SharedLibraryPtr = std::unique_ptr<SharedLibrary>;

}

#endif //MODLOADER_SHAREDLIBRARY_H
