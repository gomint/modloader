//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <cstdio>
#include <unordered_map>
#include <fstream>

#include "../Symbol.h"

bool generateLoader(std::unordered_map<std::string, Symbol>& symbols, const char* outputFile) {
    std::string file = std::string(outputFile) + ".cpp";
    std::ofstream outfile(file);
    if (!outfile) {
        printf("Could not open implementation file for generation\n");
        return false;
    }

    outfile << "#include \"Symbols.h\"\n";
    outfile << "#include <Windows.h>\n";
    outfile << "namespace GoMint { namespace Symbols {\n\n";

    for (auto& it : symbols) {
        Symbol& symbol = it.second;

        outfile << "\t" << symbol.m_pointerType << " " << symbol.m_pointerVar << " = nullptr;\n";
    }
    outfile << "\n";

    outfile << "\tbool loadSymbols() {\n";
    outfile << "\t\tHMODULE hModule = GetModuleHandle(NULL);\n";
    outfile << "\t\tif (hModule == NULL) return false;\n";
    outfile << "\n";
    outfile << "\t\tUINT_PTR uBaseAddr = (UINT_PTR) hModule;\n";
    outfile << "\n";

    for (auto& it : symbols) {
        Symbol& symbol = it.second;

        outfile << "\t\t" << symbol.m_pointerVar << " = " << "(" << symbol.m_pointerType << ")(uBaseAddr + " << symbol.m_addressOffset << "ULL);\n";
    }

    outfile << "\n";
    outfile << "\t\treturn true;\n";
    outfile << "\t}\n\n";

    outfile << "} }\n";

    return true;
}