#ifndef BINARYOPERATIONNODES_H
#define BINARYOPERATIONNODES_H

#include "operationnodes.h"

class BinaryArithmeticOperationNode : public BinaryOperationNode
{
public:
    BinaryArithmeticOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class AdditionNode : public BinaryArithmeticOperationNode
{
public:
    AdditionNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class SubtractionNode : public BinaryArithmeticOperationNode
{
public:
    SubtractionNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class MultiplicationNode : public BinaryArithmeticOperationNode
{
public:
    MultiplicationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class DivisionNode : public BinaryArithmeticOperationNode
{
public:
    DivisionNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ModuloNode : public BinaryArithmeticOperationNode
{
public:
    ModuloNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryLogicalOperationNode : public BinaryOperationNode
{
public:
    BinaryLogicalOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalANDNode : public BinaryLogicalOperationNode
{
public:
    LogicalANDNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalORNode : public BinaryLogicalOperationNode
{
public:
    LogicalORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalXORNode : public BinaryLogicalOperationNode
{
public:
    LogicalXORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class GreaterNode : public BinaryLogicalOperationNode
{
public:
    GreaterNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LowerNode : public BinaryLogicalOperationNode
{
public:
    LowerNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualNode : public BinaryLogicalOperationNode
{
public:
    EqualNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrGreaterNode : public BinaryLogicalOperationNode
{
public:
    EqualOrGreaterNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrLowerNode : public BinaryLogicalOperationNode
{
public:
    EqualOrLowerNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnequalNode : public BinaryLogicalOperationNode
{
public:
    UnequalNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryBitwiseOperationNode : public BinaryOperationNode
{
public:

    BinaryBitwiseOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ANDNode : public BinaryBitwiseOperationNode
{
public:
    ANDNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ORNode : public BinaryBitwiseOperationNode
{
public:
    ORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class XORNode : public BinaryBitwiseOperationNode
{
public:
    XORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LeftShiftNode : public BinaryBitwiseOperationNode
{
public:
    LeftShiftNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class RightShiftNode : public BinaryBitwiseOperationNode
{
public:
    RightShiftNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};


#endif // BINARYOPERATIONNODES_H
