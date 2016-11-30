#ifndef UNARYOPERATIONNODES_H
#define UNARYOPERATIONNODES_H

#include "operationnodes.h"

class UnaryArithmeticOperationNode : public UnaryOperationNode
{
public:
    UnaryArithmeticOperationNode(std::unique_ptr<SimpleNode> RightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual std::unique_ptr<ValueNode> DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class TypeCastNode : public UnaryArithmeticOperationNode
{
public:
    TypeCastNode(std::unique_ptr<SimpleNode> RightChild, Node::ValueTypes typeToCastTo);
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
    std::unique_ptr<ValueNode> DoOperation() const;

    QString printValue() const;
    QString printNode() const;
private:
    Node::ValueTypes typeToCastTo;
};

class IncrementNode : public UnaryArithmeticOperationNode
{
public:
    IncrementNode(std::unique_ptr<SimpleNode> RightChild);
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
    std::unique_ptr<ValueNode> DoOperation() const;

    QString printValue() const;
    QString printNode() const;
};

class DecrementNode : public UnaryArithmeticOperationNode
{
public:
    DecrementNode(std::unique_ptr<SimpleNode> RightChild);
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
    std::unique_ptr<ValueNode> DoOperation() const;

    QString printValue() const;
    QString printNode() const;
};

class PositiveNode : public UnaryArithmeticOperationNode
{
public:
    PositiveNode(std::unique_ptr<SimpleNode> RightChild);
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
    std::unique_ptr<ValueNode> DoOperation() const;

    QString printValue() const;
    QString printNode() const;
};

class NegativeNode : public UnaryArithmeticOperationNode
{
public:
    NegativeNode(std::unique_ptr<SimpleNode> RightChild);
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
    std::unique_ptr<ValueNode> DoOperation() const;

    QString printValue() const;
    QString printNode() const;
};

class UnaryLogicalOperationNode : public UnaryOperationNode
{
public:
    UnaryLogicalOperationNode(std::unique_ptr<SimpleNode> RightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual std::unique_ptr<ValueNode> DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class LogicalNegationNode : public UnaryLogicalOperationNode
{
public:
    LogicalNegationNode(std::unique_ptr<SimpleNode> RightChild);
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
    std::unique_ptr<ValueNode> DoOperation() const;

    QString printValue() const;
    QString printNode() const;
};

class UnaryBitwiseOperationNode : public UnaryOperationNode
{
public:
    UnaryBitwiseOperationNode(std::unique_ptr<SimpleNode> RightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual std::unique_ptr<ValueNode> DoOperation() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class OnesComplementNode : public UnaryBitwiseOperationNode
{
public:
    OnesComplementNode(std::unique_ptr<SimpleNode> RightChild);
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
    virtual std::unique_ptr<ValueNode> DoOperation() const;

    virtual QString printValue() const;
    virtual QString printNode() const;
};

#endif // UNARYOPERATIONNODES_H