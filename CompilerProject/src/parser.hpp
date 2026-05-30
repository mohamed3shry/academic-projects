
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 1 "src/parser.y"

#include <iostream>
#include <string>
// Forward declarations for %union - processed before %union
class ASTNode;
class ProgramNode;



/* Line 1676 of yacc.c  */
#line 50 "src/parser.hpp"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_IDENTIFIER = 258,
     T_INTEGER = 259,
     T_FLOAT_LIT = 260,
     T_CHAR_LIT = 261,
     T_STRING_LIT = 262,
     T_INT = 263,
     T_FLOAT = 264,
     T_CHAR = 265,
     T_STRING = 266,
     T_VAR = 267,
     T_IF = 268,
     T_ELSE = 269,
     T_END = 270,
     T_PRINT = 271,
     T_WHILE = 272,
     T_ASSIGN = 273,
     T_EQ = 274,
     T_NE = 275,
     T_LT = 276,
     T_GT = 277,
     T_LE = 278,
     T_GE = 279,
     T_COLON = 280,
     T_PLUS = 281,
     T_MINUS = 282,
     T_MUL = 283,
     T_DIV = 284,
     T_MOD = 285,
     T_AND = 286,
     T_OR = 287,
     T_NOT = 288,
     T_SEMICOLON = 289,
     T_LBRACE = 290,
     T_RBRACE = 291,
     T_LPAREN = 292,
     T_RPAREN = 293,
     UMINUS = 294
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 9 "src/parser.y"

    int integer;
    double floating;
    char character;
    std::string* str;
    ASTNode* node;
    ProgramNode* program;



/* Line 1676 of yacc.c  */
#line 117 "src/parser.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


