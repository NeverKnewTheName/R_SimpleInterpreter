#include "simpleparser.h"
#include "simplesymboltable.h"
#include <QDebug>

#include "simplenode.h"

#include "variablenode.h"
#include "datanode.h"
#include "programnode.h"
#include "functioncallnode.h"
#include "assignmentnode.h"

#include "unaryoperationnodes.h"
#include "binaryoperationnodes.h"
#include "ternaryoperationnodes.h"

#include "simplesymboltableentry.h"
#include "simplesymboltable.h"
#include "variablesymbol.h"
#include "functionsymbol.h"

#include "simplestack.h"

SimpleParser::SimpleParser(SimpleLexer *lexer, QSharedPointer<SimpleSymbolTable> parentSymblTbl) :
    lexer(lexer),
    CurrentToken(lexer->getNextToken()),
    ParentSymblTbl(parentSymblTbl),
    ProgramSymbolTable(new SimpleSymbolTable(QString("ProgramSymbolTable"), ParentSymblTbl)),
    CurSymblTbl(NULL),
    ErrorOccured(false)
{
    ParentSymblTbl->addSubSymbolTable(ProgramSymbolTable);
}

SimpleParser::~SimpleParser()
{
    qDebug() << __PRETTY_FUNCTION__;
    ParentSymblTbl->removeSubSymbolTable(ProgramSymbolTable->getIdentifier());
}

std::unique_ptr<SimpleNode> SimpleParser::ParseToAST()
{
    std::unique_ptr<SimpleNode> node;
    if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
    {
        node = Program();

        if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
        {
            qDebug() << __PRETTY_FUNCTION__ << ": NOT EOF";
            if(node != nullptr)
            {
                EOFExpectedError(CurrentToken, QString("EOF was expected but not there was still input..."));
                return nullptr;
            }
        }

    }
    qDebug() << __PRETTY_FUNCTION__ << ": EOF";
    return std::move(node);
}

QSharedPointer<SimpleSymbolTable> SimpleParser::getProgramSymblTbl()
{
    return ProgramSymbolTable;
}

void SimpleParser::eat(SimpleToken::TokenType tokenType)
{
    if(CurrentToken->getTokenType() != tokenType)
    {
        qDebug() << __PRETTY_FUNCTION__ << ": ERROR --> Expected: " << SimpleToken::convertTokenTypeToString(tokenType)
                 << " but got: " << SimpleToken::convertTokenTypeToString(CurrentToken->getTokenType());
        TypeError(CurrentToken,QString("Expected: %1").arg(SimpleToken::convertTokenTypeToString(tokenType)));
    }
    else
    {
        qDebug() << "Eating: " << SimpleToken::convertTokenTypeToString(tokenType);
        CurrentToken = lexer->getNextToken();
    }
}

