//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "Field.h"

#include <utility>
#include "Schema.h"

namespace GoMint {

    Field::Field(TypeDescriptor* type, std::string  name, std::uint64_t offset)
            : m_type{type},
              m_name{std::move(name)},
              m_offset{offset} {

    }

    TypeDescriptor* Field::getType() const {
        return m_type;
    }

    const std::string& Field::getName() const {
        return m_name;
    }

    bool Field::hasOffset() const {
        return m_offset != UNKNOWN_OFFSET;
    }

    std::uint64_t Field::getOffset() const {
        return m_offset;
    }

}