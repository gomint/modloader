//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "FunctionSymbol.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::FunctionSymbol& p) {
        j["ret"] = p.m_returnType;
        j["args"] = p.m_arguments;
        to_json(j, static_cast<const Symbol&>(p));
    }

    void from_json(const nlohmann::json& j, GoMint::FunctionSymbol& p) {
        j.at("ret").get_to(p.m_returnType);
        j.at("args").get_to(p.m_arguments);
        from_json(j, static_cast<Symbol&>(p));
    }

    void FunctionSymbol::generatePointerTypeDeclaration(FileWriter& writer) {
        writer.cont() << m_returnType << " (*)(";
        for (std::size_t i = 0; i < m_arguments.size(); ++i) {
            if (i != 0) {
                writer.cont() << ", ";
            }
            auto& arg = m_arguments[i];
            writer.cont() << arg.m_type << " " << arg.m_name;
        }
        writer.cont() << ")";
    }

    void FunctionSymbol::generateVariableCast(FileWriter& writer, const std::string& expression) {
        writer.cont() << "reinterpret_cast<" << m_pointerType << ">(" << expression << ")";
    }

    void FunctionSymbol::generateFunctionDeclaration(FileWriter& writer, const std::string& name) {
        writer.cont() << m_returnType << " " << name << "(";
        for (std::size_t i = 0; i < m_arguments.size(); ++i) {
            if (i != 0) {
                writer.cont() << ", ";
            }
            auto& arg = m_arguments[i];
            writer.cont() << arg.m_type << " " << arg.m_name;
        }
        writer.cont() << ")";
    }

    void FunctionSymbol::generateFunctionDefinition(FileWriter& writer, const std::string& clazz, const std::string& name) {
        writer.cont() << m_returnType << " " << clazz << "::" << name << "(";
        for (std::size_t i = 0; i < m_arguments.size(); ++i) {
            if (i != 0) {
                writer.cont() << ", ";
            }
            auto& arg = m_arguments[i];
            writer.cont() << arg.m_type << " " << arg.m_name;
        }
        writer.cont() << ")";
    }
}