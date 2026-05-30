#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "symbol_table.hpp"
#include "ast.hpp"
#include "code_generator.hpp"

// Globals declared in parser/scanner
extern FILE *yyin;
extern int yyparse();

// External declarations for globals defined in globals.cpp
extern SymbolTable global_symbol_table;
extern ProgramNode *ast_root;
extern int yylineno;
extern bool semantic_error;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: compiler <input_file>\n";
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1;
    }

    std::cout << "=== Starting Compilation ===\n";
    std::cout << "Input file: " << argv[1] << std::endl;
    std::cout << "=== Phase 1: Lexical Analysis (Scanner) ===\n";

    int result = yyparse();
    fclose(yyin);

    if (result != 0)
    {
        std::cerr << "\n=== Compilation Failed ===" << std::endl;
        std::cerr << "Please fix the errors above and try again." << std::endl;
        return result;
    }

    if (semantic_error)
    {
        std::cerr << "\n=== Compilation Failed (Semantic Errors) ===" << std::endl;
        std::cerr << "Please fix the semantic errors above and try again." << std::endl;
        return 1;
    }

    std::cout << "=== Phase 2: Syntax Analysis (Parser) ===\n";
    std::cout << "=== Parsing Successful ===\n";

    // Symbol table print
    std::cout << "\n=== Symbol Table ===" << std::endl;
    global_symbol_table.print();

    // Write AST to file
    std::cout << "\n=== Phase 3: Building Syntax Tree ===" << std::endl;
    if (ast_root)
    {
        std::ofstream out("syntax_tree.txt");
        if (out)
        {
            std::streambuf *old = std::cout.rdbuf(out.rdbuf());
            ast_root->print(0);
            std::cout.rdbuf(old);
            std::cout << "Syntax tree saved to syntax_tree.txt" << std::endl;
        }
        else
        {
            std::cerr << "Warning: Could not create syntax_tree.txt" << std::endl;
        }
    }

    std::cout << "\n=== Phase 4: Code Generation ===" << std::endl;
    CodeGenerator generator("output.c");

    if (ast_root)
    {
        ast_root->generateCode(generator);
        std::cout << "Output saved to output.c\n";
    }

    delete ast_root;
    return 0;
}
