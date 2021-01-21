//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <ModLoader/SharedLibrary.h>
#include <ModLoader/ModLoader.h>

#if defined(MODLOADER_WINDOWS)
#   include <ModLoader/Windows/SharedLibraryImpl.h>
#else
#   error "No implementation provided for SharedLibrary"
#endif

namespace GoMint {

    std::unique_ptr<SharedLibrary> SharedLibrary::load(const std::string& file) {
        Native::SharedLibraryImplPtr impl = Native::SharedLibraryImpl::load(file);
        if (impl == nullptr) return nullptr;
        return SharedLibraryPtr(new SharedLibrary(std::move(impl)));
    }

    SharedLibrary::SharedLibrary(std::unique_ptr<Native::SharedLibraryImpl> impl)
        : m_impl(std::move(impl)) {

    }

    SharedLibrary::~SharedLibrary() = default;

    void * SharedLibrary::getProcedureAddress(const std::string& name) {
        return m_impl->getProcedureAddress(name);
    }

}