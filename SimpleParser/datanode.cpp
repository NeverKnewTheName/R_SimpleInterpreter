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
#ifdef __DEBUG_OUTPUT__
        qDebug() << "TRIED TO SET A DATANODE WITH A SYMBOL TABLE THAT WAS NOT THE GLOBAL SYMBOL TABLE!";
#endif
    }
}

DataNode::~DataNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
#endif
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
