//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_TYPE_H
#define MODLOADER_TYPE_H

#include "Field.h"
#include "Function.h"
#include "TypeDescriptor.h"
#include "TypeInheritance.h"

#include <vector>

namespace GoMint {

    class Schema;

    /**
     * Declaration of a type which may have members.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    class Type : public TypeDescriptor {

        friend class SchemaFile;

    public:

        explicit Type(std::string name);

        ~Type() override = default;

        // Type Descriptor
        [[nodiscard]] const std::string& getName() const override;

        [[nodiscard]] bool hasSize() const override;
        [[nodiscard]] std::uint64_t getSize() const override;

        [[nodiscard]] bool hasAlignment() const;
        [[nodiscard]] std::uint64_t getAlignment() const;

        [[nodiscard]] TypeInheritance getInheritance() const;

    private:

        static constexpr std::uint64_t UNKNOWN_SIZE      = static_cast<std::uint64_t>(-1LL);
        static constexpr std::uint64_t UNKNOWN_ALIGNMENT = static_cast<std::uint64_t>(-1LL);

        std::string     m_name;
        std::uint64_t   m_size;
        std::uint64_t   m_alignment;
        TypeInheritance m_inheritance;

        std::vector<Field>    m_fields;
        std::vector<Function> m_functions;
        std::vector<Function> m_memberFunctions;


    };

}

#endif //MODLOADER_TYPE_H
