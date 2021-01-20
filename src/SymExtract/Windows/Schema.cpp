//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "../Schema/Schema.h"

namespace GoMint {

    void Schema::generateNativeIncludes(FileWriter& writer) {
        writer.newline() << "#include <Windows.h>";
    }

    void Schema::generateBaseAddressLoader(FileWriter& writer) {
        writer.newline() << "std::uintptr_t getBaseAddress() {";
        writer.increaseIndent();

        writer.newline() << "HMODULE hModule = GetModuleHandle(NULL);";
        writer.newline() << "if (hModule == 0) return 0;";
        writer.newline() << "else return (std::uintptr_t) hModule;";

        writer.decreaseIndent().newline() << "}";
    }

}