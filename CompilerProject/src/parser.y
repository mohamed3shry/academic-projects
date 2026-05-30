%code requires {
#include <iostream>
#include <string>
// Forward declarations for %union - processed before %union
class ASTNode;
class ProgramNode;
}

%union {
    int integer;
    double floating;
    char character;
    std::string* str;
    ASTNode* node;
    ProgramNode* program;
}

%{
#include "ast.hpp"
#include "symbol_table.hpp"
#include "code_generator.hpp"
#include "types.hpp"

extern int yylex();
extern FILE* yyin;
extern SymbolTable global_symbol_table;
extern ProgramNode* ast_root;
extern int yylineno;
extern bool semantic_error;

static void report_semantic(const std::string& msg) {
    std::cerr << "Semantic error at line " << yylineno << ": " << msg << std::endl;
    semantic_error = true;
}

static bool ensure_declared(const std::string& name) {
    if (!global_symbol_table.exists(name)) {
        report_semantic("identifier '" + name + "' used before declaration");
        return false;
    }
    return true;
}

static ValueType stringToType(const std::string& s) {
    if (s == "int") return VT_INT;
    if (s == "float") return VT_FLOAT;
    if (s == "char") return VT_CHAR;
    if (s == "string") return VT_STRING;
    return VT_UNKNOWN;
}

static bool isNumeric(ValueType t) {
    return t == VT_INT || t == VT_FLOAT || t == VT_CHAR;
}

static ValueType promoteNumeric(ValueType a, ValueType b) {
    if (!isNumeric(a) || !isNumeric(b)) return VT_UNKNOWN;
    if (a == VT_FLOAT || b == VT_FLOAT) return VT_FLOAT;
    if (a == VT_INT || b == VT_INT) return VT_INT;
    return VT_CHAR;
}

static bool canAssign(ValueType lhs, ValueType rhs) {
    if (lhs == rhs) return true;
    // numeric widening
    if (lhs == VT_FLOAT && isNumeric(rhs)) return true;
    if (lhs == VT_INT && (rhs == VT_CHAR)) return true;
    if (lhs == VT_CHAR && rhs == VT_INT) return true; // allow narrowing int to char (simple model)
    return false;
}

void yyerror(const char* s){
  std::cerr<<"Syntax error: "<<s<<" at line "<<yylineno<<std::endl;
}
%}

%token <str> T_IDENTIFIER
%token <integer> T_INTEGER
%token <floating> T_FLOAT_LIT
%token <character> T_CHAR_LIT
%token <str> T_STRING_LIT
%token T_INT T_FLOAT T_CHAR T_STRING
%token T_VAR T_IF T_ELSE T_END T_PRINT T_WHILE
%token T_ASSIGN T_EQ T_NE T_LT T_GT T_LE T_GE T_COLON
%token T_PLUS T_MINUS T_MUL T_DIV T_MOD
%token T_AND T_OR T_NOT
%token T_SEMICOLON T_LBRACE T_RBRACE T_LPAREN T_RPAREN

%type <program> program statement_list
%type <node> statement print_stmt var_declaration assignment_stmt if_stmt while_stmt block
%type <node> expression or_expr and_expr equality_expr comparison_expr add_expr mul_expr unary_expr primary_expr
%type <integer> type_spec

%left T_OR
%left T_AND
%left T_EQ T_NE
%left T_LT T_GT T_LE T_GE
%left T_PLUS T_MINUS
%left T_MUL T_DIV T_MOD
%right T_NOT UMINUS

%%

program:
    statement_list {
        ast_root = $1;
    }
;

statement_list:
    /* empty */ {
        $$ = new ProgramNode();
    }
    | statement_list statement {
        $1->addStatement($2);
        $$ = $1;
    }
;

statement:
    print_stmt T_SEMICOLON {
        $$ = $1;
    }
    | var_declaration T_SEMICOLON {
        $$ = $1;
    }
    | assignment_stmt T_SEMICOLON {
        $$ = $1;
    }
    | if_stmt {
        $$ = $1;
    }
    | while_stmt {
        $$ = $1;
    }
;

print_stmt:
    T_PRINT expression {
        $$ = new PrintStatementNode($2);
    }
    | T_PRINT T_LPAREN expression T_RPAREN {
        $$ = new PrintStatementNode($3);
    }
;

