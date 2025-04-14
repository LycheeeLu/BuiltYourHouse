#ifndef FURNITURE_H
#define FURNITURE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMenu>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyle>
#include "wall.h"

class Furniture : public QGraphicsItem
{
public:

  enum FurnitureType { Sofa, Chair, Table };


  Furniture(int width, int height, FurnitureType type);
// Copy constructor for copy-paste functionality
  Furniture(const Furniture &other);

  //create a rectangle to detect collision (wrapped around the main furniture)
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  // Check for collisions with other furniture or walls
  bool collidesWithFurnitureOrWalls();

  // Try to rotate, return true if successful
  bool tryRotate(qreal angle);

  FurnitureType getType() const;


protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
  int width;
  int height;
  QPointF dragStartPosition;
  QPointF originalPos;
  FurnitureType furnitureType;
};

#endif // FURNITURE_H
