#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#include <QVariant>
#include <QSharedPointer>
#include <memory>


class ASTNode;
class ValueNode;
class SimpleSymbolTable;
class SimpleStack;

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

class SimpleNode
{
public:

    SimpleNode(Node::ValueTypes valueType = Node::ErrorType);
    virtual ~SimpleNode();

    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;

    virtual ASTNode *VisualizeNode(ASTNode *parentNode = nullptr) const;
    virtual QString printValue() const;
    virtual QString printNode() const;

    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode>>> FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode>>> FlatAST, int &maxStackSize, int &CurrentPosition) const;
    virtual uint8_t FlatCompileOPCode(int &curStackOffset) const;


    virtual bool IsTerminal() const = 0;

    static QString getHumanReadableTypeNameToValueType(const Node::ValueTypes type);
    static bool canConvertTypes(const Node::ValueTypes OrigType, const Node::ValueTypes NewType);

protected:
    Node::ValueTypes valueType;
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
    ASTNode *VisualizeNode(ASTNode *parentNode) const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const;

private:
    static unsigned int ScopeCntr;
    const QString ScopeName;
    QSharedPointer<SimpleSymbolTable> ScopeSymbolTable;
    Node::ValueTypes ScopeReturnType;
    std::vector<std::unique_ptr<SimpleNode>> ScopeExpressions;
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

    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    virtual uint8_t FlatCompileOPCode(int &curStackOffset) const;

    std::unique_ptr<SimpleNode> deepCopy() const;
};
#endif // SIMPLENODE_H
