#ifndef SIMPLENODEVISITABLE_H
#define SIMPLENODEVISITABLE_H

class SimpleNodeVisitor;

class SimpleNodeVisitable
{
public:
    SimpleNodeVisitable(){}

    virtual void accept(SimpleNodeVisitor *visitor) const = 0;
};

#endif // SIMPLENODEVISITABLE_H
