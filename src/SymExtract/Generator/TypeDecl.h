//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_TYPEDECL_H
#define MODLOADER_TYPEDECL_H

#include "MemberFunctionDecl.h"

#include <vector>

namespace GoMint {

    /**
     * Declaration of a type which may have members.
     *
     * @author Ciel DeVille
     * @verison 1.0
     * @stability experimental
     */
    struct TypeDecl {

        std::string m_name;
        std::string m_type;
        std::vector<MemberFunctionDecl> m_memberFunctions;

    };

    void to_json(nlohmann::json& j, const GoMint::TypeDecl& p);
    void from_json(const nlohmann::json& j, GoMint::TypeDecl& p);

}

#endif //MODLOADER_TYPEDECL_H
