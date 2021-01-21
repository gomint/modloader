//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include <ModLoader/Filesystem.h>
#include <ModLoader/ModLoader.h>

#if defined(MODLOADER_WINDOWS)
#   include <ModLoader/Windows/FilesystemImpl.h>
#else
#   error "No implementation for Filesystem provided"
#endif

namespace GoMint {

    bool Filesystem::isDirectory(const std::string& directory) {
        return Native::FilesystemImpl::isDirectory(directory);
    }

    bool Filesystem::createDirectory(const std::string& directory) {
        return Native::FilesystemImpl::createDirectory(directory);
    }

    std::vector<std::string> Filesystem::listSharedLibraries(const std::string& directory) {
        return Native::FilesystemImpl::listSharedLibraries(directory);
    }

    std::string Filesystem::getAbsolutePath(const std::string& path) {
        return Native::FilesystemImpl::getAbsolutePath(path);
    }

}
