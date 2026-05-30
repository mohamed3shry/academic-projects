%{
#include <string>
#include <iostream>
#include "ast.hpp"
#include "parser.hpp"
#include "symbol_table.hpp"

extern SymbolTable global_symbol_table;
extern int yylineno;

void yyerror(const char *s);
#define YY_USER_INIT   yylineno = 1;

// Disable interactive mode to avoid fileno issues
#define YY_NEVER_INTERACTIVE 1
%}

%option noyywrap

DIGIT   [0-9]
LETTER  [a-zA-Z]
IDENT   {LETTER}({LETTER}|{DIGIT})*

%%

"int"        { return T_INT; }
"float"      { return T_FLOAT; }
"char"       { return T_CHAR; }
"string"     { return T_STRING; }
"var"        { return T_VAR; }
"if"         { return T_IF; }
"else"       { return T_ELSE; }
"end"        { return T_END; }
"print"      { return T_PRINT; }

"=="         { return T_EQ; }
"!="         { return T_NE; }
">="         { return T_GE; }
"<="         { return T_LE; }
"="          { return T_ASSIGN; }
"<"          { return T_LT; }
">"          { return T_GT; }
"&&"         { return T_AND; }
"||"         { return T_OR; }
"!"          { return T_NOT; }
"+"          { return T_PLUS; }
"-"          { return T_MINUS; }
"*"          { return T_MUL; }
"/"          { return T_DIV; }
"%"          { return T_MOD; }
";"          { return T_SEMICOLON; }
":"          { return T_COLON; }
"{"          { return T_LBRACE; }
"}"          { return T_RBRACE; }
"("          { return T_LPAREN; }
")"          { return T_RPAREN; }

"while"      { return T_WHILE; }

{IDENT} {
    yylval.str = new std::string(yytext);
    return T_IDENTIFIER;
}

{DIGIT}+\.({DIGIT})*([eE][+-]?{DIGIT}+)? {
    yylval.floating = atof(yytext);
    return T_FLOAT_LIT;
}

{DIGIT}+ {
    yylval.integer = atoi(yytext);
    return T_INTEGER;
}

\'([^\\]|\\.)\' {
    yylval.character = yytext[1];
    return T_CHAR_LIT;
}

\"[^\"]*\" {
    yylval.str = new std::string(yytext);
    return T_STRING_LIT;
}

[ \t]+    { /* skip */ }
\r\n      { yylineno++; }
\r        { /* skip */ }
"\xEF\xBB\xBF" { /* skip UTF-8 BOM */ }
[\xA0]+   { /* skip NBSP */ }
\n        { yylineno++; }

"//".*    { /* skip single-line comment */ }
"/*"([^*]|\*+[^*/])*"*/"  { /* skip multi-line comment */ }

. {
    std::string msg = "Lexical error at line ";
    msg += std::to_string(yylineno);
    msg += ": '";
    msg += yytext;
    msg += "'";
    yyerror(msg.c_str());
}

%%