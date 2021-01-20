//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "FileWriter.h"

namespace GoMint {

    FileWriter::FileWriter(const std::filesystem::path& file)
        : m_indent{0}, m_indentLevel{0}, m_lineno{0}, m_out(file) {

    }

    bool FileWriter::isOpen() const {
        return static_cast<bool>(m_out);
    }


    FileWriter & FileWriter::increaseIndent() {
        if (m_indentLevel < k_maxIndent) {
            m_indent[m_indentLevel++] = '\t';
        }
        return *this;
    }

    FileWriter& FileWriter::decreaseIndent() {
        if (m_indentLevel > 0) {
            m_indent[--m_indentLevel] = '\0';
        }
        return *this;
    }

    std::ofstream & FileWriter::newline() {
        if (m_lineno++ > 0) m_out << std::endl;
        m_out << m_indent;
        return m_out;
    }
    std::ofstream& FileWriter::cont() {
        return m_out;
    }

}
