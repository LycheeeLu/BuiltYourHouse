#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>
#include <QPainter>


class Wall : public QGraphicsItem
{
public:
    Wall(const QPointF &startPoint, const QPointF &endPoint);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPointF startPoint() const { return start; }
    QPointF endPoint() const { return end; }
private:
    QPointF start;
    QPointF end;
};

#endif // WALL_H
