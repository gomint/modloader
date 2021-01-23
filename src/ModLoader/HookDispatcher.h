//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_HOOKDISPATCHER_H
#define MODLOADER_HOOKDISPATCHER_H

#include <SymExtract/Helpers.h>
#include <funchook.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>

namespace GoMint {

    typedef std::unique_ptr<funchook_t, std::function<int (funchook_t*)>> FunchookPtr;

    template<typename R, typename ... Args>
    struct FunctionPointerTypeHelper {
        using Type = R (*)(Args...);
    };

    template<typename C, typename R, typename ... Args>
    struct MemberFunctionPointerTypeHelper {
        using InvocableType = R (SymExtract::SingleInvocable::*)(Args...);
        using Type = R (C::*)(Args...);
    };

    class HookDispatcher {

    public:

        HookDispatcher();

        template<typename Func>
        bool registerHook(
                Func original,
                Func target,
                Func& result
        ) {
            std::uintptr_t address = registerHook(
                    reinterpret_cast<std::uintptr_t>(original),
                    reinterpret_cast<std::uintptr_t>(target)
            );
            if (address == 0) return false;
            result = reinterpret_cast<Func>(address);
            return true;
        }

        template<typename Bound, typename Free>
        bool registerMemberHook(
                Bound original,
                Free target,
                Bound& result
        ) {
            std::uintptr_t address = registerHook(
                    SymExtract::getMemfnAddress(original),
                    SymExtract::getMemfnAddress(target)
            );
            if (address == 0) return false;
            result = SymExtract::makeMemfnPtr<Bound>(address);
            return true;
        }


        bool install();

    private:

        bool m_installed;
        FunchookPtr m_hooks;

        std::uintptr_t registerHook(std::uintptr_t originalAddress, std::uintptr_t targetAddress);

    };

}

#endif //MODLOADER_HOOKDISPATCHER_H
