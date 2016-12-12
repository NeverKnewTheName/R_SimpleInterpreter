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
#include "blocknode.h"
#include "selectioncontrolnode.h"
#include "iterationcontrolnode.h"
#include "escapecontrolnode.h"

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
    ASTNode *DoWhileASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), DoWhileASTNode);
    VisualizedASTRootNode = DoWhileASTNode;
    NodeToVisit->getIterationStatement()->accept(this);
    VisualizedASTRootNode = DoWhileASTNode;
    new ASTNode(QString("while"),DoWhileASTNode);
    new ASTNode(QString("("), DoWhileASTNode);
    NodeToVisit->getWhileCondition()->accept(this);
    VisualizedASTRootNode = DoWhileASTNode;
    new ASTNode(QString(")"), DoWhileASTNode);

    VisualizedASTRootNode = DoWhileASTNode;
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
    ASTNode *ExpressionASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), ExpressionASTNode);
    VisualizedASTRootNode = ExpressionASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ForLoopNode> NodeToVisit)
{
    ASTNode *ForASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), ForASTNode);
    VisualizedASTRootNode = ForASTNode;
    new ASTNode(QString("("), ForASTNode);

    NodeToVisit->getForLoopInitialization()->accept(this);
    VisualizedASTRootNode = ForASTNode;
    new ASTNode(QString(";"), ForASTNode);

    NodeToVisit->getForLoopCondition()->accept(this);
    VisualizedASTRootNode = ForASTNode;
    new ASTNode(QString(";"), ForASTNode);

    NodeToVisit->getForLoopUpdate()->accept(this);
    VisualizedASTRootNode = ForASTNode;

    new ASTNode(QString(")"), ForASTNode);
    NodeToVisit->getIterationStatement()->accept(this);
    VisualizedASTRootNode = ForASTNode;
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

    const std::unique_ptr<SimpleNode> &FunctionStatement = RelatedSymbol->getFunctionStatement();
    FunctionStatement->accept(this);

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
    ASTNode *StatementASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), StatementASTNode);
    VisualizedASTRootNode = StatementASTNode;
    NodeToVisit->getStatement()->accept(this);
    new ASTNode(QString(";"), StatementASTNode);
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
    ASTNode *WhileASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), WhileASTNode);
    VisualizedASTRootNode = WhileASTNode;
    new ASTNode(QString("("), WhileASTNode);
    NodeToVisit->getWhileCondition()->accept(this);
    VisualizedASTRootNode = WhileASTNode;
    new ASTNode(QString(")"), WhileASTNode);

    NodeToVisit->getIterationStatement()->accept(this);
    VisualizedASTRootNode = WhileASTNode;
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

void ASTVisualizerVisitor::visit(std::unique_ptr<CaseNode> NodeToVisit)
{
    ASTNode *CaseASTNode = new ASTNode(NodeToVisit->printNode(), VisualizedASTRootNode);
    VisualizedASTRootNode = CaseASTNode;
    new ASTNode(QString("case"), CaseASTNode);
    NodeToVisit->getCaseArgument()->accept(this);
    VisualizedASTRootNode = CaseASTNode;
    new ASTNode(QString(":"), CaseASTNode);
    NodeToVisit->getSwitchLabelStatement()->accept(this);
}

void ASTVisualizerVisitor::visit(std::unique_ptr<BlockNode> NodeToVisit)
{
    ASTNode *BlockASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    VisualizedASTRootNode = BlockASTNode;
    new ASTNode(QString("{"), BlockASTNode);
    const std::vector<std::unique_ptr<SimpleNode>> &BlockStatements = NodeToVisit->getBlockStatements();
    for(const std::unique_ptr<SimpleNode> &stmt : BlockStatements)
    {
        stmt->accept(this);
        VisualizedASTRootNode = BlockASTNode;
    }
    new ASTNode(QString("}"), BlockASTNode);
}

void ASTVisualizerVisitor::visit(std::unique_ptr<BreakNode> NodeToVisit)
{
    ASTNode *BreakASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(QString("break"), BreakASTNode);
    new ASTNode(QString(";"), BreakASTNode);
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ContinueNode> NodeToVisit)
{
    ASTNode *ContinueASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(QString("continue"), ContinueASTNode);
    new ASTNode(QString(";"), ContinueASTNode);
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ElseNode> NodeToVisit)
{
    ASTNode *ElseASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), ElseASTNode);
    VisualizedASTRootNode = ElseASTNode;
    NodeToVisit->getElseStatementBlock()->accept(this);
    VisualizedASTRootNode = ElseASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<IfNode> NodeToVisit)
{
    ASTNode *IfASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), IfASTNode);
    VisualizedASTRootNode = IfASTNode;
    new ASTNode(QString("("), IfASTNode);
    NodeToVisit->getIfCondition()->accept(this);
    VisualizedASTRootNode = IfASTNode;
    new ASTNode(QString(")"), IfASTNode);
    NodeToVisit->getIfStatementBlock()->accept(this);
    VisualizedASTRootNode = IfASTNode;
}

void ASTVisualizerVisitor::visit(std::unique_ptr<ReturnNode> NodeToVisit)
{
    ASTNode *ReturnASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), ReturnASTNode);
    VisualizedASTRootNode = ReturnASTNode;
    NodeToVisit->getReturnExpression()->accept(this);
    new ASTNode(QString(";"), ReturnASTNode);
}

void ASTVisualizerVisitor::visit(std::unique_ptr<SwitchNode> NodeToVisit)
{
    ASTNode *SwitchASTNode = new ASTNode(NodeToVisit->printNode(),VisualizedASTRootNode);
    new ASTNode(NodeToVisit->printValue(), SwitchASTNode);
    VisualizedASTRootNode = SwitchASTNode;
    new ASTNode(QString("("), SwitchASTNode);
    NodeToVisit->getSwitchCondition()->accept(this);
    VisualizedASTRootNode = SwitchASTNode;
    new ASTNode(QString(")"), SwitchASTNode);
    const std::vector<std::unique_ptr<SwitchLabel>> &SwitchLabels = NodeToVisit->getSwitchLabels();
    for(const std::unique_ptr<SwitchLabel> &label : SwitchLabels)
    {
        label->accept(this);
        VisualizedASTRootNode = SwitchASTNode;
    }
    VisualizedASTRootNode = SwitchASTNode;
}


void ASTVisualizerVisitor::visit(std::unique_ptr<DefaultNode> NodeToVisit)
{
    ASTNode *DefaultASTNode = new ASTNode(NodeToVisit->printNode(), VisualizedASTRootNode);
    VisualizedASTRootNode = DefaultASTNode;
    new ASTNode(QString("default:"), DefaultASTNode);
    NodeToVisit->getSwitchLabelStatement()->accept(this);
}
