//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "../Symbol.h"

#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <vector>

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

HANDLE g_hProcess;
DWORD64 g_dwBaseOffset = 0x100000ULL;
std::vector<CHAR> g_nameBuffer;

bool initializeDbgHelp() {
    DWORD options = SymGetOptions();
    options &= ~SYMOPT_DEFERRED_LOADS;      // Disable lazy loading: we need all symbols at once
    options |= SYMOPT_IGNORE_NT_SYMPATH;    // Ignore other symbol files
    options |= SYMOPT_UNDNAME;              // Undecorate function names
    SymSetOptions(options);

    g_hProcess = GetCurrentProcess();
    BOOL result = SymInitialize(g_hProcess, NULL, FALSE);
    return result == TRUE;
}

bool loadInputModule(const char* file) {
    DWORD64 baseAddress = SymLoadModuleEx(g_hProcess, NULL, file, NULL, g_dwBaseOffset, 0x7FFFFFFFUL, NULL, 0);
    if (baseAddress == 0 && GetLastError() != ERROR_SUCCESS) {
        return false;
    }
    g_dwBaseOffset = baseAddress;
    return true;
}

BOOL extractSymbolsEnumerationCallback(PSYMBOL_INFO pSymInfo, ULONG symbolSize, PVOID userContext) {
    (void) symbolSize;

    std::unordered_map<std::string, Symbol>& symbols = *((std::unordered_map<std::string, Symbol>*) userContext);

    // Retrieve symbol name
    if (pSymInfo->MaxNameLen == 0) {
        // No name specified
        return TRUE;
    }

    g_nameBuffer.resize(pSymInfo->NameLen);
    std::memcpy(&g_nameBuffer[0], pSymInfo->Name, pSymInfo->NameLen * sizeof(CHAR));
    std::string symbolName(&g_nameBuffer[0], pSymInfo->NameLen);

    auto find = symbols.find(symbolName);
    if (find == symbols.end()) {
        // Not interested in this symbol
        return TRUE;
    }

    find->second.m_addressOffset = pSymInfo->Address - pSymInfo->ModBase;
    return TRUE;
}

bool extractSymbols(std::unordered_map<std::string, Symbol>& symbols) {
    SymEnumSymbols(g_hProcess, g_dwBaseOffset, "*", extractSymbolsEnumerationCallback, &symbols);
    return true;
}

bool unloadInputModule() {
    return SymUnloadModule64(g_hProcess, g_dwBaseOffset) == TRUE;
}

bool extractSymbols(std::unordered_map<std::string, Symbol>& symbols, const char* inputFile) {
    if (!initializeDbgHelp()) {
        printf("Failed to initialize DbgHelp library\n");
        return false;
    }

    if (!loadInputModule(inputFile)) {
        printf("Failed to load input module\n");
        return false;
    }

    if (!extractSymbols(symbols)) {
        printf("Could not enumerate symbols\n");
        return false;
    }

    if (!unloadInputModule()) {
        printf("Failed to unload input module\n");
        return false;
    }

    return true;
}