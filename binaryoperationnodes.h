#ifndef BINARYOPERATIONNODES_H
#define BINARYOPERATIONNODES_H

#include "operationnodes.h"

class BinaryArithmeticOperationNode : public BinaryOperationNode
{
public:
    BinaryArithmeticOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class AdditionNode : public BinaryArithmeticOperationNode
{
public:
    AdditionNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class SubtractionNode : public BinaryArithmeticOperationNode
{
public:
    SubtractionNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class MultiplicationNode : public BinaryArithmeticOperationNode
{
public:
    MultiplicationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class DivisionNode : public BinaryArithmeticOperationNode
{
public:
    DivisionNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ModuloNode : public BinaryArithmeticOperationNode
{
public:
    ModuloNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryLogicalOperationNode : public BinaryOperationNode
{
public:
    BinaryLogicalOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalANDNode : public BinaryLogicalOperationNode
{
public:
    LogicalANDNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
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
    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalORNode : public BinaryLogicalOperationNode
{
public:
    LogicalORNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
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
    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LogicalXORNode : public BinaryLogicalOperationNode
{
public:
    LogicalXORNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class GreaterNode : public BinaryLogicalOperationNode
{
public:
    GreaterNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LowerNode : public BinaryLogicalOperationNode
{
public:
    LowerNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualNode : public BinaryLogicalOperationNode
{
public:
    EqualNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrGreaterNode : public BinaryLogicalOperationNode
{
public:
    EqualOrGreaterNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class EqualOrLowerNode : public BinaryLogicalOperationNode
{
public:
    EqualOrLowerNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnequalNode : public BinaryLogicalOperationNode
{
public:
    UnequalNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class BinaryBitwiseOperationNode : public BinaryOperationNode
{
public:

    BinaryBitwiseOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ANDNode : public BinaryBitwiseOperationNode
{
public:
    ANDNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class ORNode : public BinaryBitwiseOperationNode
{
public:
    ORNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class XORNode : public BinaryBitwiseOperationNode
{
public:
    XORNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class LeftShiftNode : public BinaryBitwiseOperationNode
{
public:
    LeftShiftNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class RightShiftNode : public BinaryBitwiseOperationNode
{
public:
    RightShiftNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};


#endif // BINARYOPERATIONNODES_H
