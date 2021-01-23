//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTION_H
#define MODLOADER_FUNCTION_H

#include "FunctionSignature.h"
#include "FunctionResolver.h"

namespace GoMint {

    /**
     * A structure describing a function consisting of its function signature
     * and a way of resolving it at runtime.
     */
    class Function {

    public:

        Function(FunctionSignature signature, FunctionResolverPtr resolver);

        [[nodiscard]] const FunctionSignature& getSignature() const;
        [[nodiscard]] const FunctionResolver* getResolver() const;

    private:

        FunctionSignature   m_signature;
        FunctionResolverPtr m_resolver;

    };

}

#endif //MODLOADER_FUNCTION_H
