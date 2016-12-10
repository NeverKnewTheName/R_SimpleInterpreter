#include "astvisualizer.h"

#include <QPainter>


#include <QDebug>

ASTVisualizer::ASTVisualizer(QObject *parent) :
    QObject(parent),
    ASTScene(parent)
{

}

ASTNode::ASTNode(const QString &nodeText, QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    ASTNodeText(nodeText),
    ASTNodeLevel(0),
    XOffset(0),
    YOffset(0),
    ChildNodeOffset(0)
{
    setBrush(QBrush(Qt::white));
    if(parent != nullptr)
    {
        ASTNodeLevel += 1 + dynamic_cast<ASTNode*>(parent)->getLevel();
        YOffset = ASTNodeLevel * (50 + 25);
//        if(parent->childItems().size() == 1)
//        {
//            XOffset = dynamic_cast<ASTNode*>(parent)->getXOffset();
//        }
//        else
        {
            ChildNodeOffset += dynamic_cast<ASTNode*>(parent)->getChildNodeOffset();
            XOffset = ChildNodeOffset;
        }
        dynamic_cast<ASTNode*>(parent)->addChildConnection(XOffset+50,YOffset+25);
        ChildNodeOffset += 100 + 25;
        dynamic_cast<ASTNode*>(parent)->setChildNodeOffset(ChildNodeOffset);
    }
    QGraphicsEllipseItem::setRect(QRectF(QPointF(XOffset,YOffset),QSizeF(100,50)));
}

int ASTNode::getChildCount() const
{
    return childItems().size() - connections.size();
}

int ASTNode::getLevel() const
{
    return ASTNodeLevel;
}

qreal ASTNode::getChildNodeOffset() const
{
    return ChildNodeOffset;
}

void ASTNode::setChildNodeOffset(const qreal &value)
{
    ASTNode *parentNode = dynamic_cast<ASTNode*>(parentItem());
    if(parentNode != nullptr)
    {
        parentNode->setChildNodeOffset(value);
    }
    ChildNodeOffset = value;
}

void ASTNode::addChildConnection(qreal ChildXCenter, qreal ChildYCenter)
{
    qreal xCenter = XOffset+50;
    qreal yCenter = YOffset+25;

    QGraphicsLineItem *newConnection = new QGraphicsLineItem(xCenter,yCenter,ChildXCenter,ChildYCenter,this);
    newConnection->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    connections.append(newConnection);
}

void ASTNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);

    painter->drawText(boundingRect(), ASTNodeText, QTextOption(Qt::AlignCenter));
}

qreal ASTNode::getXOffset() const
{
    return XOffset;
}

qreal ASTNode::getYOffset() const
{
    return YOffset;
}

void ASTNode::setYOffset(const qreal &value)
{
    YOffset = value;
}

void ASTNode::setXOffset(const qreal &value)
{
    XOffset = value;
}

