//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FUNCTIONARGUMENT_H
#define MODLOADER_FUNCTIONARGUMENT_H

#include <string>

namespace GoMint {

    struct FunctionArgument {

        std::string m_type;
        std::string m_name;

        FunctionArgument(std::string type, std::string name) : m_type{std::move(type)}, m_name{std::move(name)} {}

    };

}

#endif //MODLOADER_FUNCTIONARGUMENT_H
