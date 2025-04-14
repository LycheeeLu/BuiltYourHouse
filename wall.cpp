#include "wall.h"


Wall::Wall(const QPointF &startPoint, const QPointF &endPoint)
    : start(startPoint), end(endPoint)
{

    // checkcing if the wall should be vertical or horizontal
    /*qreal dx = qAbs(endPoint.x() - startPoint.x());
    qreal dy = qAbs(endPoint.y() - startPoint.y());

    if (dx > dy) {
        // vertical
        start = startPoint;
        end = QPointF(endPoint.x(), startPoint.y());
    } else {
        // horizontal
        start = startPoint;
        end = QPointF(startPoint.x(), endPoint.y());
    }
    // Walls can't be selected*/
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

QRectF Wall::boundingRect() const
{
    // Add a small margin for the wall thickness
    qreal extra = 2.0;
    return QRectF(
        qMin(start.x(), end.x()) - extra,
        qMin(start.y(), end.y()) - extra,
        qAbs(end.x() - start.x()) + extra * 2,
        qAbs(end.y() - start.y()) + extra * 2
        );

}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen(Qt::black, 4);
    painter->setPen(pen);
    painter->drawLine(start - pos(), end - pos());

}
