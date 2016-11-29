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
    ProgramSymbolTable(new SymbolTable(QString("ProgramSymbolTable"), ParentSymblTbl)),
    CurSymblTbl(NULL),
    ErrorOccured(false)
{
}

SimpleNodeUniquePtr SimpleParser::parse()
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

SymbolTablePtr SimpleParser::getProgramSymblTbl()
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

ProgramNodeUniquePtr SimpleParser::Program()
{
    SharedSimpleTokenPtr token = CurrentToken;
    SymbolTable ProgramSymbolTable(QString("ProgramSymblTbl"),ParentSymblTbl);

    ProgramNodeUniquePtr program(new ProgramNode(QString("Program"), ProgramSymbolTable));

    // // // Variable or Function Definitions // // //
    while(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        // // // Variable Definition // // //
        token = CurrentToken;
        VariableSymbolPtr newVarDefinition = VarDefinition();
        if(newVarDefinition == nullptr)
        {
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
        }
        else
        {
            program->addVariableDefinition(newVarDefinition);
            continue;
        }
        // // // Function Definition // // //
        FunctionSymbolPtr newFuncDefinition = FunctionDefinition();
        if(newFuncDefinition == nullptr)
        {
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
            break;
        }
        else
        {
            program->addFunctionDefinition(newFuncDefinition);
        }
    }

    while(CurrentToken->getTokenType() != SimpleToken::ReturnKeyword)
    {
        token = CurrentToken;
        SimpleNodeUniquePtr ProgramExpression = Expression();
        if(ProgramExpression == nullptr)
        {
            SyntacticError(token, QString("Expected Expression!"));
            return Q_NULLPTR;
        }
        program->addExpression(ProgramExpression);
    }


    SimpleNodeUniquePtr ProgramReturnStatement = ReturnStatement();
    if(ProgramReturnStatement == nullptr)
    {
        SyntacticError(token, QString("Expected Return Statement!"));
        return Q_NULLPTR;
    }
    program->addReturnStatement(std::move(ProgramReturnStatement));

    qDebug() << __PRETTY_FUNCTION__ << ": " << program->printNode();

    return program;
}

