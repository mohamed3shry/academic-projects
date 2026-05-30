#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include <string>
#include "parser.hpp"
#include "types.hpp"

// Forward declaration to break circular dependency
class CodeGenerator;

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual void print(int depth = 0) const = 0;
    virtual void generateCode(CodeGenerator &generator) = 0;
};

class ProgramNode : public ASTNode
{
private:
    std::vector<ASTNode *> statements;

public:
    ProgramNode() = default;
    ~ProgramNode()
    {
        for (ASTNode *s : statements)
            delete s;
    }

    void addStatement(ASTNode *stmt)
    {
        statements.push_back(stmt);
    }

    const std::vector<ASTNode *> &getStatements() const
    {
        return statements;
    }

    void print(int depth = 0) const override
    {
        std::cout << "Program" << std::endl;
        for (auto s : statements)
            s->print(depth + 1);
    }

    void generateCode(CodeGenerator &generator) override;
};

class PrintStatementNode : public ASTNode
{
    ASTNode *expression;

public:
    explicit PrintStatementNode(ASTNode *expr) : expression(expr) {}
    ~PrintStatementNode() { delete expression; }

    void print(int depth = 0) const override
    {
        for (int i = 0; i < depth; i++)
            std::cout << "  ";
        std::cout << "Print" << std::endl;
        if (expression)
            expression->print(depth + 1);
    }

    void generateCode(CodeGenerator &generator) override;
};

class VarDeclarationNode : public ASTNode
{
    std::string identifier;
    ValueType varType;
    ASTNode *expression;

public:
    VarDeclarationNode(const std::string &id, ValueType t, ASTNode *expr) : identifier(id), varType(t), expression(expr) {}
    ~VarDeclarationNode() { delete expression; }

    void print(int depth = 0) const override
    {
        for (int i = 0; i < depth; i++)
            std::cout << "  ";
        std::cout << "Var " << identifier << " = " << std::endl;
        if (expression)
            expression->print(depth + 1);
    }

    void generateCode(CodeGenerator &generator) override;
};

class AssignmentNode : public ASTNode
{
    std::string identifier;
    ASTNode *expression;

public:
    AssignmentNode(const std::string &id, ASTNode *expr) : identifier(id), expression(expr) {}
    ~AssignmentNode() { delete expression; }

    void print(int depth = 0) const override
    {
        for (int i = 0; i < depth; i++)
            std::cout << "  ";
        std::cout << "Assign " << identifier << " = " << std::endl;
        if (expression)
            expression->print(depth + 1);
    }

    void generateCode(CodeGenerator &generator) override;
};

class IfStatementNode : public ASTNode
{
    ASTNode *condition;
    ASTNode *thenBlock;
    ASTNode *elseBlock;

public:
    IfStatementNode(ASTNode *cond, ASTNode *then, ASTNode *els = nullptr)
        : condition(cond), thenBlock(then), elseBlock(els) {}
    ~IfStatementNode()
    {
        delete condition;
        delete thenBlock;
        if (elseBlock)
            delete elseBlock;
    }

    void print(int depth = 0) const override
    {
        for (int i = 0; i < depth; i++)
            std::cout << "  ";
        std::cout << "If" << std::endl;
        if (condition)
            condition->print(depth + 1);
        if (thenBlock)
            thenBlock->print(depth + 1);
        if (elseBlock)
        {
            for (int i = 0; i < depth; i++)
                std::cout << "  ";
            std::cout << "Else" << std::endl;
            elseBlock->print(depth + 1);
        }
    }

    void generateCode(CodeGenerator &generator) override;
};

class WhileStatementNode : public ASTNode
{
    ASTNode *condition;
    ASTNode *body;

public:
    WhileStatementNode(ASTNode *cond, ASTNode *blk) : condition(cond), body(blk) {}
    ~WhileStatementNode()
    {
        delete condition;
        delete body;
    }

    void print(int depth = 0) const override
    {
        for (int i = 0; i < depth; i++)
            std::cout << "  ";
        std::cout << "While" << std::endl;
        if (condition)
            condition->print(depth + 1);
        if (body)
            body->print(depth + 1);
    }

