//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOL_H
#define MODLOADER_SYMBOL_H

#include "../FileWriter.h"

#include <string>
#include <vector>

namespace GoMint {

    /**
     * Declaration of a symbol to be extracted.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    class Symbol {

    public:

        Symbol(std::string  name, std::string  lookup);
        virtual ~Symbol() = default;

        const std::string& getName() const;
        const std::string& getLookup() const;

        bool hasAddressOffset() const;
        std::uint64_t getAddressOffset() const;
        void setAddressOffset(std::uint64_t offset);

    private:

        const std::string m_name;
        const std::string m_lookup;

        // Extracted information:
        std::uintptr_t m_addressOffset = 0ULL;

    };

}

#endif //MODLOADER_SYMBOL_H
