#include "variablenode.h"
#include "simplestack.h"

#include <QDebug>

#include <vector>
#include <memory>

#include "astvisualizer.h"
#include "simplenodevisitor.h"

VariableNode::VariableNode(QSharedPointer<ValueSymbol> relatedVariableSymbol) :
    TerminalNode(),
    RelatedVariableSymbol(relatedVariableSymbol)
{
}

VariableNode::VariableNode(const VariableNode &ToCopy) :
    RelatedVariableSymbol(ToCopy.RelatedVariableSymbol)
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

std::unique_ptr<SimpleNode> VariableNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new VariableNode(*this));
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

QSharedPointer<ValueSymbol> VariableNode::getRelatedVariableSymbol() const
{
    return RelatedVariableSymbol;
}


void VariableNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<VariableNode>(new VariableNode(*this)));
}
