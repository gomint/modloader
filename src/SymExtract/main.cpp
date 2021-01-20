//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//
#include "extract.h"

#include <cstdio>
#include <unordered_map>

#include <cxxopts.hpp>

using namespace GoMint;

const char* g_usage =
        "symextract - extract symbols from .pdb and executable files"           "\n"
        "             and generate glue code to access them."                   "\n"
        "  Ciel DeVille (ciel.dev [at] pm.me)"                                  "\n"
        "\n"
        "    Usage: symextract <table> <input> <output>"                        "\n"
        "      <table>: A .csv file which contains all symbols to extract"      "\n"
        "      <input>: Input file (.pdb or .exe on Windows)"                   "\n"
        "      <output>: The name and path without extension"                   "\n"
        "\n"
        "  The specified table file should contain four columns and no"         "\n"
        "  superfluous whitespace. The first column should specify the"         "\n"
        "  symbols name, the second the symbol's function pointer declaration." "\n"
        "  The third column should specify the name of the function pointer"    "\n"
        "  type and the last column should specify the name of the variable"    "\n"
        "  into which the function address will be loaded."                     "\n"
        "\n"
        "  Example: symextract symbol_table.csv main.exe symbols.cpp"           "\n";


cxxopts::Options g_optionDeclarations("symextract", "Extracts symbols from an executable or program debugging information and generates schematic glue code");
std::unique_ptr<cxxopts::ParseResult> g_options;

std::string g_schemaDefinitionFile;
std::string g_symbolNameTableFile;
std::string g_inputFile;
std::string g_outputDirectory;
std::string g_includePrefix;

bool parseCommandLineArguments(int argc, char* argv[]) {
    g_optionDeclarations.add_options()
            ("s,schema", "The schema definition file ; same as <schema>", cxxopts::value<std::string>())
            ("n,symbol-names", "The symbol name table file ; same as <symbol-names>", cxxopts::value<std::string>())
            ("i,input-file", "The file to analyze ; same as <input-file>", cxxopts::value<std::string>())
            ("o,output-directory", "Directory for generated files", cxxopts::value<std::string>()->default_value("."))
            ("p,include-prefix", "Prefix for generated includes", cxxopts::value<std::string>()->default_value(""))
            ("h,help", "Displays usage");

    g_optionDeclarations.positional_help("<schema> <symbol-names> <input-file>");
    g_optionDeclarations.show_positional_help();
    g_optionDeclarations.parse_positional({"schema", "symbol-names", "input-file", "additional"});

    try {
        g_options = std::make_unique<cxxopts::ParseResult>(g_optionDeclarations.parse(argc, argv));
    } catch (cxxopts::OptionException& e) {
        printf("%s\n", e.what());
        return false;
    }

    if (g_options->count("help")) {
        printf("%s\n", g_optionDeclarations.help().c_str());
        exit(0);
    }

    if (!g_options->count("schema")) {
        printf("Missing schema definition file\n");
        return false;
    }
    g_schemaDefinitionFile = (*g_options)["schema"].as<std::string>();

    if (!g_options->count("symbol-names")) {
        printf("Missing symbol name table file\n");
        return false;
    }
    g_symbolNameTableFile = (*g_options)["symbol-names"].as<std::string>();

    if (!g_options->count("input-file")) {
        printf("Missing input file\n");
        return false;
    }
    g_inputFile = (*g_options)["input-file"].as<std::string>();

    if (g_options->count("output-directory")) {
        g_outputDirectory = (*g_options)["output-directory"].as<std::string>();
    } else {
        g_outputDirectory = ".";
    }

    if (g_options->count("include-prefix")) {
        g_includePrefix = (*g_options)["include-prefix"].as<std::string>();
    } else {
        g_includePrefix = "";
    }

    if (!g_includePrefix.empty() && g_includePrefix[g_includePrefix.length() - 1] != '/') {
        g_includePrefix += '/';
    }

    return true;
}

bool readSchema(Schema& schema, const std::string& file) {
    std::filesystem::path path(file);
    if (!std::filesystem::exists(path)) {
        printf("Schema file does not exist\n");
        return false;
    }

    if (!std::filesystem::is_regular_file(path)) {
        printf("Schema file is not a regular file\n");
        return false;
    }

    return schema.loadFile(file);
}

bool readSymbolNames(Schema& schema, const std::string& file) {
    std::filesystem::path path(file);
    if (!std::filesystem::exists(path)) {
        printf("Symbol names file does not exist\n");
        return false;
    }

    if (!std::filesystem::is_regular_file(path)) {
        printf("Symbol names file is not a regular file\n");
        return false;
    }

    return schema.loadSymbolNames(path);
}


int main(int argc, char* argv[]) {
    if (!parseCommandLineArguments(argc, argv)) {
        printf("%s\n", g_optionDeclarations.help().c_str());
        return 1;
    }

    Schema schema;

    if (!readSchema(schema, g_schemaDefinitionFile)) {
        printf("Failed to read definition table\n");
        return 1;
    }

    if (!readSymbolNames(schema, g_symbolNameTableFile)) {
        printf("Failed to read symbol names table\n");
        return 1;
    }

    if (!extractSymbols(schema, g_inputFile)) {
        printf("Failed to extract symbols\n");
        return 1;
    }

    if (!schema.validate(true)) {
        printf("Missing one or more symbols ; aborting\n");
        return 1;
    }

    if (!schema.generate(g_outputDirectory, g_includePrefix)) {
        printf("Generation failed\n");
        return 1;
    }

    printf("Generation complete\n");
    return 0;
}