#include "ast.hpp"
#include "code_generator.hpp"

// Implementation of generateCode methods that need full CodeGenerator definition

void PrintStatementNode::generateCode(CodeGenerator &generator)
{
    generator.generatePrint(expression);
}

void VarDeclarationNode::generateCode(CodeGenerator &generator)
{
    generator.generateDeclaration(identifier, varType);
    if (expression)
    {
        ExpressionNode *exprNode = dynamic_cast<ExpressionNode *>(expression);
        if (exprNode)
        {
            std::string expr = generator.generateExpression(expression);
            generator.generateAssignment(identifier, expr);
        }
    }
}

void AssignmentNode::generateCode(CodeGenerator &generator)
{
    ExpressionNode *exprNode = dynamic_cast<ExpressionNode *>(expression);
    if (exprNode)
    {
        std::string expr = generator.generateExpression(expression);
        generator.generateAssignment(identifier, expr);
    }
}

void IfStatementNode::generateCode(CodeGenerator &generator)
{
    generator.generateIf(condition, thenBlock, elseBlock);
}
void WhileStatementNode::generateCode(CodeGenerator &generator)
{
    generator.generateWhile(condition, body);
}

void ProgramNode::generateCode(CodeGenerator &generator)
{
    for (auto s : statements)
        s->generateCode(generator);
}

void ExpressionNode::generateCode(CodeGenerator &generator)
{
    // ExpressionNode's generateCode is empty - expressions are handled
    // by CodeGenerator::generateExpression() method
    // This implementation is here to ensure vtable is complete
}
