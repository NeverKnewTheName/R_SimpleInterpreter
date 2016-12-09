#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include <vector>
#include "simplenode.h"

class SimpleSymbolTable;
class FunctionSymbol;

class FunctionCallNode : public NonTerminalNode
{
public:
    FunctionCallNode(
            const QString &FunctionName,
            QSharedPointer<SimpleSymbolTable> CurSymblTable,
            std::vector<std::unique_ptr<SimpleNode>> &FunctionArguments
            );
    FunctionCallNode(const FunctionCallNode &ToCopy);
    virtual ~FunctionCallNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;

    std::unique_ptr<SimpleNode> deepCopy() const;
    QSharedPointer<FunctionSymbol> getRelatedSymbol() const;

    const std::vector<std::unique_ptr<SimpleNode> > &getFuncArgs() const;

    QSharedPointer<SimpleSymbolTable> getCurrentSymbolTable() const;

private:
    QString FunctionName;
    std::vector<std::unique_ptr<SimpleNode>> FuncArgs;
    QSharedPointer<FunctionSymbol> RelatedSymbol;
    QSharedPointer<SimpleSymbolTable> CurrentSymbolTable;
    Node::ValueTypes returnType;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

#endif // FUNCTIONCALLNODE_H
