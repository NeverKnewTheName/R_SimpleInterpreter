#include "assignmentnode.h"

#include <QDebug>

#include "valuenode.h"
#include "variablenode.h"
#include "simplestack.h"

AssignmentNode::AssignmentNode(std::unique_ptr<VariableNode> VariableToAssign, std::unique_ptr<SimpleNode> ValueToAssign) :
    SimpleNode(VariableToAssign->getReturnType()),
    VariableToAssign(std::move(VariableToAssign)),
    ValueToAssign(std::move(ValueToAssign))
{
    if(this->VariableToAssign->getReturnType() == Node::ErrorType) valueType = Node::ErrorType;
    if(this->ValueToAssign->getReturnType() == Node::ErrorType) valueType = Node::ErrorType;
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
    return VariableToAssign->getReturnType();
}

QString AssignmentNode::printValue() const
{
    return QString("%1 = %2").arg(VariableToAssign->printValue()).arg(ValueToAssign->printValue());
}

QString AssignmentNode::printNode() const
{
    return QString("{AssignmentNode}:{%1}").arg(printValue());
}

std::unique_ptr<ValueNode> AssignmentNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    VariableToAssign->getRelatedVariableSymbol()->assignValue(*ValueToAssign);

    return VariableToAssign->visit();
}

