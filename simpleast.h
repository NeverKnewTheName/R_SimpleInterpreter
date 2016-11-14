#ifndef SIMPLEAST_H
#define SIMPLEAST_H

class ASTNode
{
public:
    typedef enum _NodeType
    {
        ValueNode,
        DataNode,
        OperationNode,
        ERRORNode
    }NodeType;

    ASTNode();
    virtual NodeType getType() const = 0;
    virtual const ValueToken * const visit(const Msg * const msg) const = 0;
};

//class OperationNode : public ASTNode
//{
//public:
//    OperationNode();
//    virtual NodeType getType() const;
//    virtual const ValueToken * const visit(const Msg * const msg) const = 0;
//};

//class UnaryOperationNode : public ASTNode
//{
//public:
//    UnaryOperationNode(const UnaryOperationToken *const unOp, const ASTNode * const right);
//    virtual const ValueToken * const visit(const Msg * const msg) const;

//private:
//    const UnaryOperationToken *unOp;
//    const ASTNode * const right;
//};

//class BinaryOperationNode : public ASTNode
//{
//public:
//    BinaryOperationNode(const ASTNode * const left, const BinaryOperationToken *const binOp, const ASTNode * const right);
//    virtual const ValueToken * const visit(const Msg * const msg) const;

//private:
//    const BinaryOperationToken *binOp;
//    const ASTNode * const left;
//    const ASTNode * const right;
//};

//class TernaryOperationNode : public ASTNode
//{
//public:
//    TernaryOperationNode(const ASTNode * const left, const TernaryOperationToken *const binOp, const ASTNode * const mid, const ASTNode * const right);
//    virtual const ValueToken * const visit(const Msg * const msg) const;

//private:
//    const TernaryOperationToken *binOp;
//    const ASTNode * const left;
//    const ASTNode * const mid;
//    const ASTNode * const right;
//};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

#include <QVariant>
#include <simplesymboltable.h>

class SimpleNode
{
public:
    typedef enum _NodeType
    {
        Value,
        Data,
        Operation,
        EOFNode
    }NodeType;
    SimpleNode();

    virtual NodeType getNodeType() const = 0;
    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    virtual ValueNode &visit() = 0;
};

class EOFNode : public SimpleNode
{
public:
    EOFNode();

    virtual NodeType getNodeType() const;
    virtual QString printValue() const;
    virtual QString printNode() const;

    virtual ValueNode &visit();
private:
    ValueNode InvalidValue;
};

class ValueNode : public SimpleNode
{
public:
    typedef enum _ValueTypes
    {
        Integer,
        Double,
        Bool,
        String,
        ErrorType
    }ValueTypes;

    ValueNode();
    ValueNode(const ValueNode &valueNodeToCopy);
    ValueNode(ValueNode &&valueNodeToMove);
    ValueNode(const int value);
    ValueNode(const double value);
    ValueNode(const bool value);
    ValueNode(QString const &value);
    virtual NodeType getNodeType() const;

    ValueNode &operator=(ValueNode const&value);

    const QVariant getValue() const;

    ValueTypes getValueType() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
    virtual ValueNode &visit();

private:
    ValueTypes valueType;
    QVariant value;
};

class DataNode : public SimpleNode
{
public:
    DataNode(const unsigned int dataIndex, const SymbolTable * const SymblTbl);
    virtual NodeType getNodeType() const;

    virtual ValueNode &visit();

    virtual QString printValue() const;
    virtual QString printNode() const;

private:
    ValueNode Result;
    const unsigned int dataIndex;
    const SymbolTable * const SymblTbl;
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
    virtual NodeType getNodeType() const;
    virtual ArityTypes getArityType() const = 0;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    virtual ValueNode &visit();
protected:
    ValueNode Result;
};

class UnaryOperationNode : public OperationNode
{
public:
    UnaryOperationNode(SimpleNode *rightChild);
    virtual ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNode *rightChild;
};

