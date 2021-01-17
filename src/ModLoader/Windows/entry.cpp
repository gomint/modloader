//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <Windows.h>

#include "../ModLoaderEntry.h"

BOOL WINAPI DllMain(HINSTANCE hInstanceDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    return GoMint::ModLoaderEntry::initialize() ? TRUE : FALSE;
}