//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOLFUNCTIONDECL_H
#define MODLOADER_SYMBOLFUNCTIONDECL_H

#include "SymbolDecl.h"

namespace GoMint {

    struct SymbolFunctionDecl : public SymbolDecl {

    public:

        std::string m_returnType;
        std::vector<std::string> m_argumentTypes;

    };

    void to_json(nlohmann::json& j, const GoMint::SymbolFunctionDecl& p);
    void from_json(const nlohmann::json& j, GoMint::SymbolFunctionDecl& p);

}

#endif //MODLOADER_SYMBOLFUNCTIONDECL_H
