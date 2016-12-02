#include "simplestack.h"

SimpleStack::SimpleStack(const size_t MaximumStackSize) :
    MaxSize(MaximumStackSize)
{
    TheStack.reserve(MaximumStackSize);
}

std::unique_ptr<ValueNode> SimpleStack::StackAt(size_t offset)
{
    return std::unique_ptr<ValueNode>(new ValueNode(TheStack.at(offset)));
}

std::unique_ptr<ValueNode> SimpleStack::StackReplaceAt(size_t offset, std::unique_ptr<ValueNode> ValueToReplaceWith)
{
    TheStack.at(offset) = *(ValueToReplaceWith.get());
    return ValueToReplaceWith;
}

size_t SimpleStack::StackPush(ValueNode valueToPush)
{
    size_t currentTopIndex = TheStack.size();

    TheStack.push_back(valueToPush);

    return currentTopIndex;
}

std::unique_ptr<ValueNode> SimpleStack::StackPop()
{
    std::unique_ptr<ValueNode> PoppedElement(new ValueNode(TheStack.back()));
    TheStack.pop_back();
    return PoppedElement;
}

bool SimpleStack::isEmpty() const
{
    return static_cast<bool>(TheStack.size());
}
