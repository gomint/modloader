//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_TYPE_H
#define MODLOADER_TYPE_H

#include "Function.h"
#include "Member.h"
#include "MemberFunction.h"
#include "TypeSizeDescriptor.h"

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
    struct Type : public TypeSizeDescriptor {

        static constexpr std::uint64_t UNKNOWN_SIZE = static_cast<std::uint64_t>(-1LL);
        static constexpr std::uint64_t UNKNOWN_ALIGNMENT = static_cast<std::uint64_t>(-1LL);

        std::string m_name;
        std::uint64_t m_size;
        std::uint64_t m_alignment;

        std::vector<Member> m_members;
        std::vector<Function> m_functions;
        std::vector<MemberFunction> m_memberFunctions;

        ~Type() override = default;
        bool resolveSymbols(Schema* schema);

        const std::string& getName() override;
        std::uint64_t getSize() override;

    };

    void to_json(nlohmann::json& j, const GoMint::Type& p);
    void from_json(const nlohmann::json& j, GoMint::Type& p);

}

#endif //MODLOADER_TYPE_H
