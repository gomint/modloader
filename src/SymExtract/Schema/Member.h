//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MEMBER_H
#define MODLOADER_MEMBER_H

#include <string>
#include <nlohmann/json.hpp>

namespace GoMint {

    class Schema;

    struct Member {

    public:

        static constexpr std::uint64_t INVALID_OFFSET = static_cast<std::uint64_t>(-1LL);
        static constexpr std::uint64_t INVALID_SIZE = static_cast<std::uint64_t>(-1LL);

        std::string   m_type;
        std::string   m_name;
        std::uint64_t m_offset;

        std::uint64_t m_size;
        std::string   m_sizeDescriptor;

        bool resolveSize(Schema* schema);

    };

    void to_json(nlohmann::json& j, const Member& p);
    void from_json(const nlohmann::json& j, Member& p);

}

#endif //MODLOADER_MEMBER_H
