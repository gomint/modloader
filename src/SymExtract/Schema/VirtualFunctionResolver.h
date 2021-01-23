//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_VIRTUALFUNCTIONRESOLVER_H
#define MODLOADER_VIRTUALFUNCTIONRESOLVER_H

#include "FunctionResolver.h"

namespace GoMint {

    /**
     * Implements the virtual function resolution scheme. At runtime a lookup into a class's vtable will be
     * performed to retrieve the correct function address.
     */
    class VirtualFunctionResolver : public FunctionResolver {

    public:

        explicit VirtualFunctionResolver(std::uint64_t vindex);
        ~VirtualFunctionResolver() override = default;

        [[nodiscard]] FunctionResolution getResolution() const override;
        [[nodiscard]] bool hasFunctionPointer() const override;
        [[nodiscard]] bool generateFunctionInvocation(FileWriter& writer,
                                                      Type* type,
                                                      Function* function,
                                                      const std::string& pointerType,
                                                      const std::string& pointerName,
                                                      bool isMember) const override;
        [[nodiscard]] bool generateFunctionPointerLoader(FileWriter& writer,
                                                         const std::string& baseaddrVar,
                                                         Type* type,
                                                         Function* function,
                                                         const std::string& pointerType,
                                                         const std::string& pointerName,
                                                         bool isMember) const override;

    private:

        std::uint64_t m_vindex;

    };

}

#endif //MODLOADER_VIRTUALFUNCTIONRESOLVER_H