FunctionSymbolPtr SimpleParser::FunctionDefinition()
{
    FunctionSymbolPtr DeclaredFuncSymbol = FunctionDeclaration();

    if(DeclaredFuncSymbol == nullptr)
    {
        return FunctionSymbolPtr();
    }
    SharedSimpleTokenPtr token = CurrentToken;
    SymbolTablePtr FuncSubSymblTbl = DeclaredFuncSymbol->getFunctionSymbolTable();
    //    CurSymblTbl = FuncSubSymblTbl;

    if(CurrentToken->getTokenType() == SimpleToken::LCurlyParan)
    {
        eat(SimpleToken::LCurlyParan);
        QVector<SimpleNodeUniquePtr> FuncExpressions;

        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            VariableSymbolPtr varDef = VarDefinition();
            //Add to function symbol table
            //            if(varDef->getNodeType() == SimpleNode::Assignment)
            //            {
            //                SimpleNodeScopedPtr Assignment;
            //                FuncExpressions.append(SimpleNodeScopedPtr(new AssignmentNode(VariableNodeScopedPtr(new VariableNode(varDef)),Assignment)));
            //            }
        }

        SimpleNodeUniquePtr ExpressionNode;
        do
        {
            ExpressionNode = Expression();
            if(ExpressionNode != nullptr)
            {
                eat(SimpleToken::SemiColonDelim);
                //ToDO !!!
//                FuncExpressions.push_back(std::move(ExpressionNode));
            }
            CurSymblTbl = FuncSubSymblTbl;
        }while(ExpressionNode != nullptr);

        SimpleNodeUniquePtr ReturnStatementNode = ReturnStatement();

        eat(SimpleToken::RCurlyParan);

        DeclaredFuncSymbol->addFunctionExpressions(FuncExpressions);
        DeclaredFuncSymbol->addFunctionReturnStatement(std::move(ReturnStatementNode));
        if(DeclaredFuncSymbol->getReturnType() == ValueNode::ErrorType)
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
            if(varSymbolDeclaration == nullptr)
            {
                qDebug() << "INVALID Variable Declaration";
                return FunctionSymbolPtr();
            }
            parameters.append(varSymbolDeclaration);
        }

        eat(SimpleToken::RParan);

        FuncSymbol = FunctionSymbolPtr(new FunctionSymbol(FuncName, std::move(parameters), returnType));
        //        SymblTbl.addEntry(FuncName, FuncSymbol);
    }

    if(FuncSymbol == nullptr)
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
    SimpleNodeUniquePtr nodeTwo;
    if(VarDeclarationSymbol == nullptr)
    {
        return VariableSymbolPtr();
    }

    SharedSimpleTokenPtr token = CurrentToken;
    if(CurrentToken->getTokenType() == SimpleToken::Assign)
    {
        eat(SimpleToken::Assign);
        nodeTwo = Expression();
        if(nodeTwo == nullptr)
        {
            return VariableSymbolPtr();
        }
        SimpleNode::ValueTypes exprReturnType = nodeTwo->getReturnType();
        if( ( exprReturnType == ValueNode::ErrorType ) || ( !SimpleNode::canConvertTypes(exprReturnType, VarDeclarationSymbol->getReturnType()) ) )
        {
            TypeError(
                        token,
                        QString("Expected: %1, was: %2")
                        .arg(SimpleNode::getHumanReadableTypeNameToValueType(VarDeclarationSymbol->getReturnType()))
                        .arg(SimpleNode::getHumanReadableTypeNameToValueType(nodeTwo->getReturnType()))
                        );
            return VariableSymbolPtr();
        }
        VarDeclarationSymbol->assignValue(*(nodeTwo.get()));
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

    if(varDeclarationSymbol == nullptr)
    {
        return VariableSymbolPtr();
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << varDeclarationSymbol->PrintToSymbolToString();
    return varDeclarationSymbol;
}

SimpleNodeUniquePtr SimpleParser::ReturnStatement()
{
    SimpleNodeUniquePtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::ReturnKeyword)
    {
        eat(SimpleToken::ReturnKeyword);
        node = Expression();
        if(node == nullptr)
        {
            return Q_NULLPTR;
        }
        eat(SimpleToken::SemiColonDelim);
    }

    if(node == nullptr)
    {
        return Q_NULLPTR;
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::Expression()
{
    SimpleNodeUniquePtr node = AssignmentExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return std::move(node);
}

SimpleNodeUniquePtr SimpleParser::AssignmentExpression()
{
    SimpleNodeUniquePtr node = ConditionalExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SharedSimpleTokenPtr token = CurrentToken;

    while(node->getNodeType() == SimpleNode::Variable)
    {
        VariableNodeUniquePtr varNode(dynamic_cast<VariableNode*>(node.release()));

        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Assign:
            eat(SimpleToken::Assign);
            token = CurrentToken;
            node = AssignmentExpression();
            if(node == nullptr)
            {
                SyntacticError(CurrentToken, QString("Assignable Value expected!"));
                return Q_NULLPTR;
            }
            if(SimpleNode::canConvertTypes(varNode->getReturnType(), node->getReturnType()))
            {
                node.reset(new AssignmentNode(std::move(varNode), std::move(node)));
            }
            else
            {
                TypeError(
                            token,
                            QString("Expected: %1, but got %2")
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(varNode->getReturnType()))
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(node->getReturnType()))
                            );
                return Q_NULLPTR;
            }
            break;
        default:
            return node;
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::ConditionalExpression()
{
    SimpleNodeUniquePtr node = LogicalORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    SimpleNodeUniquePtr nodeThree;
    SharedSimpleTokenPtr token = CurrentToken;


    if( CurrentToken->getTokenType() == SimpleToken::QMark )
    {
        token = CurrentToken;
        eat(SimpleToken::QMark);
        nodeTwo = ConditionalExpression();
        if(nodeTwo == nullptr)
        {
            SyntacticError(token);
            return Q_NULLPTR;
        }
        token = CurrentToken;
        eat(SimpleToken::Colon);
        nodeThree = ConditionalExpression();
        if(nodeThree == nullptr)
        {
            SyntacticError(token);
            return Q_NULLPTR;
        }
        node.reset(new ConditionalNode(std::move(node), std::move(nodeTwo), std::move(nodeThree)));
        if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: bool ? Integer|Double|Bool|String : Integer|Double|Bool|String ..."));
            return Q_NULLPTR;
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::LogicalORExpression()
{
    SimpleNodeUniquePtr node = LogicalXORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalOR:
            eat(SimpleToken::LogicalOR);
            nodeTwo = LogicalXORExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new LogicalORNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer || (Integer|Double|Bool) | Double || (Integer|Double|Bool) | Bool || (Integer|Double|Bool) ..."));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::LogicalXORExpression()
{
    SimpleNodeUniquePtr node = LogicalANDExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalXOR:
            eat(SimpleToken::LogicalXOR);
            nodeTwo = LogicalANDExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new LogicalXORNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer ^^ (Integer|Double|Bool) | Double ^^ (Integer|Double|Bool) | Bool ^^ (Integer|Double|Bool)..."));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::LogicalANDExpression()
{
    SimpleNodeUniquePtr node = BitwiseORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LogicalAND:
            eat(SimpleToken::LogicalAND);
            nodeTwo = BitwiseORExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new LogicalANDNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer && (Integer|Double|Bool) | Double && (Integer|Double|Bool) | Bool && (Integer|Double|Bool) ..."));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::BitwiseORExpression()
{
    SimpleNodeUniquePtr node = BitwiseXORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseOR:
            eat(SimpleToken::BitwiseOR);
            nodeTwo = BitwiseXORExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new ORNode(std::move(node), std::move(nodeTwo)));
                    if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer | Integer"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::BitwiseXORExpression()
{
    SimpleNodeUniquePtr node = BitwiseANDExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseXOR:
            eat(SimpleToken::BitwiseXOR);
            nodeTwo = BitwiseANDExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new XORNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer ^ Integer"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::BitwiseANDExpression()
{
    SimpleNodeUniquePtr node = EqualityExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::BitwiseAND:
            eat(SimpleToken::BitwiseAND);
            nodeTwo = EqualityExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new ANDNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer & Integer"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::EqualityExpression()
{
    SimpleNodeUniquePtr node = RelationalExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Equal:
            eat(SimpleToken::Equal);
            nodeTwo = RelationalExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new EqualNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer == (Integer|Double)| Double == (Double|Integer) | Bool == (Integer|Double|Bool) | String == string"));
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::Unequal:
            eat(SimpleToken::Unequal);
            nodeTwo = RelationalExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new UnequalNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer != (Integer|Double|Bool)| Double != (Double|Integer|Bool) | Bool != (Integer|Double|Bool) | String != string"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::RelationalExpression()
{
    SimpleNodeUniquePtr node = ShiftExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Greater:
            eat(SimpleToken::Greater);
            nodeTwo = ShiftExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new GreaterNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer > (Integer|Double)| Double > (Double|Integer)"));
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::Lower:
            eat(SimpleToken::Lower);
            nodeTwo = ShiftExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new LowerNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer < (Integer|Double)| Double < (Double|Integer)"));
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::EqualOrGreater:
            eat(SimpleToken::EqualOrGreater);
            nodeTwo = ShiftExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new EqualOrGreaterNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer >= (Integer|Double)| Double >= (Double|Integer)"));
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::EqualOrLower:
            eat(SimpleToken::EqualOrLower);
            nodeTwo = ShiftExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new EqualOrLowerNode(std::move(node), std::move(nodeTwo)));
                    if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer <= (Integer|Double)| Double <= (Double|Integer)"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::ShiftExpression()
{
    SimpleNodeUniquePtr node = AdditiveExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::LeftShift:
            eat(SimpleToken::LeftShift);
            nodeTwo = AdditiveExpression();
            if(node == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset( new LeftShiftNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer"));
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::RightShift:
            eat(SimpleToken::RightShift);
            nodeTwo = AdditiveExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new RightShiftNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::AdditiveExpression()
{
    SimpleNodeUniquePtr node = MultiplicativeExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Plus:
            eat(SimpleToken::Plus);
            nodeTwo = MultiplicativeExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new AdditionNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(
                            token,
                            QString("Expected: Integer + (Integer|Double) | Double + (Double|Integer) | String + (Integer|Double|Bool|String\nWas: %1)")
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(nodeTwo->getReturnType()))
                            );
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::Minus:
            eat(SimpleToken::Minus);
            nodeTwo = MultiplicativeExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new SubtractionNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer - (Integer|Double) | Double - (Double|Integer)"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::MultiplicativeExpression()
{
    SimpleNodeUniquePtr node = UnaryExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SimpleNodeUniquePtr nodeTwo;
    bool ContinueLoop = true;

    do
    {
        SharedSimpleTokenPtr token = CurrentToken;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Multiplication:
            eat(SimpleToken::Multiplication);
            nodeTwo = UnaryExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new MultiplicationNode(std::move(node), std::move(nodeTwo)));
            if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer * (Integer|Double) | Double * (Double|Integer) | String * Integer"));
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::Division:
            eat(SimpleToken::Division);
            nodeTwo = UnaryExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new DivisionNode(std::move(node), std::move(nodeTwo)));
                    if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer / (Integer|Double) | Double / (Double|Integer)"));
                return Q_NULLPTR;
            }
            break;
        case SimpleToken::Modulo:
            eat(SimpleToken::Modulo);
            nodeTwo = UnaryExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            node.reset(new ModuloNode(std::move(node), std::move(nodeTwo)));
                    if(node == nullptr)
            {
                return Q_NULLPTR;
            }
            if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer % Integer"));
                return Q_NULLPTR;
            }
            break;
        default:
            ContinueLoop = false;
        }
    }while(ContinueLoop);

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::UnaryExpression()
{
    SimpleNodeUniquePtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Increment:
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::VariableID )
        {
            eat(SimpleToken::Increment);
            node = Symbol();
            if(node == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            if(node->getNodeType() != SimpleNode::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return Q_NULLPTR;
            }
            node.reset(new IncrementNode(std::move(node)));
                    if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer Variable"));

                return Q_NULLPTR;
            }
        }
        else
        {
            SyntacticError(CurrentToken, QString("PreIncrement -> Expected Variable!"));
            return Q_NULLPTR;
        }
        break;
    case SimpleToken::Decrement:
        if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::VariableID )
        {
            eat(SimpleToken::Decrement);
            node = Symbol();
            if(node == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }
            if(node->getNodeType() != SimpleNode::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return Q_NULLPTR;
            }
            node.reset(new DecrementNode(std::move(node)));
                    if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer Variable"));

                return Q_NULLPTR;
            }
        }
        else
        {
            SyntacticError(CurrentToken, QString("PreDecrement -> Expected Variable!"));
            return Q_NULLPTR;
        }
        break;
    case SimpleToken::Plus:
        eat(SimpleToken::Plus);
        node = UnaryExpression();
        if(node == nullptr)
        {
            SyntacticError(CurrentToken);
            return Q_NULLPTR;
        }
        node.reset(new PositiveNode(std::move(node)));
                   if(node->getReturnType() == ValueNode::ErrorType)
        {
                       TypeError(token, QString("Expected: Integer | Double"));

                       return Q_NULLPTR;
                   }
                   break;
            case SimpleToken::Minus:
            eat(SimpleToken::Minus);
        node = UnaryExpression();
        if(node == nullptr)
        {
            SyntacticError(CurrentToken);
            return Q_NULLPTR;
        }
        node.reset(new NegativeNode(std::move(node)));
                if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: Integer | Double"));

            return Q_NULLPTR;
        }
        break;
    case SimpleToken::LogicalNegation:
        eat(SimpleToken::LogicalNegation);
        node = UnaryExpression();
        if(node == nullptr)
        {
            SyntacticError(CurrentToken);
            return Q_NULLPTR;
        }
        node.reset(new LogicalNegationNode(std::move(node)));
                if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: Integer | Double | Bool"));

            return Q_NULLPTR;
        }
        break;
    case SimpleToken::OnesComplement:
        eat(SimpleToken::OnesComplement);
        node = UnaryExpression();
        if(node == nullptr)
        {
            SyntacticError(CurrentToken);
            return Q_NULLPTR;
        }
        node.reset(new OnesComplementNode(std::move(node)));
                if(node->getReturnType() == ValueNode::ErrorType)
        {
            TypeError(token, QString("Expected: Integer"));

            return Q_NULLPTR;
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
            if(node == nullptr)
            {
                SyntacticError(CurrentToken);
                return Q_NULLPTR;
            }

            SimpleNode::ValueTypes typeToCastTo = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            node.reset(new TypeCastNode(std::move(node), typeToCastTo));
                    if(node->getReturnType() == ValueNode::ErrorType)
            {
                TypeError(token, QString("Expected: Integer and cast to (Integer|Double|Bool|String) | Double and cast to (Integer|Double|Bool|String) | Bool and cast to (Integer|Double|Bool|String) | String and cast to String"));
                return Q_NULLPTR;
            }
            break;
        }
    }
    default:
        node = PostFixExpression();
        if(node == nullptr)
        {
            return Q_NULLPTR;
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::PostFixExpression()
{
    SimpleNodeUniquePtr node = PrimaryExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
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
                node.reset(new IncrementNode(std::move(node)));
                if(node->getReturnType() == ValueNode::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | IncrementExpression!"));
                    return Q_NULLPTR;
                }
                break;
            case SimpleToken::Decrement:
                token = CurrentToken;
                eat(SimpleToken::Decrement);
                node.reset(new DecrementNode(std::move(node)));
                if(node->getReturnType() == ValueNode::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | DecrementExpression!"));
                    return Q_NULLPTR;
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

SimpleNodeUniquePtr SimpleParser::PrimaryExpression()
{
    SimpleNodeUniquePtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Value:
        eat(SimpleToken::Value);
        switch(qSharedPointerDynamicCast<ValueToken>(token)->getValueType())
        {
        case SimpleNode::Integer:
            node.reset(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getInt()));
            break;
        case SimpleNode::Double:
            node.reset(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getDouble()));
            break;
        case SimpleNode::Bool:
            node.reset(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getBool()));
            break;
        case SimpleNode::String:
            node.reset(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getString()));
            break;
        default:
            qDebug() << "ERROR IN PRIMARY EXPRESSION: ValueType Unknown";
            //            node = new ValueNode();
        }
        break;
    case SimpleToken::LParan:
        eat(SimpleToken::LParan);
        node = Expression();
        if(node == nullptr)
        {
            return Q_NULLPTR;
        }
        eat(SimpleToken::RParan);
        break;
    case SimpleToken::EOFToken:
        qDebug() << __PRETTY_FUNCTION__ << "EOF reached too soon!!!";
        CurrentToken = SharedSimpleTokenPtr(new EOFToken(CurrentToken->getTokenPos(),0));
        EOFUnexpectedError(CurrentToken);
        return Q_NULLPTR;
        break;
    default:
        node = Symbol();
        if(node == nullptr)
        {
            return Q_NULLPTR;
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

SimpleNodeUniquePtr SimpleParser::Symbol()
{
    SimpleNodeUniquePtr node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Data:
        eat(SimpleToken::Data);
        node.reset(new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), ParentSymblTbl ));
        break;
    case SimpleToken::VariableID:
        eat(SimpleToken::VariableID);
    {
        QString varID = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();
        if(CurSymblTbl->lookup(varID) == nullptr)
        {
            SyntacticError(token, QString("Variable was not declared!"));
            return Q_NULLPTR;
        }
        if(CurrentToken->getTokenType() == SimpleToken::LParan)
        {
            QVector<SimpleNodeUniquePtr> FuncParams;
            SimpleNodeUniquePtr curParam;
            eat(SimpleToken::LParan);
            do
            {
                curParam = Expression();
                if(curParam != nullptr)
                {
                    //ToDO !!!
//                    FuncParams.push_back(std::move(curParam));
                }
            }while(curParam != nullptr);
            eat(SimpleToken::RParan);
            node.reset(new FunctionCallNode(varID, CurSymblTbl, FuncParams));
            if(node->getReturnType() == SimpleNode::ErrorType)
            {
                SyntacticError(token, QString("Error in FunctionInvocation for Function: %1!").arg(varID));
                return Q_NULLPTR;
            }
        }
        else
        {
            node.reset(new VariableNode(CurSymblTbl->lookup(varID)));
            if(node->getReturnType() == SimpleNode::ErrorType)
            {
                SyntacticError(token, QString("Variable \"%1\" not found!!!").arg(varID));
                return Q_NULLPTR;
            }
        }
    }
        break;
        //    case SimpleToken::Function:
        //        eat(SimpleToken::Data);
        //        node = new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), &SymblTbl);
        //        break;
    default:
        return Q_NULLPTR;
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
