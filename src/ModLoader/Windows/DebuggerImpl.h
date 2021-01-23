//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_DEBUGGERIMPL_H
#define MODLOADER_DEBUGGERIMPL_H

namespace GoMint::Native {

    class DebuggerImpl {

    public:

        static void waitForDebugger();
        static void breakpoint();

    };

}

#endif //MODLOADER_DEBUGGERIMPL_H
