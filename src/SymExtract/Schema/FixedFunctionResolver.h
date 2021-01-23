//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FIXEDFUNCTIONRESOLVER_H
#define MODLOADER_FIXEDFUNCTIONRESOLVER_H

#include "FunctionResolver.h"
#include "Symbol.h"

#include <variant>

namespace GoMint {

    /**
     * Implements fixed function resolution scheme. The function's address offset is known statically
     * either by hardcoding it in the schema or referencing an extracted symbol and its address can therefore
     * be discovered at runtime by adding the module's base address to the known offset.
     */
    class FixedFunctionResolver : public FunctionResolver {

    public:

        using AddressOffsetReference = std::variant<std::uint64_t, Symbol*>;

        explicit FixedFunctionResolver(AddressOffsetReference address);
        ~FixedFunctionResolver() override = default;

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

        AddressOffsetReference m_address;

    };

}

#endif //MODLOADER_FIXEDFUNCTIONRESOLVER_H
