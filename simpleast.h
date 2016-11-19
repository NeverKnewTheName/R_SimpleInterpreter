#ifndef SIMPLEAST_H
#define SIMPLEAST_H

#include <QVariant>
#include <QVector>

class SymbolTable;
class FunctionSymbol;
class VariableSymbol;
class ValueNode;
class AssignmentNode;

class SimpleNode
{
public:
    typedef enum _NodeType
    {
        Value,
        Data,
        Variable,
        Operation,
        Function,
        FunctionCall,
        Assignment,
        EOFNode
    }NodeType;
    typedef enum _ValueTypes
    {
        Integer,
        Double,
        Bool,
        String,
        Void,
        ErrorType
    }ValueTypes;

    SimpleNode();
    virtual ~SimpleNode() = 0;

    virtual NodeType getNodeType() const = 0;
    virtual ValueTypes getReturnType() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    virtual ValueNode &visit() = 0;

    static QString getHumanReadableTypeNameToValueType(const ValueTypes type);
    static bool canConvertTypes(const ValueTypes OrigType, const ValueTypes NewType);
};

class ValueNode : public SimpleNode
{
public:

    ValueNode();
    ValueNode(const ValueNode &valueNodeToCopy);
    ValueNode(ValueNode &&valueNodeToMove);
    ValueNode(const int value);
    ValueNode(const double value);
    ValueNode(const bool value);
    ValueNode(QString const &value);
    ~ValueNode();

    NodeType getNodeType() const;
    ValueTypes getReturnType() const;

    ValueNode &operator=(ValueNode const&value);

    const QVariant getValue() const;

    ValueTypes getValueType() const;

    QString printValue() const;
    QString printNode() const;
    ValueNode &visit();

private:
    ValueTypes valueType;
    QVariant value;
};


class DataNode : public SimpleNode
{
public:
    DataNode(const unsigned int dataIndex, const SymbolTable * const SymblTbl);
    ~DataNode();
    NodeType getNodeType() const;
    SimpleNode::ValueTypes getReturnType() const;

    ValueNode &visit();

    QString printValue() const;
    QString printNode() const;

private:
    ValueNode Result;
    const unsigned int dataIndex;
    const SymbolTable * const SymblTbl;
};

class VariableNode : public SimpleNode
{
public:
    VariableNode(const QString &VariableName, SymbolTable * const SymblTbl, SimpleNode::ValueTypes type = SimpleNode::ErrorType);
    ~VariableNode();
    NodeType getNodeType() const;
    SimpleNode::ValueTypes getReturnType() const;
    void setAssignment(SimpleNode *assignment);

    ValueNode &visit();

    QString printValue() const;
    QString printNode() const;

    QString getVariableName() const;

    VariableSymbol *getRelatedVariableSymbol() const;

private:
    SimpleNode *Assignment;
    ValueNode Result;
    const QString VariableName;
    VariableSymbol * const RelatedVariableSymbol;
    SimpleNode::ValueTypes type;
};

class FunctionNode : public SimpleNode
{
public:
    FunctionNode(
            QString FunctionName,
            QVector<VariableNode *> ParametersInOrder,
            SimpleNode::ValueTypes returnType,
            SymbolTable *SubSymbolTable
            );
    ~FunctionNode();

    void addFuncExpressions(QVector<SimpleNode *> FuncExpressions);
    void addReturnStatement(SimpleNode *returnNode);
    void addAssignment(AssignmentNode *paramAssignment);

    SymbolTable *getFuncSymbolTable() const;

    QVector<VariableNode *> getParametersInOrder() const;

    QString getFunctionName() const;

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNode &visit();

private:
    ValueNode Result;
    QString FunctionName;
    const QVector<VariableNode *> ParametersInOrder;
    QVector<AssignmentNode*> ParameterAssignments;
    SimpleNode::ValueTypes returnType;
    QVector<SimpleNode *> FuncExpressions;
    SimpleNode *returnNode;
    SymbolTable * const FuncSymbolTable;
};

class FunctionCallNode : public SimpleNode
{
public:
    FunctionCallNode(const QString &FunctionName, SymbolTable *CurSymblTable, QVector<SimpleNode*> FunctionParameters = QVector<SimpleNode*>());
    ~FunctionCallNode();

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNode &visit();

private:
    QString FunctionName;
    QVector<SimpleNode*> FuncParams;
    FunctionSymbol *RelatedSymbol;
    ValueNode Result;
    SimpleNode::ValueTypes returnType;
};

