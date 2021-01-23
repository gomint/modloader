//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "Member.h"
#include "Schema.h"

namespace GoMint {

    bool Member::resolveSize(Schema* schema) {
        if (m_size == Member::INVALID_SIZE) {
            const std::string& sizeRef = (m_sizeDescriptor.empty()) ? m_type : m_sizeDescriptor;
            auto* descriptor = schema->findTypeSizeByName(sizeRef);
            if (descriptor != nullptr) {
                m_size = descriptor->getSize();
            }
        }
        return m_size != Member::INVALID_SIZE;
    }

    void to_json(nlohmann::json& j, const Member& p) {
        j["name"] = p.m_name;
        j["type"] = p.m_type;
        if (p.m_offset != Member::INVALID_OFFSET) j["offset"] = p.m_offset;

        if (p.m_sizeDescriptor == "~") {
            j["size"] = p.m_size;
        } else if (!p.m_sizeDescriptor.empty()) {
            j["size"] = p.m_sizeDescriptor;
        }
    }

    void from_json(const nlohmann::json& j, Member& p) {
        j.at("name").get_to(p.m_name);
        j.at("type").get_to(p.m_type);
        if (j.contains("offset")) {
            j.at("offset").get_to(p.m_offset);
        } else {
            p.m_offset = Member::INVALID_OFFSET;
        }

        p.m_size = Member::INVALID_SIZE;
        p.m_sizeDescriptor.clear();

        if (j.contains("size")) {
            auto& size = j.at("size");
            if (size.is_number_unsigned()) {
                size.get_to(p.m_size);
                p.m_sizeDescriptor = "~";
            } else if (size.is_string()) {
                size.get_to(p.m_sizeDescriptor);
            }
        }
    }

}