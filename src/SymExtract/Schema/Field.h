//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FIELD_H
#define MODLOADER_FIELD_H

#include "TypeDescriptor.h"

namespace GoMint {

    /**
     * Description of a field within a class.
     */
    class Field {

    public:

        static constexpr std::uint64_t UNKNOWN_OFFSET = static_cast<std::uint64_t>(-1);

        Field(TypeDescriptor* type, std::string  name, std::uint64_t offset = UNKNOWN_OFFSET);

        /** The field's type ; can be a custom defined type or a pre-defined type */
        [[nodiscard]] TypeDescriptor* getType() const;
        /** The field's name */
        [[nodiscard]] const std::string& getName() const;
        /** Whether or not the field's offset has been specified absolutely */
        [[nodiscard]] bool hasOffset() const;
        /** The field's offset within the containing class's memory layout */
        [[nodiscard]] std::uint64_t getOffset() const;

    private:

        TypeDescriptor* m_type;
        std::string   m_name;
        std::uint64_t m_offset;

    };

}

#endif //MODLOADER_FIELD_H
