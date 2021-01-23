//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "FixedFunctionResolver.h"

#include "Type.h"

namespace GoMint {

    FixedFunctionResolver::FixedFunctionResolver(AddressOffsetReference address)
            : m_address{std::move(address)} {

    }

    FunctionResolution FixedFunctionResolver::getResolution() const {
        return FunctionResolution::Fixed;
    }

    bool FixedFunctionResolver::hasFunctionPointer() const {
        return true;
    }

    bool FixedFunctionResolver::generateFunctionInvocation(FileWriter& writer,
                                                           Type* type,
                                                           Function* function,
                                                           const std::string& pointerType,
                                                           const std::string& pointerName,
                                                           bool isMember) const {
        writer.newline();

        auto& sig = function->getSignature();
        if (sig.m_returnType != "void") {
            writer.cont() << "return ";
        }

        if (isMember) {
            std::string pointerContext;
            switch (type->getInheritance()) {
                case TypeInheritance::Single:
                    pointerContext = "SingleInvocable";
                    break;

                case TypeInheritance::Multi:
                    pointerContext = "MultiInvocable";
                    break;

                default:
                    return false;
            }

            writer.cont() << "(reinterpret_cast<" << pointerContext << "*>(this)->*" << pointerName << ")";
        } else {
            writer.cont() << pointerName;
        }

        writer.cont() << "(";
        for (std::size_t i = 0; i < sig.m_arguments.size(); ++i) {
            if (i != 0) {
                writer.cont() << ", ";
            }
            writer.cont() << sig.m_arguments[i].m_name;
        }
        writer.cont() << ");";
        return true;
    }

    bool FixedFunctionResolver::generateFunctionPointerLoader(FileWriter& writer,
                                                              const std::string& baseaddrVar,
                                                              Type* type,
                                                              Function* function,
                                                              const std::string& pointerType,
                                                              const std::string& pointerName,
                                                              bool isMember) const {
        if (m_address.index() == std::variant_npos) return false;
        if (m_address.index() == 1 && !std::get<Symbol*>(m_address)->hasAddressOffset()) return false;
        std::uint64_t offset = m_address.index() == 0 ?
                               std::get<std::uint64_t>(m_address) :
                               std::get<Symbol*>(m_address)->getAddressOffset();

        writer.newline() << type->getName() << "::" << pointerName << " = ";
        if (isMember) {
            writer.cont() << "makeMemfnPtr<" << type->getName() << "::" << pointerType << ">(" << baseaddrVar << " + static_cast<std::uintptr_t>(" << offset << "ULL));";
        } else {
            writer.cont() << "reinterpret_cast<" << type->getName() << "::" << pointerType << ">(" << baseaddrVar
                          << " + static_cast<std::uintptr_t>(" << offset << "ULL));";
        }
        return true;
    }

}