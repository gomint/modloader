//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include "VirtualFunctionResolver.h"

#include "Type.h"

namespace GoMint {

    VirtualFunctionResolver::VirtualFunctionResolver(std::uint64_t vindex)
            : m_vindex{vindex} {

    }

    FunctionResolution VirtualFunctionResolver::getResolution() const {
        return FunctionResolution::Virtual;
    }

    bool VirtualFunctionResolver::hasFunctionPointer() const {
        return false;
    }

    bool VirtualFunctionResolver::generateFunctionInvocation(FileWriter& writer,
                                                             Type* type,
                                                             Function* function,
                                                             const std::string& pointerType,
                                                             const std::string& pointerName,
                                                             bool isMember) const {
        if (type->getInheritance() != TypeInheritance::Single) {
            // TODO: Implement multi-inheritance!
            return false;
        }

        auto& sig = function->getSignature();

        writer.newline() << type->getName() << "::" << pointerType << " ptr = makeMemfnPtr<" << pointerType << ">(vtableLookup(this, " << m_vindex
                         << "));";
        writer.newline();

        if (sig.m_returnType != "void") {
            writer.cont() << "return ";
        }

        writer.cont() << "(reinterpret_cast<SingleInvocable*>(this)->*ptr)(";
        for (std::size_t i = 0; i < sig.m_arguments.size(); ++i) {
            if (i != 0) writer.cont() << ", ";
            writer.cont() << sig.m_arguments[i].m_name;
        }
        writer.cont() << ");";
        return true;
    }

    bool VirtualFunctionResolver::generateFunctionPointerLoader(FileWriter& writer,
                                                                const std::string& baseaddrVar,
                                                                Type* type,
                                                                Function* function,
                                                                const std::string& pointerType,
                                                                const std::string& pointerName,
                                                                bool isMember) const {
        return false;
    }

}