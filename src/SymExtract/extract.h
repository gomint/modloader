//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_EXTRACT_H
#define MODLOADER_EXTRACT_H

#include "Generator/DefinitionTable.h"

namespace GoMint {

    bool extractSymbols(DefinitionTable& definitions, const char* file);

}

#endif //MODLOADER_EXTRACT_H
