#include "variablenode.h"

#include <QDebug>

VariableNode::VariableNode(QSharedPointer<VariableSymbol> relatedVariableSymbol) :
    SimpleNode(relatedVariableSymbol->getVariableType()),
    RelatedVariableSymbol(relatedVariableSymbol)
{
}

VariableNode::~VariableNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType VariableNode::getNodeType() const
{
    return Node::Variable;
}

Node::ValueTypes VariableNode::getReturnType() const
{
    return RelatedVariableSymbol->getReturnType();
}

std::unique_ptr<ValueNode> VariableNode::visit() const
{
    return RelatedVariableSymbol->getAssignedValue();
}

QString VariableNode::printValue() const
{
    return RelatedVariableSymbol->getIdentifier();
}

QString VariableNode::printNode() const
{
    QString NodeType("VariableNode");
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

QString VariableNode::getVariableName() const
{
    return RelatedVariableSymbol->getIdentifier();
}

QSharedPointer<VariableSymbol> VariableNode::getRelatedVariableSymbol() const
{
    return RelatedVariableSymbol;
}
