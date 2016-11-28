#include "simpleast.h"
#include "simplesymboltable.h"

#include <QVariant>
#include <QDebug>

SimpleNode::SimpleNode()
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

ValueNode &ValueNode::visit()
{
    return *this;
}

DataNode::DataNode(const unsigned int dataIndex, const SymbolTable * const SymblTbl) :
    dataIndex(dataIndex),
    SymblTbl(SymblTbl)
{
}

DataNode::~DataNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
}

SimpleNode::NodeType DataNode::getNodeType() const
{
    return SimpleNode::Data;
}

SimpleNode::ValueTypes DataNode::getReturnType() const
{
    return ValueNode::Integer;
}

ValueNode &DataNode::visit()
{
    Result = ValueNode(dynamic_cast<VariableSymbol*>(const_cast<SymbolTable*>(SymblTbl)->lookup(QString("D%1").arg(dataIndex)))->getAssignedValue()->getValue().value<int>());
    return Result;
}

QString DataNode::printValue() const
{
    return QString("D%1").arg(dataIndex);
}

QString DataNode::printNode() const
{
    QString NodeType = "DataNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

VariableNode::VariableNode(const QString &VariableName, SymbolTable * const SymblTbl, SimpleNode::ValueTypes type) :
    VariableName(VariableName),
    RelatedVariableSymbol(dynamic_cast<VariableSymbol*>(SymblTbl->lookup(VariableName))),
    type(type)
{
}

VariableNode::~VariableNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType VariableNode::getNodeType() const
{
    return SimpleNode::Variable;
}

SimpleNode::ValueTypes VariableNode::getReturnType() const
{
    return RelatedVariableSymbol->getVariableType();;
}

void VariableNode::setAssignment(SimpleNode *assignment)
{
    RelatedAssignmentNode = assignment;
    RelatedVariableSymbol->assignValue(assignment);
}

ValueNode &VariableNode::visit()
{
    ValueNode *symblTblEntryValue = RelatedVariableSymbol->getAssignedValue();
    if(symblTblEntryValue == NULL)
        Result = ValueNode();
    else
        Result = ValueNode(symblTblEntryValue->visit());

    return Result;
}

QString VariableNode::printValue() const
{
    return QString(VariableName);
}

QString VariableNode::printNode() const
{
    QString NodeType("VariableNode");
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

QString VariableNode::getVariableName() const
{
    return VariableName;
}

VariableSymbol *VariableNode::getRelatedVariableSymbol() const
{
    return RelatedVariableSymbol;
}

OperationNode::OperationNode()
{
}

OperationNode::~OperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType OperationNode::getNodeType() const
{
    return SimpleNode::Operation;
}

SimpleNode::ValueTypes OperationNode::getReturnType() const
{
    return returnType;
}

ValueNode &OperationNode::visit()
{
    return DoOperation();
}

UnaryOperationNode::UnaryOperationNode(SimpleNode *rightChild) :
    rightChild(rightChild)
{
}

UnaryOperationNode::~UnaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete rightChild;
}

OperationNode::ArityTypes UnaryOperationNode::getArityType() const
{
    return OperationNode::Unary;
}

UnaryArithmeticOperationNode::UnaryArithmeticOperationNode(SimpleNode *rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

TypeCastNode::TypeCastNode(SimpleNode *rightChild, SimpleNode::ValueTypes typeToCastTo) :
    UnaryArithmeticOperationNode(rightChild),
    typeToCastTo(typeToCastTo)
{
    returnType = rightChild->getReturnType();

    switch(returnType)
    {
    case SimpleNode::Integer:
    case SimpleNode::Double:
    case SimpleNode::Bool:
        if(typeToCastTo != SimpleNode::ErrorType )
        {
            implicitCastRightChild = typeToCastTo;
            returnType = typeToCastTo;
        }
        else
        {
            implicitCastRightChild = SimpleNode::ErrorType;
            returnType = ErrorType;
        }
        break;
    case SimpleNode::String:
        if(typeToCastTo == SimpleNode::String)
        {
            implicitCastRightChild = SimpleNode::String;
            returnType = SimpleNode::String;
        }
        else
        {
            implicitCastRightChild = SimpleNode::ErrorType;
            returnType = ErrorType;
        }
        break;
    case SimpleNode::ErrorType:
        implicitCastRightChild = SimpleNode::ErrorType;
        returnType = ErrorType;
        break;
    }
}

OperationNode::Operation TypeCastNode::getOp() const
{
    return OperationNode::TypeCast;
}

OperationNode::Associativity TypeCastNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence TypeCastNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

ValueNode &TypeCastNode::DoOperation()
{
    ValueNode &value = rightChild->visit();

    switch(typeToCastTo)
    {
    case SimpleNode::Integer:
        Result = ValueNode(value.getValue().value<int>());
        break;
    case SimpleNode::Double:
        Result = ValueNode(value.getValue().value<double>());
        break;
    case SimpleNode::Bool:
        Result = ValueNode(value.getValue().value<bool>());
        break;
    case SimpleNode::String:
        Result = ValueNode(value.getValue().value<QString>());
        break;
    case SimpleNode::ErrorType:
        Result = ValueNode();
        break;
    }

    return Result;
}

QString TypeCastNode::printValue() const
{
    QString TypeClearName;
    switch(typeToCastTo)
    {
    case SimpleNode::Integer:
        TypeClearName = QString("Integer");
        break;
    case SimpleNode::Double:
        TypeClearName = QString("Double");
        break;
    case SimpleNode::Bool:
        TypeClearName = QString("Bool");
        break;
    case SimpleNode::String:
        TypeClearName = QString("String");
        break;
    case SimpleNode::ErrorType:
        TypeClearName = QString("ErrorType");
        break;
    }

    return QString("(%1)").arg(TypeClearName);
}

QString TypeCastNode::printNode() const
{
    return QString("{(TypeCast):(%1)}").arg(printValue());
}

IncrementNode::IncrementNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if(returnType == ValueNode::Integer)
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else
    {
        returnType = ValueNode::ErrorType;
    }
}

OperationNode::Operation IncrementNode::getOp() const
{
    return OperationNode::Increment;
}

OperationNode::Associativity IncrementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence IncrementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

ValueNode &IncrementNode::DoOperation()
{
    bool IsVariable = false;
    ValueNode &value = rightChild->visit();

    Result = ValueNode(value.getValue().value<int>() + 1);

    dynamic_cast<VariableNode*>(rightChild)->setAssignment(new ValueNode(Result));

    return Result;
}

QString IncrementNode::printValue() const
{
    return QString("++");
}

QString IncrementNode::printNode() const
{
    QString NodeType = "IncrementNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

DecrementNode::DecrementNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if(returnType == ValueNode::Integer)
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else
    {
        returnType = ValueNode::ErrorType;
    }
}

OperationNode::Operation DecrementNode::getOp() const
{
    return OperationNode::Decrement;
}

OperationNode::Associativity DecrementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence DecrementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

ValueNode &DecrementNode::DoOperation()
{
    ValueNode &value = rightChild->visit();

    Result = ValueNode(value.getValue().value<int>() - 1);

    dynamic_cast<VariableNode*>(rightChild)->setAssignment(new ValueNode(Result));

    return Result;
}

QString DecrementNode::printValue() const
{
    return QString("--");
}

QString DecrementNode::printNode() const
{
    QString NodeType = "DecrementNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

PositiveNode::PositiveNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( returnType == ValueNode::Integer )
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else if( returnType == ValueNode::Double )
    {
        implicitCastRightChild = SimpleNode::Double;
    }
    else
    {
        returnType = ValueNode::ErrorType;
    }
}

OperationNode::Operation PositiveNode::getOp() const
{
    return OperationNode::Positive;
}

OperationNode::Associativity PositiveNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence PositiveNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

ValueNode &PositiveNode::DoOperation()
{
    ValueNode &value = rightChild->visit();
    switch(implicitCastRightChild)
    {
    case ValueNode::Integer:
        Result = ValueNode(value.getValue().value<int>());
        break;
    case ValueNode::Double:
        Result = ValueNode(value.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString PositiveNode::printValue() const
{
    return QString("+");
}

QString PositiveNode::printNode() const
{
    QString NodeType = "PositiveNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

NegativeNode::NegativeNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( returnType == ValueNode::Integer )
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else if( returnType == ValueNode::Double )
    {
        implicitCastRightChild = SimpleNode::Double;
    }
    else
    {
        returnType = ValueNode::ErrorType;
    }
}

OperationNode::Operation NegativeNode::getOp() const
{
    return OperationNode::Negative;
}

OperationNode::Associativity NegativeNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence NegativeNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

ValueNode &NegativeNode::DoOperation()
{
    ValueNode &value = rightChild->visit();
    switch(implicitCastRightChild)
    {
    case ValueNode::Integer:
        Result = ValueNode(value.getValue().value<int>() * -1);
        break;
    case ValueNode::Double:
        Result = ValueNode(value.getValue().value<double>() * -1);
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString NegativeNode::printValue() const
{
    return QString("-");
}

QString NegativeNode::printNode() const
{
    QString NodeType = "NegativeNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

UnaryLogicalOperationNode::UnaryLogicalOperationNode(SimpleNode *rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalNegationNode::LogicalNegationNode(SimpleNode *rightChild) :
    UnaryLogicalOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( ( returnType != ValueNode::Integer ) && ( returnType != ValueNode::Double )  && ( returnType != ValueNode::Bool ) )
        returnType = ValueNode::ErrorType;
}

OperationNode::Operation LogicalNegationNode::getOp() const
{
    return OperationNode::LogicalNegation;
}

OperationNode::Associativity LogicalNegationNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence LogicalNegationNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

ValueNode &LogicalNegationNode::DoOperation()
{
    ValueNode &value = rightChild->visit();
    bool IsTrue = value.getValue().value<bool>();
    switch(implicitCastRightChild)
    {
    case ValueNode::Integer:
        Result = ValueNode(IsTrue ? 0 : 1);
        break;
    case ValueNode::Double:
        Result = ValueNode(IsTrue ? 0.0 : 1.0);
        break;
    case ValueNode::Bool:
        Result =  ValueNode(IsTrue ? false : true);
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString LogicalNegationNode::printValue() const
{
    return QString("!");
}

QString LogicalNegationNode::printNode() const
{
    QString NodeType = "LogicalNegationNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

UnaryBitwiseOperationNode::UnaryBitwiseOperationNode(SimpleNode *rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

OnesComplementNode::OnesComplementNode(SimpleNode *rightChild) :
    UnaryBitwiseOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( ( returnType != ValueNode::Integer ) )
        returnType = ValueNode::ErrorType;
}

OperationNode::Operation OnesComplementNode::getOp() const
{
    return OperationNode::OnesComplement;
}

OperationNode::Associativity OnesComplementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence OnesComplementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

ValueNode &OnesComplementNode::DoOperation()
{
    ValueNode &value = rightChild->visit();

    Result =  ValueNode(~(value.getValue().value<int>()));

    return Result;
}

QString OnesComplementNode::printValue() const
{
    return QString("~");
}

QString OnesComplementNode::printNode() const
{
    QString NodeType = "OnesComplementNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

BinaryOperationNode::BinaryOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    leftChild(leftChild),
    rightChild(rightChild)
{
}

BinaryOperationNode::~BinaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete leftChild;
    delete rightChild;
}

OperationNode::ArityTypes BinaryOperationNode::getArityType() const
{
    return OperationNode::Binary;
}

BinaryArithmeticOperationNode::BinaryArithmeticOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{
}

OperationNode::OperationTypes BinaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

AdditionNode::AdditionNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer + Integer -> Integer
         * (implicit cast to) Integer + Double  -> Double
         */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = ValueNode::Integer;
            implicitCastRightChild = ValueNode::Integer;
            returnType = ValueNode::Integer;
        }
        else if( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = ValueNode::Double;
            implicitCastRightChild = ValueNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Double:
        /*
         * Double + Double   -> Double
         * Double + (implicit cast to) Integer  -> Double
         */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = ValueNode::Double;
            implicitCastRightChild = ValueNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Bool: // Bool + ??? does not make sense
        break;
    case ValueNode::String:
        /*
         * String + String  -> Concatenation
         * String + Integer/Double -> String + String(Integer/Double)
         * String + Bool -> String + Bool ? String("true") : String("false")
         */
        if( returnTypeRChild != ValueNode::ErrorType )
        {
            implicitCastLeftChild = ValueNode::String;
            implicitCastRightChild = ValueNode::String;
            returnType = ValueNode::String;
        }
        break;
    }
}

OperationNode::Operation AdditionNode::getOp() const
{
    return OperationNode::Addition;
}

OperationNode::Associativity AdditionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence AdditionNode::getPrecedence() const
{
    return OperationNode::AdditivePrec;
}

ValueNode &AdditionNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == ValueNode::Double)
        {
            Result =  ValueNode(value1.getValue().value<double>() + value2.getValue().value<double>());
        }
        else
        {
            Result =  ValueNode(value1.getValue().value<int>() + value2.getValue().value<int>());
        }
        break;
    case ValueNode::Double:
        //RightChild can only be Integer or Double -> cast anyway
        Result =  ValueNode(value1.getValue().value<double>() + value2.getValue().value<double>());
        break;
    case ValueNode::String:
        //RightChild is cast to String anyway
        Result =  ValueNode(value1.getValue().value<QString>() + value2.getValue().value<QString>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString AdditionNode::printValue() const
{
    return QString("+");
}

QString AdditionNode::printNode() const
{
    QString NodeType = "AdditionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

SubtractionNode::SubtractionNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer - Integer -> Integer
         * Integer - Double  -> (implicit cast to) Double
         */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Integer;
        }
        else if( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Double:
        /*
         * Double - Double   -> Double
         * Double - Integer  -> (implicit cast to) Double
         */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Bool: // Bool - ??? does not make sense
        break;
    case ValueNode::String: // String - ??? does not make sense
        break;
    }
}

OperationNode::Operation SubtractionNode::getOp() const
{
    return OperationNode::Subtraction;
}

OperationNode::Associativity SubtractionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence SubtractionNode::getPrecedence() const
{
    return OperationNode::AdditivePrec;
}

ValueNode &SubtractionNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            Result =  ValueNode(value1.getValue().value<double>() - value2.getValue().value<double>());
        }
        else
        {
            Result =  ValueNode(value1.getValue().value<int>() - value2.getValue().value<int>());
        }
        break;
    case ValueNode::Double:
        Result =  ValueNode(value1.getValue().value<double>() - value2.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString SubtractionNode::printValue() const
{
    return QString("-");
}

QString SubtractionNode::printNode() const
{
    QString NodeType = "SubtractionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

MultiplicationNode::MultiplicationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
          * Integer * Integer -> Integer
          * Integer * Double  -> (implicit cast to) Double
          */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Integer;
        }
        else if( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Double:
        /*
          * Double * Double   -> Double
          * Double * Integer  -> (implicit cast to) Double
          */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Bool: // Bool + ??? does not make sense
        break;
    case ValueNode::String:
        /*
          * String * Integer -> String repeated for the value of the Integer and concatenated
          */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::String;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::String;
        }
        break;
    }
}

OperationNode::Operation MultiplicationNode::getOp() const
{
    return OperationNode::Multiplication;
}

OperationNode::Associativity MultiplicationNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence MultiplicationNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

ValueNode &MultiplicationNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            Result =  ValueNode(value1.getValue().value<double>() * value2.getValue().value<double>());
        }
        else
        {
            Result =  ValueNode(value1.getValue().value<int>() * value2.getValue().value<int>());
        }
        break;
    case ValueNode::Double:
        Result =  ValueNode(value1.getValue().value<double>() * value2.getValue().value<double>());
        break;
    case SimpleNode::String:
    {
        QString input(value1.getValue().value<QString>());
        QString output(input);
        int cntr = value2.getValue().value<int>();
        if(cntr--)
        {
            while(cntr)
            {
                output.append(input);
                cntr--;
            }
        }
        Result =  ValueNode( output );
    }
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString MultiplicationNode::printValue() const
{
    return QString("*");
}

QString MultiplicationNode::printNode() const
{
    QString NodeType = "MultiplicationNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

DivisionNode::DivisionNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case SimpleNode::Integer:
        /*
          * Integer / Integer -> Integer
          * Integer / Double  -> (implicit cast to) Double
          */
        if( returnTypeRChild == SimpleNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = SimpleNode::Integer;
        }
        else if( returnTypeRChild == SimpleNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case SimpleNode::Double:
        /*
          * Double / Double   -> Double
          * Double / Integer  -> (implicit cast to) Double
          */
        if( ( returnTypeRChild == SimpleNode::Double ) || ( returnTypeRChild == SimpleNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = SimpleNode::Double;
        }
        break;
    case SimpleNode::Bool: // Bool / ??? does not make sense
        break;
    case SimpleNode::String: // String / ??? does not make sense
        break;
    }
}

OperationNode::Operation DivisionNode::getOp() const
{
    return OperationNode::Division;
}

OperationNode::Associativity DivisionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence DivisionNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

ValueNode &DivisionNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            Result =  ValueNode(value1.getValue().value<double>() / value2.getValue().value<double>());
        }
        else
        {
            Result =  ValueNode(value1.getValue().value<int>() / value2.getValue().value<int>());
        }
        break;
    case ValueNode::Double:
        Result =  ValueNode(value1.getValue().value<double>() / value2.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString DivisionNode::printValue() const
{
    return QString("/");
}

QString DivisionNode::printNode() const
{
    QString NodeType = "DivisionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

ModuloNode::ModuloNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
           * Integer % Integer -> Integer
           */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // No
        break;
    case ValueNode::Bool: // Bool % ??? does not make sense
        break;
    case ValueNode::String: // String % ??? does not make sense
        break;
    }
}

OperationNode::Operation ModuloNode::getOp() const
{
    return OperationNode::Modulo;
}

OperationNode::Associativity ModuloNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ModuloNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

ValueNode &ModuloNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result =  ValueNode(value1.getValue().value<int>() % value2.getValue().value<int>());

    return Result;
}

QString ModuloNode::printValue() const
{
    return QString("%");
}

QString ModuloNode::printNode() const
{
    QString NodeType = "ModuloNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

BinaryLogicalOperationNode::BinaryLogicalOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{
}

OperationNode::OperationTypes BinaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalANDNode::LogicalANDNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer && Integer -> Bool (true if both are non zero)
         * Integer && Double -> Bool (true if both are non zero)
         * Integer && Bool -> Bool (true if Integer is non zero and Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double && Integer -> Bool (true if both are non zero)
         * Double && Double -> Bool (true if both are non zero)
         * Double && Bool -> Bool (true if Double is non zero and Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Bool: // Bool % ??? does not make sense
        /*
         * Bool && Integer -> Bool (true if both are non zero)
         * Bool && Double -> Bool (true if both are non zero)
         * Bool && Bool -> Bool (true if both are true)
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String: // String && ??? does not make sense
        break;
    }
}

OperationNode::Operation LogicalANDNode::getOp() const
{
    return OperationNode::LogicalAND;
}

OperationNode::Associativity LogicalANDNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalANDNode::getPrecedence() const
{
    return OperationNode::LogicalANDPrec;
}

ValueNode &LogicalANDNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result =  ValueNode(value1.getValue().value<bool>() && value2.getValue().value<bool>());

    return Result;
}

QString LogicalANDNode::printValue() const
{
    return QString("&&");
}

QString LogicalANDNode::printNode() const
{
    QString NodeType = "LogicalANDNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LogicalORNode::LogicalORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer || Integer -> Bool (true if either is non zero)
         * Integer || Double -> Bool (true if either is non zero)
         * Integer || Bool -> Bool (true if Integer is non zero or Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double || Integer -> Bool (true if either is non zero)
         * Double || Double -> Bool (true if either is non zero)
         * Double || Bool -> Bool (true if Double is non zero or Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Bool: // Bool % ??? does not make sense
        /*
         * Bool || Integer -> Bool (true if either Bool is true or Integer is non zero)
         * Bool || Double -> Bool (true if either Bool is true or Double is non zero)
         * Bool || Bool -> Bool (true if any of both are true)
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String: // String || ??? does not make sense
        break;
    }
}

OperationNode::Operation LogicalORNode::getOp() const
{
    return OperationNode::LogicalOR;
}

OperationNode::Associativity LogicalORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalORNode::getPrecedence() const
{
    return OperationNode::LogicalORPrec;
}

ValueNode &LogicalORNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result =  ValueNode(value1.getValue().value<bool>() || value2.getValue().value<bool>());

    return Result;
}

QString LogicalORNode::printValue() const
{
    return QString("||");
}

QString LogicalORNode::printNode() const
{
    QString NodeType = "LogicalORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LogicalXORNode::LogicalXORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer ^^ Integer -> Bool (true if only one is non zero)
         * Integer ^^ Double -> Bool (true if only one is non zero)
         * Integer ^^ Bool -> Bool (true if Integer is non zero or Bool is false and vice versa)
         */
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double ^^ Integer -> Bool (true if only one is non zero)
         * Double ^^ Double -> Bool (true if only one is non zero)
         * Double ^^ Bool -> Bool (true if Double is non zero or Bool is false and vice versa)
         */
        //FALLTHROUGH
    case ValueNode::Bool: // Bool % ??? does not make sense
        /*
         * Bool ^^ Integer -> Bool (true if Bool is true and Integer is zero or vice versa)
         * Bool ^^ Double -> Bool (true if Bool is true and Double is zero or vice versa)
         * Bool ^^ Bool -> Bool (true if only one of both is true)
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String: // String ^^ ??? does not make sense
        break;
    }
}

OperationNode::Operation LogicalXORNode::getOp() const
{
    return OperationNode::LogicalXOR;
}

OperationNode::Associativity LogicalXORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalXORNode::getPrecedence() const
{
    return OperationNode::LogicalORPrec;
}

ValueNode &LogicalXORNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result = ValueNode(value1.getValue().value<bool>() != value2.getValue().value<bool>());

    return Result;
}

QString LogicalXORNode::printValue() const
{
    return QString("^^");
}

QString LogicalXORNode::printNode() const
{
    QString NodeType = "LogicalXORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

GreaterNode::GreaterNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer > Integer -> Bool
         * (implicit cast to Double)Integer > Double -> Bool
         */
        //ToDO IMPLICIT CASTS
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double > (implicit cast to Double)Integer -> Bool
         * Double > Double -> Bool
         */
        if( ( returnTypeRChild == ValueNode::Integer ) || ( returnTypeRChild == ValueNode::Double ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool > ??? does not make sense
        break;
    case ValueNode::String: // String > ??? does not make sense
        break;
    }
}

OperationNode::Operation GreaterNode::getOp() const
{
    return OperationNode::Greater;
}

OperationNode::Associativity GreaterNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence GreaterNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

ValueNode &GreaterNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result = ValueNode((value1.getValue().value<double>() > value2.getValue().value<double>()) ?  true : false );

    return Result;
}

QString GreaterNode::printValue() const
{
    return QString(">");
}

QString GreaterNode::printNode() const
{
    QString NodeType = "GreaterNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LowerNode::LowerNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer < Integer -> Bool
         * (implicit cast to Double)Integer < Double -> Bool
         */
        //ToDO IMPLICIT CASTS
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double < (implicit cast to Double)Integer -> Bool
         * Double < Double -> Bool
         */
        if( ( returnTypeRChild == ValueNode::Integer ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool < ??? does not make sense
        break;
    case ValueNode::String: // String < ??? does not make sense
        break;
    }
}

OperationNode::Operation LowerNode::getOp() const
{
    return OperationNode::Lower;
}

OperationNode::Associativity LowerNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LowerNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

ValueNode &LowerNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();
    Result = ValueNode((value1.getValue().value<double>() < value2.getValue().value<double>()) ?  true : false );
    return Result;
}

QString LowerNode::printValue() const
{
    return QString("<");
}

QString LowerNode::printNode() const
{
    QString NodeType = "LowerNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

EqualNode::EqualNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer == Integer -> Bool (true if values match)
         * (implicit cast to Double)Integer == Double -> Bool (true if values match)
         * (implicit cast to Bool)Integer == Bool -> Bool
         */
        if(returnTypeRChild == SimpleNode::Integer)
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Bool;
        }
        else if( returnTypeRChild == SimpleNode::Double)
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        else if( returnTypeRChild == SimpleNode::Bool)
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Double:
        /*
         * Double == (implicit cast to Double)Integer -> Bool (true if values match)
         * Double == Double -> Bool (true if values match)
         * (implicit cast to Bool)Double == Bool -> Bool
         */
        if( ( returnTypeRChild == SimpleNode::Double) || (returnTypeRChild == SimpleNode::Integer) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        else if( returnTypeRChild == SimpleNode::Bool)
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool:
        /*
         * Bool == (implicit cast to Double)Integer -> Bool
         * Bool == (implicit cast to Bool)Double -> Bool
         * Bool == Bool -> Bool
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String:
        // String == String
        if( returnTypeRChild == ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::String;
            implicitCastRightChild = SimpleNode::String;
            returnType = ValueNode::Bool;
        }
        break;
    }
}

OperationNode::Operation EqualNode::getOp() const
{
    return OperationNode::Equal;
}

OperationNode::Associativity EqualNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualNode::getPrecedence() const
{
    return OperationNode::EqualityPrec;
}

ValueNode &EqualNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            Result = ValueNode((value1.getValue().value<int>() == value2.getValue().value<int>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Double )
        {
            Result = ValueNode((value1.getValue().value<double>() == value2.getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            Result = ValueNode((value1.getValue().value<bool>() == value2.getValue().value<bool>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        if((implicitCastRightChild == SimpleNode::Double) || ( implicitCastRightChild == SimpleNode::Integer ))
        {
            Result = ValueNode((value1.getValue().value<double>() == value2.getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            Result = ValueNode((value1.getValue().value<bool>() == value2.getValue().value<bool>()) ? true : false );
        }
        break;
    case SimpleNode::Bool:
        Result = ValueNode((value1.getValue().value<bool>() == value2.getValue().value<bool>()) ? true : false );
        break;
    case ValueNode::String:
        Result = ValueNode((!value1.getValue().value<QString>().compare(value2.getValue().value<QString>())) ? true : false );
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString EqualNode::printValue() const
{
    return QString("==");
}

QString EqualNode::printNode() const
{
    QString NodeType = "EqualNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

EqualOrGreaterNode::EqualOrGreaterNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer >= Integer -> Bool (true if values match or LChild is Greater)
         * (implicit cast to Double)Integer >= Double -> Bool (true if values match or LChild is Greater)
         */
        if ( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Bool;
        }
        else if ( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Double:
        /*
         * Double >= (implicit cast to Double)Integer -> Bool (true if values match or LChild is Greater)
         * Double >= Double -> Bool (true if values match or LChild is Greater)
         */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool >= ?? does not make sense
        break;
    case ValueNode::String: // String >= ?? does not make sense
        break;
    }
}

OperationNode::Operation EqualOrGreaterNode::getOp() const
{
    return OperationNode::EqualOrGreater;
}

OperationNode::Associativity EqualOrGreaterNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualOrGreaterNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

ValueNode &EqualOrGreaterNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            Result = ValueNode((value1.getValue().value<int>() >= value2.getValue().value<int>()) ? true : false );
        }
        else
        {
            Result = ValueNode((value1.getValue().value<double>() >= value2.getValue().value<double>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        Result = ValueNode((value1.getValue().value<double>() >= value2.getValue().value<double>()) ? true : false );
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString EqualOrGreaterNode::printValue() const
{
    return QString(">=");
}

QString EqualOrGreaterNode::printNode() const
{
    QString NodeType = "EqualOrGreaterNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

EqualOrLowerNode::EqualOrLowerNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer <= Integer -> Bool (true if values match or LChild is Lower)
         * (implicit cast to Double)Integer <= Double -> Bool (true if values match or LChild is Lower)
         */
        if ( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Bool;
        }
        else if ( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Double:
        /*
         * Double <= (implicit cast to Double)Integer -> Bool (true if values match or LChild is Lower)
         * Double <= Double -> Bool (true if values match or LChild is Lower)
         */
        if( ( returnTypeRChild == ValueNode::Integer ) || ( returnTypeRChild == ValueNode::Double ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool <= ?? does not make sense
        break;
    case ValueNode::String: // String <= ?? does not make sense
        break;
    }
}

OperationNode::Operation EqualOrLowerNode::getOp() const
{
    return OperationNode::EqualOrLower;
}

OperationNode::Associativity EqualOrLowerNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualOrLowerNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

ValueNode &EqualOrLowerNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            Result = ValueNode((value1.getValue().value<int>() <= value2.getValue().value<int>()) ? true : false );
        }
        else
        {
            Result = ValueNode((value1.getValue().value<double>() <= value2.getValue().value<double>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        Result = ValueNode((value1.getValue().value<double>() <= value2.getValue().value<double>()) ? true : false );
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString EqualOrLowerNode::printValue() const
{
    return QString("<=");
}

QString EqualOrLowerNode::printNode() const
{
    QString NodeType = "EqualOrLowerNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

UnequalNode::UnequalNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation UnequalNode::getOp() const
{
    return OperationNode::Unequal;
}

OperationNode::Associativity UnequalNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence UnequalNode::getPrecedence() const
{
    return OperationNode::EqualityPrec;
}

ValueNode &UnequalNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            Result = ValueNode((value1.getValue().value<int>() != value2.getValue().value<int>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Double )
        {
            Result = ValueNode((value1.getValue().value<double>() != value2.getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            Result = ValueNode((value1.getValue().value<bool>() != value2.getValue().value<bool>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        if((implicitCastRightChild == SimpleNode::Double) || ( implicitCastRightChild == SimpleNode::Integer ))
        {
            Result = ValueNode((value1.getValue().value<double>() != value2.getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            Result = ValueNode((value1.getValue().value<bool>() != value2.getValue().value<bool>()) ? true : false );
        }
        break;
    case SimpleNode::Bool:
        Result = ValueNode((value1.getValue().value<bool>() != value2.getValue().value<bool>()) ? true : false );
        break;
    case ValueNode::String:
        Result = ValueNode((value1.getValue().value<QString>().compare(value2.getValue().value<QString>())) ? true : false );
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString UnequalNode::printValue() const
{
    return QString("!=");
}

QString UnequalNode::printNode() const
{
    QString NodeType = "UneqalNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

BinaryBitwiseOperationNode::BinaryBitwiseOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{
}

OperationNode::OperationTypes BinaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

ANDNode::ANDNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer & Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double & ?? does not make sense
        break;
    case ValueNode::Bool: // Bool & ?? does not make sense
        break;
    case ValueNode::String: // String & ?? does not make sense
        break;
    }
}

OperationNode::Operation ANDNode::getOp() const
{
    return OperationNode::AND;
}

OperationNode::Associativity ANDNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ANDNode::getPrecedence() const
{
    return OperationNode::BitwiseANDPrec;
}

ValueNode &ANDNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result = ValueNode(value1.getValue().value<int>() & value2.getValue().value<int>());

    return Result;
}

QString ANDNode::printValue() const
{
    return QString("&");
}

QString ANDNode::printNode() const
{
    QString NodeType = "ANDNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

ORNode::ORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer | Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double | ?? does not make sense
        break;
    case ValueNode::Bool: // Bool | ?? does not make sense
        break;
    case ValueNode::String: // String | ?? does not make sense
        break;
    }
}

OperationNode::Operation ORNode::getOp() const
{
    return OperationNode::OR;
}

OperationNode::Associativity ORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ORNode::getPrecedence() const
{
    return OperationNode::BitwiseORPrec;
}

ValueNode &ORNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result = ValueNode(value1.getValue().value<int>() | value2.getValue().value<int>());

    return Result;
}

QString ORNode::printValue() const
{
    return QString("|");
}

QString ORNode::printNode() const
{
    QString NodeType = "ORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

XORNode::XORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer ^ Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double ^ ?? does not make sense
        break;
    case ValueNode::Bool: // Bool ^ ?? does not make sense
        break;
    case ValueNode::String: // String ^ ?? does not make sense
        break;
    }
}

OperationNode::Operation XORNode::getOp() const
{
    return OperationNode::XOR;
}

OperationNode::Associativity XORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence XORNode::getPrecedence() const
{
    return OperationNode::BitwiseXORPrec;
}

ValueNode &XORNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result = ValueNode(value1.getValue().value<int>() ^ value2.getValue().value<int>());

    return Result;
}

QString XORNode::printValue() const
{
    return QString("^");
}

QString XORNode::printNode() const
{
    QString NodeType = "XORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LeftShiftNode::LeftShiftNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer << Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double << ?? does not make sense
        break;
    case ValueNode::Bool: // Bool << ?? does not make sense
        break;
    case ValueNode::String: // String << ?? does not make sense
        break;
    }
}

OperationNode::Operation LeftShiftNode::getOp() const
{
    return OperationNode::LeftShift;
}

OperationNode::Associativity LeftShiftNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LeftShiftNode::getPrecedence() const
{
    return OperationNode::ShiftPrec;
}

ValueNode &LeftShiftNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result = ValueNode(value1.getValue().value<int>() << value2.getValue().value<int>());

    return Result;
}

QString LeftShiftNode::printValue() const
{
    return QString("<<");
}

QString LeftShiftNode::printNode() const
{
    QString NodeType = "LeftShiftNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

RightShiftNode::RightShiftNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer >> Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double >> ?? does not make sense
        break;
    case ValueNode::Bool: // Bool >> ?? does not make sense
        break;
    case ValueNode::String: // String >> ?? does not make sense
        break;
    }
}

OperationNode::Operation RightShiftNode::getOp() const
{
    return OperationNode::RightShift;
}

OperationNode::Associativity RightShiftNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence RightShiftNode::getPrecedence() const
{
    return OperationNode::ShiftPrec;
}

ValueNode &RightShiftNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode &value2 = rightChild->visit();

    Result = ValueNode(value1.getValue().value<int>() >> value2.getValue().value<int>());

    return Result;
}

QString RightShiftNode::printValue() const
{
    return QString(">>");
}

QString RightShiftNode::printNode() const
{
    QString NodeType = "RightShiftNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

TernaryOperationNode::TernaryOperationNode(SimpleNode *leftChild, SimpleNode *midChild, SimpleNode *rightChild) :
    leftChild(leftChild),
    midChild(midChild),
    rightChild(rightChild)
{
}

TernaryOperationNode::~TernaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete leftChild;
    delete midChild;
    delete rightChild;
}

OperationNode::ArityTypes TernaryOperationNode::getArityType() const
{
    return OperationNode::Ternary;
}

ConditionalNode::ConditionalNode(SimpleNode *leftChild, SimpleNode *midChild, SimpleNode *rightChild) :
    TernaryLogicalOperationNode(leftChild, midChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeMChild = midChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    if( ( returnTypeLChild == ValueNode::Bool ) ||
            ( returnTypeLChild == ValueNode::Integer ) ||
            ( returnTypeLChild == ValueNode::Double ) )
    {
        implicitCastLeftChild = ValueNode::Bool;
        switch(returnTypeMChild)
        {
        case ValueNode::Integer:
            if(returnTypeRChild == ValueNode::Double)
            {
                implicitCastMidChild = ValueNode::Double;
                implicitCastRightChild = ValueNode::Double;
                returnType = ValueNode::Double;
            }
            else if( returnTypeRChild == ValueNode::Bool )
            {
                implicitCastMidChild = ValueNode::Bool;
                implicitCastRightChild = ValueNode::Bool;
                returnType = ValueNode::Bool;
            }
            else if( returnTypeRChild != ValueNode::String )
            {
                implicitCastMidChild = ValueNode::Integer;
                implicitCastRightChild = ValueNode::Integer;
                returnType = ValueNode::Integer;
            }
            break;
        case ValueNode::Double:
            if( returnTypeRChild == ValueNode::Bool )
            {
                implicitCastMidChild = ValueNode::Bool;
                implicitCastRightChild = ValueNode::Bool;
                returnType = ValueNode::Bool;
            }
            else if( returnTypeRChild != ValueNode::String )
            {
                implicitCastMidChild = ValueNode::Double;
                implicitCastRightChild = ValueNode::Double;
                returnType = ValueNode::Double; // Implicitly casts Integer to Double
            }
            break;
        case ValueNode::Bool:
            if( returnTypeRChild != ValueNode::String )
            {
                implicitCastMidChild = ValueNode::Bool;
                implicitCastRightChild = ValueNode::Bool;
                returnType = ValueNode::Bool; // Implicitly casts Integer and Double to Bool
            }
            break;
        case ValueNode::String: // if one return ChildNode is a String, the other node must also be a String
            if( returnTypeRChild == ValueNode::String )
            {
                implicitCastMidChild = ValueNode::String;
                implicitCastRightChild = ValueNode::String;
                returnType = ValueNode::String;
            }
            break;
        }
    }
}

OperationNode::Operation ConditionalNode::getOp() const
{
    return OperationNode::Conditional;
}

OperationNode::Associativity ConditionalNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence ConditionalNode::getPrecedence() const
{
    return OperationNode::ConditionalPrec;
}

ValueNode &ConditionalNode::DoOperation()
{
    ValueNode &value1 = leftChild->visit();
    ValueNode value2 = midChild->visit();
    ValueNode &value3 = rightChild->visit();

    bool IsTrue = value1.getValue().value<bool>();

    switch(implicitCastMidChild)
    {
    case SimpleNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            Result = ValueNode((IsTrue) ? value2.getValue().value<double>() : value3.getValue().value<double>() );
        }
        else if(implicitCastRightChild == SimpleNode::Bool)
        {
            Result = ValueNode((IsTrue) ? value2.getValue().value<bool>() : value3.getValue().value<bool>() );
        }
        else
        {
            Result = ValueNode((IsTrue) ? value2.getValue().value<int>() : value3.getValue().value<int>() );
        }
        break;
    case SimpleNode::Double:
        if(implicitCastRightChild == SimpleNode::Bool)
        {
            Result = ValueNode((IsTrue) ? value2.getValue().value<bool>() : value3.getValue().value<bool>() );
        }
        else
        {
            Result = ValueNode((IsTrue) ? value2.getValue().value<double>() : value3.getValue().value<double>() );
        }
        break;
    case SimpleNode::Bool:
        Result = ValueNode((IsTrue) ? value2.getValue().value<bool>() : value3.getValue().value<bool>() );
        break;
    case SimpleNode::String:
        Result = ValueNode((IsTrue) ? value2.getValue().value<QString>() : value3.getValue().value<QString>() );
        break;
    default:
        Result = ValueNode();
    }

    return Result;
}

QString ConditionalNode::printValue() const
{
    return QString("?:");
}

QString ConditionalNode::printNode() const
{
    QString NodeType = "ConditionalNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
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

ValueNode &EOFNode::visit()
{
    return InvalidValue;
}


FunctionNode::FunctionNode(QString FunctionName, QVector<VariableNode *> ParametersInOrder, SimpleNode::ValueTypes returnType, SymbolTable *SubSymbolTable) :
    FunctionName(FunctionName),
    ParametersInOrder(ParametersInOrder),
    returnType(returnType),
    FuncSymbolTable(SubSymbolTable)
{
}

FunctionNode::~FunctionNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void FunctionNode::addFuncExpressions(QVector<SimpleNode *> FuncExpressions)
{
    this->FuncExpressions = FuncExpressions;
}

void FunctionNode::addReturnStatement(SimpleNode *returnNode)
{
    if( ( (returnNode == NULL) && ( returnType != SimpleNode::Void ) ) || ( !SimpleNode::canConvertTypes(returnNode->getReturnType(), returnType) ) )
    {
        this->returnType = SimpleNode::ErrorType;
        this->returnNode = NULL;
    }

    this->returnNode = returnNode;
}

void FunctionNode::addAssignment(AssignmentNode *paramAssignment)
{
    ParameterAssignments.append(paramAssignment);
}

SimpleNode::NodeType FunctionNode::getNodeType() const
{
    return SimpleNode::Function;
}

SimpleNode::ValueTypes FunctionNode::getReturnType() const
{
    return returnType;
}

QString FunctionNode::printValue() const
{
    QString funcParams = QString("(");

    for(VariableNode *param : this->ParametersInOrder)
    {
        funcParams.append(QString(" %1 %2").arg(SimpleNode::getHumanReadableTypeNameToValueType(param->getReturnType())).arg(param->getVariableName()));
    }

    funcParams.append(" )");
    return QString("%1 %2 %3").arg(SimpleNode::getHumanReadableTypeNameToValueType(returnType)).arg(FunctionName).arg(funcParams);
}

QString FunctionNode::printNode() const
{
    return QString("{(FunctionNode):(%1)}").arg(printValue());
}

ValueNode &FunctionNode::visit()
{
    for(AssignmentNode *assignment : ParameterAssignments)
    {
        assignment->visit();
    }

    for(SimpleNode *expr : FuncExpressions)
    {
        ValueNode exprRes = expr->visit();
        qDebug() << "FunctionNode visited --> " << exprRes.printNode();
    }

    switch(returnType)
    {
    case SimpleNode::Integer:
        Result = ValueNode(returnNode->visit().getValue().value<int>());
        break;
    case SimpleNode::Double:
        Result = ValueNode(returnNode->visit().getValue().value<double>());
        break;
    case SimpleNode::Bool:
        Result = ValueNode(returnNode->visit().getValue().value<bool>());
        break;
    case SimpleNode::String:
        Result = ValueNode(returnNode->visit().getValue().value<QString>());
        break;
    case SimpleNode::Void:
    default:
        Result = ValueNode();
    }

    return Result;
}

QString FunctionNode::getFunctionName() const
{
    return FunctionName;
}

QVector<VariableNode *> FunctionNode::getParametersInOrder() const
{
    return ParametersInOrder;
}

SymbolTable *FunctionNode::getFuncSymbolTable() const
{
    return FuncSymbolTable;
}

FunctionCallNode::FunctionCallNode(const QString &FunctionName, SymbolTable *CurSymblTable, QVector<SimpleNode *> FunctionParameters) :
    FunctionName(FunctionName),
    FuncParams(FunctionParameters)
{
    RelatedSymbol = dynamic_cast<FunctionSymbol*>(CurSymblTable->lookup(FunctionName));
    if(RelatedSymbol == NULL)
    {
        returnType = SimpleNode::ErrorType;
    }
    else
    {
        FunctionNode *funcNode = RelatedSymbol->GetFunctionNode();
        if(funcNode == NULL)
        {
            returnType = SimpleNode::ErrorType;
            return;
        }
        QVector<VariableNode *> paramsInOrder = funcNode->getParametersInOrder();
        int NrOfPassedFuncParams = FunctionParameters.size();
        if(NrOfPassedFuncParams != FunctionParameters.size())
        {
            returnType = SimpleNode::ErrorType;
            return;
        }
        for(int i = 0; i < NrOfPassedFuncParams; i++)
        {
            VariableNode *paramDecl = paramsInOrder.at(i);
            SimpleNode *paramDef = FunctionParameters.at(i);
            if(!SimpleNode::canConvertTypes(paramDef->getReturnType(),paramDecl->getReturnType()))
            {
                returnType = SimpleNode::ErrorType;
                return;
            }
            funcNode->addAssignment(new AssignmentNode(paramDecl,paramDef));
//            paramDecl->setAssignment(paramDef);
        }
        returnType = funcNode->getReturnType();
    }
}

FunctionCallNode::~FunctionCallNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType FunctionCallNode::getNodeType() const
{
    return SimpleNode::FunctionCall;
}

SimpleNode::ValueTypes FunctionCallNode::getReturnType() const
{
    return returnType;
}

QString FunctionCallNode::printValue() const
{
    return QString("%1()").arg(FunctionName);
}

QString FunctionCallNode::printNode() const
{
    return QString("{FunctionCall}:{%1}").arg(printValue());
}

ValueNode &FunctionCallNode::visit()
{
    FunctionNode *symblTblEntryValue = RelatedSymbol->GetFunctionNode();
    if(symblTblEntryValue == NULL)
        Result = ValueNode();
    else
        Result = ValueNode(symblTblEntryValue->visit());

    return Result;
}

AssignmentNode::AssignmentNode(VariableNode *VariableToAssign, SimpleNode *ValueToAssign) :
    VariableToAssign(VariableToAssign),
    ValueToAssign(ValueToAssign)
{

}

AssignmentNode::~AssignmentNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType AssignmentNode::getNodeType() const
{
    return SimpleNode::Assignment;
}

SimpleNode::ValueTypes AssignmentNode::getReturnType() const
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

ValueNode &AssignmentNode::visit()
{
    VariableToAssign->setAssignment(ValueToAssign);

    Result = VariableToAssign->visit();

    return Result;
}

TernaryArithmeticOperationNode::TernaryArithmeticOperationNode(SimpleNode *leftChild, SimpleNode *midChild, SimpleNode *rightChild) :
    TernaryOperationNode(leftChild, midChild, rightChild)
{

}

OperationNode::OperationTypes TernaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

TernaryLogicalOperationNode::TernaryLogicalOperationNode(SimpleNode *leftChild, SimpleNode *midChild, SimpleNode *rightChild) :
    TernaryOperationNode(leftChild, midChild, rightChild)
{

}

OperationNode::OperationTypes TernaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

TernaryBitwiseOperationNode::TernaryBitwiseOperationNode(SimpleNode *leftChild, SimpleNode *midChild, SimpleNode *rightChild) :
    TernaryOperationNode(leftChild, midChild, rightChild)
{

}

OperationNode::OperationTypes TernaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

//SimpleNode::NodeType ProgramNode::getNodeType() const
//{
//    return SimpleNode::Program;
//}

//SimpleNode::ValueTypes ProgramNode::getReturnType() const
//{
//    return type;
//}

//QString ProgramNode::printValue() const
//{
//    return QString("%1").arg(Result.visit().getValue().value<QString>());
//}

//QString ProgramNode::printNode() const
//{
//    return QString("{ProgramNode}:{%1}").arg(printValue());
//}

//ValueNode &ProgramNode::visit()
//{
//    for(AssignmentNode &&assign : ProgramAssignments)
//    {
////        assign.
//    }
//}
