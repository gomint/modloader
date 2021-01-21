//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SHAREDLIBRARYIMPL_H
#define MODLOADER_SHAREDLIBRARYIMPL_H

#include <memory>
#include <string>

namespace GoMint::Native {

    class SharedLibraryImpl {

    public:

        static std::unique_ptr<SharedLibraryImpl> load(const std::string& file);

        ~SharedLibraryImpl();
        void* getProcedureAddress(const std::string& name);

    private:

        explicit SharedLibraryImpl(void* module);

        void* m_module;

    };

    using SharedLibraryImplPtr = std::unique_ptr<SharedLibraryImpl>;

}

#endif //MODLOADER_SHAREDLIBRARYIMPL_H
