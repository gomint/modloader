//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "Generator.h"

#include <fstream>

using namespace std;

namespace GoMint {

    extern void generatePlatformIncludes(char* indent, ofstream& out);
    extern void generatePlatformLoader(char* indent, ofstream& out);

    Generator::Generator() {

    }

    bool Generator::run(const Schema& definitions,
                        const std::string& headerFile,
                        const std::string& sourceFile) {
        return generateHeader(definitions, headerFile) && generateSource(definitions, headerFile, sourceFile);
    }

    bool Generator::generateHeader(const Schema& definitions, const std::string& headerFile) {
        ofstream out(headerFile);
        if (!out) {
            return false;
        }

        resetIndent();

        out << m_indent << "#ifndef __SYMEXTRACT_GENERATED_HEADER__" << endl;
        out << m_indent << "#define __SYMEXTRACT_GENERATED_HEADER__" << endl;
        out << endl;
        out << m_indent << "#include <SymExtract/Helpers.h>" << endl;
        generateHeaderIncludes(definitions, out);
        out << endl;
        out << m_indent << "namespace GoMint { namespace SymExtract {" << endl;
        out << endl;

        increaseIndent();

        generateHeaderForwardDeclarations(definitions, out);
        generateHeaderTypes(definitions, out);
        generateHeaderSymbols(definitions, out);

        out << endl;
        out << m_indent << "bool loadSymbols(SymbolTable* tbl);" << endl;
        out << m_indent << "void installTypes(const SymbolTable* tbl);" << endl;

        decreaseIndent();

        out << endl;
        out << m_indent << "} }" << endl;

        out << m_indent << "#endif // __SYMEXTRACT_GENERATED_HEADER__" << endl;
        out.close();
        return true;
    }

    void Generator::generateHeaderIncludes(const Schema& definitionTable, ofstream& out) {
        out << endl;
        for (const auto& include : definitionTable.m_includes) {
            out << m_indent << "#include " << include << endl;
        }
        out << endl;
    }

    void Generator::generateHeaderForwardDeclarations(const Schema& definitions, ofstream& out) {
        out << m_indent << "//" << endl;
        out << m_indent << "// Forward Declarations" << endl;
        out << m_indent << "//" << endl;
        out << endl;

        for (auto& decl : definitions.m_forwardDecls) {
            out << m_indent << decl.m_type << " " << decl.m_name << ";" << endl;
        }

        out << endl;

        for (auto& symbol : definitions.m_symbolDecls) {
            switch (symbol->m_type) {
                case SymbolType::Function:
                case SymbolType::MemberFunction:
                    // Typedef
                    out << m_indent << "typedef ";
                    generateFunctionPointerDecl(out, *reinterpret_cast<FunctionSymbol*>(symbol.get()),
                                                symbol->m_pointerName);
                    out << ";" << endl;
                    break;

                default:
                    continue;
            }
        }

        out << endl;
    }

    void Generator::generateHeaderTypes(const Schema& definitions, ofstream& out) {
        out << m_indent << "//" << endl;
        out << m_indent << "// Type Declarations" << endl;
        out << m_indent << "//" << endl;
        out << endl;

        for (auto& decl : definitions.m_typeDecls) {
            out << m_indent << decl.m_type << " " << decl.m_name << " {" << endl;
            out << m_indent << "public:" << endl;
            increaseIndent();

            for (auto& memfuncDecl : decl.m_memberFunctions) {
                auto symbolDecl = definitions.m_symbolDeclsByName.find(memfuncDecl.m_symbol);
                if (symbolDecl != definitions.m_symbolDeclsByName.end()) {
                    Symbol* symbol = symbolDecl->second;
                    if (symbol->m_type == SymbolType::MemberFunction) {
                        generateHeaderMemberFunction(definitions, out, decl, memfuncDecl,
                                                     *reinterpret_cast<MemberFunctionSymbol*>(symbol));
                    }
                }
            }

            decreaseIndent();
            out << m_indent << "};" << endl;
        }

        out << endl;
    }

    void Generator::generateHeaderMemberFunction(const Schema& definitionTable, std::ofstream& out,
                                                 const Type& type, const MemberFunction& memfunc,
                                                 const MemberFunctionSymbol& symbol) {
        // Generate member function pointer
        std::string memfunptr = memfunc.m_name + "_ptr";
        out << m_indent << "static " << symbol.m_pointerName << " " << memfunptr << ";" << endl;

        // Generate invokable member method
        out << m_indent
            << symbol.m_returnType
            << " " << memfunc.m_name;
        generateFunctionArgumentList(out, symbol, true);
        out << ";" << endl;
    }

    void Generator::generateHeaderSymbols(const Schema& definitions, std::ofstream& out) {
        out << m_indent << "//" << endl;
        out << m_indent << "// Symbols" << endl;
        out << m_indent << "//" << endl;
        out << endl;

        out << m_indent << "struct SymbolTable {" << endl;
        increaseIndent();

        for (auto& symbol : definitions.m_symbolDecls) {
            switch (symbol->m_type) {
                case SymbolType::Function:
                case SymbolType::MemberFunction:
                    // Pointer
                    out << m_indent << "" << symbol->m_pointerName << " " << symbol->m_variableName << ";" << endl;
                    break;

                default:
                    continue;
            }
        }

        decreaseIndent();
        out << m_indent << "};" << endl;

    }

