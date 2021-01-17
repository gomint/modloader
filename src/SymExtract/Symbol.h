//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SYMBOL_H
#define MODLOADER_SYMBOL_H

#include <cstdint>
#include <string>
#include <utility>

struct Symbol {

    Symbol(std::string lookupName,
           std::string pointerTypeDecl,
           std::string pointerType,
           std::string pointerVar) :
           m_lookupName{std::move(lookupName)},
           m_pointerTypeDecl{std::move(pointerTypeDecl)},
           m_pointerType{std::move(pointerType)},
           m_pointerVar{std::move(pointerVar)} {

    }

    // Defined in symbol table
    std::string m_lookupName;
    std::string m_pointerTypeDecl;
    std::string m_pointerType;
    std::string m_pointerVar;

    // To be extracted from image
    std::uintptr_t m_addressOffset = 0;

};


#endif //MODLOADER_SYMBOL_H
