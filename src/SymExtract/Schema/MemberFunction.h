//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_MEMBERFUNCTION_H
#define MODLOADER_MEMBERFUNCTION_H

#include "MemberFunctionSymbol.h"

#include <nlohmann/json.hpp>
#include <string>

namespace GoMint {

    class Schema;

    struct MemberFunction {

        std::string m_name;
        std::string m_symbolName;

        MemberFunctionSymbol* m_symbol;

        bool resolveSymbol(Schema* schema);

    };

    void to_json(nlohmann::json& j, const GoMint::MemberFunction& p);
    void from_json(const nlohmann::json& j, GoMint::MemberFunction& p);

}

#endif //MODLOADER_MEMBERFUNCTION_H
