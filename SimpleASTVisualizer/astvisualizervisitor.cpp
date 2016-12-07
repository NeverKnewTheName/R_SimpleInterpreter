#include "astvisualizervisitor.h"

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

#include "astvisualizer.h"

#include <QDebug>

ASTVisualizerVisitor::ASTVisualizerVisitor() :
    VisualizedASTRootNode(nullptr)
{

}

ASTVisualizerVisitor::~ASTVisualizerVisitor()
{
    if(VisualizedASTRootNode != nullptr)
    {
        delete VisualizedASTRootNode;
    }
    qDebug() << __PRETTY_FUNCTION__;
}


ASTNode *ASTVisualizerVisitor::getVisualizedASTRootNode()
{
    ASTNode *returnValue = VisualizedASTRootNode;
    VisualizedASTRootNode = nullptr;
    return returnValue;
}


void ASTVisualizerVisitor::visit(std::unique_ptr<SimpleNode> NodeToVisit)
{
    NodeToVisit->accept(this);
}

void ASTVisualizerVisitor::visit(std::unique_ptr<AssignmentNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getVariableToAssign()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getValueToAssign()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ANDNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<AdditionNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ConditionalNode> NodeToVisit)
{
    ASTNode *TernaryOPASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = TernaryOPASTNode;
    QStringList TernaryOpSplit = NodeToVisit->printValue().split(' ');
    NodeToVisit->getTernaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = TernaryOPASTNode;
    new ASTNode(TernaryOpSplit.first(),TernaryOPASTNode);
    NodeToVisit->getTernaryOPMidChild()->accept(this);
    VisualizedASTRootNode = TernaryOPASTNode;
    new ASTNode(TernaryOpSplit.last(),TernaryOPASTNode);
    NodeToVisit->getTernaryOPRightChild()->accept(this);
    VisualizedASTRootNode = TernaryOPASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<DecrementNode> NodeToVisit)
{
    ASTNode *UnaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = UnaryASTNode;
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    new ASTNode(NodeToVisit->printValue(), UnaryASTNode);
    VisualizedASTRootNode = UnaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<DivisionNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<DoWhileLoopNode> NodeToVisit)
{
    //ToDO5
}

void ASTVisualizerVisitor::visit(std::unique_ptr<EOFNode> NodeToVisit)
{
    VisualizedASTRootNode = new ASTNode(NodeToVisit->printValue());
}

void ASTVisualizerVisitor::visit(std::unique_ptr<EqualNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<EqualOrGreaterNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<EqualOrLowerNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ExpressionNode> NodeToVisit)
{
    //ToDO
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ForLoopNode> NodeToVisit)
{
    //ToDO
}

