//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "extract.h"
#include "Generator/Generator.h"

#include "SymExtract/Helpers.h"

#include <cstdio>
#include <fstream>
#include <unordered_map>

using namespace GoMint;

const char* g_usage =
        "symextract - extract symbols from .pdb and executable files"           "\n"
        "             and generate glue code to access them."                   "\n"
        "  Ciel DeVille (ciel.dev [at] pm.me)"                                  "\n"
        "\n"
        "    Usage: symextract <table> <input> <output>"                        "\n"
        "      <table>: A .csv file which contains all symbols to extract"      "\n"
        "      <input>: Input file (.pdb or .exe on Windows)"                   "\n"
        "      <output>: The name and path without extension"                   "\n"
        "\n"
        "  The specified table file should contain four columns and no"         "\n"
        "  superfluous whitespace. The first column should specify the"         "\n"
        "  symbols name, the second the symbol's function pointer declaration." "\n"
        "  The third column should specify the name of the function pointer"    "\n"
        "  type and the last column should specify the name of the variable"    "\n"
        "  into which the function address will be loaded."                     "\n"
        "\n"
        "  Example: symextract symbol_table.csv main.exe symbols.cpp"           "\n";

bool readDefinitionTable(DefinitionTable& definitions, const char* file) {
    std::ifstream infile(file);
    if (!infile) {
        return false;
    }

    nlohmann::json j;
    infile >> j;

    definitions = j.get<GoMint::DefinitionTable>();
    return true;
}

bool checkSymbolAddresses(const DefinitionTable& definitions) {
    bool foundAllAddresses = true;

    for (auto& it : definitions.m_symbolDeclsByName) {
        SymbolDecl& symbol = *(it.second);

        if (symbol.m_addressOffset == 0) {
            foundAllAddresses = false;
            printf("%p: %s (missing)\n", (void*) symbol.m_addressOffset, symbol.m_name.c_str());
        } else {
            printf("%p: %s (found)\n", (void*) symbol.m_addressOffset, symbol.m_name.c_str());
        }
    }

    return foundAllAddresses;
}

//bool generateHeader(const char* file) {
//    std::string headerFile = std::string(file) + ".h";
//    std::ofstream outfile(headerFile);
//    if (!outfile) {
//        printf("Could not open header file for generation\n");
//        return false;
//    }
//
//    outfile << "#ifndef __SYMEXTRACT_GENERATED_HEADER__\n";
//    outfile << "#define __SYMEXTRACT_GENERATED_HEADER__\n\n";
//
//    outfile << "namespace GoMint { namespace Symbols {\n\n";
//
//    for (auto& it : g_symbolTable) {
//        Symbol& symbol = it.second;
//
//        outfile << "\ttypedef " << symbol.m_pointerTypeDecl << ";\n";
//        outfile << "\textern " << symbol.m_pointerType << " " << symbol.m_pointerVar << ";\n";
//        outfile << "\n";
//    }
//
//    outfile << "\tbool loadSymbols();\n\n";
//
//    outfile << "} }\n\n";
//
//    outfile << "#endif // __SYMEXTRACT_GENERATED_HEADER__\n";
//
//    return true;
//}
//
//bool generateGlueCode(const char* file) {
//    return generateHeader(file) && generateLoader(g_symbolTable, file);
//}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("%s", g_usage);
        return 1;
    }

    const char* symbolTablePath = argv[1];
    const char* inputPath = argv[2];
    const char* outputPath = argv[3];

    DefinitionTable definitions;

    if (!readDefinitionTable(definitions, symbolTablePath)) {
        printf("Failed to read definition table\n");
        return 1;
    }

    if (!extractSymbols(definitions, inputPath)) {
        printf("Failed to extract symbols\n");
        return 1;
    }

    if (!checkSymbolAddresses(definitions)) {
        printf("Could not resolve all symbol addresses\n");
        return 1;
    }

    Generator generator;
    std::string baseFile(outputPath);
    if (!generator.run(definitions, baseFile + ".h", baseFile + ".cpp")) {
        printf("Glue code generation failed\n");
        return 1;
    }

    printf("Generation complete\n");
    return 0;
}