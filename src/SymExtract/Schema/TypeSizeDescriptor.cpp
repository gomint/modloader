//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "TypeSizeDescriptor.h"

namespace GoMint {

    TypeSizeDeclaration::TypeSizeDeclaration(const std::string& name, std::uint64_t size)
        : m_name{name}, m_size{size} {

    }

    const std::string& TypeSizeDeclaration::getName() {
        return m_name;
    }

    std::uint64_t TypeSizeDeclaration::getSize() {
        return m_size;
    }

}