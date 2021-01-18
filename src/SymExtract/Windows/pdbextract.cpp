//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nullptr"

#include "../Generator/DefinitionTable.h"

#include <cstdio>
#include <cstring>
#include <vector>

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

namespace GoMint {

    struct ModuleInfo {
        GoMint::DefinitionTable* m_definitions = NULL;

        HANDLE m_hProcess = NULL;
        DWORD64 m_dwBaseAddress = 0x100000ULL;
        std::vector<CHAR> m_nameBuffer;
    };

    bool initializeDbgHelp(ModuleInfo* module) {
        DWORD options = SymGetOptions();
        options &= ~SYMOPT_DEFERRED_LOADS;      // Disable lazy loading: we need all symbols at once
        options |= SYMOPT_IGNORE_NT_SYMPATH;    // Ignore other symbol files
        options |= SYMOPT_UNDNAME;              // Undecorate function names
        SymSetOptions(options);

        module->m_hProcess = GetCurrentProcess();
        BOOL result = SymInitialize(module->m_hProcess, NULL, FALSE);
        return result == TRUE;
    }

    bool loadInputModule(ModuleInfo* module, const char* file) {
        DWORD64 baseAddress = SymLoadModuleEx(module->m_hProcess, NULL, file, NULL, module->m_dwBaseAddress,
                                              0x7FFFFFFFUL, NULL, 0);
        if (baseAddress == 0 && GetLastError() != ERROR_SUCCESS) {
            return false;
        }
        module->m_dwBaseAddress = baseAddress;
        return true;
    }

    BOOL extractSymbolsEnumerationCallback(PSYMBOL_INFO pSymInfo, ULONG symbolSize, PVOID userContext) {
        (void) symbolSize;

        auto* module = reinterpret_cast<ModuleInfo*>(userContext);
        GoMint::DefinitionTable& definitions = *module->m_definitions;

        // Retrieve symbol name
        if (pSymInfo->MaxNameLen == 0) {
            // No name specified
            return TRUE;
        }

        module->m_nameBuffer.resize(pSymInfo->NameLen);
        std::memcpy(&module->m_nameBuffer[0], pSymInfo->Name, pSymInfo->NameLen * sizeof(CHAR));
        std::string symbolName(&module->m_nameBuffer[0], pSymInfo->NameLen);

        auto find = definitions.m_symbolDeclsByName.find(symbolName);
        if (find == definitions.m_symbolDeclsByName.end()) {
            // Not interested in this symbol
            return TRUE;
        }

        find->second->m_addressOffset = pSymInfo->Address - pSymInfo->ModBase;
        return TRUE;
    }

    bool extractSymbols(ModuleInfo* module) {
        SymEnumSymbols(module->m_hProcess, module->m_dwBaseAddress, "*", extractSymbolsEnumerationCallback, module);
        return true;
    }

    bool unloadInputModule(ModuleInfo* module) {
        return SymUnloadModule64(module->m_hProcess, module->m_dwBaseAddress) == TRUE;
    }

    bool extractSymbols(GoMint::DefinitionTable& definitions, const char* inputFile) {
        ModuleInfo module;
        module.m_definitions = &definitions;

        if (!initializeDbgHelp(&module)) {
            printf("Failed to initialize DbgHelp library\n");
            return false;
        }

        if (!loadInputModule(&module, inputFile)) {
            printf("Failed to load input module\n");
            return false;
        }

        if (!extractSymbols(&module)) {
            printf("Could not enumerate symbols\n");
            return false;
        }

        if (!unloadInputModule(&module)) {
            printf("Failed to unload input module\n");
            return false;
        }

        return true;
    }

}

#pragma clang diagnostic pop