void ASTVisualizerVisitor::visit(std::unique_ptr<FunctionCallNode> NodeToVisit)
{
    ASTNode *FunctionASTNode = new ASTNode(NodeToVisit->printNode(), VisualizedASTRootNode);
    VisualizedASTRootNode = FunctionASTNode;

    QSharedPointer<FunctionSymbol> RelatedSymbol = NodeToVisit->getRelatedSymbol();

    const std::vector<QSharedPointer<VariableSymbol>> &params = RelatedSymbol->getFunctionParameters();
    std::vector<QSharedPointer<VariableSymbol>>::const_iterator itParams = params.begin();
    std::vector<QSharedPointer<VariableSymbol>>::const_iterator itParamsEnd = params.end();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itArgs = NodeToVisit->getFuncArgs().begin();
    for(;itParams != itParamsEnd; ++itParams, ++itArgs)
    {
        ASTNode *paramAssignment = new ASTNode(QString("ParameterAssignment"),FunctionASTNode);
        ASTNode *parameterASTNode = new ASTNode(QString("Parameter: ").append((*itParams)->PrintToSymbolToString()), paramAssignment);
        new ASTNode((*itParams)->getIdentifier(), parameterASTNode);
        new ASTNode(QString("="), paramAssignment);
        VisualizedASTRootNode = paramAssignment;
        (*itArgs)->accept(this);
    }

    VisualizedASTRootNode = FunctionASTNode;

    const std::vector<std::unique_ptr<SimpleNode>> &FunctionExpressions = RelatedSymbol->getFunctionExpressions();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator it = FunctionExpressions.begin();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itEnd = FunctionExpressions.end();
    for( ;it != itEnd; ++it)
    {
        (*it)->accept(this);
        new ASTNode(QString(";"), FunctionASTNode);
        VisualizedASTRootNode = FunctionASTNode;
    }

    RelatedSymbol->getFunctionReturnNode()->accept(this);
    new ASTNode(QString(";"), FunctionASTNode);

    VisualizedASTRootNode = FunctionASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<GreaterNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<IncrementNode> NodeToVisit)
{
    ASTNode *UnaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = UnaryASTNode;
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    new ASTNode(NodeToVisit->printValue(), UnaryASTNode);
    VisualizedASTRootNode = UnaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<LeftShiftNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<LogicalANDNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<LogicalNegationNode> NodeToVisit)
{
    ASTNode *UnaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = UnaryASTNode;
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    new ASTNode(NodeToVisit->printValue(), UnaryASTNode);
    VisualizedASTRootNode = UnaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<LogicalORNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<LogicalXORNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<LowerNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ModuloNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<MultiplicationNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<NegativeNode> NodeToVisit)
{
    ASTNode *UnaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = UnaryASTNode;
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    new ASTNode(NodeToVisit->printValue(), UnaryASTNode);
    VisualizedASTRootNode = UnaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ORNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<PositiveNode> NodeToVisit)
{
    ASTNode *UnaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = UnaryASTNode;
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    new ASTNode(NodeToVisit->printValue(), UnaryASTNode);
    VisualizedASTRootNode = UnaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ProgramNode> NodeToVisit)
{
    ASTNode *ProgramASTNode = new ASTNode(NodeToVisit->printNode(), VisualizedASTRootNode);

    VisualizedASTRootNode = ProgramASTNode;

    const std::vector<std::unique_ptr<SimpleNode>> &ProgramExpressions = NodeToVisit->getProgramExpressions();

    for( const std::unique_ptr<SimpleNode> &expr : ProgramExpressions)
    {
        expr->accept(this);
        VisualizedASTRootNode = ProgramASTNode;
    }

    const std::unique_ptr<SimpleNode> &ProgramReturnStatement = NodeToVisit->getProgramReturnStatement();

    ProgramReturnStatement->accept(this);

    VisualizedASTRootNode = ProgramASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<RightShiftNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ScopeNode> NodeToVisit)
{
    //ToDO
}

void ASTVisualizerVisitor::visit(std::unique_ptr<StatementNode> NodeToVisit)
{
    //ToDO
}

void ASTVisualizerVisitor::visit(std::unique_ptr<SubtractionNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<TypeCastNode> NodeToVisit)
{
    ASTNode *UnaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = UnaryASTNode;
    NodeToVisit->getUnaryOPRightChild()->accept(this);
    new ASTNode(NodeToVisit->printValue(), UnaryASTNode);
    VisualizedASTRootNode = UnaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<UnequalNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ValueNode> NodeToVisit)
{
    ASTNode *simpleASTNode = new ASTNode(NodeToVisit->printNode(), VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), simpleASTNode);
    VisualizedASTRootNode = simpleASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<VariableNode> NodeToVisit)
{
    ASTNode *simpleASTNode = new ASTNode(NodeToVisit->printNode(), VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), simpleASTNode);
    VisualizedASTRootNode = simpleASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<VoidValueNode> NodeToVisit)
{
    ASTNode *simpleASTNode = new ASTNode(NodeToVisit->printNode(), VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), simpleASTNode);
    VisualizedASTRootNode = simpleASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<WhileLoopNode> NodeToVisit)
{
    //ToDO
}

void ASTVisualizerVisitor::visit(std::unique_ptr<XORNode> NodeToVisit)
{
    ASTNode *BinaryASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BinaryASTNode;
    NodeToVisit->getBinaryOPLeftChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
    new ASTNode(NodeToVisit->printValue(), BinaryASTNode);
    NodeToVisit->getBinaryOPRightChild()->accept(this);
    VisualizedASTRootNode = BinaryASTNode;
}
