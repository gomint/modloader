//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_DEBUGGER_H
#define MODLOADER_DEBUGGER_H

namespace GoMint {

    class Debugger {

    public:

        static void waitForDebugger();
        static void breakpoint();

    };

}

#endif //MODLOADER_DEBUGGER_H
