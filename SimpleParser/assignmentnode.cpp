#include "assignmentnode.h"

#include <QDebug>

#include "valuenode.h"
#include "variablenode.h"
#include "simplestack.h"

#include "astvisualizer.h"
#include "simplenodevisitor.h"

AssignmentNode::AssignmentNode(std::unique_ptr<VariableNode> VariableToAssign, std::unique_ptr<SimpleNode> ValueToAssign) :
    VariableToAssign(std::move(VariableToAssign)),
    ValueToAssign(std::move(ValueToAssign)),
    ReturnType(this->VariableToAssign->getReturnType())
{
    if(this->VariableToAssign->getReturnType() == Node::ErrorType) ReturnType = Node::ErrorType;
    if(this->ValueToAssign->getReturnType() == Node::ErrorType) ReturnType = Node::ErrorType;
}

AssignmentNode::AssignmentNode(const AssignmentNode &ToCopy) :
    VariableToAssign(dynamic_cast<VariableNode*>(ToCopy.VariableToAssign->deepCopy().release())),
    ValueToAssign(ToCopy.ValueToAssign->deepCopy()),
    ReturnType(this->VariableToAssign->getReturnType())
{
}

AssignmentNode::~AssignmentNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType AssignmentNode::getNodeType() const
{
    return Node::Assignment;
}

Node::ValueTypes AssignmentNode::getReturnType() const
{
    return ReturnType;
}

QString AssignmentNode::printValue() const
{
    return QString("=");
}

QString AssignmentNode::printNode() const
{
    return QString("{AssignmentNode}:{%1}").arg(printValue());
}

std::unique_ptr<SimpleNode> AssignmentNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new AssignmentNode(*this));
}

const std::unique_ptr<VariableNode> &AssignmentNode::getVariableToAssign() const
{
    return VariableToAssign;
}

const std::unique_ptr<SimpleNode> &AssignmentNode::getValueToAssign() const
{
    return ValueToAssign;
}

void AssignmentNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<AssignmentNode>(new AssignmentNode(*this)));
}