class UnaryArithmeticOperationNode : public UnaryOperationNode
{
public:
    UnaryArithmeticOperationNode(SimpleNode *rightChild);
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class IncrementNode : public UnaryArithmeticOperationNode
{
public:
    IncrementNode(SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class DecrementNode : public UnaryArithmeticOperationNode
{
public:
    DecrementNode(SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class PositiveNode : public UnaryArithmeticOperationNode
{
public:
    PositiveNode(SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class NegativeNode : public UnaryArithmeticOperationNode
{
public:
    NegativeNode(SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class UnaryLogicalOperationNode : public UnaryOperationNode
{
public:
    UnaryLogicalOperationNode(SimpleNode *rightChild);
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalNegationNode : public UnaryLogicalOperationNode
{
public:
    LogicalNegationNode(SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class UnaryBitwiseOperationNode : public UnaryOperationNode
{
public:
    UnaryBitwiseOperationNode(SimpleNode *rightChild);
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class BinaryOperationNode : public OperationNode
{
public:
    BinaryOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNode *leftChild;
    SimpleNode *rightChild;
};

class BinaryArithmeticOperationNode : public BinaryOperationNode
{
public:
    BinaryArithmeticOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class AdditionNode : public BinaryArithmeticOperationNode
{
public:
    AdditionNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class SubtractionNode : public BinaryArithmeticOperationNode
{
public:
    SubtractionNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class MultiplicationNode : public BinaryArithmeticOperationNode
{
public:
    MultiplicationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class DivisionNode : public BinaryArithmeticOperationNode
{
public:
    DivisionNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class ModuloNode : public BinaryArithmeticOperationNode
{
public:
    ModuloNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class BinaryLogicalOperationNode : public BinaryOperationNode
{
public:
    BinaryLogicalOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalANDNode : public BinaryLogicalOperationNode
{
public:
    LogicalANDNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class LogicalORNode : public BinaryLogicalOperationNode
{
public:
    LogicalORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

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
    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class LogicalXORNode : public BinaryLogicalOperationNode
{
public:
    LogicalXORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class GreaterNode : public BinaryLogicalOperationNode
{
public:
    GreaterNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class LowerNode : public BinaryLogicalOperationNode
{
public:
    LowerNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class EqualNode : public BinaryLogicalOperationNode
{
public:
    EqualNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class EqualOrGreaterNode : public BinaryLogicalOperationNode
{
public:
    EqualOrGreaterNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class EqualOrLowerNode : public BinaryLogicalOperationNode
{
public:
    EqualOrLowerNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class UnequalNode : public BinaryLogicalOperationNode
{
public:
    UnequalNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class BinaryBitwiseOperationNode : public BinaryOperationNode
{
public:

    BinaryBitwiseOperationNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ANDNode : public BinaryBitwiseOperationNode
{
public:
    ANDNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class ORNode : public BinaryBitwiseOperationNode
{
public:
    ORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class XORNode : public BinaryBitwiseOperationNode
{
public:
    XORNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class LeftShiftNode : public BinaryBitwiseOperationNode
{
public:
    LeftShiftNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class RightShiftNode : public BinaryBitwiseOperationNode
{
public:
    RightShiftNode(SimpleNode *leftChild, SimpleNode *rightChild);
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

class TernaryOperationNode : public OperationNode
{
public:
    TernaryOperationNode(SimpleNode *leftChild, SimpleNode *midChild ,SimpleNode *rightChild);
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() const = 0;
    virtual ArityTypes getArityType() const;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNode *leftChild;
    SimpleNode *midChild;
    SimpleNode *rightChild;
};

class ConditionalNode : public TernaryOperationNode
{
public:
    ConditionalNode(SimpleNode *leftChild, SimpleNode *midChild ,SimpleNode *rightChild);
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const;
    virtual Associativity getAssociativity() const;
    virtual Precedence getPrecedence() const;

    virtual const ValueNode &DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

#endif // SIMPLEAST_H
