//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_TYPEDESCRIPTOR_H
#define MODLOADER_TYPEDESCRIPTOR_H

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>

namespace GoMint {

    /**
     * A descriptor for types. It provides meta-information about a type such as its name and size.
     */
    class TypeDescriptor {

    public:

        virtual ~TypeDescriptor() = default;

        /** @return The type's name */
        virtual const std::string& getName() const = 0;
        /** @return Whether or not the type's size is known */
        virtual bool hasSize() const = 0;
        /** @return The type's size in bytes if known */
        virtual std::uint64_t getSize() const = 0;

    };

}

#endif //MODLOADER_TYPEDESCRIPTOR_H
