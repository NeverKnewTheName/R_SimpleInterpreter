#ifndef ASTVISUALIZERVISITOR_H
#define ASTVISUALIZERVISITOR_H

#include "simplenodevisitor.h"

#include "astvisualizer.h"

class ASTNode;

class ASTVisualizerVisitor : public SimpleNodeVisitor
{
public:
    ASTVisualizerVisitor();
    ~ASTVisualizerVisitor();

    ASTNode *getVisualizedASTRootNode();

    // SimpleNodeVisitor interface
public:
    void visit(std::unique_ptr<SimpleNode> NodeToVisit);
    void visit(std::unique_ptr<AssignmentNode> NodeToVisit);
    void visit(std::unique_ptr<ANDNode> NodeToVisit);
    void visit(std::unique_ptr<AdditionNode> NodeToVisit);
    void visit(std::unique_ptr<ConditionalNode> NodeToVisit);
    void visit(std::unique_ptr<DecrementNode> NodeToVisit);
    void visit(std::unique_ptr<DivisionNode> NodeToVisit);
    void visit(std::unique_ptr<DoWhileLoopNode> NodeToVisit);
    void visit(std::unique_ptr<EOFNode> NodeToVisit);
    void visit(std::unique_ptr<EqualNode> NodeToVisit);
    void visit(std::unique_ptr<EqualOrGreaterNode> NodeToVisit);
    void visit(std::unique_ptr<EqualOrLowerNode> NodeToVisit);
    void visit(std::unique_ptr<ExpressionNode> NodeToVisit);
    void visit(std::unique_ptr<ForLoopNode> NodeToVisit);
    void visit(std::unique_ptr<FunctionCallNode> NodeToVisit);
    void visit(std::unique_ptr<GreaterNode> NodeToVisit);
    void visit(std::unique_ptr<IncrementNode> NodeToVisit);
    void visit(std::unique_ptr<LeftShiftNode> NodeToVisit);
    void visit(std::unique_ptr<LogicalANDNode> NodeToVisit);
    void visit(std::unique_ptr<LogicalNegationNode> NodeToVisit);
    void visit(std::unique_ptr<LogicalORNode> NodeToVisit);
    void visit(std::unique_ptr<LogicalXORNode> NodeToVisit);
    void visit(std::unique_ptr<LowerNode> NodeToVisit);
    void visit(std::unique_ptr<ModuloNode> NodeToVisit);
    void visit(std::unique_ptr<MultiplicationNode> NodeToVisit);
    void visit(std::unique_ptr<NegativeNode> NodeToVisit);
    void visit(std::unique_ptr<ORNode> NodeToVisit);
    void visit(std::unique_ptr<PositiveNode> NodeToVisit);
    void visit(std::unique_ptr<ProgramNode> NodeToVisit);
    void visit(std::unique_ptr<RightShiftNode> NodeToVisit);
    void visit(std::unique_ptr<ScopeNode> NodeToVisit);
    void visit(std::unique_ptr<StatementNode> NodeToVisit);
    void visit(std::unique_ptr<SubtractionNode> NodeToVisit);
    void visit(std::unique_ptr<TypeCastNode> NodeToVisit);
    void visit(std::unique_ptr<UnequalNode> NodeToVisit);
    void visit(std::unique_ptr<ValueNode> NodeToVisit);
    void visit(std::unique_ptr<VariableNode> NodeToVisit);
    void visit(std::unique_ptr<VoidValueNode> NodeToVisit);
    void visit(std::unique_ptr<WhileLoopNode> NodeToVisit);
    void visit(std::unique_ptr<XORNode> NodeToVisit);

private:
    ASTNode *VisualizedASTRootNode;

    // SimpleNodeVisitor interface
public:
    void visit(std::unique_ptr<CaseNode> NodeToVisit);
    void visit(std::unique_ptr<DefaultNode> NodeToVisit);
    void visit(std::unique_ptr<BlockNode> NodeToVisit);
    void visit(std::unique_ptr<BreakNode> NodeToVisit);
    void visit(std::unique_ptr<ContinueNode> NodeToVisit);
    void visit(std::unique_ptr<ElseNode> NodeToVisit);
    void visit(std::unique_ptr<IfNode> NodeToVisit);
    void visit(std::unique_ptr<ReturnNode> NodeToVisit);
    void visit(std::unique_ptr<SwitchNode> NodeToVisit);

};

#endif // ASTVISUALIZERVISITOR_H
