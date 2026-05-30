// globals.cpp - Single definition point for global variables
// This file should be the ONLY place where these globals are defined

#include "parser.hpp"
#include "symbol_table.hpp"
#include "ast.hpp"

// Global symbol table instance
SymbolTable global_symbol_table;

// Global AST root node
ProgramNode *ast_root = nullptr;

// Global line number counter (used by scanner and parser)
int yylineno = 1;

// Semantic error flag
bool semantic_error = false;
