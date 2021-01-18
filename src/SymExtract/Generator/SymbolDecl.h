//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOLDECL_H
#define MODLOADER_SYMBOLDECL_H

#include "SymbolType.h"

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace GoMint {

    /**
     * Declaration of a symbol to be extracted.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    struct SymbolDecl {

        std::string m_name;
        SymbolType m_type = SymbolType::Invalid;
        std::string m_pointerName;
        std::string m_variableName;


        // Extracted information:
        std::uintptr_t m_addressOffset;

    };

    void to_json(nlohmann::json& j, const GoMint::SymbolDecl& p);
    void from_json(const nlohmann::json& j, GoMint::SymbolDecl& p);

}

#endif //MODLOADER_SYMBOLDECL_H
