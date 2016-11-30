#include "variablesymbol.h"
#include "simplestack.h"

#include <QDebug>

VariableSymbol::VariableSymbol(const QString &identifier,
                               Node::ValueTypes VariableType
                               ) :
    SimpleSymbol(identifier),
    AssignedNode(ValueNode()),
    VariableType(VariableType)
{

}

VariableSymbol::~VariableSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

std::unique_ptr<ValueNode> VariableSymbol::getAssignedValue() const
{
    return std::unique_ptr<ValueNode>( new ValueNode( AssignedNode ) );
}

bool VariableSymbol::assignValue(const SimpleNode &NodeToAssign)
{
    ValueNode newValueNode;// = valueNode; //Save the previous value because it could be referenced in the assignment!!!

    if(!SimpleNode::canConvertTypes(VariableType, NodeToAssign.getReturnType()))
    {
        qDebug() << "ERROR: TypeMismatch";
        return false;
    }

    switch(VariableType)
    {
    case Node::Integer:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign.visit()->getValue().value<int>())));
                break;
    case Node::Double:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign.visit()->getValue().value<double>())));
                break;
    case Node::Bool:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign.visit()->getValue().value<bool>())));
                break;
    case Node::String:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign.visit()->getValue().value<QString>())));
                break;
    case Node::Void:
    case Node::ErrorType:
    default:
        newValueNode = std::move(ValueNode());
    }

    qDebug() << "Previous Value: " << AssignedNode.visit()->getValue();

    AssignedNode = std::move(newValueNode);

    qDebug() << "Value Assigned: " << AssignedNode.visit()->getValue();

    return true;
}

SimpleSymbolTableEntry::SymbolTableEntryType VariableSymbol::getType() const
{
    return SimpleSymbolTableEntry::Variable;
}

QString VariableSymbol::PrintToSymbolToString() const
{
    return QString("%1 %2").arg(SimpleNode::getHumanReadableTypeNameToValueType(getVariableType())).arg(identifier);
}

QString VariableSymbol::PrintSymbolType() const
{
    return QString("Variable");
}

Node::ValueTypes VariableSymbol::getReturnType() const
{
    return VariableType;
}

Node::ValueTypes VariableSymbol::getVariableType() const
{
    return VariableType;
}
