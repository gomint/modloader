//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FILESYSTEM_H
#define MODLOADER_FILESYSTEM_H

#include <string>
#include <vector>

namespace GoMint {

    /**
     * Simple helper class which is unfortunately required as on Linux (future target)
     * the C++ 17 filesystems library is actually not included in the STL but provided
     * in a separate library.
     */
    class Filesystem {

    public:

        static bool isDirectory(const std::string& directory);
        static bool createDirectory(const std::string& directory);
        static std::vector<std::string> listDLLsInDirectory(const std::string& directory);
        static std::string getAbsolutePath(const std::string& path);

    };

}

#endif //MODLOADER_FILESYSTEM_H
