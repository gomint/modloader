//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <unordered_map>
#include <fstream>

using namespace std;

namespace GoMint {
    void generatePlatformIncludes(char* indent, ofstream& out) {
        out << indent << "#include <Windows.h>" << endl;
    }

    void generatePlatformLoader(char* indent, ofstream& out) {
        out << indent << "std::uintptr_t loadBaseAddress() {" << endl;
        out << indent << "\tHMODULE hModule = GetModuleHandle(NULL);" << endl;
        out << indent << "\tif (hModule == NULL) return 0;" << endl;
        out << indent << "\telse return (std::uintptr_t) hModule;" << endl;
        out << indent << "}" << endl;
    }
}