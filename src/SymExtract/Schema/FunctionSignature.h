//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTIONSIGNATURE_H
#define MODLOADER_FUNCTIONSIGNATURE_H

#include "FunctionArgument.h"
#include <vector>

namespace GoMint {

    /**
     * Denotes a full function signature (name, return type and arguments).
     */
    struct FunctionSignature {

        std::string m_name;
        std::string m_returnType;
        std::vector<FunctionArgument> m_arguments;

    };

}

#endif //MODLOADER_FUNCTIONSIGNATURE_H