std::unique_ptr<ProgramNode> SimpleParser::Program()
{
    SharedSimpleTokenPtr token = CurrentToken;
    QSharedPointer<SimpleSymbolTable> SavedSymbolTable = CurSymblTbl;

    std::unique_ptr<ProgramNode> programNode(new ProgramNode(QString("Program"), ProgramSymbolTable));

    CurSymblTbl = ProgramSymbolTable;

    // // // Variable or Function Definitions // // //
    while(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        // // // Variable Definition // // //
        token = CurrentToken;
        std::unique_ptr<SimpleNode> newVarDefinition = VarDefinition();
        if(newVarDefinition == nullptr)
        {
            if(ErrorOccured==true)
            {
                CurSymblTbl = SavedSymbolTable;
                return nullptr;
            }
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
        }
        else
        {
            programNode->addExpression(newVarDefinition);
            continue;
        }
        // // // Function Definition // // //
        QSharedPointer<FunctionSymbol> newFuncDefinition = FunctionDefinition();
        if(newFuncDefinition == nullptr)
        {
            lexer->ResetLexerToToken(token);
            CurrentToken = token;
            break;
        }
        else
        {
            programNode->addFunctionDefinition(newFuncDefinition);
        }
    }

    while(CurrentToken->getTokenType() != SimpleToken::ReturnKeyword)
    {
        token = CurrentToken;
        std::unique_ptr<SimpleNode> ProgramExpression = Expression();
        if(ProgramExpression == nullptr)
        {
            SyntacticError(token, QString("Expected Expression!"));
            CurSymblTbl = SavedSymbolTable;
            return Q_NULLPTR;
        }
        programNode->addExpression(ProgramExpression);
        eat(SimpleToken::SemiColonDelim);
    }


    std::unique_ptr<SimpleNode> ProgramReturnStatement = ReturnStatement();
    if(ProgramReturnStatement == nullptr)
    {
        SyntacticError(token, QString("Expected Return Statement!"));
        CurSymblTbl = SavedSymbolTable;
        return Q_NULLPTR;
    }
    programNode->addReturnStatement(std::move(ProgramReturnStatement));

    qDebug() << __PRETTY_FUNCTION__ << ": " << programNode->printNode();
    CurSymblTbl = SavedSymbolTable;
    return programNode;
}

QSharedPointer<FunctionSymbol> SimpleParser::FunctionDefinition()
{
    QSharedPointer<SimpleSymbolTable> SavedSymbolTable = CurSymblTbl;
    QSharedPointer<FunctionSymbol> DeclaredFuncSymbol = FunctionDeclaration();

    if(DeclaredFuncSymbol == nullptr)
    {
        CurSymblTbl = SavedSymbolTable;
        return QSharedPointer<FunctionSymbol>();
    }
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::LCurlyParan)
    {
        eat(SimpleToken::LCurlyParan);
        std::vector<std::unique_ptr<SimpleNode>> FuncExpressions;

        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            // // // Variable Definition // // //
            token = CurrentToken;
            std::unique_ptr<SimpleNode> newVarDefinition = VarDefinition();
            if(newVarDefinition == nullptr)
            {
                continue;
            }
            FuncExpressions.emplace_back(std::move(newVarDefinition));
        }

        std::unique_ptr<SimpleNode> ExpressionNode;

        while(CurrentToken->getTokenType() != SimpleToken::ReturnKeyword)
        {
            ExpressionNode = Expression();
            if(ExpressionNode != nullptr)
            {
                eat(SimpleToken::SemiColonDelim);
                //ToDO !!!
                FuncExpressions.emplace_back(std::move(ExpressionNode));
            }
            else
            {
                SyntacticError(CurrentToken, QString("Expected Expression or Return... got neither!"));
            }
        }

        std::unique_ptr<SimpleNode> ReturnStatementNode = ReturnStatement();

        eat(SimpleToken::RCurlyParan);

        DeclaredFuncSymbol->addFunctionExpressions(FuncExpressions);
        DeclaredFuncSymbol->addFunctionReturnStatement(std::move(ReturnStatementNode));
        if(DeclaredFuncSymbol->getReturnType() == Node::ErrorType)
        {
            //            CurSymblTbl = &SymblTbl;

            CurSymblTbl = SavedSymbolTable;
            return QSharedPointer<FunctionSymbol>();
        }

        //        eat(SimpleToken::SemiColonDelim);
    }
    else
    {
        //        CurSymblTbl = &SymblTbl;

        SyntacticError(CurrentToken, QString("Expected FunctionDeclaration!"));
        CurSymblTbl = SavedSymbolTable;
        return QSharedPointer<FunctionSymbol>();
    }

    SavedSymbolTable->addEntry(qSharedPointerDynamicCast<SimpleSymbolTableEntry>(DeclaredFuncSymbol));

    CurSymblTbl = SavedSymbolTable;

    qDebug() << __PRETTY_FUNCTION__ << ": " << DeclaredFuncSymbol->PrintToSymbolToString();
    //    CurSymblTbl = &SymblTbl;
    return DeclaredFuncSymbol;
}

