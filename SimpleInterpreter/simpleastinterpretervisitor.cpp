#include "simpleastinterpretervisitor.h"

#include "simplenode.h"
#include "assignmentnode.h"
#include "expressionnode.h"
#include "functioncallnode.h"
#include "programnode.h"
#include "statementnode.h"
#include "valuenode.h"
#include "variablenode.h"
#include "unaryoperationnodes.h"
#include "binaryoperationnodes.h"
#include "ternaryoperationnodes.h"

#include "simplesymboltable.h"
#include "variablesymbol.h"
#include "functionsymbol.h"

#include <QDebug>

SimpleASTInterpreterVisitor::SimpleASTInterpreterVisitor() :
    InterpreterStack(new SimpleStack(1000))
{

}

SimpleASTInterpreterVisitor::~SimpleASTInterpreterVisitor()
{
    qDebug() << __PRETTY_FUNCTION__;
}

std::unique_ptr<ValueNode> SimpleASTInterpreterVisitor::getInterpreterResult() const
{
    if(InterpreterResult != nullptr)
    {
        return std::unique_ptr<ValueNode>(new ValueNode(*InterpreterResult));
    }
    else
    {
        return std::unique_ptr<ValueNode>(new VoidValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<SimpleNode> NodeToVisit)
{
    NodeToVisit->accept(this);
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<AssignmentNode> NodeToVisit)
{
    NodeToVisit->getValueToAssign()->accept(this);
    std::unique_ptr<ValueNode> value = std::move(InterpreterResult);

    QSharedPointer<ValueSymbol> relatedSymbol = NodeToVisit->getVariableToAssign()->getRelatedVariableSymbol();

    if(relatedSymbol->getType() == SimpleSymbolTableEntry::Variable)
    {
//        qSharedPointerDynamicCast<VariableSymbol>(relatedSymbol)->assignValue(std::move(value), InterpreterStack);
        size_t AddressInStack = qSharedPointerDynamicCast<VariableSymbol>(relatedSymbol)->getStackAddress();
        InterpreterStack->StackReplaceAt(AddressInStack, std::move(value));
    }

    const size_t AddressInstack = relatedSymbol->getStackAddress();
    InterpreterResult = InterpreterStack->StackAt(AddressInstack);
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ANDNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() & value2->getValue().value<int>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<AdditionNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();
    Node::ValueTypes implicitCastLeft = NodeToVisit->getImplicitCastLeftChild();

    switch(implicitCastLeft)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Double)
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() + value2->getValue().value<double>()));
        }
        else
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() + value2->getValue().value<int>()));
        }
        break;
    case Node::Double:
        //RightChild can only be Integer or Double -> cast anyway
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() + value2->getValue().value<double>()));
        break;
    case Node::String:
        //RightChild is cast to String anyway
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<QString>() + value2->getValue().value<QString>()));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ConditionalNode> NodeToVisit)
{
    NodeToVisit->getTernaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastMid = NodeToVisit->getImplicitCastMidChild();
    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();

    bool IsTrue = value1->getValue().value<bool>();

    std::unique_ptr<ValueNode> returnValue;
    if(IsTrue)
    {
        NodeToVisit->getTernaryOPMidChild()->accept(this);
    }
    else
    {
        NodeToVisit->getTernaryOPRightChild()->accept(this);
    }

    switch(implicitCastMid)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Double)
        {
            InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode(InterpreterResult->getValue().value<double>()));
        }
        else if(implicitCastRight == Node::Bool)
        {
            InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode(InterpreterResult->getValue().value<bool>()));
        }
        else
        {
            InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode(InterpreterResult->getValue().value<int>()));
        }
        break;
    case Node::Double:
        if(implicitCastRight == Node::Bool)
        {
            InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode(InterpreterResult->getValue().value<bool>()));
        }
        else
        {
            InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode(InterpreterResult->getValue().value<double>()));
        }
        break;
    case Node::Bool:
        InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode(InterpreterResult->getValue().value<bool>()));
        break;
    case Node::String:
        InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode(InterpreterResult->getValue().value<QString>()));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>(new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<DecrementNode> NodeToVisit)
{
    const bool isPre = NodeToVisit->getIsPre();
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value = std::move(InterpreterResult);

    std::unique_ptr<ValueNode> Result( new ValueNode(InterpreterResult->getValue().value<int>() - 1) );

    QSharedPointer<ValueSymbol> relatedSymbol = dynamic_cast<VariableNode*>(NodeToVisit->getUnaryOPRightChild().get())->getRelatedVariableSymbol();

    if(relatedSymbol->getType() == SimpleSymbolTableEntry::Variable)
    {
//        qSharedPointerDynamicCast<VariableSymbol>(relatedSymbol)->assignValue(std::move(Result), InterpreterStack);
        size_t AddressInStack = qSharedPointerDynamicCast<VariableSymbol>(relatedSymbol)->getStackAddress();
        InterpreterStack->StackReplaceAt(AddressInStack, std::move(Result));
    }

    if(isPre)
    {
        InterpreterResult = std::move(value);
    }
    else
    {
        NodeToVisit->getUnaryOPRightChild()->accept(this);
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<DivisionNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();
    Node::ValueTypes implicitCastLeft = NodeToVisit->getImplicitCastLeftChild();

    switch(implicitCastLeft)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Double)
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() / value2->getValue().value<double>()));
        }
        else
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() / value2->getValue().value<int>()));
        }
        break;
    case Node::Double:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() / value2->getValue().value<double>()));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<DoWhileLoopNode> NodeToVisit)
{
    //ToDO
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<EOFNode> NodeToVisit)
{
    Q_UNUSED(NodeToVisit)
    InterpreterResult = std::unique_ptr<ValueNode>(new VoidValueNode());
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<EqualNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();
    Node::ValueTypes implicitCastLeft = NodeToVisit->getImplicitCastLeftChild();

    switch(implicitCastLeft)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Integer)
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() == value2->getValue().value<int>()) ? true : false ));
        }
        else if( implicitCastRight == Node::Double )
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() == value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRight == Node::Bool )
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Double:
        if((implicitCastRight == Node::Double) || ( implicitCastRight == Node::Integer ))
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() == value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRight == Node::Bool )
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Bool:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false ));
        break;
    case Node::String:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((!value1->getValue().value<QString>().compare(value2->getValue().value<QString>())) ? true : false ));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<EqualOrGreaterNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();
    Node::ValueTypes implicitCastLeft = NodeToVisit->getImplicitCastLeftChild();

    switch(implicitCastLeft)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Integer)
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() >= value2->getValue().value<int>()) ? true : false ));
        }
        else
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() >= value2->getValue().value<double>()) ? true : false ));
        }
        break;
    case Node::Double:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() >= value2->getValue().value<double>()) ? true : false ));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<EqualOrLowerNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();
    Node::ValueTypes implicitCastLeft = NodeToVisit->getImplicitCastLeftChild();

    switch(implicitCastLeft)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Integer)
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() <= value2->getValue().value<int>()) ? true : false ));
        }
        else
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() <= value2->getValue().value<double>()) ? true : false ));
        }
        break;
    case Node::Double:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() <= value2->getValue().value<double>()) ? true : false ));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ExpressionNode> NodeToVisit)
{
    //ToDO
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ForLoopNode> NodeToVisit)
{
    //ToDO
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<FunctionCallNode> NodeToVisit)
{
    //    return RelatedSymbol->CallFunction(FuncArgs, StackToUse);
    QSharedPointer<FunctionSymbol> RelatedSymbol = NodeToVisit->getRelatedSymbol();
    const std::vector<std::unique_ptr<SimpleNode>> &FuncArgs = NodeToVisit->getFuncArgs();

    const std::vector<QSharedPointer<VariableSymbol> > &FunctionParameters = RelatedSymbol->getFunctionParameters();
    const std::vector<std::unique_ptr<SimpleNode>> &FunctionExpressions = RelatedSymbol->getFunctionExpressions();
    const std::unique_ptr<SimpleNode> &FunctionReturnNode = RelatedSymbol->getFunctionReturnNode();
    QSharedPointer<SimpleSymbolTable> FunctionSymbolTable = RelatedSymbol->getFunctionSymbolTable();

    if(FunctionParameters.size() != FuncArgs.size())
    {
        qDebug() << "Number of passed arguments does not match function parameters!";
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }

    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itFuncArgsBegin = FuncArgs.begin();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itFuncArgsEnd = FuncArgs.end();
    std::vector<std::unique_ptr<ValueNode>> EvaluatedFuncArgs;
    for(; itFuncArgsBegin != itFuncArgsEnd; itFuncArgsBegin++)
    {
        (*itFuncArgsBegin)->accept(this);
        EvaluatedFuncArgs.emplace_back(std::move(InterpreterResult));
    }
    FunctionSymbolTable->EnterScope(InterpreterStack);

    const size_t NrOfParameters = FunctionParameters.size();
    for(size_t i = 0; i < NrOfParameters; i++)
    {
        const QSharedPointer<VariableSymbol> &param = FunctionParameters.at(i);
        std::unique_ptr<ValueNode> &argument = EvaluatedFuncArgs.at(i);
//        param->assignValue(std::move(argument), InterpreterStack);
        size_t AddressInStack = param->getStackAddress();
        InterpreterStack->StackReplaceAt(AddressInStack, std::move(argument));
    }


    for(const std::unique_ptr<SimpleNode> &expression : FunctionExpressions)
    {
        expression->accept(this);
    }
    FunctionReturnNode->accept(this);

    FunctionSymbolTable->ExitScope(InterpreterStack);
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<GreaterNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() > value2->getValue().value<double>()) ?  true : false ));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<IncrementNode> NodeToVisit)
{
    const bool isPre = NodeToVisit->getIsPre();
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value = std::move(InterpreterResult);

    std::unique_ptr<ValueNode> Result( new ValueNode(InterpreterResult->getValue().value<int>() + 1) );

    QSharedPointer<ValueSymbol> relatedSymbol = dynamic_cast<VariableNode*>(NodeToVisit->getUnaryOPRightChild().get())->getRelatedVariableSymbol();

    if(relatedSymbol->getType() == SimpleSymbolTableEntry::Variable)
    {
//        qSharedPointerDynamicCast<VariableSymbol>(relatedSymbol)->assignValue(std::move(Result), InterpreterStack);
        size_t AddressInStack = qSharedPointerDynamicCast<VariableSymbol>(relatedSymbol)->getStackAddress();
        InterpreterStack->StackReplaceAt(AddressInStack, std::move(Result));
    }

    if(isPre)
    {
        InterpreterResult = std::move(value);
    }
    else
    {
        NodeToVisit->getUnaryOPRightChild()->accept(this);
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<LeftShiftNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() << value2->getValue().value<int>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<LogicalANDNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult =  std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<bool>() && value2->getValue().value<bool>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<LogicalNegationNode> NodeToVisit)
{
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<LogicalORNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult =  std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<bool>() || value2->getValue().value<bool>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<LogicalXORNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<bool>() != value2->getValue().value<bool>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<LowerNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() < value2->getValue().value<double>()) ?  true : false ));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ModuloNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() % value2->getValue().value<int>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<MultiplicationNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() * value2->getValue().value<int>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<NegativeNode> NodeToVisit)
{
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();

    switch(implicitCastRight)
    {
    case Node::Integer:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<int>() * -1));
        break;
    case Node::Double:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<double>() * -1));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ORNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() | value2->getValue().value<int>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<PositiveNode> NodeToVisit)
{
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value = std::move(InterpreterResult);
    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();

    switch(implicitCastRight)
    {
    case Node::Integer:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<int>()));
        break;
    case Node::Double:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<double>()));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ProgramNode> NodeToVisit)
{
    QSharedPointer<SimpleSymbolTable> ProgramSymbolTable = NodeToVisit->getProgramSymbolTable();
    const std::vector<std::unique_ptr<SimpleNode>> &ProgramExpressions = NodeToVisit->getProgramExpressions();
    const std::unique_ptr<SimpleNode> &ProgramReturnStatement = NodeToVisit->getProgramReturnStatement();

    ProgramSymbolTable->EnterScope(InterpreterStack);

    for(const std::unique_ptr<SimpleNode> &expr : ProgramExpressions)
    {
        expr->accept(this);
    }

    ProgramReturnStatement->accept(this);

    ProgramSymbolTable->ExitScope(InterpreterStack);
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<RightShiftNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() >> value2->getValue().value<int>()));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ScopeNode> NodeToVisit)
{
    //ToDO
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<StatementNode> NodeToVisit)
{
    //ToDO
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<SubtractionNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();
    Node::ValueTypes implicitCastLeft = NodeToVisit->getImplicitCastLeftChild();

    switch(implicitCastLeft)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Double)
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() - value2->getValue().value<double>()));
        }
        else
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() - value2->getValue().value<int>()));
        }
        break;
    case Node::Double:
        //RightChild can only be Integer or Double -> cast anyway
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() - value2->getValue().value<double>()));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<TypeCastNode> NodeToVisit)
{
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value = std::move(InterpreterResult);
    Node::ValueTypes typeToCastTo = NodeToVisit->getTypeToCastTo();

    switch(typeToCastTo)
    {
    case Node::Integer:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<int>()));
        break;
    case Node::Double:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<double>()));
        break;
    case Node::Bool:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<bool>()));
        break;
    case Node::String:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<QString>()));
        break;
    case Node::ErrorType:
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<UnequalNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    Node::ValueTypes implicitCastRight = NodeToVisit->getImplicitCastRightChild();
    Node::ValueTypes implicitCastLeft = NodeToVisit->getImplicitCastLeftChild();

    switch(implicitCastLeft)
    {
    case Node::Integer:
        if(implicitCastRight == Node::Integer)
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() != value2->getValue().value<int>()) ? true : false ));
        }
        else if( implicitCastRight == Node::Double )
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() != value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRight == Node::Bool )
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Double:
        if((implicitCastRight == Node::Double) || ( implicitCastRight == Node::Integer ))
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() != value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRight == Node::Bool )
        {
            InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Bool:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false ));
        break;
    case Node::String:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<QString>().compare(value2->getValue().value<QString>())) ? true : false ));
        break;
    default:
        InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode());
    }
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<ValueNode> NodeToVisit)
{
    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(*NodeToVisit));
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<VariableNode> NodeToVisit)
{
    QSharedPointer<ValueSymbol> RelatedVariableSymbol = NodeToVisit-> getRelatedVariableSymbol();
//    InterpreterResult = RelatedVariableSymbol->getValue(InterpreterStack);

    const size_t AddressInstack = RelatedVariableSymbol->getStackAddress();
    InterpreterResult = InterpreterStack->StackAt(AddressInstack);
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<VoidValueNode> NodeToVisit)
{
    InterpreterResult = std::unique_ptr<ValueNode>( new VoidValueNode());
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<WhileLoopNode> NodeToVisit)
{
    //ToDO
}

void SimpleASTInterpreterVisitor::visit(std::unique_ptr<XORNode> NodeToVisit)
{
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    std::unique_ptr<ValueNode> value1 = std::move(InterpreterResult);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    std::unique_ptr<ValueNode> value2 = std::move(InterpreterResult);

    InterpreterResult = std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() ^ value2->getValue().value<int>()));
}

