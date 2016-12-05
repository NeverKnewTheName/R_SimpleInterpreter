#include "operationnodes.h"

#include <QDebug>

#include "valuenode.h"

#include "simplestack.h"

#include "astvisualizer.h"

OperationNode::OperationNode()
{
}

OperationNode::~OperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType OperationNode::getNodeType() const
{
    return Node::Operation;
}

Node::ValueTypes OperationNode::getReturnType() const
{
    return returnType;
}

std::unique_ptr<ValueNode> OperationNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    return DoOperation(StackToUse);
}

UnaryOperationNode::UnaryOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOPRightChild(std::move(rightChild))
{
}

UnaryOperationNode::UnaryOperationNode(const UnaryOperationNode &ToCopy) :
    UnaryOPRightChild(ToCopy.UnaryOPRightChild->deepCopy())
{

}

UnaryOperationNode::~UnaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

OperationNode::ArityTypes UnaryOperationNode::getArityType() const
{
    return OperationNode::Unary;
}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > UnaryOperationNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const
{
    FlatAST = UnaryOPRightChild->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);

    FlatAST->emplace_back(deepCopy());
    CurrentPosition++;

    return std::move(FlatAST);
}

ASTNode *UnaryOperationNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *UnaryOPASTNode = new ASTNode(printNode(),parentNode);
    new ASTNode(printValue(),UnaryOPASTNode);
    UnaryOPRightChild->VisualizeNode(UnaryOPASTNode);

    return UnaryOPASTNode;
}


BinaryOperationNode::BinaryOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryOPLeftChild(std::move(leftChild)),
    BinaryOPRightChild(std::move(rightChild))
{
}

BinaryOperationNode::BinaryOperationNode(const BinaryOperationNode &ToCopy) :
    BinaryOPLeftChild(std::unique_ptr<SimpleNode>(ToCopy.BinaryOPLeftChild->deepCopy())),
    BinaryOPRightChild(std::unique_ptr<SimpleNode>(ToCopy.BinaryOPRightChild->deepCopy()))
{
}

BinaryOperationNode::~BinaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

OperationNode::ArityTypes BinaryOperationNode::getArityType() const
{
    return OperationNode::Binary;
}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > BinaryOperationNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const
{
    FlatAST = BinaryOPRightChild->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);
    FlatAST = BinaryOPLeftChild->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);

    FlatAST->emplace_back(deepCopy());
    CurrentPosition++;

    return std::move(FlatAST);
}

ASTNode *BinaryOperationNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *BinaryOPASTNode = new ASTNode(printNode(),parentNode);
    BinaryOPLeftChild->VisualizeNode(BinaryOPASTNode);
    new ASTNode(printValue(),BinaryOPASTNode);
    BinaryOPRightChild->VisualizeNode(BinaryOPASTNode);

    return BinaryOPASTNode;
}

TernaryOperationNode::TernaryOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild) :
    TernaryOPLeftChild(std::move(leftChild)),
    TernaryOPMidChild(std::move(midChild)),
    TernaryOPRightChild(std::move(rightChild))
{
}

TernaryOperationNode::TernaryOperationNode(const TernaryOperationNode &ToCopy) :
    TernaryOPLeftChild(ToCopy.TernaryOPLeftChild->deepCopy()),
    TernaryOPMidChild(ToCopy.TernaryOPMidChild->deepCopy()),
    TernaryOPRightChild(ToCopy.TernaryOPRightChild->deepCopy())
{

}

TernaryOperationNode::~TernaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

OperationNode::ArityTypes TernaryOperationNode::getArityType() const
{
    return OperationNode::Ternary;
}

/*
 * When parsing the tree temp variables will store immediate values
 *
 * as long as the walker does not catch a node that requires the previous value, it will keep it and simply use another temporary storage
 * -> might be an array with a reasonable size (3 for ternary operations?? what about nesting?)
 */

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > TernaryOperationNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const
{
    //The special operation node has already been put into the FlatAST -> Add children from here on!
    FlatAST = TernaryOPRightChild->FlatCompile(std::move(FlatAST),maxStackSize, CurrentPosition);
    FlatAST = TernaryOPMidChild->FlatCompile(std::move(FlatAST),maxStackSize, CurrentPosition);
    FlatAST = TernaryOPLeftChild->FlatCompile(std::move(FlatAST),maxStackSize, CurrentPosition);

    FlatAST->emplace_back(deepCopy());
    CurrentPosition++;

    return std::move(FlatAST);
}

ASTNode *TernaryOperationNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *TernaryOPASTNode = new ASTNode(printNode(),parentNode);
    QStringList TernaryOpSplit = printValue().split(' ');
    TernaryOPLeftChild->VisualizeNode(TernaryOPASTNode);
    new ASTNode(TernaryOpSplit.first(),TernaryOPASTNode);
    TernaryOPMidChild->VisualizeNode(TernaryOPASTNode);
    new ASTNode(TernaryOpSplit.last(),TernaryOPASTNode);
    TernaryOPRightChild->VisualizeNode(TernaryOPASTNode);

    return TernaryOPASTNode;
}
