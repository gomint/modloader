//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <ModLoader/Windows/SharedLibrary.h>

#include <Windows.h>

namespace GoMint {

    std::unique_ptr<SharedLibrary> SharedLibrary::load(const std::string& file) {
        HMODULE hModule = LoadLibraryA(file.c_str());
        if (hModule == NULL) {
            return nullptr;
        }
        return std::unique_ptr<SharedLibrary>(new SharedLibrary(hModule));
    }


    void* SharedLibrary::getProcedureAddress(const std::string& name) {
        return (void*) GetProcAddress((HMODULE) m_module, name.c_str());
    }


    SharedLibrary::SharedLibrary(void* module)
        : m_module{module} {

    }

    SharedLibrary::~SharedLibrary() {
        FreeLibrary((HMODULE) m_module);
    }

}