QSharedPointer<FunctionSymbol> SimpleParser::FunctionDeclaration()
{
    QSharedPointer<FunctionSymbol> FuncSymbol;
    SharedSimpleTokenPtr token;

    QSharedPointer<SimpleSymbolTable> SavedSymbolTable = CurSymblTbl;

    if(CurrentToken->getTokenType() == SimpleToken::TypeName)
    {
        token = CurrentToken;
        eat(SimpleToken::TypeName);
        Node::ValueTypes returnType = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();

        token = CurrentToken;
        eat(SimpleToken::VariableID);
        QString FuncName = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();

        QSharedPointer<SimpleSymbolTable> functionSymbolTable(new SimpleSymbolTable(QString("%1_SymbolTable").arg(FuncName)));
        CurSymblTbl = functionSymbolTable;

        eat(SimpleToken::LParan);
        std::vector<QSharedPointer<VariableSymbol>> parameters;
        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            QSharedPointer<VariableSymbol> varSymbolDeclaration = VarDeclaration();
            if(varSymbolDeclaration == nullptr)
            {
                qDebug() << "INVALID Variable Declaration";
                return QSharedPointer<FunctionSymbol>();
            }
            parameters.push_back(varSymbolDeclaration);
        }

        eat(SimpleToken::RParan);

        FuncSymbol = QSharedPointer<FunctionSymbol>(
                    new FunctionSymbol(
                        FuncName,
                        functionSymbolTable,
                        std::move(parameters),
                        returnType
                        )
                    );
        functionSymbolTable->addParentSymbolTable(SavedSymbolTable);
    }

    if(FuncSymbol == nullptr)
    {
        SyntacticError(CurrentToken,QString("Expected Function Declaration"));
        return QSharedPointer<FunctionSymbol>();
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << FuncSymbol->PrintToSymbolToString();
    return FuncSymbol;
}

std::unique_ptr<SimpleNode> SimpleParser::VarDefinition()
{
    QSharedPointer<VariableSymbol> VarDeclarationSymbol = VarDeclaration();

    std::unique_ptr<SimpleNode> node;
    std::unique_ptr<SimpleNode> nodeTwo;
    if(VarDeclarationSymbol == nullptr)
    {
        return nullptr;
    }

    SharedSimpleTokenPtr token = CurrentToken;
    if(CurrentToken->getTokenType() == SimpleToken::Assign)
    {
        eat(SimpleToken::Assign);
        nodeTwo = Expression();
        if(nodeTwo == nullptr)
        {
            return nullptr;
        }
        Node::ValueTypes exprReturnType = nodeTwo->getReturnType();
        if( ( exprReturnType == Node::ErrorType ) || ( !SimpleNode::canConvertTypes(VarDeclarationSymbol->getReturnType(), exprReturnType) ) )
        {
            TypeError(
                        token,
                        QString("Expected: %1, was: %2")
                        .arg(SimpleNode::getHumanReadableTypeNameToValueType(VarDeclarationSymbol->getReturnType()))
                        .arg(SimpleNode::getHumanReadableTypeNameToValueType(nodeTwo->getReturnType()))
                        );
            return nullptr;
        }

        node = std::unique_ptr<SimpleNode>(
                    new AssignmentNode(
//                        std::move(
                            std::unique_ptr<VariableNode>(
                                new VariableNode(
                                    VarDeclarationSymbol
                                    )
//                                )
                            ),
                        std::move(nodeTwo)
                        )
                    );
    }

    eat(SimpleToken::SemiColonDelim);

    if(node == nullptr)
    {
        return nullptr;
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return node;
}

QSharedPointer<VariableSymbol> SimpleParser::VarDeclaration()
{
    QSharedPointer<VariableSymbol> varDeclarationSymbol;
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
                return QSharedPointer<VariableSymbol>();
            }
            Node::ValueTypes type = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            token = CurrentToken;
            eat(SimpleToken::VariableID);
            QString VariableID = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();
            //            SymbolTableToRegisterVariableTo->addEntry(VariableID, new VariableSymbol(VariableID, type));
            if(CurSymblTbl->DoesIdentifierExistInCurrentScope(VariableID))
            {
                qDebug() << "VARIABLE DOES ALREADY EXIST!";
                SyntacticError(token, QString("Variable was already declared in this scope!"));
                return QSharedPointer<VariableSymbol>();
            }
            varDeclarationSymbol = QSharedPointer<VariableSymbol>(new VariableSymbol(VariableID, type));
        }
    }

    if(varDeclarationSymbol == nullptr)
    {
        return QSharedPointer<VariableSymbol>();
    }

    CurSymblTbl->addEntry(varDeclarationSymbol);

    qDebug() << __PRETTY_FUNCTION__ << ": " << varDeclarationSymbol->PrintToSymbolToString();
    return varDeclarationSymbol;
}

