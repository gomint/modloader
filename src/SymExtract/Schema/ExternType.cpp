//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "ExternType.h"

namespace GoMint {

    ExternType::ExternType(std::string name, std::uint64_t size)
            : m_name{std::move(name)},
              m_size{size} {

    }

    const std::string& ExternType::getName() const {
        return m_name;
    }

    bool ExternType::hasSize() const {
        return true;
    }

    std::uint64_t ExternType::getSize() const {
        return m_size;
    }

}