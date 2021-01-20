//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTION_H
#define MODLOADER_FUNCTION_H

#include "FunctionSymbol.h"
#include <string>
#include <nlohmann/json.hpp>

namespace GoMint {

    class Schema;

    struct Function {

        std::string m_name;
        std::string m_symbolName;

        FunctionSymbol* m_symbol;

        bool resolveSymbols(Schema* schema);

    };

    void to_json(nlohmann::json& j, const GoMint::Function& p);
    void from_json(const nlohmann::json& j, GoMint::Function& p);

}

#endif //MODLOADER_FUNCTION_H
