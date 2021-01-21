//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_SHAREDLIBRARY_H
#define MODLOADER_SHAREDLIBRARY_H

#include <memory>
#include <string>

namespace GoMint {

    namespace Native { class SharedLibraryImpl; }

    /**
     * Wrapper class for platform specific shared libraries. Can be used to activate shared libraries
     * and retrieve function addresses from them.
     *
     * @author Ciel DeVille
     * @version 1.0.0
     * @stability unstable
     */
    class SharedLibrary {

    public:

        /**
         * Attempts to activate a shared library specified given its file on disk. If the library could not be
         * loaded, nullptr will be returned instead.
         *
         * @param file The file of the shared library
         * @return The shared library if it could be loaded or nullptr otherwise
         */
        static std::unique_ptr<SharedLibrary> load(const std::string& file);

        ~SharedLibrary();

        /**
         * Retrieves a named procedure address from the library.
         *
         * @param name The name of the procedure whose address to retrieve
         * @return The address if found or nullptr otherwise
         */
        void* getProcedureAddress(const std::string& name);

    private:

        explicit SharedLibrary(std::unique_ptr<Native::SharedLibraryImpl> impl);

        std::unique_ptr<Native::SharedLibraryImpl> m_impl;

    };

    using SharedLibraryPtr = std::unique_ptr<SharedLibrary>;

}

#endif //MODLOADER_SHAREDLIBRARY_H
