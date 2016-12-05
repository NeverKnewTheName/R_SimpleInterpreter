#include "variablenode.h"
#include "simplestack.h"

#include <QDebug>

#include <vector>
#include <memory>

VariableNode::VariableNode(QSharedPointer<ValueSymbol> relatedVariableSymbol) :
    SimpleNode(relatedVariableSymbol->getReturnType()),
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

ASTNode *VariableNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *simpleASTNode = new ASTNode(printNode(), parentNode);
//    RelatedVariableSymbol->getValue() //ToDo...
    new ASTNode(printValue(), simpleASTNode);
    return simpleASTNode;
}

std::unique_ptr<ValueNode> VariableNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    return RelatedVariableSymbol->getValue(StackToUse);
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
