#include "simpleparser.h"

#include <QDebug>

SimpleParser::SimpleParser(SimpleLexer *lexer) :
    lexer(lexer),
    CurrentToken(lexer->getNextToken())
{
}

ValueNode *SimpleParser::parse()
{
    ValueNode *node = Expression();
    if(node->getNodeType() != SimpleNode::EOFNode)
        qDebug() << __PRETTY_FUNCTION__ << ": ERROR";
    return node;
}

SymbolTable &SimpleParser::getSymblTbl()
{
    return SymblTbl;
}

void SimpleParser::eat(const SimpleToken &token, SimpleToken::TokenType tokenType)
{
    if(token->getTokenType() != tokenType)
        qDebug() << __PRETTY_FUNCTION__ << ": ERROR";
    else
        CurrentToken = token;
}

const ASTNode * const SimpleParser::Expression()
{

}

const ASTNode * const SimpleParser::ConditionalExpression()
{

}

const ASTNode * const SimpleParser::LogicalORExpression()
{

}

const ASTNode * const SimpleParser::LogicalXORExpression()
{

}

const ASTNode * const SimpleParser::LogicalANDExpression()
{

}

const ASTNode * const SimpleParser::BitwiseORExpression()
{

}

const ASTNode * const SimpleParser::BitwiseXORExpression()
{

}

const ASTNode * const SimpleParser::BitwiseANDExpression()
{

}

const ASTNode * const SimpleParser::EqualityExpression()
{

}

const ASTNode * const SimpleParser::RelationalExpression()
{

}

const ASTNode * const SimpleParser::ShiftExpression()
{

}

const ASTNode * const SimpleParser::AdditiveExpression()
{

}

const ASTNode * const SimpleParser::MultiplicativeExpression()
{

}

const ASTNode * const SimpleParser::UnaryExpression()
{

}

const ASTNode * const SimpleParser::PostFixExpression()
{

}

const ASTNode * const SimpleParser::PrimaryExpression()
{

}


