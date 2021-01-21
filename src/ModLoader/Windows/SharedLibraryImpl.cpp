//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <ModLoader/Windows/SharedLibraryImpl.h>

#include <Windows.h>

namespace GoMint::Native {

    std::unique_ptr<SharedLibraryImpl> SharedLibraryImpl::load(const std::string& file) {
        HMODULE hModule = LoadLibraryA(file.c_str());
        if (hModule == NULL) {
            return nullptr;
        }
        return std::unique_ptr<SharedLibraryImpl>(new SharedLibraryImpl(hModule));
    }


    void* SharedLibraryImpl::getProcedureAddress(const std::string& name) {
        return (void*) GetProcAddress((HMODULE) m_module, name.c_str());
    }


    SharedLibraryImpl::SharedLibraryImpl(void* module)
        : m_module{module} {

    }

    SharedLibraryImpl::~SharedLibraryImpl() {
        FreeLibrary((HMODULE) m_module);
    }

}