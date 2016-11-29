#ifndef UNARYOPERATIONNODES_H
#define UNARYOPERATIONNODES_H

#include "operationnodes.h"

class UnaryArithmeticOperationNode : public UnaryOperationNode
{
public:
    UnaryArithmeticOperationNode(SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class TypeCastNode : public UnaryArithmeticOperationNode
{
public:
    TypeCastNode(SimpleNodeUniquePtr rightChild, SimpleNode::ValueTypes typeToCastTo);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
private:
    SimpleNode::ValueTypes typeToCastTo;
};

class IncrementNode : public UnaryArithmeticOperationNode
{
public:
    IncrementNode(SimpleNodeUniquePtr rightChild);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class DecrementNode : public UnaryArithmeticOperationNode
{
public:
    DecrementNode(SimpleNodeUniquePtr rightChild);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class PositiveNode : public UnaryArithmeticOperationNode
{
public:
    PositiveNode(SimpleNodeUniquePtr rightChild);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class NegativeNode : public UnaryArithmeticOperationNode
{
public:
    NegativeNode(SimpleNodeUniquePtr rightChild);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnaryLogicalOperationNode : public UnaryOperationNode
{
public:
    UnaryLogicalOperationNode(SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalNegationNode : public UnaryLogicalOperationNode
{
public:
    LogicalNegationNode(SimpleNodeUniquePtr rightChild);
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
    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class UnaryBitwiseOperationNode : public UnaryOperationNode
{
public:
    UnaryBitwiseOperationNode(SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class OnesComplementNode : public UnaryBitwiseOperationNode
{
public:
    OnesComplementNode(SimpleNodeUniquePtr rightChild);
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
    virtual ValueNodeUniquePtr DoOperation();

    virtual QString printValue() const;
    virtual QString printNode() const;
};

#endif // UNARYOPERATIONNODES_H
