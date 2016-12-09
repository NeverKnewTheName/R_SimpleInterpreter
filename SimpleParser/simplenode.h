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
    EOFNode,
    ERRORNode
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
