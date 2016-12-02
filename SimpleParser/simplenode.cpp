#include "simplenode.h"

#include <QVariant>
#include <QDebug>

#include "valuenode.h"
#include "operationnodes.h"
#include "simplesymboltable.h"

#include "simplestack.h"

#include <memory>
#include <vector>

SimpleNode::SimpleNode(Node::ValueTypes valueType) :
    valueType(valueType)
{
}

QString SimpleNode::getHumanReadableTypeNameToValueType(const Node::ValueTypes type)
{
    QString HumanReadableTypeName("UNDEFINED");
    switch(type)
    {
    case Node::Integer:
        HumanReadableTypeName = QString("Integer");
        break;
    case Node::Double:
        HumanReadableTypeName = QString("Double");
        break;
    case Node::Bool:
        HumanReadableTypeName = QString("Bool");
        break;
    case Node::String:
        HumanReadableTypeName = QString("String");
        break;
    case Node::Void:
        HumanReadableTypeName = QString("Void");
        break;
    case Node::ErrorType:
        HumanReadableTypeName = QString("ErrorType");
        break;
    }

    return HumanReadableTypeName;
}

bool SimpleNode::canConvertTypes(const Node::ValueTypes OrigType, const Node::ValueTypes NewType)
{
    switch(OrigType)
    {
    case Node::Integer:
        //FALLTHROUGH
    case Node::Double:
        //FALLTHROUGH
    case Node::Bool:
        if(NewType == Node::Integer  || NewType == Node::Double || NewType == Node::Bool || NewType == Node::String)
        {
            return true;
        }
        break;
    case Node::String:
        if(NewType == Node::String)
        {
            return true;
        }
        break;
    case Node::Void:
        if(NewType == Node::Void)
        {
            return true;
        }
        break;
    case Node::ErrorType:
    default:
        break;
    }
    return false;
}

SimpleNode::~SimpleNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType SimpleNode::getNodeType() const
{
    return Node::ERRORNode;
}

Node::ValueTypes SimpleNode::getReturnType() const
{
    return Node::ErrorType;
}

QString SimpleNode::printValue() const
{
    return QString("INVALID VALUE");
}

QString SimpleNode::printNode() const
{
    return QString("{(INVALID NODE):(%1)}").arg(printValue());
}

std::unique_ptr<ValueNode> SimpleNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    Q_UNUSED(StackToUse)
    return std::unique_ptr<ValueNode>( new ValueNode());
}

EOFNode::EOFNode()
{
}

EOFNode::~EOFNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
}

Node::NodeType EOFNode::getNodeType() const
{
    return Node::EOFNode;
}

Node::ValueTypes EOFNode::getReturnType() const
{
    return Node::ErrorType;
}

QString EOFNode::printValue() const
{
    return QString("EOF");
}

QString EOFNode::printNode() const
{
    return QString("{(EOF):(EOF)}");
}

std::unique_ptr<ValueNode> EOFNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    Q_UNUSED(StackToUse)
    return std::unique_ptr<ValueNode>(new ValueNode());
}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > EOFNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, QSharedPointer<SimpleStack> StackToUse) const
{
    Q_UNUSED(StackToUse)
    FlatAST->emplace_back(deepCopy());
    return FlatAST;
}

std::unique_ptr<SimpleNode> EOFNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new EOFNode());
}


