//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "Function.h"
#include "Schema.h"

namespace GoMint {

    Function::Function(FunctionSignature signature, FunctionResolverPtr resolver)
        : m_signature{std::move(signature)},
          m_resolver{std::move(resolver)} {

    }

    const FunctionSignature & Function::getSignature() const {
        return m_signature;
    }

    const FunctionResolver * Function::getResolver() const {
        return m_resolver.get();
    }

}