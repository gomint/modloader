//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/Windows/FilesystemImpl.h>

#include <Windows.h>

namespace GoMint::Native {

    bool FilesystemImpl::isDirectory(const std::string& directory) {
        DWORD attribs = GetFileAttributesA(directory.c_str());
        if (attribs == INVALID_FILE_ATTRIBUTES) {
            return false;
        }
        return (attribs & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    bool FilesystemImpl::createDirectory(const std::string& directory) {
        BOOL result = CreateDirectoryA(directory.c_str(), NULL);
        return (result != FALSE);
    }

    std::vector<std::string> FilesystemImpl::listSharedLibraries(const std::string& directory) {
        WIN32_FIND_DATA ffd;
        std::string wildcard = directory + "\\*.dll";

        HANDLE handle = FindFirstFileA(wildcard.c_str(), &ffd);
        if (handle == INVALID_HANDLE_VALUE) {
            return {};
        }

        std::vector<std::string> files;
        do {
            if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                files.emplace_back(ffd.cFileName);
            }
        } while (FindNextFile(handle, &ffd) != 0);

        FindClose(handle);
        return files;
    }

    std::string FilesystemImpl::getAbsolutePath(const std::string& path) {
        std::vector<char> buffer;
        DWORD result = MAX_PATH + 1;
        do {
            buffer.resize(result);
            result = GetFullPathNameA(path.c_str(), result, &buffer[0], NULL);
        } while (result > buffer.size());

        if (result == 0) {
            return "";
        }

        return std::string(&buffer[0], result);
    }

}