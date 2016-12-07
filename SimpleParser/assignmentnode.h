#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "simplenode.h"

class VariableNode;

class AssignmentNode : public NonTerminalNode
{
public:
//    AssignmentNode();
    AssignmentNode(std::unique_ptr<VariableNode> VariableToAssign, std::unique_ptr<SimpleNode> ValueToAssign);
    AssignmentNode(const AssignmentNode &ToCopy);
    virtual ~AssignmentNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    ASTNode *VisualizeNode(ASTNode *parentNode) const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    virtual uint8_t FlatCompileOPCode(int &curStackOffset) const;
    std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    const std::unique_ptr<VariableNode> &getVariableToAssign() const;

    const std::unique_ptr<SimpleNode> &getValueToAssign() const;

private:
    std::unique_ptr<VariableNode> VariableToAssign;
    std::unique_ptr<SimpleNode> ValueToAssign;


    // SimpleNode interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

#endif // ASSIGNMENTNODE_H
