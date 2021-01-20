//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "MemberFunctionSymbol.h"

namespace GoMint {

    void to_json(nlohmann::json& j, const GoMint::MemberFunctionSymbol& p) {
        j["ret"] = p.m_returnType;
        j["args"] = p.m_arguments;
        j["inheritance"] = p.m_inheritance;
        if (p.m_inheritance == TypeInheritance::Multi) {
            j["thisdelta"] = p.m_thisdelta;
        }
        to_json(j, static_cast<const Symbol&>(p));
    }

    void from_json(const nlohmann::json& j, GoMint::MemberFunctionSymbol& p) {
        j.at("ret").get_to(p.m_returnType);
        j.at("args").get_to(p.m_arguments);
        j.at("inheritance").get_to(p.m_inheritance);
        if (p.m_inheritance == TypeInheritance::Multi) {
            j.at("thisdelta").get_to(p.m_thisdelta);
        } else {
            p.m_thisdelta = 0;
        }
        from_json(j, static_cast<Symbol&>(p));
    }

    void MemberFunctionSymbol::generatePointerTypeDeclaration(FileWriter& writer) {
        writer.cont() << m_returnType << " (";
        switch (m_inheritance) {
            case TypeInheritance::Single:
                writer.cont() << "SingleInvocable::*";
                break;
            case TypeInheritance::Multi:
                writer.cont() << "MultiInvocable::*";
                break;
            default:
                writer.cont() << "*";
                break;
        }
        writer.cont() << ")(";
        for (std::size_t i = 0; i < m_arguments.size(); ++i) {
            if (i != 0) {
                writer.cont() << ", ";
            }
            auto& arg = m_arguments[i];
            writer.cont() << arg.m_type << " " << arg.m_name;
        }
        writer.cont() << ")";
    }

    void MemberFunctionSymbol::generateVariableCast(FileWriter& writer, const std::string& expression) {
        writer.cont() << "createMemberFunctionPointer<" << m_pointerType << ">(static_cast<std::uintptr_t>(" << expression << "))";
    }

    void MemberFunctionSymbol::generateMemberFunctionDeclaration(FileWriter& writer, const std::string& name) {
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
    void MemberFunctionSymbol::generateMemberFunctionDefinition(FileWriter& writer,
                                                                const std::string& clazz,
                                                                const std::string& name) {
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