//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "Type.h"

namespace GoMint {

    Type::Type(std::string name)
        : m_name{std::move(name)},
          m_size{UNKNOWN_SIZE},
          m_alignment{UNKNOWN_ALIGNMENT},
          m_inheritance{TypeInheritance::Invalid} {

    }

    const std::string& Type::getName() const {
        return m_name;
    }

    bool Type::hasSize() const {
        return m_size != UNKNOWN_SIZE;
    }

    std::uint64_t Type::getSize() const {
        return m_size;
    }

    bool Type::hasAlignment() const {
        return m_alignment != UNKNOWN_ALIGNMENT;
    }

    std::uint64_t Type::getAlignment() const {
        return m_alignment;
    }

    TypeInheritance Type::getInheritance() const {
        return m_inheritance;
    }

}
