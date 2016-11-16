#include "simpleparser.h"

#include <QDebug>

SimpleParser::SimpleParser(SimpleLexer *lexer) :
    lexer(lexer),
    CurrentToken(lexer->getNextToken()),
    ErrorOccured(false)
{
}

SimpleNode *SimpleParser::parse()
{
    SimpleNode *node = NULL;
    if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
    {
        node = Expression();

        if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
        {
            qDebug() << __PRETTY_FUNCTION__ << ": NOT EOF";
            if(node != NULL)
            {
                delete node;
                EOFExpectedError(CurrentToken);
                return NULL;
            }
        }

    }
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
        SyntacticError(CurrentToken);
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
    if(node == NULL)
    {
        return NULL;
    }
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::ConditionalExpression()
{
    SimpleNode *node = LogicalORExpression();
    if(node == NULL)
    {
        return NULL;
    }
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalOR:
            eat(SimpleToken::LogicalOR);
            nodeTwo = LogicalXORExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new LogicalORNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalXOR:
            eat(SimpleToken::LogicalXOR);
            nodeTwo = LogicalANDExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new LogicalXORNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalAND:
            eat(SimpleToken::LogicalAND);
            nodeTwo = BitwiseORExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new LogicalANDNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseOR:
            eat(SimpleToken::BitwiseOR);
            nodeTwo = BitwiseXORExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new ORNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseXOR:
            eat(SimpleToken::BitwiseXOR);
            nodeTwo = BitwiseANDExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new XORNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseAND:
            eat(SimpleToken::BitwiseAND);
            nodeTwo = EqualityExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new ANDNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Equal:
            eat(SimpleToken::Equal);
            nodeTwo = RelationalExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new EqualNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::Unequal:
            eat(SimpleToken::Unequal);
            nodeTwo = RelationalExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new UnequalNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Greater:
            eat(SimpleToken::Greater);
            nodeTwo = ShiftExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new GreaterNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::Lower:
            eat(SimpleToken::Lower);
            nodeTwo = ShiftExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new LowerNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::EqualOrGreater:
            eat(SimpleToken::EqualOrGreater);
            nodeTwo = ShiftExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new EqualOrGreaterNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::EqualOrLower:
            eat(SimpleToken::EqualOrLower);
            nodeTwo = ShiftExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new EqualOrLowerNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LeftShift:
            eat(SimpleToken::LeftShift);
            nodeTwo = AdditiveExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new LeftShiftNode(node, nodeTwo);
            if(node == NULL)
                return node;
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::RightShift:
            eat(SimpleToken::RightShift);
            nodeTwo = AdditiveExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new RightShiftNode(node, nodeTwo);
            if(node == NULL)
            {
                return NULL;
                delete nodeTwo;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Plus:
            eat(SimpleToken::Plus);
            nodeTwo = MultiplicativeExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new AdditionNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::Minus:
            eat(SimpleToken::Minus);
            nodeTwo = MultiplicativeExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new SubtractionNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
    if(node == NULL)
    {
        return NULL;
    }
    SimpleNode *nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Multiplication:
            eat(SimpleToken::Multiplication);
            nodeTwo = UnaryExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new MultiplicationNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::Division:
            eat(SimpleToken::Division);
            nodeTwo = UnaryExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new DivisionNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
            }
            break;
        case SimpleToken::Modulo:
            eat(SimpleToken::Modulo);
            nodeTwo = UnaryExpression();
            if(nodeTwo == NULL)
            {
                SyntacticError(CurrentToken);
                delete node;
                return NULL;
            }
            node = new ModuloNode(node, nodeTwo);
            if(node == NULL)
            {
                delete nodeTwo;
                return NULL;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token);
                delete node;
                return NULL;
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
        node = UnaryExpression();
        if(node == NULL)
        {
            SyntacticError(CurrentToken);
            return NULL;
        }
        node = new PositiveNode(node);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token);
            delete node;
            return NULL;
        }
        break;
    case SimpleToken::Minus:
        eat(SimpleToken::Minus);
        node = UnaryExpression();
        if(node == NULL)
        {
            SyntacticError(CurrentToken);
            return NULL;
        }
        node = new NegativeNode(node);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token);
            delete node;
            return NULL;
        }
        break;
    case SimpleToken::LogicalNegation:
        eat(SimpleToken::LogicalNegation);
        node = UnaryExpression();
        if(node == NULL)
        {
            SyntacticError(CurrentToken);
            return NULL;
        }
        node = new LogicalNegationNode(node);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token);
            delete node;
            return NULL;
        }
        break;
    case SimpleToken::OnesComplement:
        eat(SimpleToken::OnesComplement);
        node = UnaryExpression();
        if(node == NULL)
        {
            SyntacticError(CurrentToken);
            return NULL;
        }
        node = new OnesComplementNode(node);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token);
            delete node;
            return NULL;
        }
        break;
    case SimpleToken::TypeCast:
    {
        eat(SimpleToken::TypeCast);
        SimpleToken::TokenType typeToCastToTokenType = qSharedPointerDynamicCast<TypeCastToken>(token)->getTypeToCastTo();
        SimpleNode::ValueTypes typeToCastToValueType = SimpleNode::Integer;
        switch(typeToCastToTokenType)
        {
        case SimpleToken::Integer:
            typeToCastToValueType = SimpleNode::Integer;
            break;
        case SimpleToken::Double:
            typeToCastToValueType = SimpleNode::Double;
            break;
        case SimpleToken::Bool:
            typeToCastToValueType = SimpleNode::Bool;
            break;
        case SimpleToken::String:
            typeToCastToValueType = SimpleNode::String;
            break;
        default:
            break;
        }
        node = UnaryExpression();
        if(node == NULL)
        {
            SyntacticError(CurrentToken);
            return NULL;
        }
        node = new TypeCastNode(node, typeToCastToValueType);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token);
            delete node;
            return NULL;
        }
    }
        break;
    default:
        node = PostFixExpression();
        if(node == NULL)
        {
            return NULL;
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::PostFixExpression()
{
    SimpleNode *node = PrimaryExpression();
    if(node == NULL)
    {
        return NULL;
    }
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
        if(node == NULL)
        {
            return NULL;
        }
        eat(SimpleToken::RParan);
        break;
    case SimpleToken::EOFToken:
        qDebug() << __PRETTY_FUNCTION__ << "EOF reached too soon!!!";
        CurrentToken = SharedSimpleTokenPtr(new EOFToken(CurrentToken->getTokenPos(),0));
        EOFUnexpectedError(CurrentToken);
        return NULL;
        break;
    default:
//        SyntacticError(CurrentToken);
        return NULL;
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}


void SimpleParser::SyntacticError(SharedSimpleTokenPtr Token)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 0);
}

void SimpleParser::TypeError(SharedSimpleTokenPtr Token)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 1);
}

void SimpleParser::EOFUnexpectedError(SharedSimpleTokenPtr Token)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 2);
}

void SimpleParser::EOFExpectedError(SharedSimpleTokenPtr Token)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 3);
}
