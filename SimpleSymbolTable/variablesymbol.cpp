#include "variablesymbol.h"
#include "simplestack.h"

#include <QDebug>

VariableSymbol::VariableSymbol(const QString &identifier,
                               const Node::ValueTypes VariableType
                               ) :
    ValueSymbol(identifier, VariableType)
{
}

VariableSymbol::~VariableSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

std::unique_ptr<ValueNode> VariableSymbol::getAssignedValue(QSharedPointer<SimpleStack> StackToUse) const
{
    //The currently assigned value will be at the address of the current scope (last scope for symbol) in the stack
    return StackToUse->StackAt(AddressPerScope.back());
}

size_t VariableSymbol::getStackAddress() const
{
    return AddressPerScope.back();
}

bool VariableSymbol::assignValue(std::unique_ptr<ValueNode> NodeToAssign, QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> newValueNode;// = valueNode; //Save the previous value because it could be referenced in the assignment!!!

    Node::ValueTypes returnType = getReturnType();
    //ToDO
    if(!SimpleNode::canConvertTypes(returnType, NodeToAssign->getReturnType()))
    {
        qDebug() << "ERROR: TypeMismatch";
        return false;
    }

    //CAST
    switch(returnType)
    {
    case Node::Integer:
        newValueNode.reset(new ValueNode(std::move(NodeToAssign->getValue().value<int>())));
                break;
    case Node::Double:
        newValueNode.reset(new ValueNode(std::move(NodeToAssign->getValue().value<double>())));
                break;
    case Node::Bool:
        newValueNode.reset(new ValueNode(std::move(NodeToAssign->getValue().value<bool>())));
                break;
    case Node::String:
        newValueNode.reset(new ValueNode(std::move(NodeToAssign->getValue().value<QString>())));
                break;
    case Node::Void:
    case Node::ErrorType:
    default:
        newValueNode.reset(new ValueNode());
    }

    size_t Address = AddressPerScope.back();

//    qDebug() << "Previous Value: " << StackToUse->StackAt(Address)->getValue();

    StackToUse->StackReplaceAt(Address, std::move(newValueNode));

//    qDebug() << "Value Assigned: " << StackToUse->StackAt(Address)->getValue();

    return true;
}

SimpleSymbolTableEntry::SymbolTableEntryType VariableSymbol::getType() const
{
    return SimpleSymbolTableEntry::Variable;
}

QString VariableSymbol::PrintToSymbolToString() const
{
    return QString("%1 %2").arg(SimpleNode::getHumanReadableTypeNameToValueType(getReturnType())).arg(getIdentifier());
}

QString VariableSymbol::PrintSymbolType() const
{
    return QString("Variable");
}

std::unique_ptr<ValueNode> VariableSymbol::getValue(QSharedPointer<SimpleStack> StackToUse) const
{
    return getAssignedValue(StackToUse);
}

bool VariableSymbol::VarEnterScope(QSharedPointer<SimpleStack> StackToUse)
{
    qDebug() << "Scope entered";
    //Push Symbol on Stack with default Value
    //Enter new scope
    AddressPerScope.push_back(
                StackToUse->StackPush(
                    ValueNode()
                    )
                );
}

bool VariableSymbol::VarExitScope(QSharedPointer<SimpleStack> StackToUse)
{
    qDebug() << "Scope exited";
    //Delete Symbol from stack
    AddressPerScope.pop_back();
    //Delete Scope
    StackToUse->StackPop();
}














ValueSymbol::ValueSymbol(const QString &identifier, const Node::ValueTypes ValueType) :
    SimpleSymbol(identifier),
    ValueType(ValueType)
{

}

ValueSymbol::~ValueSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::ValueTypes ValueSymbol::getReturnType() const
{
    return ValueType;
}

ConstantValueSymbol::ConstantValueSymbol(const QString &identifier, ValueNode value) :
    ValueSymbol(identifier, value.getReturnType()),
    ConstVal(value)
{
}

ConstantValueSymbol::~ConstantValueSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleSymbolTableEntry::SymbolTableEntryType ConstantValueSymbol::getType() const
{
    return SimpleSymbolTableEntry::ConstantValue;
}

QString ConstantValueSymbol::PrintToSymbolToString() const
{
    return QString("%1 %2").arg(SimpleNode::getHumanReadableTypeNameToValueType(getReturnType())).arg(getIdentifier());
}

QString ConstantValueSymbol::PrintSymbolType() const
{
    return QString("ConstantValue");
}

std::unique_ptr<ValueNode> ConstantValueSymbol::getValue(QSharedPointer<SimpleStack> StackToUse) const
{
    Q_UNUSED(StackToUse)
    return std::unique_ptr<ValueNode>(new ValueNode(ConstVal));
}


size_t ConstantValueSymbol::getStackAddress() const
{
    return -1; //ToDO
}
