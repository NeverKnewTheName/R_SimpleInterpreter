#include "expressionnode.h"

#include "simplenodevisitor.h"


ExpressionNode::ExpressionNode()
{

}


void ExpressionNode::accept(SimpleNodeVisitor *visitor) const
{
}

Node::NodeType ExpressionNode::getNodeType() const
{
}

Node::ValueTypes ExpressionNode::getReturnType() const
{
}

QString ExpressionNode::printValue() const
{
}

QString ExpressionNode::printNode() const
{
}

std::unique_ptr<SimpleNode> ExpressionNode::deepCopy() const
{
}
