#include "simpleparser.h"
#include "simplesymboltable.h"
#include <QDebug>

#include "simplenode.h"

#include "blocknode.h"
#include "statementnode.h"
#include "expressionnode.h"

#include "variablenode.h"
#include "datanode.h"
#include "assignmentnode.h"

#include "programnode.h"
#include "functioncallnode.h"

#include "unaryoperationnodes.h"
#include "binaryoperationnodes.h"
#include "ternaryoperationnodes.h"

#include "selectioncontrolnode.h"
#include "iterationcontrolnode.h"
#include "escapecontrolnode.h"

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
#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

std::unique_ptr<SimpleNode> SimpleParser::ParseToAST()
{
    std::unique_ptr<SimpleNode> node;
    BlockNode::ResetBlockCntr();
    if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
    {
        node = Program();

        if(CurrentToken->getTokenType() != SimpleToken::EOFToken)
        {
#ifdef __DEBUG_OUTPUT__
            qDebug() << __PRETTY_FUNCTION__ << ": NOT EOF";
#endif
            if(node != nullptr)
            {
                EOFExpectedError(CurrentToken, QString("EOF was expected but not there was still input..."));
                return nullptr;
            }
        }
    }
#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": EOF";
#endif
    return std::move(node);
}

QSharedPointer<SimpleSymbolTable> SimpleParser::getProgramSymblTbl()
{
    return ProgramSymbolTable;
}