    bool Generator::generateSource(const Schema& definitions, const std::string& headerFile,
                                   const std::string& sourceFile) {
        ofstream out(sourceFile);
        if (!out) {
            return false;
        }

        resetIndent();

        out << m_indent << "#include \"" << headerFile << "\"" << endl;
        out << m_indent << "#include <cstdint>" << endl;
        generatePlatformIncludes(m_indent, out);

        out << endl;
        out << m_indent << "namespace GoMint { namespace SymExtract {" << endl;
        out << endl;
        increaseIndent();

        generatePlatformLoader(m_indent, out);
        out << endl;

        generateSourceTypes(definitions, out);
        generateSourceLoadSymbols(definitions, out);
        out << endl;

        decreaseIndent();
        out << endl;
        out << m_indent << "} }" << endl;

        out.close();
        return true;
    }

    void Generator::generateSourceTypes(const Schema& definitions, std::ofstream& out) {
        out << m_indent << "//" << endl;
        out << m_indent << "// Types" << endl;
        out << m_indent << "//" << endl;
        out << endl;

        for (auto& type : definitions.m_typeDecls) {
            for (auto& memfunc : type.m_memberFunctions) {
                auto it = definitions.m_symbolDeclsByName.find(memfunc.m_symbol);
                if (it != definitions.m_symbolDeclsByName.end()) {
                    auto& symbol = *(it->second);
                    if (symbol.m_type != SymbolType::MemberFunction) continue;

                    auto& memsymbol = *reinterpret_cast<const MemberFunctionSymbol*>(&symbol);

                    out << m_indent << symbol.m_pointerName << " " << type.m_name << "::" << memfunc.m_name << "_ptr = nullptr;" << endl;
                    out << m_indent << memsymbol.m_returnType << " " << type.m_name << "::" << memfunc.m_name;
                    generateFunctionArgumentList(out, memsymbol, true);
                    out << " {" << endl;
                    increaseIndent();

                    out << m_indent << "auto* thisptr = reinterpret_cast<";
                    generateMemberFunctionInvocable(out, memsymbol);
                    out << "*>(this);" << endl;

                    out << m_indent;
                    if (memsymbol.m_returnType != "void") {
                        out << "return ";
                    }
                    out << "(thisptr->*" << memfunc.m_name << "_ptr)(";
                    for (std::size_t i = 0; i < memsymbol.m_argumentTypes.size(); ++i) {
                        if (i != 0) out << ", ";
                        out << "arg" << i;
                    }
                    out << ");" << endl;


                    decreaseIndent();
                    out << m_indent << "}" << endl;
                    out << endl;
                }
            }
        }

        out << endl;
    }

    void Generator::generateSourceLoadSymbols(const Schema& definitions, std::ofstream& out) {
        out << m_indent << "//" << endl;
        out << m_indent << "// Symbols" << endl;
        out << m_indent << "//" << endl;
        out << endl;
        out << m_indent << "bool loadSymbols(SymbolTable* tbl) {" << endl;
        increaseIndent();

        out << m_indent << "std::uintptr_t baseAddress = loadBaseAddress();" << endl;
        out << m_indent << "if (baseAddress == 0) return false;" << endl;

        for (auto& symbol : definitions.m_symbolDecls) {
            out << m_indent << "tbl->" << symbol->m_variableName << " = createMemberFunctionPointer<" << symbol->m_pointerName << ">(static_cast<std::uintptr_t>(baseAddress + " << symbol->m_addressOffset << "ULL));" << endl;
        }
        out << endl;
        out << m_indent << "return true;" << endl;

        decreaseIndent();
        out << m_indent << "}" << endl;
        out << endl;

        out << m_indent << "void installTypes(const SymbolTable* tbl) {" << endl;
        increaseIndent();

        for (auto& type : definitions.m_typeDecls) {
            for (auto& memfunc : type.m_memberFunctions) {
                auto it = definitions.m_symbolDeclsByName.find(memfunc.m_symbol);
                if (it != definitions.m_symbolDeclsByName.end()) {
                    auto& symbol = *(it->second);
                    out << m_indent << type.m_name << "::" << memfunc.m_name << "_ptr = tbl->" << symbol.m_variableName << ";" << endl;
                }
            }
        }

        decreaseIndent();
        out << m_indent << "}" << endl;
        out << endl;
    }

    void Generator::generateMemberFunctionInvocable(ofstream& out, const MemberFunctionSymbol& symbol) const {
        switch (symbol.m_inheritance) {
            case TypeInheritance::Single:
                out << "SingleInvocable";
                break;
            case TypeInheritance::Multi:
                out << "MultiInvocable";
                break;
            default:
                out << "void";
                break;
        }
    }

    void Generator::generateFunctionPointerDecl(std::ofstream& out, const FunctionSymbol& function, const std::string& varname) {
        out << function.m_returnType
            << " (";

        if (function.m_type == SymbolType::MemberFunction) {
            const auto& memfunc = *reinterpret_cast<const MemberFunctionSymbol*>(&function);
            generateMemberFunctionInvocable(out, memfunc);
            out << "::";
        }

        out << "*" << varname << ")";
        generateFunctionArgumentList(out, function, false);
    }

    void Generator::generateFunctionArgumentList(ofstream& out, const FunctionSymbol& function, bool named) {
        out << "(";
        int argc = 0;
        for (auto it = function.m_argumentTypes.begin(); it != function.m_argumentTypes.end(); ++it) {
            if (it != function.m_argumentTypes.begin()) {
                out << ", ";
            }
            out << *it;
            if (named) {
                out << " arg" << argc++;
            }
        }
        out << ")";
    }

    void Generator::resetIndent() {
        m_indentLevel = 0;
        std::memset(m_indent, 0, k_maxIndentLevel * sizeof(char));
    }

    void Generator::increaseIndent() {
        if (m_indentLevel < k_maxIndentLevel) {
            m_indent[m_indentLevel++] = '\t';
        }
    }

    void Generator::decreaseIndent() {
        if (m_indentLevel > 0) {
            m_indent[--m_indentLevel] = '\0';
        }
    }

}