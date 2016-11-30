#ifndef DATANODE_H
#define DATANODE_H

#include "variablenode.h"

class SymbolTable;

class DataNode : public VariableNode
{
public:
    DataNode(const unsigned int dataIndex, const QSharedPointer<SymbolTable> GlobalSymbolTable);
    virtual ~DataNode();
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;

    QString printNode() const;

private:
    const unsigned int dataIndex;
};

#endif // DATANODE_H