bool SimpleParser::eat(SimpleToken::TokenType tokenType)
{
    if(CurrentToken->getTokenType() != tokenType)
    {
#ifdef __DEBUG_OUTPUT__
        qDebug() << __PRETTY_FUNCTION__ << ": ERROR --> Expected: " << SimpleToken::convertTokenTypeToString(tokenType)
                 << " but got: " << SimpleToken::convertTokenTypeToString(CurrentToken->getTokenType());
#endif
        TypeError(
                    CurrentToken,
                    QString("Expected: %1, but got: %2")
                    .arg(SimpleToken::convertTokenTypeToString(tokenType))
                    .arg(SimpleToken::convertTokenTypeToString(CurrentToken->getTokenType()))
                  );
        return false;
    }
    else
    {
#ifdef __DEBUG_OUTPUT__
        qDebug() << "Eating: " << SimpleToken::convertTokenTypeToString(tokenType);
#endif
        CurrentToken = lexer->getNextToken();
        return true;
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
        std::unique_ptr<SimpleNode> ProgramStatement = Block();
        if(ProgramStatement == nullptr)
        {
            SyntacticError(token, QString("Expected Expression!"));
            CurSymblTbl = SavedSymbolTable;
            return Q_NULLPTR;
        }
        programNode->addExpression(ProgramStatement);
    }


    std::unique_ptr<SimpleNode> ProgramReturnStatement = ReturnStatement();
    if(ProgramReturnStatement == nullptr)
    {
        SyntacticError(token, QString("Expected Return Statement!"));
        CurSymblTbl = SavedSymbolTable;
        return Q_NULLPTR;
    }
    programNode->addReturnStatement(std::move(ProgramReturnStatement));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << programNode->printNode();
#endif
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
    Node::ValueTypes returnType = DeclaredFuncSymbol->getReturnType();
    SharedSimpleTokenPtr token = CurrentToken;

    std::unique_ptr<SimpleNode> node = Block();

    if((node == nullptr) || (node->getNodeType() != Node::Block))
    {
        CurSymblTbl = SavedSymbolTable;
        SyntacticError(token, QString("Expected Block Statement after Function Declaration!"));
        return QSharedPointer<FunctionSymbol>();
    }

    if(node->getReturnType() != returnType)
    {
        if(SimpleNode::canConvertTypes(node->getReturnType(), returnType))
        {
            node = std::unique_ptr<SimpleNode>(new TypeCastNode(std::move(node), returnType));
        }
        else
        {
            CurSymblTbl = SavedSymbolTable;
            SyntacticError(token, QString("Returntype of Block does not match FunctionDeclaration - expected %1, but got: %2")
                           .arg(SimpleNode::getHumanReadableTypeNameToValueType(returnType))
                           .arg(SimpleNode::getHumanReadableTypeNameToValueType(node->getReturnType()))
                           );
            return QSharedPointer<FunctionSymbol>();
        }
    }
    DeclaredFuncSymbol->setFunctionStatement(std::move(node));
    //ToDO Check last statement for return...

    //    if(CurrentToken->getTokenType() == SimpleToken::LCurlyParan)
    //    {
    //        if(!eat(SimpleToken::LCurlyParan))
//            {
//                return nullptr;
//            }
    //        std::vector<std::unique_ptr<SimpleNode>> FuncExpressions;

    //        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
    //        {
    //            // // // Variable Definition // // //
    //            token = CurrentToken;
    //            std::unique_ptr<SimpleNode> newVarDefinition = VarDefinition();
    //            if(newVarDefinition == nullptr)
    //            {
    //                continue;
    //            }
    //            FuncExpressions.emplace_back(std::move(newVarDefinition));
    //        }

    //        std::unique_ptr<SimpleNode> ExpressionNode;

    //        while(CurrentToken->getTokenType() != SimpleToken::ReturnKeyword)
    //        {
    //            ExpressionNode = Expression();
    //            if(ExpressionNode != nullptr)
    //            {
    //                if(!eat(SimpleToken::SemiColonDelim))
//                    {
//                        return nullptr;
//                    }
    //                //ToDO !!!
    //                FuncExpressions.emplace_back(std::move(ExpressionNode));
    //            }
    //            else
    //            {
    //                SyntacticError(CurrentToken, QString("Expected Expression or Return... got neither!"));
    //            }
    //        }

    //        std::unique_ptr<SimpleNode> ReturnStatementNode = ReturnStatement();
    //        if(!SimpleNode::canConvertTypes(returnType, ReturnStatementNode->getReturnType()))
    //        {
    //            TypeError(CurrentToken, QString("Expected: %1 but got: %2")
    //                      .arg(SimpleNode::getHumanReadableTypeNameToValueType(returnType))
    //                      .arg(SimpleNode::getHumanReadableTypeNameToValueType(ReturnStatementNode->getReturnType()))
    //                      );
    //            return QSharedPointer<FunctionSymbol>();
    //        }
    //        if(!eat(SimpleToken::RCurlyParan))
//            {
//                return nullptr;
//            }

    //        DeclaredFuncSymbol->addFunctionExpressions(FuncExpressions);
    //        DeclaredFuncSymbol->addFunctionReturnStatement(std::move(ReturnStatementNode));
    //        if(returnType == Node::ErrorType)
    //        {
    //            CurSymblTbl = SavedSymbolTable;
    //            return QSharedPointer<FunctionSymbol>();
    //        }
    //    }
    //    else
    //    {
    //        SyntacticError(CurrentToken, QString("Expected FunctionDeclaration!"));
    //        CurSymblTbl = SavedSymbolTable;
    //        return QSharedPointer<FunctionSymbol>();
    //    }

    SavedSymbolTable->addEntry(qSharedPointerDynamicCast<SimpleSymbolTableEntry>(DeclaredFuncSymbol));

    CurSymblTbl = SavedSymbolTable;

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << DeclaredFuncSymbol->PrintToSymbolToString();
#endif
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
        if(!eat(SimpleToken::TypeName))
        {
            CurSymblTbl = SavedSymbolTable;
            return QSharedPointer<FunctionSymbol>();
        }
        Node::ValueTypes returnType = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();

        token = CurrentToken;
        if(!eat(SimpleToken::VariableID))
        {
            CurSymblTbl = SavedSymbolTable;
            return QSharedPointer<FunctionSymbol>();
        }
        QString FuncName = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();

        QSharedPointer<SimpleSymbolTable> functionSymbolTable(new SimpleSymbolTable(QString("%1_SymbolTable").arg(FuncName)));
        CurSymblTbl = functionSymbolTable;

        if(!eat(SimpleToken::LParan))
        {
            CurSymblTbl = SavedSymbolTable;
            return QSharedPointer<FunctionSymbol>();
        }
        std::vector<QSharedPointer<VariableSymbol>> parameters;
        while(CurrentToken->getTokenType() == SimpleToken::TypeName)
        {
            QSharedPointer<VariableSymbol> varSymbolDeclaration = VarDeclaration();
            if(varSymbolDeclaration == nullptr)
            {
                CurSymblTbl = SavedSymbolTable;
#ifdef __DEBUG_OUTPUT__
                qDebug() << "INVALID Variable Declaration";
#endif
                SymbolError(CurrentToken, QString("Invalid Variable Declaration!"));
                return QSharedPointer<FunctionSymbol>();
            }
            parameters.push_back(varSymbolDeclaration);
            if(CurrentToken->getTokenType() != SimpleToken::CommaDelim)
            {
                break;
            }
            else
            {
                if(!eat(SimpleToken::CommaDelim))
                {
                    CurSymblTbl = SavedSymbolTable;
                    return QSharedPointer<FunctionSymbol>();
                }
                if(CurrentToken->getTokenType() != SimpleToken::TypeName)
                {
                    CurSymblTbl = SavedSymbolTable;
                    SyntacticError(CurrentToken, QString("Expected another VariableDeclaration after CommaDelimiter!"));
                    return QSharedPointer<FunctionSymbol>();
                }
            }
        }

        if(!eat(SimpleToken::RParan))
        {
            CurSymblTbl = SavedSymbolTable;
            return QSharedPointer<FunctionSymbol>();
        }

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
        CurSymblTbl = SavedSymbolTable;
        SyntacticError(CurrentToken,QString("Expected Function Declaration"));
        return QSharedPointer<FunctionSymbol>();
    }