var_declaration:
    type_spec T_IDENTIFIER {
        if (global_symbol_table.existsCurrent(*$2)) {
            report_semantic("redeclaration of '" + *$2 + "'");
        } else {
            global_symbol_table.insertCurrent(*$2, SymbolInfo{static_cast<ValueType>($1)});
        }
        $$ = new VarDeclarationNode(*$2, static_cast<ValueType>($1), nullptr);
        delete $2;
    }
    | type_spec T_IDENTIFIER T_ASSIGN expression {
        if (global_symbol_table.existsCurrent(*$2)) {
            report_semantic("redeclaration of '" + *$2 + "'");
        } else {
            global_symbol_table.insertCurrent(*$2, SymbolInfo{static_cast<ValueType>($1)});
        }
        // ensure initializer type compatible
        ExpressionNode* exprNode = dynamic_cast<ExpressionNode*>($4);
        if (exprNode && !canAssign(static_cast<ValueType>($1), exprNode->exprType)) {
            report_semantic("cannot assign expression of type '" + std::string(valueTypeName(exprNode->exprType)) + "' to variable '" + *$2 + "' of type '" + valueTypeName(static_cast<ValueType>($1)) + "'");
        }
        $$ = new VarDeclarationNode(*$2, static_cast<ValueType>($1), $4);
        delete $2;
    }
;

assignment_stmt:
    T_IDENTIFIER T_ASSIGN expression {
        ensure_declared(*$1);
        SymbolInfo info = global_symbol_table.lookup(*$1);
        ExpressionNode* exprNode = dynamic_cast<ExpressionNode*>($3);
        if (exprNode && !canAssign(info.type, exprNode->exprType)) {
            report_semantic("cannot assign expression of type '" + std::string(valueTypeName(exprNode->exprType)) + "' to variable '" + *$1 + "' of type '" + valueTypeName(info.type) + "'");
        }
        $$ = new AssignmentNode(*$1, $3);
        delete $1;
    }
;

if_stmt:
    T_IF T_LPAREN expression T_RPAREN block {
        $$ = new IfStatementNode($3, $5);
    }
    | T_IF T_LPAREN expression T_RPAREN block T_ELSE block {
        $$ = new IfStatementNode($3, $5, $7);
    }
    | T_IF expression block {
        $$ = new IfStatementNode($2, $3);
    }
    | T_IF expression block T_ELSE block {
        $$ = new IfStatementNode($2, $3, $5);
    }
    | T_IF T_LPAREN expression T_RPAREN T_COLON { global_symbol_table.pushScope(); } statement_list T_ELSE { global_symbol_table.popScope(); global_symbol_table.pushScope(); } T_COLON statement_list T_END {
        $$ = new IfStatementNode($3, $7, $12);
        global_symbol_table.popScope();
    }
    | T_IF expression T_COLON { global_symbol_table.pushScope(); } statement_list T_ELSE { global_symbol_table.popScope(); global_symbol_table.pushScope(); } T_COLON statement_list T_END {
        $$ = new IfStatementNode($2, $5, $10);
        global_symbol_table.popScope();
    }
;

while_stmt:
    T_WHILE T_LPAREN expression T_RPAREN block {
        $$ = new WhileStatementNode($3, $5);
    }
    | T_WHILE expression block {
        $$ = new WhileStatementNode($2, $3);
    }
;

block:
    T_LBRACE { global_symbol_table.pushScope(); } statement_list T_RBRACE {
        $$ = $3;
        global_symbol_table.popScope();
    }
    | T_COLON { global_symbol_table.pushScope(); } statement_list T_END {
        $$ = $3;
        global_symbol_table.popScope();
    }
;

expression:
    or_expr { $$ = $1; }
;

or_expr:
    and_expr { $$ = $1; }
    | or_expr T_OR and_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        if (!l || !r || !isNumeric(l->exprType) || !isNumeric(r->exprType)) {
            report_semantic("logical OR requires numeric operands");
        }
        auto* n = new ExpressionNode(T_OR, $1, $3);
        n->exprType = VT_INT;
        $$ = n;
    }
;

and_expr:
    equality_expr { $$ = $1; }
    | and_expr T_AND equality_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        if (!l || !r || !isNumeric(l->exprType) || !isNumeric(r->exprType)) {
            report_semantic("logical AND requires numeric operands");
        }
        auto* n = new ExpressionNode(T_AND, $1, $3);
        n->exprType = VT_INT;
        $$ = n;
    }
;