    void generateCode(CodeGenerator &generator) override;
};

class ExpressionNode : public ASTNode
{
public:
    enum Type
    {
        IDENTIFIER,
        INTEGER,
        BINARY_OP,
        COMPARISON,
        UNARY_OP,
        FLOAT_LITERAL,
        CHAR_LITERAL,
        STRING_LITERAL
    };
    Type type;
    std::string value; // for identifier or integer
    int intValue;      // for integer
    int op;            // token type for operator
    ASTNode *left;
    ASTNode *right;
    ValueType exprType;

    ExpressionNode(const std::string &id, ValueType t)
        : type(IDENTIFIER), value(id), intValue(0), op(0), left(nullptr), right(nullptr), exprType(t) {}

    ExpressionNode(int val)
        : type(INTEGER), value(""), intValue(val), op(0), left(nullptr), right(nullptr), exprType(VT_INT) {}

    ExpressionNode(double val)
        : type(FLOAT_LITERAL), value(""), intValue(0), op(0), left(nullptr), right(nullptr), exprType(VT_FLOAT)
    {
        value = std::to_string(val);
    }

    ExpressionNode(char ch)
        : type(CHAR_LITERAL), value(""), intValue(static_cast<int>(ch)), op(0), left(nullptr), right(nullptr), exprType(VT_CHAR) {}

    ExpressionNode(const std::string &lit, bool isStringLiteral)
        : type(STRING_LITERAL), value(lit), intValue(0), op(0), left(nullptr), right(nullptr), exprType(VT_STRING) {}

    ExpressionNode(int opType, ASTNode *l, ASTNode *r)
        : type(BINARY_OP), value(""), intValue(0), op(opType), left(l), right(r), exprType(VT_UNKNOWN) {}

    ExpressionNode(int opType, ASTNode *l, ASTNode *r, bool isComparison)
        : type(isComparison ? COMPARISON : BINARY_OP), value(""), intValue(0), op(opType), left(l), right(r), exprType(VT_UNKNOWN) {}

    static ExpressionNode *makeUnary(int opType, ASTNode *operand, bool isComparison = false)
    {
        auto *node = new ExpressionNode(opType, nullptr, operand, isComparison);
        node->type = UNARY_OP;
        node->exprType = VT_UNKNOWN;
        return node;
    }

    ~ExpressionNode()
    {
        if (left)
            delete left;
        if (right)
            delete right;
    }

    static std::string opToString(int op)
    {
        switch (op)
        {
        case T_PLUS:
            return "+";
        case T_MINUS:
            return "-";
        case T_MUL:
            return "*";
        case T_DIV:
            return "/";
        case T_MOD:
            return "%";
        case T_EQ:
            return "==";
        case T_NE:
            return "!=";
        case T_LT:
            return "<";
        case T_GT:
            return ">";
        case T_LE:
            return "<=";
        case T_GE:
            return ">=";
        case T_AND:
            return "&&";
        case T_OR:
            return "||";
        case T_NOT:
            return "!";
        default:
            return "?";
        }
    }

    void print(int depth = 0) const override
    {
        for (int i = 0; i < depth; i++)
            std::cout << "  ";
        if (type == IDENTIFIER)
        {
            std::cout << "Identifier: " << value << std::endl;
        }
        else if (type == INTEGER)
        {
            std::cout << "Integer: " << intValue << std::endl;
        }
        else if (type == FLOAT_LITERAL)
        {
            std::cout << "Float: " << value << std::endl;
        }
        else if (type == CHAR_LITERAL)
        {
            std::cout << "Char: '" << static_cast<char>(intValue) << "'" << std::endl;
        }
        else if (type == STRING_LITERAL)
        {
            std::cout << "String: " << value << std::endl;
        }
        else
        {
            std::cout << (type == COMPARISON ? "Comparison" : (type == UNARY_OP ? "UnaryOp" : "BinaryOp")) << ": " << opToString(op) << std::endl;
            if (left)
                left->print(depth + 1);
            if (right)
                right->print(depth + 1);
        }
    }

    void generateCode(CodeGenerator &generator) override;
};

#endif