#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << FuncSymbol->PrintToSymbolToString();
#endif
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
        if(!eat(SimpleToken::Assign))
        {
            return nullptr;
        }
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
                        std::unique_ptr<VariableNode>(
                            new VariableNode(
                                VarDeclarationSymbol
                                )
                            ),
                        std::move(nodeTwo)
                        )
                    );
    }

    if(!eat(SimpleToken::SemiColonDelim))
    {
        return nullptr;
    }

    if(node == nullptr)
    {
        return nullptr;
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
        if(!eat(SimpleToken::TypeName))
        {
            return QSharedPointer<VariableSymbol>();
        }
        if(CurrentToken->getTokenType() == SimpleToken::VariableID)
        {
            if(lexer->peekAtNextToken()->getTokenType() == SimpleToken::LParan)
            {
                return QSharedPointer<VariableSymbol>();
            }
            Node::ValueTypes type = qSharedPointerDynamicCast<TypeNameToken>(token)->getType();
            token = CurrentToken;
            if(!eat(SimpleToken::VariableID))
            {
                return QSharedPointer<VariableSymbol>();
            }
            QString VariableID = qSharedPointerDynamicCast<VariableIDToken>(token)->getID();
            //            SymbolTableToRegisterVariableTo->addEntry(VariableID, new VariableSymbol(VariableID, type));
            if(CurSymblTbl->DoesIdentifierExistInCurrentScope(VariableID))
            {

#ifdef __DEBUG_OUTPUT__
                qDebug() << "VARIABLE DOES ALREADY EXIST!";
#endif
                SymbolError(token, QString("Variable was already declared in this scope!"));
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << varDeclarationSymbol->PrintToSymbolToString();
#endif
    return varDeclarationSymbol;
}

std::unique_ptr<SimpleNode> SimpleParser::Block()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::LCurlyParan)
    {
        QSharedPointer<SimpleSymbolTable> SavedSymbolTable = CurSymblTbl;
        if(!eat(SimpleToken::LCurlyParan))
        {
            CurSymblTbl = SavedSymbolTable;
            return nullptr;
        }
        node.reset(new BlockNode());
        CurSymblTbl = dynamic_cast<BlockNode*>(node.get())->getBlockSymbolTable();
        SavedSymbolTable->addEntry(CurSymblTbl);
        CurSymblTbl->addParentSymbolTable(SavedSymbolTable);
        std::unique_ptr<SimpleNode> nodeTwo;
        while(CurrentToken->getTokenType() != SimpleToken::RCurlyParan)
        {
            bool NoMoreVarDef = false;
            do
            {
                nodeTwo = VarDefinition();
                if(nodeTwo != nullptr)
                {
                    dynamic_cast<BlockNode*>(node.get())->addStatement(std::move(nodeTwo));
                }
                else
                {
                    NoMoreVarDef = true;
                }
            }while(!NoMoreVarDef);

            nodeTwo = Block();
            if(nodeTwo == nullptr)
            {
                break;
            }
            dynamic_cast<BlockNode*>(node.get())->addStatement(std::move(nodeTwo));
        }
        if(!eat(SimpleToken::RCurlyParan))
        {
            CurSymblTbl = SavedSymbolTable;
            return nullptr;
        }
        CurSymblTbl = SavedSymbolTable;
    }
    else
    {
        node = Statement();
        if(node == nullptr)
        {
            return nullptr;
        }
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::Statement()
{
    std::unique_ptr<SimpleNode> node = Expression();

    if(node != nullptr)
    {
        if(CurrentToken->getTokenType() == SimpleToken::SemiColonDelim)
        {
            if(!eat(SimpleToken::SemiColonDelim))
            {
                return nullptr;
            }
        }
        else
        {
            SyntacticError(CurrentToken, QString("Expected Statement to end with a SemiColonDelim!"));
            return nullptr;
        }
    }
    else
    {
        node = ControlStatement();
    }

    if(node == nullptr)
    {
        return nullptr;
    }

    node.reset(new StatementNode(std::move(node)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::ControlStatement()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::If)
    {
        node = IfStatement();
    }
    else if(CurrentToken->getTokenType() == SimpleToken::Switch)
    {
        node = SwitchStatement();
    }
    else if(CurrentToken->getTokenType() == SimpleToken::For)
    {
        node = ForStatement();
    }
    else if(CurrentToken->getTokenType() == SimpleToken::Do)
    {
        node = DoWhileStatement();
    }
    else if(CurrentToken->getTokenType() == SimpleToken::While)
    {
        node = WhileStatement();
    }
    else if(CurrentToken->getTokenType() == SimpleToken::Continue)
    {
        node = ContinueStatement();
    }
    else if(CurrentToken->getTokenType() == SimpleToken::Break)
    {
        node = BreakStatement();
    }
    else if(CurrentToken->getTokenType() == SimpleToken::ReturnKeyword)
    {
        node = ReturnStatement();
    }
    else
    {
        return nullptr;
    }

    if(node == nullptr)
    {
        return nullptr;
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::IfStatement()
{
    std::unique_ptr<SimpleNode> node;
    QSharedPointer<SimpleToken> token = CurrentToken;

    if(!eat(SimpleToken::If))
    {
        return nullptr;
    }
    if(!eat(SimpleToken::LParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> Condition = Expression();
    if(Condition == nullptr)
    {
        SyntacticError(token, QString("Expected expression for IfStatement!"));
    }
    if(!eat(SimpleToken::RParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> StatementBlock = Block();
    if(StatementBlock == nullptr)
    {
        SyntacticError(token, QString("Expected Statement Block after IfStatement!"));
        return nullptr;
    }

    node.reset(new IfNode(std::move(Condition), std::move(StatementBlock)));

    if(CurrentToken->getTokenType() == SimpleToken::Else)
    {
        if(!eat(SimpleToken::Else))
        {
            return nullptr;
        }
        std::unique_ptr<SimpleNode> nodeTwo = Block();
        if(nodeTwo == nullptr)
        {
            SyntacticError(token, QString("Expected Block Statement or If Statement after else!"));
            return nullptr;
        }
        dynamic_cast<IfNode*>(node.get())->addElse(std::unique_ptr<ElseNode>(new ElseNode(std::move(nodeTwo))));
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::ElseStatement()
{
    std::unique_ptr<SimpleNode> node;
    QSharedPointer<SimpleToken> token = CurrentToken;

    if(!eat(SimpleToken::Else))
    {
        return nullptr;
    }
    if(CurrentToken->getTokenType() == SimpleToken::If)
    {
        node = IfStatement();
    }
    else
    {
        node = Block();
    }

    if(node == nullptr)
    {
        SyntacticError(token, QString("Expected either IF or Block after Else!"));
        return nullptr;
    }

    node.reset(new ElseNode(std::move(node)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::SwitchStatement()
{
    std::unique_ptr<SimpleNode> node;
    std::unique_ptr<SimpleNode> nodeTwo;
    QSharedPointer<SimpleToken> token = CurrentToken;

    if(!eat(SimpleToken::Switch))
    {
        return nullptr;
    }
    if(!eat(SimpleToken::LParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> Condition = Expression();
    if(Condition == nullptr)
    {
        SyntacticError(token, QString("Expected expression for IfStatement!"));
        return nullptr;
    }
    node.reset(new SwitchNode(std::move(Condition)));
    if(!eat(SimpleToken::RParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    if(!eat(SimpleToken::LCurlyParan))
    {
        return nullptr;
    }
    while(CurrentToken->getTokenType() != SimpleToken::RCurlyParan)
    {
        if(CurrentToken->getTokenType() == SimpleToken::Case)
        {
            nodeTwo = CaseStatement();
        }
        else if(CurrentToken->getTokenType() == SimpleToken::DefaultLabel)
        {
            nodeTwo = DefaultStatement();
        }

        if(nodeTwo == nullptr)
        {
            SyntacticError(token, QString("Expected Case Label or Default Lable after Switch Statement!"));
            return nullptr;
        }
        dynamic_cast<SwitchNode*>(node.get())->addSwitchLabel(std::unique_ptr<SwitchLabel>(dynamic_cast<SwitchLabel*>(nodeTwo.release())));
    }
    if(!eat(SimpleToken::RCurlyParan))
    {
        return nullptr;
    }
    const std::vector<std::unique_ptr<SwitchLabel>> &SwitchLables = dynamic_cast<SwitchNode*>(node.get())->getSwitchLabels();
    if(SwitchLables.size() == 0)
    {
        SyntacticError(token, QString("Expected Labels after Switch!"));
        return nullptr;
    }
    if(SwitchLables.back()->getSwitchLabelType() != SwitchLabel::DefaultLable)
    {
        SyntacticError(CurrentToken, QString("Default Label is missing at the end of Switch Statement!"));
        return nullptr;
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::CaseStatement()
{
    std::unique_ptr<SimpleNode> node;
    std::unique_ptr<SimpleNode> argument;
    QSharedPointer<SimpleToken> token = CurrentToken;
    if(!eat(SimpleToken::Case))
    {
        return nullptr;
    }
    argument = PrimaryExpression();
    if(argument == nullptr)
    {
        SyntacticError(token, QString("Expected Integer after Case Label!"));
        return nullptr;
    }
    if(argument->getNodeType() == Node::Value)
    {
        if(!((dynamic_cast<ValueNode*>(argument.get())->getReturnType()) & Node::Integer))
        {
            SyntacticError(token, QString("Expected Integer after Case Label!"));
            return nullptr;
        }
    }
    else
    {
        SyntacticError(token, QString("Expected Integer after Case Label!"));
        return nullptr;
    }
    if(!eat(SimpleToken::Colon))
    {
        return nullptr;
    }
    node = Block();
    if(node == nullptr)
    {
        SyntacticError(token, QString("Expected Statement (Block) for Case Label!"));
        return nullptr;
    }

    node.reset(new CaseNode(std::unique_ptr<ValueNode>(dynamic_cast<ValueNode*>(argument.release())), std::move(node)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::DefaultStatement()
{
    std::unique_ptr<SimpleNode> node;
    QSharedPointer<SimpleToken> token = CurrentToken;
    if(!eat(SimpleToken::DefaultLabel))
    {
        return nullptr;
    }
    if(!eat(SimpleToken::Colon))
    {
        return nullptr;
    }
    node = Block();
    if(node == nullptr)
    {
        SyntacticError(token, QString("Expected Statement (Block) for Default Label!"));
        return nullptr;
    }
    node.reset(new DefaultNode(std::move(node)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::ForStatement()
{
    std::unique_ptr<SimpleNode> node;
    QSharedPointer<SimpleToken> token = CurrentToken;

    if(!eat(SimpleToken::For))
    {
        return nullptr;
    }
    if(!eat(SimpleToken::LParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> init = Expression();
    if(!eat(SimpleToken::SemiColonDelim))
    {
        return nullptr;
    }
    if(init == nullptr)
    {
        SyntacticError(token, QString("Expected Initialization Statement for ForLoop!"));
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> condition = Expression();
    if(!eat(SimpleToken::SemiColonDelim))
    {
        return nullptr;
    }
    if(condition == nullptr)
    {
        SyntacticError(token, QString("Expected Condition Statement for ForLoop!"));
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> update = Expression();
    if(update == nullptr)
    {
        SyntacticError(token, QString("Expected Update Statement for ForLoop!"));
        return nullptr;
    }
    if(!eat(SimpleToken::RParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> StatementBlock = Block();
    if(StatementBlock == nullptr)
    {
        SyntacticError(token, QString("Expected Statement Block after ForLoop!"));
        return nullptr;
    }
    node.reset(new ForLoopNode(std::move(init), std::move(condition), std::move(update), std::move(StatementBlock)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::WhileStatement()
{
    std::unique_ptr<SimpleNode> node;
    QSharedPointer<SimpleToken> token = CurrentToken;

    if(!eat(SimpleToken::While))
    {
        return nullptr;
    }
    if(!eat(SimpleToken::LParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> Condition = Expression();
    if(Condition == nullptr)
    {
        SyntacticError(token, QString("Expected Condition Statement for WhileLoop!"));
        return nullptr;
    }
    if(!eat(SimpleToken::RParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> StatementBlock = Block();
    if(StatementBlock == nullptr)
    {
        SyntacticError(token, QString("Expected Statement Block after WhileLoop!"));
        return nullptr;
    }
    node.reset(new WhileLoopNode(std::move(Condition), std::move(StatementBlock)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::DoWhileStatement()
{
    std::unique_ptr<SimpleNode> node;
    QSharedPointer<SimpleToken> token = CurrentToken;

    if(!eat(SimpleToken::Do))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> StatementBlock = Block();
    if(StatementBlock == nullptr)
    {
        SyntacticError(token, QString("Expected Statement Block after DoWhileLoop!"));
        return nullptr;
    }
    if(!eat(SimpleToken::While))
    {
        return nullptr;
    }
    if(!eat(SimpleToken::LParan))
    {
        return nullptr;
    }
    token = CurrentToken;
    std::unique_ptr<SimpleNode> Condition = Expression();
    if(Condition == nullptr)
    {
        SyntacticError(token, QString("Expected Condition Statement for WhileLoop!"));
        return nullptr;
    }
    if(!eat(SimpleToken::RParan))
    {
        return nullptr;
    }
    if(!eat(SimpleToken::SemiColonDelim))
    {
        return nullptr;
    }
    node.reset(new DoWhileLoopNode(std::move(Condition), std::move(StatementBlock)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::ContinueStatement()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::Continue)
    {
        if(!eat(SimpleToken::Continue))
        {
            return nullptr;
        }
        if(!eat(SimpleToken::SemiColonDelim))
        {
            return nullptr;
        }
        node.reset(new ContinueNode());
    }

    if(node == nullptr)
    {
        return nullptr;
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::BreakStatement()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::Break)
    {
        if(!eat(SimpleToken::Break))
        {
            return nullptr;
        }
        if(!eat(SimpleToken::SemiColonDelim))
        {
            return nullptr;
        }
        node.reset(new BreakNode());
    }

    if(node == nullptr)
    {
        return nullptr;
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::ReturnStatement()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    if(CurrentToken->getTokenType() == SimpleToken::ReturnKeyword)
    {
        if(!eat(SimpleToken::ReturnKeyword))
        {
            return nullptr;
        }
        node = Expression();
        if(node == nullptr)
        {
            node = std::unique_ptr<SimpleNode>(new VoidValueNode());
        }
        if(!eat(SimpleToken::SemiColonDelim))
        {
            return nullptr;
        }
    }

    if(node == nullptr)
    {
        return nullptr;
    }

    node.reset(new ReturnNode(std::move(node)));

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::Assign))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
        if(!eat(SimpleToken::QMark))
        {
            return nullptr;
        }
        nodeTwo = ConditionalExpression();
        if(nodeTwo == nullptr)
        {
            SyntacticError(token);
            return Q_NULLPTR;
        }
        token = CurrentToken;
        if(!eat(SimpleToken::Colon))
        {
            return nullptr;
        }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::LogicalOR))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::LogicalXOR))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::LogicalAND))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::BitwiseOR))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::BitwiseXOR))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::BitwiseAND))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif

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
            if(!eat(SimpleToken::Equal))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::Unequal))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::Greater))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::Lower))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::EqualOrGreater))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::EqualOrLower))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::LeftShift))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::RightShift))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::Plus))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::Minus))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::Multiplication))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::Division))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::Modulo))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
            if(!eat(SimpleToken::Increment))
            {
                return nullptr;
            }
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
            if(!eat(SimpleToken::Decrement))
            {
                return nullptr;
            }
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
        if(!eat(SimpleToken::Plus))
        {
            return nullptr;
        }
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
        if(!eat(SimpleToken::Minus))
        {
            return nullptr;
        }
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
        if(!eat(SimpleToken::LogicalNegation))
        {
            return nullptr;
        }
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
        if(!eat(SimpleToken::OnesComplement))
        {
            return nullptr;
        }
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
            if(!eat(SimpleToken::LParan))
            {
                return nullptr;
            }
            token = CurrentToken;
            if(!eat(SimpleToken::TypeName))
            {
                return nullptr;
            }
            if(!eat(SimpleToken::RParan))
            {
                return nullptr;
            }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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
                if(!eat(SimpleToken::Increment))
                {
                    return nullptr;
                }
                node.reset(new IncrementNode(std::move(node), false));
                if(node->getReturnType() == Node::ErrorType)
                {
                    TypeError(token, QString("Expected: Variable | IncrementExpression!"));
                    return Q_NULLPTR;
                }
                break;
            case SimpleToken::Decrement:
                token = CurrentToken;
                if(!eat(SimpleToken::Decrement))
                {
                    return nullptr;
                }
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::PrimaryExpression()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Value:
        if(!eat(SimpleToken::Value))
        {
            return nullptr;
        }
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
            SyntacticError(token, QString("Error in PrimaryExpression: ValueType Unknown"));
        }
        break;
    case SimpleToken::LParan:
        if(!eat(SimpleToken::LParan))
        {
            return nullptr;
        }
        node = Expression();
        if(node == nullptr)
        {
            return Q_NULLPTR;
        }
        if(!eat(SimpleToken::RParan))
        {
            return nullptr;
        }
        break;
    case SimpleToken::EOFToken:
#ifdef __DEBUG_OUTPUT__
        qDebug() << __PRETTY_FUNCTION__ << "EOF reached too soon!!!";
#endif
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

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
    return std::move(node);
}

std::unique_ptr<SimpleNode> SimpleParser::Symbol()
{
    std::unique_ptr<SimpleNode> node;
    SharedSimpleTokenPtr token = CurrentToken;

    switch(CurrentToken->getTokenType())
    {
    case SimpleToken::Data:
        if(!eat(SimpleToken::Data))
        {
            return nullptr;
        }
        node.reset(new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), ParentSymblTbl ));
        break;
    case SimpleToken::VariableID:
        if(!eat(SimpleToken::VariableID))
        {
            return nullptr;
        }
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
            if(!eat(SimpleToken::LParan))
            {
                return nullptr;
            }
            bool CommaDelimSet = false;
            bool expressionFound = true;
            while(expressionFound == true)
            {
                curParam = Expression();
                if(curParam != nullptr)
                {
                    //ToDO !!!
                    FuncParams.emplace_back(std::move(curParam));
                    if(CurrentToken->getTokenType() == SimpleToken::CommaDelim)
                    {
                        if(!eat(SimpleToken::CommaDelim))
                        {
                            return nullptr;
                        }
                        CommaDelimSet = true;
                    }
                    else
                    {
                        CommaDelimSet = false;
                        break;
                    }
                } else
                {
                    expressionFound = false;
                    if (CommaDelimSet == true)
                    {
                        SyntacticError(CurrentToken, QString("Expected Expression after CommaDelimiter!"));
                    }
                }
            }
            if(!eat(SimpleToken::RParan))
            {
                return nullptr;
            }
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
        //        if(!eat(SimpleToken::Data))
//                {
//                    return nullptr;
//                }
        //        node = new DataNode(qSharedPointerDynamicCast<DataToken>(token)->getDataIndex(), &SymblTbl);
        //        break;
    default:
        return Q_NULLPTR;
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << ": " << node->printNode();
#endif
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

void SimpleParser::SymbolError(SharedSimpleTokenPtr Token, QString details)
{
    if(ErrorOccured)
        return;
    ErrorOccured = true;
    lexer->LexErrorAtToken(Token, 4, details);
}
