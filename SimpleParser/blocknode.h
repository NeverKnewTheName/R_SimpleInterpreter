#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "simplenode.h"

#include "simplesymboltable.h"

class BlockNode : public NonTerminalNode
{
public:
    BlockNode(QString BlockName = QString("Block"));
    BlockNode(const Node::ValueTypes &BlockExpectedReturnType, QString BlockName = QString("Block"));
    BlockNode(const BlockNode &ToCopy);
    virtual ~BlockNode();

    void addStatement(std::unique_ptr<SimpleNode> Statement);

    const std::vector<std::unique_ptr<SimpleNode> > &getBlockStatements() const;

    QSharedPointer<SimpleSymbolTable> getBlockSymbolTable() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

private:
    std::vector<std::unique_ptr<SimpleNode>> BlockStatements;
    Node::ValueTypes BlockReturnType;
    QSharedPointer<SimpleSymbolTable> BlockSymbolTable;

    static unsigned int BlockCntr;
};

#endif // BLOCKNODE_H
