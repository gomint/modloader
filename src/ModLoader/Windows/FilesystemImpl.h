//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FILESYSTEMIMPL_H
#define MODLOADER_FILESYSTEMIMPL_H

#include <string>
#include <vector>

namespace GoMint::Native {

    class FilesystemImpl {

    public:

        static bool isDirectory(const std::string& directory);
        static bool createDirectory(const std::string& directory);
        static std::vector<std::string> listSharedLibraries(const std::string& directory);
        static std::string getAbsolutePath(const std::string& path);

    };

}

#endif //MODLOADER_FILESYSTEMIMPL_H
