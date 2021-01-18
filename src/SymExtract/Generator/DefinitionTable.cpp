//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "DefinitionTable.h"

#include "SymbolFunctionDecl.h"
#include "SymbolMemberFunctionDecl.h"

namespace nlohmann {
    template<>
    struct adl_serializer<std::unique_ptr<GoMint::SymbolDecl>> {
        static void to_json(json& j, const std::unique_ptr<GoMint::SymbolDecl>& opt) {
            GoMint::SymbolDecl* p = opt.get();
            if (p != nullptr) {
                switch (p->m_type) {
                    case GoMint::SymbolType::Function:
                        j = *reinterpret_cast<GoMint::SymbolFunctionDecl*>(p);
                        break;
                    case GoMint::SymbolType::MemberFunction:
                        j = *reinterpret_cast<GoMint::SymbolMemberFunctionDecl*>(p);
                        break;
                }
            } else {
                j = nullptr;
            }
        }

        static std::unique_ptr<GoMint::SymbolDecl> from_json(const json& j) {
            GoMint::SymbolType type;

            j.at("type").get_to(type);
            switch (type) {
                case GoMint::SymbolType::Function: {
                    auto* decl = new GoMint::SymbolFunctionDecl;
                    j.get_to(*decl);
                    return std::unique_ptr<GoMint::SymbolDecl>(decl);
                }
                case GoMint::SymbolType::MemberFunction: {
                    auto* decl = new GoMint::SymbolMemberFunctionDecl;
                    j.get_to(*decl);
                    return std::unique_ptr<GoMint::SymbolDecl>(decl);
                }

                default:
                    return nullptr;
            }
        }
    };
}

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::DefinitionTable& p) {
        j["forward"] = p.m_forwardDecls;
        j["types"] = p.m_typeDecls;
        j["symbols"] = p.m_symbolDecls;
    }

    void from_json(const nlohmann::json& j, GoMint::DefinitionTable& p) {
        j.at("forward").get_to(p.m_forwardDecls);
        j.at("types").get_to(p.m_typeDecls);
        j.at("symbols").get_to(p.m_symbolDecls);

        p.m_symbolDeclsByName.clear();
        for (auto& symbol : p.m_symbolDecls) {
            p.m_symbolDeclsByName.insert(std::make_pair(symbol->m_name, symbol.get()));
        }
    }

}