class AssignmentNode : public SimpleNode
{
public:
    AssignmentNode(VariableNode *VariableToAssign, SimpleNode *ValueToAssign);
    ~AssignmentNode();

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNode &visit();
private:
    ValueNode Result;
    VariableNode *VariableToAssign;
    SimpleNode *ValueToAssign;
};

class OperationNode : public SimpleNode
{
public:
    typedef enum _ArityTypes
    {
        Unary,
        Binary,
        Ternary
    }ArityTypes;
    typedef enum _OperationTypes
    {
        Arithmetic,
        Logical,
        Bitwise
    }OperationTypes;
    typedef enum _Operation
    {
        TypeCast,
        Increment,
        Decrement,
        Positive,
        Negative,
        LogicalNegation,
        OnesComplement,
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo,
        LogicalAND,
        LogicalOR,
        LogicalXOR,
        Greater,
        Lower,
        Equal,
        EqualOrGreater,
        EqualOrLower,
        Unequal,
        AND,
        OR,
        XOR,
        LeftShift,
        RightShift,
        Conditional
    }Operation;
    typedef enum _Associativity
    {
        LeftToRight,
        RightToLeft
    }Associativity;
    typedef enum _Precedence
    {
        PostfixPrec,
        UnaryPrec,
        MultiplicativePrec,
        AdditivePrec,
        ShiftPrec,
        RelationalPrec,
        EqualityPrec,
        BitwiseANDPrec,
        BitwiseXORPrec,
        BitwiseORPrec,
        LogicalANDPrec,
        LogicalORPrec,
        ConditionalPrec,
        AssignmentPrec,
        CommaPrec
    }Precedence;

    OperationNode();
    virtual ~OperationNode();
    virtual NodeType getNodeType() const;
    SimpleNode::ValueTypes getReturnType() const;
    virtual ArityTypes getArityType() const = 0;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    virtual ValueNode &visit();
protected:
    ValueNode Result;
    SimpleNode::ValueTypes returnType;
};

class UnaryOperationNode : public OperationNode
{
public:
    UnaryOperationNode(SimpleNode *rightChild);
    ~UnaryOperationNode();
    ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNode *rightChild;
    SimpleNode::ValueTypes implicitCastRightChild;
};