std::unique_ptr<SimpleNode> SimpleParser::ReturnStatement()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::ReturnKeyword)
    {
        eat(SimpleToken::ReturnKeyword);
        node = Expression();
        if(node == nullptr)
        {
            return nullptr;
        }
        eat(SimpleToken::SemiColonDelim);
    }

    if(node == nullptr)
    {
        return nullptr;
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::Expression()
{
    std::unique_ptr<SimpleNode> node = AssignmentExpression();
    if(node == nullptr)
    {
        return nullptr;
    }
    SharedSimpleTokenPtr token = CurrentToken;

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::AssignmentExpression()
{
    std::unique_ptr<SimpleNode> node = ConditionalExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SharedSimpleTokenPtr token = CurrentToken;

    while(node->getNodeType() == Node::Variable)
    {
        std::unique_ptr<SimpleNode> nodeTwo;
        switch(CurrentToken->getTokenType())
        {
        case SimpleToken::Assign:
            eat(SimpleToken::Assign);
            token = CurrentToken;
            nodeTwo = AssignmentExpression();
            if(nodeTwo == nullptr)
            {
                SyntacticError(CurrentToken, QString("Assignable Value expected!"));
                return Q_NULLPTR;
            }
            if(SimpleNode::canConvertTypes(node->getReturnType(), nodeTwo->getReturnType()))
            {
                std::unique_ptr<VariableNode> varNode(dynamic_cast<VariableNode*>(node.release()));
                node.reset(new AssignmentNode(std::move(varNode), std::move(nodeTwo)));
            }
            else
            {
                TypeError(
                            token,
                            QString("Expected: %1, but got %2")
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(node->getReturnType()))
                            .arg(SimpleNode::getHumanReadableTypeNameToValueType(nodeTwo->getReturnType()))
                            );
                return Q_NULLPTR;
            }
            break;
        default:
            return std::move(node);
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::ConditionalExpression()
{
    std::unique_ptr<SimpleNode> node = LogicalORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
    std::unique_ptr<SimpleNode> nodeThree;
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
        if(node->getReturnType() == Node::ErrorType)
        {
            TypeError(token, QString("Expected: bool ? Integer|Double|Bool|String : Integer|Double|Bool|String ..."));
            return Q_NULLPTR;
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::LogicalORExpression()
{
    std::unique_ptr<SimpleNode> node = LogicalXORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::LogicalXORExpression()
{
    std::unique_ptr<SimpleNode> node = LogicalANDExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::LogicalANDExpression()
{
    std::unique_ptr<SimpleNode> node = BitwiseORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::BitwiseORExpression()
{
    std::unique_ptr<SimpleNode> node = BitwiseXORExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::BitwiseXORExpression()
{
    std::unique_ptr<SimpleNode> node = BitwiseANDExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::BitwiseANDExpression()
{
    std::unique_ptr<SimpleNode> node = EqualityExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::EqualityExpression()
{
    std::unique_ptr<SimpleNode> node = RelationalExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::RelationalExpression()
{
    std::unique_ptr<SimpleNode> node = ShiftExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::ShiftExpression()
{
    std::unique_ptr<SimpleNode> node = AdditiveExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::AdditiveExpression()
{
    std::unique_ptr<SimpleNode> node = MultiplicativeExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::MultiplicativeExpression()
{
    std::unique_ptr<SimpleNode> node = UnaryExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    std::unique_ptr<SimpleNode> nodeTwo;
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::UnaryExpression()
{
    std::unique_ptr<SimpleNode> node;
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
            if(node->getNodeType() != Node::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return Q_NULLPTR;
            }
            node.reset(new IncrementNode(std::move(node), true));
            if(node->getReturnType() == Node::ErrorType)
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
            if(node->getNodeType() != Node::Variable)
            {
                SyntacticError(CurrentToken, QString("Expected Variable!"));
                return Q_NULLPTR;
            }
            node.reset(new DecrementNode(std::move(node), true));
            if(node->getReturnType() == Node::ErrorType)
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
        if(node->getReturnType() == Node::ErrorType)
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
        if(node->getReturnType() == Node::ErrorType)
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
        if(node->getReturnType() == Node::ErrorType)
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
        if(node->getReturnType() == Node::ErrorType)
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

            Node::ValueTypes typeToCastTo = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            node.reset(new TypeCastNode(std::move(node), typeToCastTo));
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::PostFixExpression()
{
    std::unique_ptr<SimpleNode> node = PrimaryExpression();
    if(node == nullptr)
    {
        return Q_NULLPTR;
    }
    SharedSimpleTokenPtr token = CurrentToken;
    if(node->getNodeType() == Node::Variable)
    {
        bool ContinueLoop = true;

        do
        {
            switch(CurrentToken->getTokenType())
            {
            case SimpleToken::Increment:
                token = CurrentToken;
                eat(SimpleToken::Increment);
                node.reset(new IncrementNode(std::move(node), false));
                if(node->getReturnType() == Node::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | IncrementExpression!"));
                    return Q_NULLPTR;
                }
                break;
            case SimpleToken::Decrement:
                token = CurrentToken;
                eat(SimpleToken::Decrement);
                node.reset(new DecrementNode(std::move(node), false));
                if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::PrimaryExpression()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Value:
        eat(SimpleToken::Value);
        switch(qSharedPointerDynamicCast<ValueToken>(token)->getValueType())
        {
        case Node::Integer:
            node.reset(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getInt()));
            break;
        case Node::Double:
            node.reset(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getDouble()));
            break;
        case Node::Bool:
            node.reset(new ValueNode(qSharedPointerDynamicCast<ValueToken>(token)->getBool()));
            break;
        case Node::String:
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
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::Symbol()
{
    std::unique_ptr<SimpleNode> node;
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
            std::vector<std::unique_ptr<SimpleNode>> FuncParams;
            std::unique_ptr<SimpleNode> curParam;
            eat(SimpleToken::LParan);
            do
            {
                curParam = Expression();
                if(curParam != nullptr)
                {
                    //ToDO !!!
                    FuncParams.emplace_back(std::move(curParam));
                }
            }while(curParam != nullptr);
            eat(SimpleToken::RParan);
            node.reset(new FunctionCallNode(varID, CurSymblTbl, FuncParams));
            if(node->getReturnType() == Node::ErrorType)
            {
                SyntacticError(token, QString("Error in FunctionInvocation for Function: %1!").arg(varID));
                return Q_NULLPTR;
            }
        }
        else
        {
            node.reset(new VariableNode(qSharedPointerDynamicCast<ValueSymbol>(CurSymblTbl->lookup(varID))));
            if(node->getReturnType() == Node::ErrorType)
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
    return std::move(node);
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
