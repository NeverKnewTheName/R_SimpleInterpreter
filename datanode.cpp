#include "datanode.h"

#include <QDebug>

#include "symboltable.h"
#include "variablesymbol.h"

DataNode::DataNode(const unsigned int dataIndex, const QSharedPointer<SymbolTable> GlobalSymbolTable) :
    VariableNode(qSharedPointerDynamicCast<VariableSymbol>(GlobalSymbolTable->lookup(QString("D%1").arg(dataIndex)))),
    dataIndex(dataIndex)
{
    if( GlobalSymbolTable->getParentSymbolTable() != NULL )
    {
        qDebug() << "TRIED TO SET A DATANODE WITH A SYMBOL TABLE THAT WAS NOT THE GLOBAL SYMBOL TABLE!";
    }
}

DataNode::~DataNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
}

Node::NodeType DataNode::getNodeType() const
{
    return Node::Data;
}

Node::ValueTypes DataNode::getReturnType() const
{
    return Node::Integer;
}

QString DataNode::printNode() const
{
    QString NodeType = "DataNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}
