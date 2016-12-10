#include "blocknode.h"

#include "simplenodevisitor.h"

#include "controlnode.h"
#include "escapecontrolnode.h"

#include <QDebug>

unsigned int BlockNode::BlockCntr = 0;

BlockNode::BlockNode(QString BlockName) :
    BlockReturnType(Node::Void),
    BlockSymbolTable(new SimpleSymbolTable(BlockName.append("_%1").arg(BlockCntr)))
{
    BlockCntr++;
}

BlockNode::BlockNode(const Node::ValueTypes &BlockExpectedReturnType, QString BlockName) :
    BlockReturnType(BlockExpectedReturnType),
    BlockSymbolTable(new SimpleSymbolTable(BlockName.append("_%1").arg(BlockCntr)))
{
    BlockCntr++;
}

BlockNode::BlockNode(const BlockNode &ToCopy) :
    BlockReturnType(ToCopy.BlockReturnType),
    BlockSymbolTable(ToCopy.BlockSymbolTable)
{
    for(const std::unique_ptr<SimpleNode> &stmt : ToCopy.BlockStatements)
    {
        BlockStatements.emplace_back(stmt->deepCopy());
    }
}

BlockNode::~BlockNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void BlockNode::addStatement(std::unique_ptr<SimpleNode> Statement)
{
    if(Statement->getNodeType() == Node::Control)
    {
        if(dynamic_cast<ControlNode*>(Statement.get())->getControlType() == ControlNode::ESCAPE)
        {
            if(dynamic_cast<EscapeControlNode*>(Statement.get())->getSpecificControlType() == ControlNode::RETURN)
            {
                Node::ValueTypes newBlockReturnType = dynamic_cast<ReturnNode*>(Statement.get())->getReturnType();
                if(BlockReturnType == Node::Void )
                {
                    BlockReturnType = newBlockReturnType;
                }
                else if( BlockReturnType != newBlockReturnType )
                {
                    if(SimpleNode::canConvertTypes(newBlockReturnType, BlockReturnType))
                    {
                        BlockReturnType = newBlockReturnType;
                    }
                    else if(!SimpleNode::canConvertTypes(BlockReturnType, newBlockReturnType))
                    {
                        BlockReturnType = Node::ErrorType;
                    }
                }
            }
        }
    }
    BlockStatements.emplace_back(std::move(Statement));
}

const std::vector<std::unique_ptr<SimpleNode> > &BlockNode::getBlockStatements() const
{
    return BlockStatements;
}

void BlockNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<BlockNode>(new BlockNode(*this)));
}

Node::NodeType BlockNode::getNodeType() const
{
    return Node::Block;
}

Node::ValueTypes BlockNode::getReturnType() const
{
    return BlockReturnType;
}

QString BlockNode::printValue() const
{
    return QString("{ }");
}

QString BlockNode::printNode() const
{
    return QString("{(BlockNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> BlockNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new BlockNode(*this));
}

QSharedPointer<SimpleSymbolTable> BlockNode::getBlockSymbolTable() const
{
    return BlockSymbolTable;
}