class UnaryArithmeticOperationNode : public UnaryOperationNode
{
public:
    UnaryArithmeticOperationNode(SimpleNode *rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class TypeCastNode : public UnaryArithmeticOperationNode
{
public:
    TypeCastNode(SimpleNode *rightChild, SimpleNode::ValueTypes typeToCastTo);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Increments Integer Values by one and returns the result as a new ValueNode
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Integer when successful
     * \returns ValueNode of type ErrorType when failed
     *
     * \warning Only use this operation on Integer ValueNodes
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
private:
    SimpleNode::ValueTypes typeToCastTo;
};

class IncrementNode : public UnaryArithmeticOperationNode
{
public:
    IncrementNode(SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Increments Integer Values by one and returns the result as a new ValueNode
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Integer when successful
     * \returns ValueNode of type ErrorType when failed
     *
     * \warning Only use this operation on Integer ValueNodes
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class DecrementNode : public UnaryArithmeticOperationNode
{
public:
    DecrementNode(SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Decrements Integer Values by one and returns the result as a new ValueNode
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Integer when successful
     * \returns ValueNode of type ErrorType when failed
     *
     * \warning Only use this operation on Integer ValueNodes
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class PositiveNode : public UnaryArithmeticOperationNode
{
public:
    PositiveNode(SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Returns a new ValueNode with the value of the passed ValueNode
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Integer or Double when successful
     * \returns ValueNode of type ErrorType when failed
     *
     * \warning Only use this operation on Integer or Double ValueNodes
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class NegativeNode : public UnaryArithmeticOperationNode
{
public:
    NegativeNode(SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Returns a new ValueNode with the value of the passed ValueNode
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Integer or Double when successful
     * \returns ValueNode of type ErrorType when failed
     *
     * \warning Only use this operation on Integer or Double ValueNodes
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnaryLogicalOperationNode : public UnaryOperationNode
{
public:
    UnaryLogicalOperationNode(SimpleNode *rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalNegationNode : public UnaryLogicalOperationNode
{
public:
    LogicalNegationNode(SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Returns a new ValueNode with the logically negated value of the passed ValueNode
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Bool when successful
     * \returns ValueNode of type ErrorType when failed
     *
     * \warning Only use this operation on Integer, Double or Bool ValueNodes
     * \note If the passed ValueNode equals 0 (or 0.0) it is treated as Bool false, otherwise Bool true
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnaryBitwiseOperationNode : public UnaryOperationNode
{
public:
    UnaryBitwiseOperationNode(SimpleNode *rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class OnesComplementNode : public UnaryBitwiseOperationNode
{
public:
    OnesComplementNode(SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    /**
     * \brief Returns a new ValueNode with the bitwise one's complement of the passed ValueNode
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Integer when successful
     * \returns ValueNode of type ErrorType when failed
     *
     * \warning Only use this operation on Integer ValueNodes
     */
    virtual ValueNode &DoOperation();

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class BinaryOperationNode : public OperationNode
{
public:
    BinaryOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    ~BinaryOperationNode();
    ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNode *leftChild;
    SimpleNode *rightChild;
    SimpleNode::ValueTypes implicitCastLeftChild;
    SimpleNode::ValueTypes implicitCastRightChild;
};

class BinaryArithmeticOperationNode : public BinaryOperationNode
{
public:
    BinaryArithmeticOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class AdditionNode : public BinaryArithmeticOperationNode
{
public:
    AdditionNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class SubtractionNode : public BinaryArithmeticOperationNode
{
public:
    SubtractionNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class MultiplicationNode : public BinaryArithmeticOperationNode
{
public:
    MultiplicationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class DivisionNode : public BinaryArithmeticOperationNode
{
public:
    DivisionNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ModuloNode : public BinaryArithmeticOperationNode
{
public:
    ModuloNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryLogicalOperationNode : public BinaryOperationNode
{
public:
    BinaryLogicalOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalANDNode : public BinaryLogicalOperationNode
{
public:
    LogicalANDNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Returns a new ValueNode of type Bool with the result of the AND operation
     *
     * \param value1[in] first operand
     * \param value2[in] second operand
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Bool when successful
     *
     * \warning Do not use on type String
     * \note Integer and Double ValueNodes are converted to bool before the operation takes place (0 or 0.0 is false whereas everything else  is true)
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalORNode : public BinaryLogicalOperationNode
{
public:
    LogicalORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    /**
     * \brief Returns a new ValueNode of type Bool with the result of the OR operation
     *
     * \param value1[in] first operand
     * \param value2[in] second operand
     *
     * \return ValueNode with the operation's result
     * \returns ValueNode of type Bool when successful
     *
     * \warning Do not use on type String
     * \note Integer and Double ValueNodes are converted to bool before the operation takes place (0 or 0.0 is false whereas everything else  is true)
     */
    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalXORNode : public BinaryLogicalOperationNode
{
public:
    LogicalXORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class GreaterNode : public BinaryLogicalOperationNode
{
public:
    GreaterNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LowerNode : public BinaryLogicalOperationNode
{
public:
    LowerNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualNode : public BinaryLogicalOperationNode
{
public:
    EqualNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrGreaterNode : public BinaryLogicalOperationNode
{
public:
    EqualOrGreaterNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrLowerNode : public BinaryLogicalOperationNode
{
public:
    EqualOrLowerNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnequalNode : public BinaryLogicalOperationNode
{
public:
    UnequalNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryBitwiseOperationNode : public BinaryOperationNode
{
public:

    BinaryBitwiseOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ANDNode : public BinaryBitwiseOperationNode
{
public:
    ANDNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ORNode : public BinaryBitwiseOperationNode
{
public:
    ORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class XORNode : public BinaryBitwiseOperationNode
{
public:
    XORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LeftShiftNode : public BinaryBitwiseOperationNode
{
public:
    LeftShiftNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class RightShiftNode : public BinaryBitwiseOperationNode
{
public:
    RightShiftNode(SimpleNode *leftChild, SimpleNode *rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class TernaryOperationNode : public OperationNode
{
public:
    TernaryOperationNode(SimpleNode *leftChild, SimpleNode *midChild ,SimpleNode *rightChild);
    ~TernaryOperationNode();
    ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNode *leftChild;
    SimpleNode *midChild;
    SimpleNode *rightChild;
    SimpleNode::ValueTypes implicitCastLeftChild;
    SimpleNode::ValueTypes implicitCastMidChild;
    SimpleNode::ValueTypes implicitCastRightChild;
};

class ConditionalNode : public TernaryOperationNode
{
public:
    ConditionalNode(SimpleNode *leftChild, SimpleNode *midChild ,SimpleNode *rightChild);
    OperationTypes getOpType() const;
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

class EOFNode : public SimpleNode
{
public:
    EOFNode();
    ~EOFNode();

    NodeType getNodeType() const;
    SimpleNode::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;

    ValueNode &visit();
private:
    ValueNode InvalidValue;
};
#endif // SIMPLEAST_H
