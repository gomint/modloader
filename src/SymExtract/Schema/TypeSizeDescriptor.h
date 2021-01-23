//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_TYPESIZEDESCRIPTOR_H
#define MODLOADER_TYPESIZEDESCRIPTOR_H

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>

namespace GoMint {

    class TypeSizeDescriptor {

    public:

        virtual ~TypeSizeDescriptor() = default;
        virtual const std::string& getName() = 0;
        virtual std::uint64_t getSize() = 0;

    };

    struct TypeSizeDeclaration : public TypeSizeDescriptor {

        std::string m_name;
        std::uint64_t m_size;

        TypeSizeDeclaration(const std::string& name, std::uint64_t size);

        ~TypeSizeDeclaration() override = default;
        const std::string& getName() override;
        uint64_t getSize() override;

    };

}

#endif //MODLOADER_TYPESIZEDESCRIPTOR_H
