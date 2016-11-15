#include "simpleparser.h"

#include <QDebug>

SimpleParser::SimpleParser(SimpleLexer *lexer) :
    lexer(lexer),
    CurrentToken(lexer->getNextToken())
{
}

SimpleNode *SimpleParser::parse()
{
    SimpleNode *node = Expression();
    return node;
}

SymbolTable &SimpleParser::getSymblTbl()
{
    return SymblTbl;
}

void SimpleParser::eat(SimpleToken::TokenType tokenType)
{
    if(CurrentToken->getTokenType() != tokenType)
    {
        qDebug() << __PRETTY_FUNCTION__ << ": ERROR";
//        CurrentToken = new EOFToken();
    }
    else
    {
        qDebug() << "Eating: " << tokenType;
        CurrentToken = lexer->getNextToken();
    }
}

SimpleNode * const SimpleParser::Expression()
{
    SimpleNode *node = ConditionalExpression();

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::ConditionalExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = LogicalORExpression();
    //    bool ContinueLoop = true;

    //    do
    //    {
    //        switch(CurrentToken->getTokenType())
    //        {
    //        case SimpleToken::LogicalOR:
    //            eat(SimpleToken::LogicalOR);
    //            node = LogicalORNode(node, LogicalORExpression());
    //            break;
    //        default:
    //            ContinueLoop = false;
    //        }
    //    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::LogicalORExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = LogicalXORExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalOR:
            eat(SimpleToken::LogicalOR);
            node = new LogicalORNode(node, LogicalXORExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::LogicalXORExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = LogicalANDExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalXOR:
            eat(SimpleToken::LogicalXOR);
            node = new LogicalXORNode(node, LogicalANDExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::LogicalANDExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = BitwiseORExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalAND:
            eat(SimpleToken::LogicalAND);
            node = new LogicalANDNode(node, BitwiseORExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::BitwiseORExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = BitwiseXORExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseOR:
            eat(SimpleToken::BitwiseOR);
            node = new ORNode(node, BitwiseXORExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::BitwiseXORExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = BitwiseANDExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseXOR:
            eat(SimpleToken::BitwiseXOR);
            node = new XORNode(node, BitwiseANDExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::BitwiseANDExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = EqualityExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseAND:
            eat(SimpleToken::BitwiseAND);
            node = new ANDNode(node, EqualityExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::EqualityExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = RelationalExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Equal:
            eat(SimpleToken::Equal);
            node = new EqualNode(node, RelationalExpression());
            break;
        case SimpleToken::Unequal:
            eat(SimpleToken::Unequal);
            node = new UnequalNode(node, RelationalExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::RelationalExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = ShiftExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Greater:
            eat(SimpleToken::Greater);
            node = new GreaterNode(node, ShiftExpression());
            break;
        case SimpleToken::Lower:
            eat(SimpleToken::Lower);
            node = new LowerNode(node, ShiftExpression());
            break;
        case SimpleToken::EqualOrGreater:
            eat(SimpleToken::EqualOrGreater);
            node = new EqualOrGreaterNode(node, ShiftExpression());
            break;
        case SimpleToken::EqualOrLower:
            eat(SimpleToken::EqualOrLower);
            node = new EqualOrLowerNode(node, ShiftExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::ShiftExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = AdditiveExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LeftShift:
            eat(SimpleToken::LeftShift);
            node = new LeftShiftNode(node, AdditiveExpression());
            break;
        case SimpleToken::RightShift:
            eat(SimpleToken::RightShift);
            node = new RightShiftNode(node, AdditiveExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::AdditiveExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = MultiplicativeExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Plus:
            eat(SimpleToken::Plus);
            node = new AdditionNode(node, MultiplicativeExpression());
            break;
        case SimpleToken::Minus:
            eat(SimpleToken::Minus);
            node = new SubtractionNode(node, MultiplicativeExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::MultiplicativeExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = UnaryExpression();
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Multiplication:
            eat(SimpleToken::Multiplication);
            node = new MultiplicationNode(node, UnaryExpression());
            break;
        case SimpleToken::Division:
            eat(SimpleToken::Division);
            node = new DivisionNode(node, UnaryExpression());
            break;
        case SimpleToken::Modulo:
            eat(SimpleToken::Modulo);
            node = new ModuloNode(node, UnaryExpression());
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::UnaryExpression()
{
    SimpleToken *token = CurrentToken;
    SimpleNode *node = NULL;
    bool ContinueLoop = true;

    do
    {
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Plus:
            eat(SimpleToken::Plus);
            node = new PositiveNode(UnaryExpression());
            break;
        case SimpleToken::Minus:
            eat(SimpleToken::Minus);
            node = new NegativeNode(UnaryExpression());
            break;
        case SimpleToken::LogicalNegation:
            eat(SimpleToken::LogicalNegation);
            node = new LogicalNegationNode(UnaryExpression());
            break;
        case SimpleToken::OnesComplement:
            eat(SimpleToken::OnesComplement);
            node = new OnesComplementNode(UnaryExpression());
            break;
        default:
            if(node == NULL)
                node = PostFixExpression();
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::PostFixExpression()
{
    SimpleNode *node = PrimaryExpression();

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode * const SimpleParser::PrimaryExpression()
{
    SimpleNode *node;
    SimpleToken *token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Integer:
        eat(SimpleToken::Integer);
        node = new ValueNode(dynamic_cast<IntegerToken*>(token)->getValue());
        break;
    case SimpleToken::Double:
        eat(SimpleToken::Double);
        node = new ValueNode(dynamic_cast<DoubleToken*>(token)->getValue());
        break;
    case SimpleToken::Bool:
        eat(SimpleToken::Bool);
        node = new ValueNode(dynamic_cast<BoolToken*>(token)->getValue());
        break;
    case SimpleToken::String:
        eat(SimpleToken::String);
        node = new ValueNode(dynamic_cast<StringToken*>(token)->getValue());
        break;
    case SimpleToken::Data:
        eat(SimpleToken::Data);
        node = new DataNode(dynamic_cast<DataToken*>(token)->getDataIndex(), &SymblTbl);
        break;
    case SimpleToken::LParan:
        eat(SimpleToken::LParan);
        node = Expression();
        eat(SimpleToken::RParan);
        break;
    default:
        qDebug() << __PRETTY_FUNCTION__ << "EOF reached";
        node = new EOFNode();
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}


