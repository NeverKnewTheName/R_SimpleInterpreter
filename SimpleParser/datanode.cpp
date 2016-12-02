#include "datanode.h"

#include <QDebug>

#include "simplesymboltable.h"
#include "variablesymbol.h"
#include "simplestack.h"

DataNode::DataNode(const unsigned int dataIndex, const QSharedPointer<SimpleSymbolTable> GlobalSymbolTable) :
    VariableNode(qSharedPointerDynamicCast<ConstantValueSymbol>(GlobalSymbolTable->lookup(QString("D%1").arg(dataIndex)))),
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

std::unique_ptr<SimpleNode> DataNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new DataNode(dataIndex, /*ToDo*/))
}
