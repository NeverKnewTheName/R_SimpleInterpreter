#ifndef SIMPLESTACK_H
#define SIMPLESTACK_H

#include "valuenode.h"

/**
 * @brief The SimpleStack class implements a simple stack that allows
 * push, pop and random retrieval of \ref ValueNodes
 */
class SimpleStack
{
public:
    SimpleStack(const size_t MaximumStackSize );

    /**
     * @brief Returns a newly created unique pointer that points to
     * to the \ref ValueNode that is at the provided offset in the stack
     *
     * @param offset Offset of the queried entry to retrieve
     *
     * @return A newly constructed unique pointer that holds the queried \ref ValueNode
     */
    std::unique_ptr<ValueNode> StackAt( size_t offset );

    /**
     * @brief Replaces the \ref ValueNode that is at the provided
     * offset with the provided \ref ValueNode
     *
     * @param offset                offset in the stack whose value shall be replaced
     * @param ValueToReplaceWith    value that will replace that current value at offset
     *
     * @return Returns a unique pointer that holds the new \ref ValueNode
     */
    std::unique_ptr<ValueNode> StackReplaceAt( size_t offset, std::unique_ptr<ValueNode> ValueToReplaceWith );

    /**
     * @brief Pushes the provided \ref ValueNode on top of the stack
     * and returns the index of the pushed element
     *
     * @param valueToPush \ref ValueNode to push onto the stack
     *
     * @return Index of the pushed element
     */
    size_t StackPush( ValueNode valueToPush );
    /**
     * @brief Returns a newly created unique pointer that points
     * to the \ref ValueNode that is currently on top
     * of the stack and removes it from the stack
     *
     * @return Unique pointer that points to the popped \ref ValueNode
     *
     * \note A unique pointer is returned to keep confirmity with the nodes.
     */
    std::unique_ptr<ValueNode> StackPop();

    /**
     * @brief Queries the stack whether it is empty or not
     *
     * @return Boolean value expressing whether the stack is empty or not
     */
    bool isEmpty() const;

private:
    /**
     * @brief The maximum size the stack can grow
     *
     * \note There is a limit to the stack, because
     * the same program shall be run in an embedded environment
     * with restricted ressources
     */
    const size_t MaxSize;

    /**
     * @brief The data structur (std::vector) that holds the current stack entries
     */
    std::vector<ValueNode> TheStack;
};

#endif // SIMPLESTACK_H
