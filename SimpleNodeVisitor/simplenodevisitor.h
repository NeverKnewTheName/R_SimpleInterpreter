#ifndef SIMPLENODEVISITOR_H
#define SIMPLENODEVISITOR_H

#include <memory>

class SimpleNode;
class AssignmentNode;
class ANDNode;
class AdditionNode;
class BlockNode;
class BreakNode;
class CaseNode;
class ConditionalNode;
class ContinueNode;
class DecrementNode;
class DefaultNode;
class DivisionNode;
class DoWhileLoopNode;
class EOFNode;
class ElseNode;
class EqualNode;
class EqualOrGreaterNode;
class EqualOrLowerNode;
class ExpressionNode;
class ForLoopNode;
class FunctionCallNode;
class GreaterNode;
class IfNode;
class IncrementNode;
class LeftShiftNode;
class LogicalANDNode;
class LogicalNegationNode;
class LogicalORNode;
class LogicalXORNode;
class LowerNode;
class ModuloNode;
class MultiplicationNode;
class NegativeNode;
class ORNode;
class PositiveNode;
class ProgramNode;
class ReturnNode;
class RightShiftNode;
class ScopeNode;
class StatementNode;
class SubtractionNode;
class SwitchNode;
class TypeCastNode;
class UnequalNode;
class ValueNode;
class VariableNode;
class VoidValueNode;
class WhileLoopNode;
class XORNode;


class SimpleNodeVisitor
{
public:
    SimpleNodeVisitor();
    virtual ~SimpleNodeVisitor();

    virtual void visit(std::unique_ptr<SimpleNode> NodeToVisit) = 0;
    virtual void visit(std::unique_ptr<AssignmentNode> NodeToVisit) = 0;
    virtual void visit(std::unique_ptr<ANDNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<AdditionNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<BlockNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<BreakNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<CaseNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ConditionalNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ContinueNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<DecrementNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<DefaultNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<DivisionNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<DoWhileLoopNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<EOFNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ElseNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<EqualNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<EqualOrGreaterNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<EqualOrLowerNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ExpressionNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ForLoopNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<FunctionCallNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<GreaterNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<IfNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<IncrementNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<LeftShiftNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<LogicalANDNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<LogicalNegationNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<LogicalORNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<LogicalXORNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<LowerNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ModuloNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<MultiplicationNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<NegativeNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ORNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<PositiveNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ProgramNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ReturnNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<RightShiftNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ScopeNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<StatementNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<SubtractionNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<SwitchNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<TypeCastNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<UnequalNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<ValueNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<VariableNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<VoidValueNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<WhileLoopNode> NodeToVisit)= 0;
    virtual void visit(std::unique_ptr<XORNode> NodeToVisit)= 0;
};

#endif // SIMPLENODEVISITOR_H
