//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <ModLoader/Windows/DebuggerImpl.h>

#include <Windows.h>

namespace GoMint::Native {

    void DebuggerImpl::waitForDebugger() {
        while (IsDebuggerPresent() != TRUE) {
            Sleep(1000);
        }
    }

    void DebuggerImpl::breakpoint() {
        DebugBreak();
    }

}