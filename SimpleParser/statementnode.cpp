#include "statementnode.h"

#include "simplenodevisitor.h"

#include <QDebug>

StatementNode::StatementNode(std::unique_ptr<SimpleNode> Statement) :
    Statement(std::move(Statement))
{

}

StatementNode::StatementNode(const StatementNode &ToCopy) :
    Statement(ToCopy.Statement->deepCopy())
{

}

StatementNode::~StatementNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const std::unique_ptr<SimpleNode> &StatementNode::getStatement() const
{
    return Statement;
}

void StatementNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<StatementNode>(new StatementNode(*this)));
}

Node::NodeType StatementNode::getNodeType() const
{
    return Node::Statement;
}

Node::ValueTypes StatementNode::getReturnType() const
{
    return Node::Void; //ToDO
}

QString StatementNode::printValue() const
{
    return QString("Statement"); //ToDO
}

QString StatementNode::printNode() const
{
    return QString("{(StatementNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> StatementNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new StatementNode(*this));
}
