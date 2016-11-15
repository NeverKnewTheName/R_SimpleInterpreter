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
    if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
        qDebug() << __PRETTY_FUNCTION__ << ": NOT EOF";
    else
        qDebug() << __PRETTY_FUNCTION__ << ": EOF";
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
        lexer->LexErrorAtToken(CurrentToken,0);
    }
    else
    {
        qDebug() << "Eating: " << tokenType;
        CurrentToken = lexer->getNextToken();
    }
}

SimpleNode *SimpleParser::Expression()
{
    SimpleNode *node = ConditionalExpression();
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::ConditionalExpression()
{
    SimpleNode *node = LogicalORExpression();
    SharedSimpleTokenPtr token = CurrentToken;
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

SimpleNode *SimpleParser::LogicalORExpression()
{
    SimpleNode *node = LogicalXORExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalOR:
            eat(SimpleToken::LogicalOR);
            node = new LogicalORNode(node, LogicalXORExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::LogicalXORExpression()
{
    SimpleNode *node = LogicalANDExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalXOR:
            eat(SimpleToken::LogicalXOR);
            node = new LogicalXORNode(node, LogicalANDExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::LogicalANDExpression()
{
    SimpleNode *node = BitwiseORExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalAND:
            eat(SimpleToken::LogicalAND);
            node = new LogicalANDNode(node, BitwiseORExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::BitwiseORExpression()
{
    SimpleNode *node = BitwiseXORExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseOR:
            eat(SimpleToken::BitwiseOR);
            node = new ORNode(node, BitwiseXORExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::BitwiseXORExpression()
{
    SimpleNode *node = BitwiseANDExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseXOR:
            eat(SimpleToken::BitwiseXOR);
            node = new XORNode(node, BitwiseANDExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::BitwiseANDExpression()
{
    SimpleNode *node = EqualityExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseAND:
            eat(SimpleToken::BitwiseAND);
            node = new ANDNode(node, EqualityExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::EqualityExpression()
{
    SimpleNode *node = RelationalExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Equal:
            eat(SimpleToken::Equal);
            node = new EqualNode(node, RelationalExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::Unequal:
            eat(SimpleToken::Unequal);
            node = new UnequalNode(node, RelationalExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::RelationalExpression()
{
    SimpleNode *node = ShiftExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Greater:
            eat(SimpleToken::Greater);
            node = new GreaterNode(node, ShiftExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::Lower:
            eat(SimpleToken::Lower);
            node = new LowerNode(node, ShiftExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::EqualOrGreater:
            eat(SimpleToken::EqualOrGreater);
            node = new EqualOrGreaterNode(node, ShiftExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::EqualOrLower:
            eat(SimpleToken::EqualOrLower);
            node = new EqualOrLowerNode(node, ShiftExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::ShiftExpression()
{
    SimpleNode *node = AdditiveExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LeftShift:
            eat(SimpleToken::LeftShift);
            node = new LeftShiftNode(node, AdditiveExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::RightShift:
            eat(SimpleToken::RightShift);
            node = new RightShiftNode(node, AdditiveExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::AdditiveExpression()
{
    SimpleNode *node = MultiplicativeExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Plus:
            eat(SimpleToken::Plus);
            node = new AdditionNode(node, MultiplicativeExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::Minus:
            eat(SimpleToken::Minus);
            node = new SubtractionNode(node, MultiplicativeExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::MultiplicativeExpression()
{
    SimpleNode *node = UnaryExpression();
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Multiplication:
            eat(SimpleToken::Multiplication);
            node = new MultiplicationNode(node, UnaryExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::Division:
            eat(SimpleToken::Division);
            node = new DivisionNode(node, UnaryExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        case SimpleToken::Modulo:
            eat(SimpleToken::Modulo);
            node = new ModuloNode(node, UnaryExpression());
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                lexer->LexErrorAtToken(token,1);
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::UnaryExpression()
{
    SimpleNode *node = NULL;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Plus:
        eat(SimpleToken::Plus);
        node = new PositiveNode(UnaryExpression());
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            lexer->LexErrorAtToken(token,1);
        }
        break;
    case SimpleToken::Minus:
        eat(SimpleToken::Minus);
        node = new NegativeNode(UnaryExpression());
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            lexer->LexErrorAtToken(token,1);
        }
        break;
    case SimpleToken::LogicalNegation:
        eat(SimpleToken::LogicalNegation);
        node = new LogicalNegationNode(UnaryExpression());
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            lexer->LexErrorAtToken(token,1);
        }
        break;
    case SimpleToken::OnesComplement:
        eat(SimpleToken::OnesComplement);
        node = new OnesComplementNode(UnaryExpression());
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            lexer->LexErrorAtToken(token,1);
        }
        break;
    default:
        node = PostFixExpression();
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::PostFixExpression()
{
    SimpleNode *node = PrimaryExpression();
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::PrimaryExpression()
{
    SimpleNode *node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Integer:
        eat(SimpleToken::Integer);
        node = new ValueNode(qSharedPointerDynamicCast<IntegerToken>(token)->getValue());
        break;
    case SimpleToken::Double:
        eat(SimpleToken::Double);
        node = new ValueNode(qSharedPointerDynamicCast<DoubleToken>(token)->getValue());
        break;
    case SimpleToken::Bool:
        eat(SimpleToken::Bool);
        node = new ValueNode(qSharedPointerDynamicCast<BoolToken>(token)->getValue());
        break;
    case SimpleToken::String:
        eat(SimpleToken::String);
        node = new ValueNode(qSharedPointerDynamicCast<StringToken>(token)->getValue());
        break;
    case SimpleToken::Data:
        eat(SimpleToken::Data);
        node = new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), &SymblTbl);
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
