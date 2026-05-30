#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <fstream>
#include <string>
#include <sstream>
#include "ast.hpp"

class CodeGenerator
{
private:
    std::ofstream output_file;
    int tempCounter;

    std::string getOpString(int op)
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

public:
    CodeGenerator(const std::string &filename) : output_file(filename), tempCounter(0)
    {
        output_file << "#include <stdio.h>\n";
        output_file << "int main() {\n";
    }

    ~CodeGenerator()
    {
        output_file << "return 0;\n";
        output_file << "}\n";
    }

    void generateDeclaration(const std::string &id, ValueType t)
    {
        const char *ctype = "int";
        switch (t)
        {
        case VT_INT:
            ctype = "int";
            break;
        case VT_FLOAT:
            ctype = "float";
            break;
        case VT_CHAR:
            ctype = "char";
            break;
        case VT_STRING:
            ctype = "const char*";
            break;
        default:
            ctype = "int";
            break;
        }
        output_file << "    " << ctype << " " << id << ";\n";
    }

    void generateAssignment(const std::string &id, const std::string &expr)
    {
        output_file << "    " << id << " = " << expr << ";\n";
    }

    void generatePrint(ASTNode *expr)
    {
        ExpressionNode *exprNode = dynamic_cast<ExpressionNode *>(expr);
        std::string fmt = "%d";
        if (exprNode)
        {
            switch (exprNode->exprType)
            {
            case VT_FLOAT:
                fmt = "%f";
                break;
            case VT_CHAR:
                fmt = "%c";
                break;
            case VT_STRING:
                fmt = "%s";
                break;
            default:
                fmt = "%d";
                break;
            }
        }
        const std::string value = generateExpression(expr);
        output_file << "    printf(\"" << fmt << "\\n\", " << value << ");\n";
    }

    std::string generateExpression(ASTNode *expr)
    {
        ExpressionNode *exprNode = dynamic_cast<ExpressionNode *>(expr);
        if (!exprNode)
            return "0";

        if (exprNode->type == ExpressionNode::IDENTIFIER)
        {
            return exprNode->value;
        }
        else if (exprNode->type == ExpressionNode::INTEGER)
        {
            return std::to_string(exprNode->intValue);
        }
        else if (exprNode->type == ExpressionNode::FLOAT_LITERAL)
        {
            return exprNode->value;
        }
        else if (exprNode->type == ExpressionNode::CHAR_LITERAL)
        {
            char c = static_cast<char>(exprNode->intValue);
            std::string s;
            s.push_back('\'');
            s.push_back(c);
            s.push_back('\'');
            return s;
        }
        else if (exprNode->type == ExpressionNode::STRING_LITERAL)
        {
            return exprNode->value;
        }
        else
        {
            if (exprNode->type == ExpressionNode::UNARY_OP)
            {
                std::string right = generateExpression(exprNode->right);
                return "(" + getOpString(exprNode->op) + right + ")";
            }
            std::string left = generateExpression(exprNode->left);
            std::string right = generateExpression(exprNode->right);
            return "(" + left + " " + getOpString(exprNode->op) + " " + right + ")";
        }
    }

    void generateIf(ASTNode *condition, ASTNode *thenBlock, ASTNode *elseBlock)
    {
        std::string cond = generateExpression(condition);
        output_file << "    if(" << cond << ") {\n";
        if (thenBlock)
        {
            thenBlock->generateCode(*this);
        }
        output_file << "    }";
        if (elseBlock)
        {
            output_file << " else {\n";
            elseBlock->generateCode(*this);
            output_file << "    }";
        }
        output_file << "\n";
    }

    void generateWhile(ASTNode *condition, ASTNode *body)
    {
        std::string cond = generateExpression(condition);
        output_file << "    while(" << cond << ") {\n";
        if (body)
        {
            body->generateCode(*this);
        }
        output_file << "    }\n";
    }

    std::ofstream &getStream() { return output_file; }
};

#endif
