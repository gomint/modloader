//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "MemberFunction.h"
#include "Schema.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::MemberFunction& p) {
        j["name"] = p.m_name;
        j["symbol"] = p.m_symbolName;
    }

    void from_json(const nlohmann::json& j, GoMint::MemberFunction& p) {
        j.at("name").get_to(p.m_name);
        j.at("symbol").get_to(p.m_symbolName);
    }

    bool MemberFunction::resolveSymbol(Schema* schema) {
        return (m_symbol = reinterpret_cast<MemberFunctionSymbol*>(schema->findSymbolByName(m_symbolName))) != nullptr &&
                m_symbol->m_type == SymbolType::MemberFunction;
    }
}