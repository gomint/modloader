//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/HookDispatcher.h>

namespace GoMint {

    HookDispatcher::HookDispatcher() :
        m_installed{false},
        m_hooks{funchook_create(), &funchook_destroy} {

    }

    std::uintptr_t HookDispatcher::registerHook(std::uintptr_t originalAddress, std::uintptr_t targetAddress) {
        if (m_installed) return 0;

        int retval = funchook_prepare(m_hooks.get(),
                                      reinterpret_cast<void**>(&originalAddress),
                                      reinterpret_cast<void*>(targetAddress));
        if (retval != 0) return 0;
        return originalAddress;
    }

    bool HookDispatcher::install() {
        return funchook_install(m_hooks.get(), 0) == 0;
    }

}