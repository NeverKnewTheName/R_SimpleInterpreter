#include "simpleast.h"
#include "operationnodes.h"
#include "simplesymboltable.h"

#include <QVariant>
#include <QDebug>

SimpleNode::SimpleNode() :
    valueType(SimpleNode::ErrorType)
{
}

QString SimpleNode::getHumanReadableTypeNameToValueType(const SimpleNode::ValueTypes type)
{
    QString HumanReadableTypeName("UNDEFINED");
    switch(type)
    {
    case SimpleNode::Integer:
        HumanReadableTypeName = QString("Integer");
        break;
    case SimpleNode::Double:
        HumanReadableTypeName = QString("Double");
        break;
    case SimpleNode::Bool:
        HumanReadableTypeName = QString("Bool");
        break;
    case SimpleNode::String:
        HumanReadableTypeName = QString("String");
        break;
    case SimpleNode::Void:
        HumanReadableTypeName = QString("Void");
        break;
    case SimpleNode::ErrorType:
        HumanReadableTypeName = QString("ErrorType");
        break;
    }

    return HumanReadableTypeName;
}

bool SimpleNode::canConvertTypes(const SimpleNode::ValueTypes OrigType, const SimpleNode::ValueTypes NewType)
{
    switch(OrigType)
    {
    case SimpleNode::Integer:
        //FALLTHROUGH
    case SimpleNode::Double:
        //FALLTHROUGH
    case SimpleNode::Bool:
        if(NewType == Integer  || NewType == Double || NewType == Bool || NewType == String)
        {
            return true;
        }
        break;
    case SimpleNode::String:
        if(NewType == String)
        {
            return true;
        }
        break;
    case SimpleNode::Void:
        if(NewType == Void)
        {
            return true;
        }
        break;
    case SimpleNode::ErrorType:
    default:
        break;
    }
    return false;
}

SimpleNode::~SimpleNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType SimpleNode::getNodeType() const
{
    return SimpleNode::ERRORNode;
}

SimpleNode::ValueTypes SimpleNode::getReturnType() const
{
    return SimpleNode::ErrorType;
}

QString SimpleNode::printValue() const
{
    return QString("INVALID VALUE");
}

QString SimpleNode::printNode() const
{
    return QString("{(INVALID NODE):(%1)}").arg(printValue);
}

ValueNodeUniquePtr SimpleNode::visit()
{
    return ValueNodeUniquePtr( new ValueNode());
}

ValueNode::ValueNode() :
    valueType(ErrorType),
    value(QVariant())
{
}

ValueNode::ValueNode(const ValueNode &valueNodeToCopy) :
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
    valueType(Integer),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const double value) :
    valueType(Double),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const bool value) :
    valueType(Bool),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const QString &value) :
    valueType(String),
    value(QVariant::fromValue(value))
{
}

ValueNode::~ValueNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
}

SimpleNode::NodeType ValueNode::getNodeType() const
{
    return SimpleNode::Value;
}

SimpleNode::ValueTypes ValueNode::getReturnType() const
{
    return getValueType();
}

ValueNode &ValueNode::operator=(const ValueNode &value)
{
    valueType = value.getValueType();
    this->value = value.getValue();
    return *this;
}

ValueNode &operator=(ValueNode &&value)
{
    valueType = value.getValueType();
    this->value = value.getValue();
    return *this;
}

const QVariant ValueNode::getValue() const
{
    return value;
}

SimpleNode::ValueTypes ValueNode::getValueType() const
{
    return valueType;
}

QString ValueNode::printValue() const
{
    switch(valueType)
    {
    case Integer:
        return QString::number(value.value<int>());
        break;
    case Double:
        return QString::number(value.value<double>());
        break;
    case Bool:
        return QString(value.value<bool>() ? "true" : "false");
        break;
    case String:
        return value.value<QString>();
        break;
    case ErrorType:
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

ValueNodeUniquePtr ValueNode::visit()
{
    //ToTHINK DOES THIS REALLY WORK?!?!
    return ValueNodeUniquePtr( new ValueNode(*this));
}



EOFNode::EOFNode()
{
}

EOFNode::~EOFNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
}

SimpleNode::NodeType EOFNode::getNodeType() const
{
    return SimpleNode::EOFNode;
}

SimpleNode::ValueTypes EOFNode::getReturnType() const
{
    return ValueNode::ErrorType;
}

QString EOFNode::printValue() const
{
    return QString("EOF");
}

QString EOFNode::printNode() const
{
    return QString("{(EOF):(EOF)}");
}

ValueNodeUniquePtr EOFNode::visit()
{
    return ValueNodeUniquePtr(new ValueNode());
}


