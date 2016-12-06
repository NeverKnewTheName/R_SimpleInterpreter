#include "assignmentnode.h"

#include <QDebug>

#include "valuenode.h"
#include "variablenode.h"
#include "simplestack.h"

#include "astvisualizer.h"

AssignmentNode::AssignmentNode(std::unique_ptr<VariableNode> VariableToAssign, std::unique_ptr<SimpleNode> ValueToAssign) :
    SimpleNode(VariableToAssign->getReturnType()),
    VariableToAssign(std::move(VariableToAssign)),
    ValueToAssign(std::move(ValueToAssign))
{
    if(this->VariableToAssign->getReturnType() == Node::ErrorType) valueType = Node::ErrorType;
    if(this->ValueToAssign->getReturnType() == Node::ErrorType) valueType = Node::ErrorType;
}

AssignmentNode::AssignmentNode(const AssignmentNode &ToCopy) :
    SimpleNode(ToCopy.getReturnType()),
    VariableToAssign(dynamic_cast<VariableNode*>(ToCopy.VariableToAssign->deepCopy().release())),
    ValueToAssign(ToCopy.ValueToAssign->deepCopy())
{
}

AssignmentNode::~AssignmentNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType AssignmentNode::getNodeType() const
{
    return Node::Assignment;
}

Node::ValueTypes AssignmentNode::getReturnType() const
{
    return VariableToAssign->getReturnType();
}

ASTNode *AssignmentNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *AssgnmntASTNode = new ASTNode(printNode(),parentNode);
    VariableToAssign->VisualizeNode(AssgnmntASTNode);
    new ASTNode(printValue(), AssgnmntASTNode);
    ValueToAssign->VisualizeNode(AssgnmntASTNode);

    return AssgnmntASTNode;
}

QString AssignmentNode::printValue() const
{
    return QString("=");
}

QString AssignmentNode::printNode() const
{
    return QString("{AssignmentNode}:{%1}").arg(printValue());
}

std::unique_ptr<ValueNode> AssignmentNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value = ValueToAssign->visit(StackToUse);

    QSharedPointer<ValueSymbol> relatedSymbol = VariableToAssign->getRelatedVariableSymbol();

    if(relatedSymbol->getType() == SimpleSymbolTableEntry::Variable)
    {
        qSharedPointerDynamicCast<VariableSymbol>(relatedSymbol)->assignValue(std::move(value), StackToUse);
    }

    return relatedSymbol->getValue(StackToUse);
}

uint8_t AssignmentNode::FlatCompileOPCode(int &curStackOffset) const
{

}

std::unique_ptr<SimpleNode> AssignmentNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new AssignmentNode(*this));
}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > AssignmentNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const
{
    FlatAST = ValueToAssign->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);
    FlatAST = VariableToAssign->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);

    FlatAST->emplace_back(new AssignmentNode(*this));
    CurrentPosition++;

    return std::move(FlatAST);
}

