//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOL_H
#define MODLOADER_SYMBOL_H

#include "../FileWriter.h"
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
    struct Symbol {

        SymbolType m_type;
        std::string m_name;

        std::string m_symbolName;
        std::string m_pointerType;
        std::string m_variableName;

        // Extracted information:
        std::uintptr_t m_addressOffset = 0ULL;

        explicit Symbol(SymbolType type) : m_type{type} {}
        virtual ~Symbol() = default;

        virtual void generatePointerTypeDeclaration(FileWriter& writer) = 0;
        virtual void generateVariableCast(FileWriter& writer, const std::string& expression) = 0;

    };

    void to_json(nlohmann::json& j, const GoMint::Symbol& p);
    void from_json(const nlohmann::json& j, GoMint::Symbol& p);

}

#endif //MODLOADER_SYMBOL_H
