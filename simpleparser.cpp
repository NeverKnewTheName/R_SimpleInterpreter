#include "simpleparser.h"
#include "simplesymboltable.h"
#include <QDebug>

#include "symbolnodes.h"

#include "unaryoperationnodes.h"
#include "binaryoperationnodes.h"
#include "ternaryoperationnodes.h"

SimpleParser::SimpleParser(SimpleLexer *lexer, SymbolTablePtr parentSymblTbl) :
    lexer(lexer),
    CurrentToken(lexer->getNextToken()),
    ParentSymblTbl(parentSymblTbl),
    ProgramSymbolTable(QString("ProgramSymbolTable"), ParentSymblTbl),
    CurSymblTbl(NULL),
    ErrorOccured(false)
{
}

SimpleNodeScopedPtr SimpleParser::parse()
{
//    ProgramNode node;
//    if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
//    {
//        node = Program();

//        if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
//        {
//            qDebug() << __PRETTY_FUNCTION__ << ": NOT EOF";
//            if(node != NULL)
//            {

//                EOFExpectedError(CurrentToken, QString("EOF was expected but not there was still input..."));
//                return NULL;
//            }
//        }

//    }
//    qDebug() << __PRETTY_FUNCTION__ << ": EOF";
//    return node;
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

SimpleNodeScopedPtr SimpleParser::Program()
{
    SimpleNodeScopedPtr node;
    SharedSimpleTokenPtr token = CurrentToken;
    SymbolTable ProgramSymbolTable(QString("ProgramSymblTbl"),ParentSymblTbl);

    ProgramNode program(QString("Program"), ProgramSymbolTable);

    // // // Variable or Function Definitions // // //
    while(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        // // // Variable Definition // // //
        token = CurrentToken;
        node.reset(VarDefinition());
        if(!node.isNull())
        {
            continue;
        }
        else
        {
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
        }
        // // // Function Definition // // //
        node.reset(FunctionDefinition());
        if(node.isNull())
        {
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
            break;
        }
    }

    while(CurrentToken->getTokenType() != SimpleToken::ReturnKeyword)
    {
        token = CurrentToken;
        node.reset(Expression());
        if(node.isNull())
        {
            SyntacticError(token, QString("Expected Expression!"));
            SimpleNodeScopedPtr();
        }
    }

    node.reset(ReturnStatement());
    if(node.isNull())
    {
        SyntacticError(token, QString("Expected Return Statement!"));
        SimpleNodeScopedPtr();
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

FunctionSymbolPtr SimpleParser::FunctionDefinition()
{
    FunctionSymbolPtr DeclaredFuncSymbol = FunctionDeclaration();

    if(DeclaredFuncSymbol.isNull())
    {
        return FunctionSymbolPtr();
    }
    SharedSimpleTokenPtr token = CurrentToken;
//    SymbolTable *FuncSubSymblTbl = node->getFuncSymbolTable();;
//    CurSymblTbl = FuncSubSymblTbl;

    if(CurrentToken->getTokenType() == SimpleToken::LCurlyParan)
    {
        eat(SimpleToken::LCurlyParan);
        QVector<SimpleNodeScopedPtr> FuncExpressions;

        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            VariableSymbolPtr varDef = VarDefinition();
            //Add to function symbol table
            if(varDef->getNodeType() == SimpleNode::Assignment)
            {
                SimpleNodeScopedPtr Assignment;
                FuncExpressions.append(SimpleNodeScopedPtr(new AssignmentNode(VariableNodeScopedPtr(new VariableNode(varDef)),Assignment)));
            }
        }

        SimpleNodeScopedPtr ExpressionNode;
        do
        {
            ExpressionNode.reset(Expression());
            if(!ExpressionNode.isNull())
            {
                eat(SimpleToken::SemiColonDelim);
                FuncExpressions.append(std::move(ExpressionNode));
            }
            CurSymblTbl = FuncSubSymblTbl;
        }while(!ExpressionNode.isNull());

        SimpleNodeScopedPtr ReturnStatementNode(ReturnStatement().take());

        eat(SimpleToken::RCurlyParan);

        node->addFuncExpressions(FuncExpressions);
        node->addReturnStatement(ReturnStatementNode);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
//            CurSymblTbl = &SymblTbl;

            return FunctionSymbolPtr();
        }

        eat(SimpleToken::SemiColonDelim);
    }
    else
    {
//        CurSymblTbl = &SymblTbl;

        SyntacticError(CurrentToken, QString("Expected FunctionDeclaration!"));
        return FunctionSymbolPtr();
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << DeclaredFuncSymbol->PrintToSymbolToString();
//    CurSymblTbl = &SymblTbl;
    return DeclaredFuncSymbol;
}

FunctionSymbolPtr SimpleParser::FunctionDeclaration(/*SymbolTable *FuncSubSymblTbl*/)
{
    FunctionSymbolPtr FuncSymbol;
    SharedSimpleTokenPtr token;

    if(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        token = CurrentToken;
        eat(SimpleToken::TypeName);
        SimpleNode::ValueTypes returnType = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();

        token = CurrentToken;
        eat(SimpleToken::VariableID);
        QString FuncName = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();

//        SymbolTable *FuncSubSymblTbl = new SymbolTable(QString("FuncSymblTbl_%1").arg(FuncName),&SymblTbl);
        eat(SimpleToken::LParan);
        QVector<VariableSymbolPtr> parameters;
        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            VariableSymbolPtr varSymbolDeclaration = VarDeclaration();
            parameters.append(varSymbolDeclaration);
        }

        eat(SimpleToken::RParan);

        FuncSymbol = FunctionSymbolPtr(new FunctionSymbol(FuncName, std::move(parameters), returnType);
        SymblTbl.addEntry(FuncName,SymbolTableEntryPtr(new FunctionSymbol(node)));
    }

    if(FuncSymbol.isNull())
    {
        SyntacticError(CurrentToken,QString("Expected Function Declaration"));
        return FunctionSymbolPtr();
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << FuncSymbol->PrintToSymbolToString();
    return FuncSymbol;
}

VariableSymbolPtr SimpleParser::VarDefinition()
{
    VariableSymbolPtr VarDeclarationSymbol = VarDeclaration();
    SimpleNodeScopedPtr nodeTwo;
    if(VarDeclarationSymbol.isNull())
    {
        return VariableSymbolPtr();
    }

    SharedSimpleTokenPtr token = CurrentToken;
    if(CurrentToken->getTokenType() == SimpleToken::Assign)
    {
        eat(SimpleToken::Assign);
        nodeTwo.reset(Expression());
        if(nodeTwo.isNull())
        {
            return VariableSymbolPtr();
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
            return VariableSymbolPtr();
        }
        VarDeclarationSymbol->assignValue(nodeTwo);
//        node = new AssignmentNode(dynamic_cast<VariableNode*>(node), nodeTwo);
//        dynamic_cast<VariableNode*>(node)->setAssignment(nodeTwo);
    }

    eat(SimpleToken::SemiColonDelim);

    qDebug() << __PRETTY_FUNCTION__ << ": " << VarDeclarationSymbol->PrintToSymbolToString();
    return VarDeclarationSymbol;
}

VariableSymbolPtr SimpleParser::VarDeclaration()
{
    VariableSymbolPtr varDeclarationSymbol;
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
                return VariableSymbolPtr();
            }
            SimpleNode::ValueTypes type = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            token = CurrentToken;
            eat(SimpleToken::VariableID);
            QString VariableID = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();
//            SymbolTableToRegisterVariableTo->addEntry(VariableID, new VariableSymbol(VariableID, type));
            varDeclarationSymbol = VariableSymbolPtr(new VariableSymbol(VariableID, type));
        }
    }

    if(varDeclarationSymbol.isNull())
    {
        return VariableSymbolPtr();
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << varDeclarationSymbol->PrintToSymbolToString();
    return varDeclarationSymbol;
}

SimpleNodeScopedPtr SimpleParser::ReturnStatement()
{
    SimpleNodeScopedPtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::ReturnKeyword)
    {
        eat(SimpleToken::ReturnKeyword);
        node.reset(Expression().take());
        if(node.isNull())
        {
            return SimpleNodeScopedPtr();
        }
        eat(SimpleToken::SemiColonDelim);
    }

    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::Expression()
{
    SimpleNodeScopedPtr node(AssignmentExpression().take());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return std::move(node);
}

SimpleNodeScopedPtr SimpleParser::AssignmentExpression()
{
    SimpleNodeScopedPtr node(ConditionalExpression().take());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SharedSimpleTokenPtr token = CurrentToken;

    while(node->getNodeType() == SimpleNode::Variable)
    {
        QScopedPointer<VariableNode> varNode(node.take());

        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Assign:
            eat(SimpleToken::Assign);
            token = CurrentToken;
            node.reset(AssignmentExpression());
            if(node.isNull())
            {
                SyntacticError(CurrentToken, QString("Assignable Value expected!"));
                return SimpleNodeScopedPtr();
            }
            if(SimpleNode::canConvertTypes(varNode->getReturnType(), node->getReturnType()))
            {
                node.reset(SimpleNodeScopedPtr(new AssignmentNode(varNode, node));
            }
            else
            {
                TypeError(
                            token,
                            QString("Expected: %1, but got %2")
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(varNode->getReturnType()))
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(node->getReturnType()))
                            );
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            return node;
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::ConditionalExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(LogicalORExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    SimpleNodeScopedPtr nodeThree;
    SharedSimpleTokenPtr token = CurrentToken;


    if( CurrentToken->getTokenType() == SimpleToken::QMark )
    {
        token = CurrentToken;
        eat(SimpleToken::QMark);
        nodeTwo.reset(ConditionalExpression());
        if(nodeTwo.isNull())
        {
            SyntacticError(token);
            return SimpleNodeScopedPtr();
        }
        token = CurrentToken;
        eat(SimpleToken::Colon);
        nodeThree.reset(ConditionalExpression());
        if(nodeThree.isNull())
        {
            SyntacticError(token);
            return SimpleNodeScopedPtr();
        }
        node.reset(SimpleNodeScopedPtr(new ConditionalNode(node, nodeTwo, nodeThree)));
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: bool ? Integer|Double|Bool|String : Integer|Double|Bool|String ..."));
            return SimpleNodeScopedPtr();
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::LogicalORExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(LogicalXORExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalOR:
            eat(SimpleToken::LogicalOR);
            nodeTwo.reset(LogicalXORExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new LogicalORNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer || (Integer|Double|Bool) | Double || (Integer|Double|Bool) | Bool || (Integer|Double|Bool) ..."));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::LogicalXORExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(LogicalANDExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalXOR:
            eat(SimpleToken::LogicalXOR);
            nodeTwo.reset(LogicalANDExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new LogicalXORNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer ^^ (Integer|Double|Bool) | Double ^^ (Integer|Double|Bool) | Bool ^^ (Integer|Double|Bool)..."));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::LogicalANDExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(BitwiseORExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalAND:
            eat(SimpleToken::LogicalAND);
            nodeTwo.reset(BitwiseORExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new LogicalANDNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer && (Integer|Double|Bool) | Double && (Integer|Double|Bool) | Bool && (Integer|Double|Bool) ..."));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::BitwiseORExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(BitwiseXORExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseOR:
            eat(SimpleToken::BitwiseOR);
            nodeTwo.reset(BitwiseXORExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new ORNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer | Integer"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::BitwiseXORExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(BitwiseANDExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseXOR:
            eat(SimpleToken::BitwiseXOR);
            nodeTwo.reset(BitwiseANDExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new XORNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer ^ Integer"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::BitwiseANDExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(EqualityExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseAND:
            eat(SimpleToken::BitwiseAND);
            nodeTwo.reset(EqualityExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new ANDNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer & Integer"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::EqualityExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(RelationalExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Equal:
            eat(SimpleToken::Equal);
            nodeTwo.reset(RelationalExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new EqualNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer == (Integer|Double)| Double == (Double|Integer) | Bool == (Integer|Double|Bool) | String == string"));
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::Unequal:
            eat(SimpleToken::Unequal);
            nodeTwo.reset(RelationalExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new UnequalNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer != (Integer|Double|Bool)| Double != (Double|Integer|Bool) | Bool != (Integer|Double|Bool) | String != string"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::RelationalExpression()
{
    SimpleNodeScopedPtr node = ShiftExpression();
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Greater:
            eat(SimpleToken::Greater);
            nodeTwo.reset(ShiftExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new GreaterNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer > (Integer|Double)| Double > (Double|Integer)"));
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::Lower:
            eat(SimpleToken::Lower);
            nodeTwo.reset(ShiftExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new LowerNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer < (Integer|Double)| Double < (Double|Integer)"));
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::EqualOrGreater:
            eat(SimpleToken::EqualOrGreater);
            nodeTwo.reset(ShiftExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new EqualOrGreaterNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer >= (Integer|Double)| Double >= (Double|Integer)"));
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::EqualOrLower:
            eat(SimpleToken::EqualOrLower);
            nodeTwo.reset(ShiftExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new EqualOrLowerNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer <= (Integer|Double)| Double <= (Double|Integer)"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::ShiftExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(AdditiveExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LeftShift:
            eat(SimpleToken::LeftShift);
            nodeTwo = AdditiveExpression();
            if(node.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node = new LeftShiftNode(node, nodeTwo);
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer"));
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::RightShift:
            eat(SimpleToken::RightShift);
            nodeTwo.reset(AdditiveExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new RightShiftNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::AdditiveExpression()
{
    SimpleNodeScopedPtr node = MultiplicativeExpression();
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Plus:
            eat(SimpleToken::Plus);
            nodeTwo.reset(MultiplicativeExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new AdditionNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(
                            token,
                            QString("Expected: Integer + (Integer|Double) | Double + (Double|Integer) | String + (Integer|Double|Bool|String\nWas: %1)")
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(nodeTwo->getReturnType()))
                          );
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::Minus:
            eat(SimpleToken::Minus);
            nodeTwo.reset(MultiplicativeExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new SubtractionNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer - (Integer|Double) | Double - (Double|Integer)"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::MultiplicativeExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(UnaryExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
    }
    SimpleNodeScopedPtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Multiplication:
            eat(SimpleToken::Multiplication);
            nodeTwo.reset(UnaryExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new MultiplicationNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer * (Integer|Double) | Double * (Double|Integer) | String * Integer"));
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::Division:
            eat(SimpleToken::Division);
            nodeTwo.reset(UnaryExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new DivisionNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer / (Integer|Double) | Double / (Double|Integer)"));
                return SimpleNodeScopedPtr();
            }
            break;
        case SimpleToken::Modulo:
            eat(SimpleToken::Modulo);
            nodeTwo.reset(UnaryExpression());
            if(nodeTwo.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new ModuloNode(node, nodeTwo));
            if(node.isNull())
            {
                return SimpleNodeScopedPtr();
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer % Integer"));
                return SimpleNodeScopedPtr();
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::UnaryExpression()
{
    SimpleNodeScopedPtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Increment:
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::VariableID )
        {
            eat(SimpleToken::Increment);
            node.reset(Symbol());
            if(node.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            if(node->getNodeType() != SimpleNode::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new IncrementNode(node));
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer Variable"));

                return SimpleNodeScopedPtr();
            }
        }
        else
        {
            SyntacticError(CurrentToken, QString("PreIncrement -> Expected Variable!"));
            return SimpleNodeScopedPtr();
        }
        break;
    case SimpleToken::Decrement:
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::VariableID )
        {
            eat(SimpleToken::Decrement);
            node.reset(Symbol());
            if(node.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }
            if(node->getNodeType() != SimpleNode::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return SimpleNodeScopedPtr();
            }
            node.reset(SimpleNodeScopedPtr(new DecrementNode(node));
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer Variable"));

                return SimpleNodeScopedPtr();
            }
        }
        else
        {
            SyntacticError(CurrentToken, QString("PreDecrement -> Expected Variable!"));
            return SimpleNodeScopedPtr();
        }
        break;
    case SimpleToken::Plus:
        eat(SimpleToken::Plus);
        node.reset(UnaryExpression());
        if(node.isNull())
        {
            SyntacticError(CurrentToken);
            return SimpleNodeScopedPtr();
        }
        node.reset(SimpleNodeScopedPtr(new PositiveNode(node);
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: Integer | Double"));

            return SimpleNodeScopedPtr();
        }
        break;
    case SimpleToken::Minus:
        eat(SimpleToken::Minus);
        node.reset(UnaryExpression());
        if(node.isNull())
        {
            SyntacticError(CurrentToken);
            return SimpleNodeScopedPtr();
        }
        node.reset(SimpleNodeScopedPtr(new NegativeNode(node));
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: Integer | Double"));

            return SimpleNodeScopedPtr();
        }
        break;
    case SimpleToken::LogicalNegation:
        eat(SimpleToken::LogicalNegation);
        node.reset(UnaryExpression());
        if(node.isNull())
        {
            SyntacticError(CurrentToken);
            return SimpleNodeScopedPtr();
        }
        node.reset(SimpleNodeScopedPtr(new LogicalNegationNode(node));
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: Integer | Double | Bool"));

            return SimpleNodeScopedPtr();
        }
        break;
    case SimpleToken::OnesComplement:
        eat(SimpleToken::OnesComplement);
        node.reset(UnaryExpression());
        if(node.isNull())
        {
            SyntacticError(CurrentToken);
            return SimpleNodeScopedPtr();
        }
        node.reset(SimpleNodeScopedPtr(new OnesComplementNode(node));
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: Integer"));

            return SimpleNodeScopedPtr();
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
            node.reset(UnaryExpression());
            if(node.isNull())
            {
                SyntacticError(CurrentToken);
                return SimpleNodeScopedPtr();
            }

            SimpleNode::ValueTypes typeToCastTo = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            node.reset(SimpleNodeScopedPtr(new TypeCastNode(node, typeToCastTo));
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer and cast to (Integer|Double|Bool|String) | Double and cast to (Integer|Double|Bool|String) | Bool and cast to (Integer|Double|Bool|String) | String and cast to String"));
                return SimpleNodeScopedPtr();
            }
            break;
        }
    }
    default:
        node.reset(PostFixExpression());
        if(node.isNull())
        {
            return SimpleNodeScopedPtr();
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::PostFixExpression()
{
    SimpleNodeScopedPtr node;
    node.reset(PrimaryExpression());
    if(node.isNull())
    {
        return SimpleNodeScopedPtr();
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
                node.reset(SimpleNodeScopedPtr(new IncrementNode(node));
                if(node->getReturnType() == ValueNode::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | IncrementExpression!"));
                    return SimpleNodeScopedPtr();
                }
                break;
            case SimpleToken::Decrement:
                token = CurrentToken;
                eat(SimpleToken::Decrement);
                node.reset(SimpleNodeScopedPtr(new DecrementNode(node));
                if(node->getReturnType() == ValueNode::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | DecrementExpression!"));
                    return SimpleNodeScopedPtr();
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

SimpleNodeScopedPtr SimpleParser::PrimaryExpression()
{
    SimpleNodeScopedPtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Value:
        eat(SimpleToken::Value);
        switch(qSharedPointerDynamicCast<ValueToken>(token)->getValueType())
        {
        case SimpleNode::Integer:
            node.reset(SimpleNodeScopedPtr(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getInt()));
            break;
        case SimpleNode::Double:
            node.reset(SimpleNodeScopedPtr(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getDouble()));
            break;
        case SimpleNode::Bool:
            node.reset(SimpleNodeScopedPtr(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getBool()));
            break;
        case SimpleNode::String:
            node.reset(SimpleNodeScopedPtr(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getString()));
            break;
        default:
            qDebug() << "ERROR IN PRIMARY EXPRESSION: ValueType Unknown";
//            node = new ValueNode();
        }
        break;
    case SimpleToken::LParan:
        eat(SimpleToken::LParan);
        node.reset(Expression());
        if(node.isNull())
        {
            return SimpleNodeScopedPtr();
        }
        eat(SimpleToken::RParan);
        break;
    case SimpleToken::EOFToken:
        qDebug() << __PRETTY_FUNCTION__ << "EOF reached too soon!!!";
        CurrentToken = SharedSimpleTokenPtr(new EOFToken(CurrentToken->getTokenPos(),0));
        EOFUnexpectedError(CurrentToken);
        return SimpleNodeScopedPtr();
        break;
    default:
        node.reset(Symbol());
        if(node.isNull())
        {
            return SimpleNodeScopedPtr();
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeScopedPtr SimpleParser::Symbol()
{
    SimpleNodeScopedPtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Data:
        eat(SimpleToken::Data);
        node.reset(SimpleNodeScopedPtr(new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), &SymblTbl));
        break;
    case SimpleToken::VariableID:
        eat(SimpleToken::VariableID);
    {
        QString varID = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();
        if(CurSymblTbl->lookup(varID).isNull())
        {
            SyntacticError(token, QString("Variable was not declared!"));
            return SimpleNodeScopedPtr();
        }
        if(CurrentToken->getTokenType() == SimpleToken::LParan)
        {
            QVector<SimpleNodeScopedPtr >FuncParams;
            SimpleNodeScopedPtr curParam;
            eat(SimpleToken::LParan);
            do
            {
                curParam.reset(Expression());
                if(!curParam.isNull())
                {
                    FuncParams.append(std::move(curParam));
                }
            }while(!curParam.isNull());
            eat(SimpleToken::RParan);
            node.reset(SimpleNodeScopedPtr(new FunctionCallNode(varID, CurSymblTbl, FuncParams));
            if(node->getReturnType() == SimpleNode::ErrorType)
            {
                SyntacticError(token, QString("Error in FunctionInvocation for Function: %1!").arg(varID));
                return SimpleNodeScopedPtr();
            }
        }
        else
        {
            node.reset(SimpleNodeScopedPtr(new VariableNode(varID, CurSymblTbl));
            if(node->getReturnType() == SimpleNode::ErrorType)
            {
                SyntacticError(token, QString("Variable \"%1\" not found!!!").arg(varID));
                return SimpleNodeScopedPtr();
            }
        }
    }
        break;
        //    case SimpleToken::Function:
        //        eat(SimpleToken::Data);
        //        node = new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), &SymblTbl);
        //        break;
    default:
        return SimpleNodeScopedPtr();
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
