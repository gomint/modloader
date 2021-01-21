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
     *
     * @author Ciel DeVille
     * @version 1.0.0
     * @stability unstable
     */
    class Filesystem {

    public:

        /**
         * Checks whether or not the specified file path denotes a directory.
         *
         * @param directory The path to check
         * @return Whether or not the specified path denotes a directory
         */
        static bool isDirectory(const std::string& directory);
        /**
         * Attempts to create the specified directory.
         *
         * @param directory The directory to create
         * @return Whether or not the directory could be created
         */
        static bool createDirectory(const std::string& directory);
        /**
         * Returns a list of shared libraries within the specified directory.
         *
         * @param directory The directory whose contents to list
         * @return A list of shared library files within the specified directory
         */
        static std::vector<std::string> listSharedLibraries(const std::string& directory);
        /**
         * Converts the given path to its absolute form.
         *
         * @param path The path to convert
         * @return The path's absolute form or empty string if path resolution failed
         */
        static std::string getAbsolutePath(const std::string& path);

    };

}

#endif //MODLOADER_FILESYSTEM_H
