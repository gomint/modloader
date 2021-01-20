//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_FILEWRITER_H
#define MODLOADER_FILEWRITER_H

#include <filesystem>
#include <fstream>

namespace GoMint {

    class FileWriter {

    public:

        explicit FileWriter(const std::filesystem::path& file);

        bool isOpen() const;

        FileWriter& increaseIndent();
        FileWriter& decreaseIndent();

        std::ofstream& newline();
        std::ofstream& cont();

    private:

        static constexpr int k_maxIndent = 15;

        char m_indent[k_maxIndent + 1];
        int m_indentLevel;
        std::size_t m_lineno;
        std::ofstream m_out;

    };

}

#endif //MODLOADER_FILEWRITER_H
