//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_DEFINITIONTABLE_H
#define MODLOADER_DEFINITIONTABLE_H

#include "ForwardDecl.h"
#include "SymbolDecl.h"
#include "TypeDecl.h"

#include <memory>

namespace GoMint {

    /**
     * Table containing all definitions loaded from a symbol table file.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    struct DefinitionTable {

        std::vector<std::string> m_includes;
        std::vector<ForwardDecl> m_forwardDecls;
        std::vector<TypeDecl> m_typeDecls;
        std::vector<std::unique_ptr<SymbolDecl>> m_symbolDecls;
        std::unordered_map<std::string, SymbolDecl*> m_symbolDeclsByName;

    };

    void to_json(nlohmann::json& j, const GoMint::DefinitionTable& p);
    void from_json(const nlohmann::json& j, GoMint::DefinitionTable& p);

}

#endif //MODLOADER_DEFINITIONTABLE_H