equality_expr:
    comparison_expr { $$ = $1; }
    | equality_expr T_EQ comparison_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        if (!l || !r || l->exprType != r->exprType) {
            report_semantic("equality operands must have same type");
        }
        auto* n = new ExpressionNode(T_EQ, $1, $3, true);
        n->exprType = VT_INT;
        $$ = n;
    }
    | equality_expr T_NE comparison_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        if (!l || !r || l->exprType != r->exprType) {
            report_semantic("inequality operands must have same type");
        }
        auto* n = new ExpressionNode(T_NE, $1, $3, true);
        n->exprType = VT_INT;
        $$ = n;
    }
;

comparison_expr:
    add_expr { $$ = $1; }
    | comparison_expr T_LT add_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("comparison requires numeric operands");
        auto* n = new ExpressionNode(T_LT, $1, $3, true);
        n->exprType = VT_INT;
        $$ = n;
    }
    | comparison_expr T_GT add_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("comparison requires numeric operands");
        auto* n = new ExpressionNode(T_GT, $1, $3, true);
        n->exprType = VT_INT;
        $$ = n;
    }
    | comparison_expr T_LE add_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("comparison requires numeric operands");
        auto* n = new ExpressionNode(T_LE, $1, $3, true);
        n->exprType = VT_INT;
        $$ = n;
    }
    | comparison_expr T_GE add_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("comparison requires numeric operands");
        auto* n = new ExpressionNode(T_GE, $1, $3, true);
        n->exprType = VT_INT;
        $$ = n;
    }
;

add_expr:
    mul_expr { $$ = $1; }
    | add_expr T_PLUS mul_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("addition requires numeric operands");
        auto* n = new ExpressionNode(T_PLUS, $1, $3);
        n->exprType = res;
        $$ = n;
    }
    | add_expr T_MINUS mul_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("subtraction requires numeric operands");
        auto* n = new ExpressionNode(T_MINUS, $1, $3);
        n->exprType = res;
        $$ = n;
    }
;

mul_expr:
    unary_expr { $$ = $1; }
    | mul_expr T_MUL unary_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("multiplication requires numeric operands");
        auto* n = new ExpressionNode(T_MUL, $1, $3);
        n->exprType = res;
        $$ = n;
    }
    | mul_expr T_DIV unary_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN) report_semantic("division requires numeric operands");
        auto* n = new ExpressionNode(T_DIV, $1, $3);
        n->exprType = res;
        $$ = n;
    }
    | mul_expr T_MOD unary_expr {
        ExpressionNode* l = dynamic_cast<ExpressionNode*>($1);
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($3);
        ValueType res = promoteNumeric(l ? l->exprType : VT_UNKNOWN, r ? r->exprType : VT_UNKNOWN);
        if (res == VT_UNKNOWN || res == VT_FLOAT) report_semantic("modulo requires integer operands");
        auto* n = new ExpressionNode(T_MOD, $1, $3);
        n->exprType = res == VT_FLOAT ? VT_INT : res;
        $$ = n;
    }
;

unary_expr:
    T_NOT unary_expr {
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($2);
        if (!r || !isNumeric(r->exprType)) report_semantic("logical NOT requires numeric operand");
        auto* n = ExpressionNode::makeUnary(T_NOT, $2, true);
        n->exprType = VT_INT;
        $$ = n;
    }
    | T_MINUS unary_expr %prec UMINUS {
        ExpressionNode* r = dynamic_cast<ExpressionNode*>($2);
        if (!r || !isNumeric(r->exprType)) report_semantic("unary minus requires numeric operand");
        auto* n = ExpressionNode::makeUnary(T_MINUS, $2, true);
        n->exprType = r ? r->exprType : VT_UNKNOWN;
        $$ = n;
    }
    | primary_expr { $$ = $1; }
;

primary_expr:
    T_IDENTIFIER {
        ensure_declared(*$1);
        SymbolInfo info = global_symbol_table.lookup(*$1);
        $$ = new ExpressionNode(*$1, info.type);
        delete $1;
    }
    | T_INTEGER {
        $$ = new ExpressionNode($1);
    }
    | T_FLOAT_LIT {
        $$ = new ExpressionNode(static_cast<double>($1));
    }
    | T_CHAR_LIT {
        $$ = new ExpressionNode(static_cast<char>($1));
    }
    | T_STRING_LIT {
        $$ = new ExpressionNode(*$1, true);
        delete $1;
    }
    | T_LPAREN expression T_RPAREN {
        $$ = $2;
    }
;

type_spec:
    T_INT { $$ = VT_INT; }
    | T_FLOAT { $$ = VT_FLOAT; }
    | T_CHAR { $$ = VT_CHAR; }
    | T_STRING { $$ = VT_STRING; }
;

%%
