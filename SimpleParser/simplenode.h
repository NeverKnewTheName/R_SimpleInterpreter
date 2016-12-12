#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#include <QVariant>
#include <QSharedPointer>
#include <memory>

#include "simplenodevisitable.h"

class ASTNode;
class ValueNode;
class SimpleSymbolTable;
class SimpleStack;

class SimpleNodeVisitor;

namespace Node {
typedef enum _NodeType
{
    Value,
    Data,
    Variable,
    Operation,
    Function,
    FunctionCall,
    Assignment,
    Program,
    Control,
    Scope,
    Block,
    Statement,
    Label,
    EOFNode,
    ERRORNode
}NodeType;

/**
  * \brief ValueTypes describes the types that are used for variables and return values
  *
  * The bit structure allows for fast evaluation of types.
  * If two types shall be converted, the one with the rightmost bit wins,
  * thus the resulting type will be the one with the rightmost bit.
  *
  * The TypeExtens is used to differentiate between Singed and Usigned Values,
  * as well as Scalar and Non-scalar types.
  */
typedef enum _ValueTypes
{
    ErrorType =     (quint64)0x0000,
    String =        (quint64)0x0001,
    Bool =          (quint64)0x0002,
    FloatingPoint = (quint64)0x0004,
    Double =        (quint64)0x0014,
    Float =         (quint64)0x0024,
    Integer =       (quint64)0x0008,
    Int64 =         (quint64)0x0018,
    Int32 =         (quint64)0x0028,
    Int16 =         (quint64)0x0048,
    Int8 =          (quint64)0x0088,
    BitCnt =        (quint64)0x00F0,
    ValType =       (quint64)0x000F,
    TypeDecl =      (quint64)0x00FF,
    SignBit =       (quint64)0x1000,
    NonScalarBit =  (quint64)0x2000,
    Void =          (quint64)0x4000,
    UserType =      (quint64)0x8000,
    TypeExtens =    (quint64)0xF000,
    UserTypeStart = (quint64)0xFFFF,
    MaxRange = (quint64)0xFFFFFFFFFFFFFFFF
}ValueTypes;
}

class SimpleNode : public SimpleNodeVisitable
{
public:

    SimpleNode();
    virtual ~SimpleNode();

    virtual Node::NodeType getNodeType() const = 0;
    virtual Node::ValueTypes getReturnType() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    virtual bool IsTerminal() const = 0;

    static QString getHumanReadableTypeNameToValueType(const Node::ValueTypes type);
    static bool canConvertTypes(const Node::ValueTypes OrigType, const Node::ValueTypes NewType);
    static Node::ValueTypes evaluateResultingType(const Node::ValueTypes type1, const Node::ValueTypes type2);
};
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //


class TerminalNode : public SimpleNode
{
public:
    TerminalNode();
    virtual ~TerminalNode();

    // SimpleNode interface
public:
    bool IsTerminal() const;
};

class NonTerminalNode : public SimpleNode
{
public:
    NonTerminalNode();
    virtual ~NonTerminalNode();

    // SimpleNode interface
public:
    bool IsTerminal() const;
};
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //


class ScopeNode : public NonTerminalNode
{
public:
    ScopeNode(const QString &ScopeName);
    ScopeNode(QSharedPointer<SimpleSymbolTable> ScopedSymbolTable);
    ScopeNode(const ScopeNode &ToCopy);
    virtual ~ScopeNode();

    const QSharedPointer<SimpleSymbolTable> &GetScopeSymbolTable() const;
    const QString &getScopeName() const;

    void AddExpressionToScope(std::unique_ptr<SimpleNode> Expression);

    void SetScopeReturnType(const Node::ValueTypes &returnType);

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;
private:
    static unsigned int ScopeCntr;
    const QString ScopeName;
    QSharedPointer<SimpleSymbolTable> ScopeSymbolTable;
    Node::ValueTypes ScopeReturnType;
    std::vector<std::unique_ptr<SimpleNode>> ScopeExpressions;

    // SimpleNode interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

class EOFNode : public TerminalNode
{
public:
    EOFNode();
    ~EOFNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // SimpleNode interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};
#endif // SIMPLENODE_H
