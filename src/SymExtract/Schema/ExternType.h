//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_EXTERNTYPE_H
#define MODLOADER_EXTERNTYPE_H

#include "TypeDescriptor.h"

namespace GoMint {

    /**
     * Simple descriptor structure that forwards constants into the TypeDescriptor interface.
     */
    class ExternType : public TypeDescriptor {

    public:

        ExternType(std::string name, std::uint64_t size);

        const std::string& getName() const override;
        bool hasSize() const override;
        std::uint64_t getSize() const override;

    private:

        std::string   m_name;
        std::uint64_t m_size;

    };

}

#endif //MODLOADER_EXTERNTYPE_H
