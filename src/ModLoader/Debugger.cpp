//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <ModLoader/Debugger.h>
#include <ModLoader/ModLoader.h>

#if defined(MODLOADER_WINDOWS)
#   include <ModLoader/Windows/DebuggerImpl.h>
#else
#   error "No implementation for Debugger provided"
#endif

namespace GoMint {

    void Debugger::waitForDebugger() {
        Native::DebuggerImpl::waitForDebugger();
    }

    void Debugger::breakpoint() {
        Native::DebuggerImpl::breakpoint();
    }

}