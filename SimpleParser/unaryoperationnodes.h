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

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;
};

class TypeCastNode : public UnaryArithmeticOperationNode
{
public:
    TypeCastNode(std::unique_ptr<SimpleNode> RightChild, Node::ValueTypes typeToCastTo);
    TypeCastNode(const TypeCastNode &ToCopy);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    Node::ValueTypes getTypeToCastTo() const;

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

    QString printValue() const;
    QString printNode() const;

    std::unique_ptr<SimpleNode> deepCopy() const;

private:
    Node::ValueTypes typeToCastTo;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

class IncrementNode : public UnaryArithmeticOperationNode
{
public:
    IncrementNode(std::unique_ptr<SimpleNode> RightChild, const bool isPre);
    IncrementNode(const IncrementNode &ToCopy);
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

    QString printValue() const;
    QString printNode() const;

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;


    bool getIsPre() const;

private:
    const bool IsPre;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

class DecrementNode : public UnaryArithmeticOperationNode
{
public:
    DecrementNode(std::unique_ptr<SimpleNode> RightChild, const bool isPre);
    DecrementNode(const DecrementNode &ToCopy);
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


    QString printValue() const;
    QString printNode() const;

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;

    bool getIsPre() const;

private:
    const bool IsPre;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
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


    QString printValue() const;
    QString printNode() const;

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;


    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
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

    QString printValue() const;
    QString printNode() const;

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;


    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

class UnaryLogicalOperationNode : public UnaryOperationNode
{
public:
    UnaryLogicalOperationNode(std::unique_ptr<SimpleNode> RightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

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


    QString printValue() const;
    QString printNode() const;

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;


    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

class UnaryBitwiseOperationNode : public UnaryOperationNode
{
public:
    UnaryBitwiseOperationNode(std::unique_ptr<SimpleNode> RightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

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



    virtual QString printValue() const;
    virtual QString printNode() const;

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;


    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

#endif // UNARYOPERATIONNODES_H
