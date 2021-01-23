//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "Symbol.h"

#include <utility>

namespace GoMint {

    Symbol::Symbol(std::string  name, std::string  lookup)
        : m_name{std::move(name)},
          m_lookup{std::move(lookup)},
          m_addressOffset{0} {

    }

    const std::string & Symbol::getName() const {
        return m_name;
    }

    const std::string & Symbol::getLookup() const {
        return m_lookup;
    }

    bool Symbol::hasAddressOffset() const {
        return m_addressOffset != 0;
    }

    std::uint64_t Symbol::getAddressOffset() const {
        return m_addressOffset;
    }

    void Symbol::setAddressOffset(std::uint64_t offset) {
        m_addressOffset = offset;
    }

}