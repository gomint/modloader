//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <Windows.h>

#include <stdio.h>
#include <stdbool.h>

/*
 *
 */
LPWSTR* argv;
int argc;

LPWSTR targetExecutablePath;
WCHAR  targetDllPath[_MAX_PATH + 1];

STARTUPINFOW startupInfo;
PROCESS_INFORMATION processInformation;

LPVOID remoteDllPath = NULL;

HANDLE loadingThread = NULL;

void cleanup(void) {
    if (loadingThread != NULL) {
        CloseHandle(loadingThread);
        loadingThread = NULL;
    }

    if (remoteDllPath != NULL) {
        VirtualFreeEx(processInformation.hProcess, remoteDllPath, 0, MEM_RELEASE);
        remoteDllPath = NULL;
    }

    if (processInformation.hProcess != NULL) {
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);
        processInformation.hProcess = NULL;
    }
}

bool parseCommandLine(PWSTR lpCmdLine) {
    argv = CommandLineToArgvW(lpCmdLine, &argc);

    if (argc != 3) {
        printf("Invalid number of arguments: invoke with preloader.exe <EXE> <DLL>\n");
        return false;
    }
    return true;
}

bool retrieveFilenames(void) {
    targetExecutablePath = argv[1];
    memset(targetDllPath, 0, (_MAX_PATH + 1) * sizeof(WCHAR));
    DWORD result = GetFullPathNameW(argv[2], _MAX_PATH, targetDllPath, NULL);
    if (result > _MAX_PATH + 1 || result == 0) {
        printf("Could not retrieve full path to DLL\n");
        return false;
    }
    return true;
}

bool startProcessSuspended(void) {
    if (!CreateProcessW(targetExecutablePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInformation)) {
        printf("Could not create process\n");
        return false;
    }
    return true;
}

bool allocateAndWriteRemoteDllPath(void) {
    size_t dllPathLength = wcslen(targetDllPath) * sizeof(WCHAR);
    remoteDllPath = VirtualAllocEx(processInformation.hProcess, NULL, dllPathLength + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!remoteDllPath) {
        printf("Failed to allocate memory in target process\n");
        return false;
    }
    if (!WriteProcessMemory(processInformation.hProcess, remoteDllPath, targetDllPath, dllPathLength, NULL)) {
        printf("Failed to write DLL path to target process memory\n");
        return false;
    }
    return true;
}

bool createDllLoadingThread(void) {
    LPVOID loadLibraryProc = GetProcAddress(GetModuleHandleW(L"KERNEL32.DLL"), "LoadLibraryW");
    if (!loadLibraryProc) {
        printf("Failed to retrieve procedure address of 'LoadLibraryW'\n");
        return false;
    }

    loadingThread = CreateRemoteThread(processInformation.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE) loadLibraryProc, remoteDllPath, 0, NULL);
    if (!loadingThread) {
        printf("Failed to create remote thread for DLL injection, code %lu\n", GetLastError());
        return false;
    }
    return true;
}

bool injectDllAndResumeProcess(void) {
    WaitForSingleObject(loadingThread, INFINITE);
    ResumeThread(processInformation.hThread);
    return true;
}

int main(void) {
    processInformation.hProcess = NULL;

    if (!parseCommandLine(GetCommandLineW())) {
        cleanup();
        return 1;
    }

    if (!retrieveFilenames()) {
        cleanup();
        return 1;
    }

    printf("[PreLoader] Creating suspend bedrock server instance\n");

    if (!startProcessSuspended()) {
        cleanup();
        return 1;
    }

    printf("[PreLoader] Injecting ModLoader DLL\n");

    if (!allocateAndWriteRemoteDllPath()) {
        cleanup();
        return 1;
    }

    if (!createDllLoadingThread()) {
        cleanup();
        return 1;
    }

    if (!injectDllAndResumeProcess()) {
        cleanup();
        return 1;
    }

    printf("[PreLoader] DLL successfully injected\n");

    WaitForSingleObject(processInformation.hProcess, INFINITE);
    cleanup();
    return 0;
}