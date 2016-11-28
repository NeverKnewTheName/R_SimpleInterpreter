#include "simpleparser.h"
#include "simplesymboltable.h"
#include <QDebug>

SimpleParser::SimpleParser(SimpleLexer *lexer, const SymbolTable &parentSymblTbl) :
    lexer(lexer),
    CurrentToken(lexer->getNextToken()),
    ParentSymblTbl(parentSymblTbl),
    ProgramSymbolTable(QString("ProgramSymbolTable"),ParentSymblTbl),
    CurSymblTbl(NULL),
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

const SymbolTable &SimpleParser::getProgramSymblTbl()
{
    return ProgramSymbolTable;
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
    SimpleNode *node = NULL;
    SharedSimpleTokenPtr token = CurrentToken;
    SymbolTable ProgramSymbolTable(QString("ProgramSymblTbl"),ParentSymblTbl);

    ProgramNode program(QString("Program"), ProgramSymbolTable);

    // // // Variable or Function Definitions // // //
    while(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        // // // Variable Definition // // //
        token = CurrentToken;
        node = VarDefinition(&SymblTbl);
        if(node != NULL)
        {
            continue;
        }
        else
        {
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
        }
        // // // Function Definition // // //
        node = FunctionDefinition();
        if(node == NULL)
        {
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
            break;
        }
    }

    while(CurrentToken->getTokenType() != SimpleToken::ReturnKeyword)
    {
        token = CurrentToken;
        node = Expression();
        if(node == NULL)
        {
            SyntacticError(token, QString("Expected Expression!"));
            return NULL;
        }
    }

    node = ReturnStatement();
    if(node == NULL)
    {
        SyntacticError(token, QString("Expected Return Statement!"));
        return NULL;
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::FunctionDefinition()
{
    FunctionNode *node = FunctionDeclaration(/*FuncSubSymblTbl*/);

    if(node == NULL)
    {
        return NULL;
    }
    SharedSimpleTokenPtr token = CurrentToken;
    SymbolTable *FuncSubSymblTbl = node->getFuncSymbolTable();;
    CurSymblTbl = FuncSubSymblTbl;

    if(CurrentToken->getTokenType() == SimpleToken::LCurlyParan)
    {
        eat(SimpleToken::LCurlyParan);
        QVector<SimpleNode*> FuncExpressions;

        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            SimpleNode *varDef = VarDefinition(FuncSubSymblTbl);
            if(varDef->getNodeType() == SimpleNode::Assignment)
            {
                FuncExpressions.append(varDef);
            }
        }

        SimpleNode * ExpressionNode = NULL;
        do
        {
            ExpressionNode = Expression();
            if(ExpressionNode != NULL)
            {
                eat(SimpleToken::SemiColonDelim);
                FuncExpressions.append(ExpressionNode);
            }
            CurSymblTbl = FuncSubSymblTbl;
        }while(ExpressionNode != NULL);
        SimpleNode *ReturnStatementNode = ReturnStatement();

        eat(SimpleToken::RCurlyParan);

        node->addFuncExpressions(FuncExpressions);
        node->addReturnStatement(ReturnStatementNode);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            CurSymblTbl = &SymblTbl;
            delete node;
            delete FuncSubSymblTbl;
            return NULL;
        }

        eat(SimpleToken::SemiColonDelim);
    }
    else
    {
        CurSymblTbl = &SymblTbl;
        delete node;
        delete FuncSubSymblTbl;
        SyntacticError(CurrentToken, QString("Expected FunctionDeclaration!"));
        return NULL;
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    CurSymblTbl = &SymblTbl;
    return node;
}

FunctionNode *SimpleParser::FunctionDeclaration(/*SymbolTable *FuncSubSymblTbl*/)
{
    FunctionNode *node = NULL;
    SharedSimpleTokenPtr token;

    if(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        token = CurrentToken;
        eat(SimpleToken::TypeName);
        SimpleNode::ValueTypes returnType = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();

        token = CurrentToken;
        eat(SimpleToken::VariableID);
        QString FuncName = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();

        SymbolTable *FuncSubSymblTbl = new SymbolTable(QString("FuncSymblTbl_%1").arg(FuncName),&SymblTbl);
        eat(SimpleToken::LParan);
        QVector<VariableNode*> parameters;
        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            VariableNode *varNode = VarDeclaration(FuncSubSymblTbl);
            parameters.append(varNode);
        }

        eat(SimpleToken::RParan);

        node = new FunctionNode(FuncName, parameters, returnType, FuncSubSymblTbl);
        SymblTbl.addEntry(FuncName,new FunctionSymbol(node));
    }

    if(node == NULL)
    {
        SyntacticError(CurrentToken,QString("Expected Function Declaration"));
        return NULL;
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNode *SimpleParser::VarDefinition(SymbolTable *SymbolTableToRegisterVariableTo)
{
    SimpleNode *node = VarDeclaration(SymbolTableToRegisterVariableTo);
    SimpleNode *nodeTwo = NULL;
    if(node == NULL)
    {
        return NULL;
    }

    SharedSimpleTokenPtr token = CurrentToken;
    if(CurrentToken->getTokenType() == SimpleToken::Assign)
    {
        eat(SimpleToken::Assign);
        nodeTwo = Expression();
        if(nodeTwo == NULL)
        {
            delete node;
            return NULL;
        }
        SimpleNode::ValueTypes exprReturnType = nodeTwo->getReturnType();
        if( ( exprReturnType == ValueNode::ErrorType ) || ( !SimpleNode::canConvertTypes(exprReturnType, node->getReturnType()) ) )
        {
            TypeError(
                        token,
                        QString("Expected: %1, was: %2")
                        .arg(SimpleNode::getHumanReadableTypeNameToValueType(node->getReturnType()))
                        .arg(SimpleNode::getHumanReadableTypeNameToValueType(nodeTwo->getReturnType()))
                        );
            delete nodeTwo;
            delete node;
            return NULL;
        }
        node = new AssignmentNode(dynamic_cast<VariableNode*>(node), nodeTwo);
//        dynamic_cast<VariableNode*>(node)->setAssignment(nodeTwo);
    }

    eat(SimpleToken::SemiColonDelim);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

VariableNode *SimpleParser::VarDeclaration(SymbolTable *SymbolTableToRegisterVariableTo)
{
    VariableNode *node = NULL;
    SharedSimpleTokenPtr InitialToken = CurrentToken;
    SharedSimpleTokenPtr token;

    if(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        token = CurrentToken;
        eat(SimpleToken::TypeName);
        if(CurrentToken->getTokenType() == SimpleToken::VariableID)
        {
            if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::LParan)
            {
                return NULL;
            }
            SimpleNode::ValueTypes type = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            token = CurrentToken;
            eat(SimpleToken::VariableID);
            QString VariableID = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();
            SymbolTableToRegisterVariableTo->addEntry(VariableID, new VariableSymbol(VariableID, type));
            node = new VariableNode(VariableID, SymbolTableToRegisterVariableTo, type);
        }
    }

    if(node == NULL)
    {
        return NULL;
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
        eat(SimpleToken::SemiColonDelim);
        if(node == NULL)
        {
            return NULL;
        }
    }

    if(node == NULL)
    {
        return NULL;
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

    while(node->getNodeType() == SimpleNode::Variable)
    {
        VariableNode *varNode = dynamic_cast<VariableNode*>(node);

        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Assign:
            eat(SimpleToken::Assign);
            token = CurrentToken;
            node = AssignmentExpression();
            if(node == NULL)
            {
                SyntacticError(CurrentToken, QString("Assignable Value expected!"));
                return NULL;
            }
            if(SimpleNode::canConvertTypes(varNode->getReturnType(), node->getReturnType()))
            {
                node = new AssignmentNode(varNode, node);
            }
            else
            {
                TypeError(
                            token,
                            QString("Expected: %1, but got %2")
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(varNode->getReturnType()))
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(node->getReturnType()))
                            );
                return NULL;
            }
            break;
        default:
            return node;
        }
    }
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
    SimpleNode *nodeTwo;
    SimpleNode *nodeThree;
    SharedSimpleTokenPtr token = CurrentToken;


    if( CurrentToken->getTokenType() == SimpleToken::QMark )
    {
        token = CurrentToken;
        eat(SimpleToken::QMark);
        nodeTwo = ConditionalExpression();
        if(nodeTwo == NULL)
        {
            SyntacticError(token);
            delete node;
            return NULL;
        }
        token = CurrentToken;
        eat(SimpleToken::Colon);
        nodeThree = ConditionalExpression();
        if(nodeThree == NULL)
        {
            SyntacticError(token);
            delete node;
            delete nodeTwo;
            return NULL;
        }
        node = new ConditionalNode(node, nodeTwo, nodeThree);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: bool ? Integer|Double|Bool|String : Integer|Double|Bool|String ..."));
            delete node;
            delete nodeTwo;
            delete nodeThree;
            return NULL;
        }
    }

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
                TypeError(
                            token,
                            QString("Expected: Integer + (Integer|Double) | Double + (Double|Integer) | String + (Integer|Double|Bool|String\nWas: %1)")
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(nodeTwo->getReturnType()))
                          );
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
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::VariableID )
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
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::VariableID )
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

            SimpleNode::ValueTypes typeToCastTo = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            node = new TypeCastNode(node, typeToCastTo);
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
    case SimpleToken::Value:
        eat(SimpleToken::Value);
        switch(qSharedPointerDynamicCast<ValueToken>(token)->getValueType())
        {
        case SimpleNode::Integer:
            node = new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getInt());
            break;
        case SimpleNode::Double:
            node = new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getDouble());
            break;
        case SimpleNode::Bool:
            node = new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getBool());
            break;
        case SimpleNode::String:
            node = new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getString());
            break;
        default:
            qDebug() << "ERROR IN PRIMARY EXPRESSION: ValueType Unknown";
            node = new ValueNode();
        }
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
        node = Symbol();
        if(node == NULL)
        {
            return NULL;
        }
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
    case SimpleToken::VariableID:
        eat(SimpleToken::VariableID);
    {
        QString varID = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();
        if(CurSymblTbl->lookup(varID) == NULL)
        {
            SyntacticError(token, QString("Variable was not declared!"));
            return NULL;
        }
        if(CurrentToken->getTokenType() == SimpleToken::LParan)
        {
            QVector<SimpleNode *>FuncParams;
            SimpleNode *curParam = NULL;
            eat(SimpleToken::LParan);
            do
            {
                curParam = Expression();
                if(curParam != NULL)
                {
                    FuncParams.append(curParam);
                }
            }while(curParam != NULL);
            eat(SimpleToken::RParan);
            node = new FunctionCallNode(varID, CurSymblTbl, FuncParams);
            if(node->getReturnType() == SimpleNode::ErrorType)
            {
                SyntacticError(token, QString("Error in FunctionInvocation for Function: %1!").arg(varID));
                return NULL;
            }
        }
        else
        {
            node = new VariableNode(varID, CurSymblTbl);
            if(node->getReturnType() == SimpleNode::ErrorType)
            {
                SyntacticError(token, QString("Variable \"%1\" not found!!!").arg(varID));
                return NULL;
            }
        }
    }
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
