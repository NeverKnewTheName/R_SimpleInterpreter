#include "valuenode.h"
#include "simplestack.h"

#include <QDebug>

ValueNode::ValueNode() :
    valueType(Node::ErrorType),
    value(QVariant())
{
}

ValueNode::ValueNode(const ValueNode &valueNodeToCopy) :
    SimpleNode(),
    valueType(valueNodeToCopy.getValueType()),
    value(valueNodeToCopy.getValue())
{
    qDebug() << __PRETTY_FUNCTION__;
}

ValueNode::ValueNode(ValueNode &&valueNodeToMove) :
    valueType(valueNodeToMove.getValueType()),
    value(valueNodeToMove.getValue())
{
    qDebug() << __PRETTY_FUNCTION__;
}

ValueNode::ValueNode(const int value) :
    valueType(Node::Integer),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const double value) :
    valueType(Node::Double),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const bool value) :
    valueType(Node::Bool),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const QString &value) :
    valueType(Node::String),
    value(QVariant::fromValue(value))
{
}

ValueNode::~ValueNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType ValueNode::getNodeType() const
{
    return Node::Value;
}

Node::ValueTypes ValueNode::getReturnType() const
{
    return getValueType();
}

ValueNode &ValueNode::operator=(const ValueNode &value)
{
    valueType = value.getValueType();
    this->value = value.getValue();
    return *this;
}

ValueNode &ValueNode::operator=(ValueNode &&value)
{
    valueType = value.getValueType();
    this->value = value.getValue();
    return *this;
}

const QVariant ValueNode::getValue() const
{
    return value;
}

Node::ValueTypes ValueNode::getValueType() const
{
    return valueType;
}

QString ValueNode::printValue() const
{
    switch(valueType)
    {
    case Node::Integer:
        return QString::number(value.value<int>());
        break;
    case Node::Double:
        return QString::number(value.value<double>());
        break;
    case Node::Bool:
        return QString(value.value<bool>() ? "true" : "false");
        break;
    case Node::String:
        return value.value<QString>();
        break;
    case Node::ErrorType:
    default:
        return QString("ERROR");
    }
}

QString ValueNode::printNode() const
{
    QString NodeType = "ValueNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<ValueNode> ValueNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    //ToTHINK DOES THIS REALLY WORK?!?!
    return std::unique_ptr<ValueNode>( new ValueNode(*this));
}
