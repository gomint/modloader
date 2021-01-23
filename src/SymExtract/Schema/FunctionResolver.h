//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTIONRESOLVER_H
#define MODLOADER_FUNCTIONRESOLVER_H

#include "FunctionResolution.h"

#include "../FileWriter.h"

#include <memory>

namespace GoMint {

    class Function;
    class Type;

    /**
     * A function resolver resolves a function's address at runtime.
     */
    class FunctionResolver {

    public:

        virtual ~FunctionResolver() = default;

        /** Gets the type of resolution performed by this resolver */
        [[nodiscard]] virtual FunctionResolution getResolution() const = 0;

        /** Checks whether or not this function resolver will output a function pointer for resolved functions */
        [[nodiscard]] virtual bool hasFunctionPointer() const = 0;

        [[nodiscard]] virtual bool generateFunctionInvocation(FileWriter& writer,
                                                              Type* type,
                                                              Function* function,
                                                              const std::string& pointerType,
                                                              const std::string& pointerName,
                                                              bool isMember) const = 0;

        [[nodiscard]] virtual bool generateFunctionPointerLoader(FileWriter& writer,
                                                                 const std::string& baseaddrVar,
                                                                 Type* type,
                                                                 Function* function,
                                                                 const std::string& pointerType,
                                                                 const std::string& pointerName,
                                                                 bool isMember) const = 0;

    };

    using FunctionResolverPtr = std::unique_ptr<FunctionResolver>;

}

#endif //MODLOADER_FUNCTIONRESOLVER_H
