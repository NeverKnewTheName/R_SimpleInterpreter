#ifndef ASTVISUALIZER_H
#define ASTVISUALIZER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include <memory>

#include "simplenode.h"

class ASTVisualizer : public QObject
{
    Q_OBJECT
public:
    explicit ASTVisualizer(QObject *parent = 0);

signals:

public slots:

private:
    QGraphicsScene ASTScene;
};

class ASTNode : public QGraphicsEllipseItem
{
public:
    ASTNode(const QString &nodeText, QGraphicsItem *parent = Q_NULLPTR);

    int getChildCount() const;
    int getLevel() const;

    qreal getChildNodeOffset() const;

    void setChildNodeOffset(const qreal &value);

    void addChildConnection(qreal ChildXCenter, qreal ChildYCenter);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    const QString ASTNodeText;
    int ASTNodeLevel;
    qreal XOffset;
    qreal YOffset;
    qreal ChildNodeOffset;
    QVector<QGraphicsLineItem*> connections;
};

#endif // ASTVISUALIZER_H
