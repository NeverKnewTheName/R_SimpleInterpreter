#ifndef BINARYOPERATIONNODES_H
#define BINARYOPERATIONNODES_H

#include "operationnodes.h"

class BinaryArithmeticOperationNode : public BinaryOperationNode
{
public:
    BinaryArithmeticOperationNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class AdditionNode : public BinaryArithmeticOperationNode
{
public:
    AdditionNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class SubtractionNode : public BinaryArithmeticOperationNode
{
public:
    SubtractionNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class MultiplicationNode : public BinaryArithmeticOperationNode
{
public:
    MultiplicationNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class DivisionNode : public BinaryArithmeticOperationNode
{
public:
    DivisionNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ModuloNode : public BinaryArithmeticOperationNode
{
public:
    ModuloNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryLogicalOperationNode : public BinaryOperationNode
{
public:
    BinaryLogicalOperationNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalANDNode : public BinaryLogicalOperationNode
{
public:
    LogicalANDNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
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
    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalORNode : public BinaryLogicalOperationNode
{
public:
    LogicalORNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
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
    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalXORNode : public BinaryLogicalOperationNode
{
public:
    LogicalXORNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class GreaterNode : public BinaryLogicalOperationNode
{
public:
    GreaterNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LowerNode : public BinaryLogicalOperationNode
{
public:
    LowerNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualNode : public BinaryLogicalOperationNode
{
public:
    EqualNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrGreaterNode : public BinaryLogicalOperationNode
{
public:
    EqualOrGreaterNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrLowerNode : public BinaryLogicalOperationNode
{
public:
    EqualOrLowerNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnequalNode : public BinaryLogicalOperationNode
{
public:
    UnequalNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryBitwiseOperationNode : public BinaryOperationNode
{
public:

    BinaryBitwiseOperationNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNode &DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ANDNode : public BinaryBitwiseOperationNode
{
public:
    ANDNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ORNode : public BinaryBitwiseOperationNode
{
public:
    ORNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class XORNode : public BinaryBitwiseOperationNode
{
public:
    XORNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LeftShiftNode : public BinaryBitwiseOperationNode
{
public:
    LeftShiftNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const  ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class RightShiftNode : public BinaryBitwiseOperationNode
{
public:
    RightShiftNode(const SimpleNode &leftChild, const SimpleNode &rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNode &DoOperation();

    QString printValue() const;
    QString printNode() const;
};


#endif // BINARYOPERATIONNODES_H
