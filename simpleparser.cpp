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
        node = Program();

        if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
        {
            qDebug() << __PRETTY_FUNCTION__ << ": NOT EOF";
            if(node != NULL)
            {
                delete node;
                EOFExpectedError(CurrentToken, QString("EOF was expected but not there was still input..."));
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
        TypeError(CurrentToken,QString("Expected: %1").arg(SimpleToken::convertTokenTypeToString(tokenType)));
    }
    else
    {
        qDebug() << "Eating: " << tokenType;
        CurrentToken = lexer->getNextToken();
    }
}

SimpleNode *SimpleParser::Program()
{
    SimpleNode *node = Function();
    if(node == NULL)
    {
        return NULL;
    }
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::Function()
{
    SimpleNode *node = NULL;
    SharedSimpleTokenPtr token;
    if(CurrentToken->getTokenType() == SimpleToken::FunctionName)
    {
        token = CurrentToken;
        eat(SimpleToken::FunctionName);t
                //        QString FuncName = qSharedPointerDynamicCast<FunctionNameToken>(token)->getFuncName();
                token = CurrentToken;
        eat(SimpleToken::TypeName);
        SimpleNode::NodeType returnType = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
        eat(SimpleToken::LParan);
        SymbolTable *FuncSubSymblTbl = new SymbolTable();
        SymblTbl.addEntry(QString("FuncSubSymblTbl_%1").arg(FuncName), SymbolTableEntry(FuncSubSymblTbl));
        QVector<VariableNode *> paramVariables;
        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            token = CurrentToken;
            eat(SimpleToken::TypeName);
            SimpleToken::TokenType type = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            token = CurrentToken;
            eat(SimpleToken::Variable);
            paramVariables.append(new VariableNode(qSharedPointerDynamicCast<VariableToken>(token)->getVariableName(),FuncSubSymblTbl,type));
        }

        eat(SimpleToken::RParan);
        eat(SimpleToken::LCurlyParan);
        QVector<VariableNode*> FuncVariables;
        SimpleNode * VarDeclNode = NULL;
        do
        {
            VarDeclNode = VarDeclaration();
            if(ExpressionNode != NULL)
            {
                FuncVariables.append(VarDeclNode);
            }
        }while(VarDeclNode != NULL);

        QVector<SimpleNode*> FuncExpressions;
        SimpleNode * ExpressionNode = NULL;
        do
        {
            ExpressionNode = Expression();
            if(ExpressionNode != NULL)
            {
                eat(SimpleToken::SemiColonDelim);
                FuncExpressions.append(ExpressionNode);
            }
        }while(ExpressionNode != NULL);
        SimpleNode *ReturnStatementNode = ReturnStatement();

        //ToDO CREATE FUNCTION NODE
        //ADD VariablesDeclarations and ParameterVariables to SubSymbolTable of the Function; ADD GLOBAL SYMBOL TABLE TO FUNCTION;
        eat(SimpleToken::RCurlyParan);
    }
    else
    {
        SyntacticError(CurrentToken, QString("Expected FunctionDeclaration!"));
    }


    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::VarDeclaration()
{
    SimpleNode *node;
    SharedSimpleTokenPtr token;

    if(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        token = CurrentToken;
        eat(SimpleToken::TypeName);
        if(CurrentToken->getTokenType() == SimpleToken::Variable)
        {
            SimpleToken::TokenType type = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            token = CurrentToken;
            eat(SimpleToken::Variable);
            eat(SimpleToken::SemiColonDelim);
            node = new VariableNode(qSharedPointerDynamicCast<VariableToken>(token)->getVariableName(),&SymblTbl, type);

            return node;
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::ReturnStatement()
{
    SimpleNode *node = NULL;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::ReturnKeyword)
    {
        eat(SimpleToken::ReturnKeyword);
        node = Expression();
        if(node == NULL)
        {
            eat(SimpleToken::SemiColonDelim);
        }
        node = ReturnNode(node);
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::Expression()
{
    SimpleNode *node = AssignmentExpression();
    if(node == NULL)
    {
        return NULL;
    }
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::AssignmentExpression()
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
                TypeError(token, QString("Expected: Integer || (Integer|Double|Bool) | Double || (Integer|Double|Bool) | Bool || (Integer|Double|Bool) ..."));
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
                TypeError(token, QString("Expected: Integer ^^ (Integer|Double|Bool) | Double ^^ (Integer|Double|Bool) | Bool ^^ (Integer|Double|Bool)..."));
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
                TypeError(token, QString("Expected: Integer && (Integer|Double|Bool) | Double && (Integer|Double|Bool) | Bool && (Integer|Double|Bool) ..."));
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
                TypeError(token, QString("Expected: Integer | Integer"));
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
                TypeError(token, QString("Expected: Integer ^ Integer"));
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
                TypeError(token, QString("Expected: Integer & Integer"));
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
                TypeError(token, QString("Expected: Integer == (Integer|Double)| Double == (Double|Integer) | Bool == (Integer|Double|Bool) | String == string"));
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
                TypeError(token, QString("Expected: Integer != (Integer|Double|Bool)| Double != (Double|Integer|Bool) | Bool != (Integer|Double|Bool) | String != string"));
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
                TypeError(token, QString("Expected: Integer > (Integer|Double)| Double > (Double|Integer)"));
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
                TypeError(token, QString("Expected: Integer < (Integer|Double)| Double < (Double|Integer)"));
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
                TypeError(token, QString("Expected: Integer >= (Integer|Double)| Double >= (Double|Integer)"));
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
                TypeError(token, QString("Expected: Integer <= (Integer|Double)| Double <= (Double|Integer)"));
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
                TypeError(token, QString("Expected: Integer"));
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
                TypeError(token, QString("Expected: Integer"));
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
                TypeError(token, QString("Expected: Integer + (Integer|Double) | Double + (Double|Integer) | String + (Integer|Double|Bool|String)"));
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
                TypeError(token, QString("Expected: Integer - (Integer|Double) | Double - (Double|Integer)"));
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
                TypeError(token, QString("Expected: Integer * (Integer|Double) | Double * (Double|Integer) | String * Integer"));
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
                TypeError(token, QString("Expected: Integer / (Integer|Double) | Double / (Double|Integer)"));
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
                TypeError(token, QString("Expected: Integer % Integer"));
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
    case SimpleToken::Increment:
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::Variable )
        {
            eat(SimpleToken::Increment);
            node = Symbol();
            if(node == NULL)
            {
                SyntacticError(CurrentToken);
                return NULL;
            }
            if(node->getNodeType() != SimpleNode::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return NULL;
            }
            node = new IncrementNode(node);
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer Variable"));
                delete node;
                return NULL;
            }
        }
        else
        {
            SyntacticError(CurrentToken, QString("PreIncrement -> Expected Variable!"));
            return NULL;
        }
        break;
    case SimpleToken::Decrement:
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::Variable )
        {
            eat(SimpleToken::Decrement);
            node = Symbol();
            if(node == NULL)
            {
                SyntacticError(CurrentToken);
                return NULL;
            }
            if(node->getNodeType() != SimpleNode::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return NULL;
            }
            node = new DecrementNode(node);
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer Variable"));
                delete node;
                return NULL;
            }
        }
        else
        {
            SyntacticError(CurrentToken, QString("PreDecrement -> Expected Variable!"));
            return NULL;
        }
        break;
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
            TypeError(token, QString("Expected: Integer | Double"));
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
            TypeError(token, QString("Expected: Integer | Double"));
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
            TypeError(token, QString("Expected: Integer | Double | Bool"));
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
            TypeError(token, QString("Expected: Integer"));
            delete node;
            return NULL;
        }
        break;
    case SimpleToken::LParan:
    {
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::TypeName)
        {
            eat(SimpleToken::LParan);
            token = CurrentToken;
            eat(SimpleToken::TypeName);
            eat(SimpleToken::RParan);
            node = UnaryExpression();
            if(node == NULL)
            {
                SyntacticError(CurrentToken);
                return NULL;
            }
            SimpleToken::TokenType typeToCastToTokenType = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
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
            node = new TypeCastNode(node, typeToCastToValueType);
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer and cast to (Integer|Double|Bool|String) | Double and cast to (Integer|Double|Bool|String) | Bool and cast to (Integer|Double|Bool|String) | String and cast to String"));
                delete node;
                return NULL;
            }
            break;
        }
    }
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
    if(node->getNodeType() == SimpleNode::Variable)
    {
        bool ContinueLoop = true;

        do
        {
            switch(CurrentToken->getTokenType())
            {
            case SimpleToken::Increment:
                token = CurrentToken;
                eat(SimpleToken::Increment);
                node = new IncrementNode(node);
                if(node->getReturnType() == ValueNode::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | IncrementExpression!"));
                    delete node;
                    return NULL;
                }
                break;
            case SimpleToken::Decrement:
                token = CurrentToken;
                eat(SimpleToken::Decrement);
                node = new DecrementNode(node);
                if(node->getReturnType() == ValueNode::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | DecrementExpression!"));
                    delete node;
                    return NULL;
                }
                break;
            default:
                ContinueLoop = false;
            }
        }while(ContinueLoop);

    }


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
        return Symbol();
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::Symbol()
{
    SimpleNode *node = NULL;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Data:
        eat(SimpleToken::Data);
        node = new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), &SymblTbl);
        break;
    case SimpleToken::Variable:
        eat(SimpleToken::Variable);
        node = new VariableNode(qSharedPointerDynamicCast<VariableToken>(token)->getVariableName(), &SymblTbl);
        break;
        //    case SimpleToken::Function:
        //        eat(SimpleToken::Data);
        //        node = new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), &SymblTbl);
        //        break;
    default:
        return NULL;
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}


void SimpleParser::SyntacticError(SharedSimpleTokenPtr Token, QString details)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 0, details);
}

void SimpleParser::TypeError(SharedSimpleTokenPtr Token, QString details)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 1, details);
}

void SimpleParser::EOFUnexpectedError(SharedSimpleTokenPtr Token, QString details)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 2, details);
}

void SimpleParser::EOFExpectedError(SharedSimpleTokenPtr Token, QString details)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 3, details);
}
