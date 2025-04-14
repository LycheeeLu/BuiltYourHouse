#include "furniture.h"
#include <QBrush>

Furniture::Furniture(int w, int h, FurnitureType type)
    : width(w), height(h), furnitureType(type)
{
    // Make furniture selectable and movable
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}


Furniture::FurnitureType Furniture::getType() const
{

    return furnitureType;
}


Furniture::Furniture(const Furniture &other)
    : QGraphicsItem(), width(other.width), height(other.height), furnitureType(other.furnitureType)
{
    // Copy properties
    setPos(other.pos());
    setRotation(other.rotation());

    // Make furniture selectable and movable
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);

}

QRectF Furniture::boundingRect() const
{
    return QRectF(-width/2, -height/2, width, height);

}

void Furniture::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    //avoid unused parameter warning, only need painter and option

    // Draw furniture based on type


    QRectF rect = boundingRect();

    QPen pen(Qt::black, 1);
    if (option->state & QStyle::State_Selected) {
        pen.setColor(Qt::blue);
        pen.setWidth(2);
    }
    painter->setPen(pen);


     // Set the blue background color for the fill
    QColor lightBlue(173, 216, 230);
    painter->setBrush(QBrush(lightBlue));


    switch (furnitureType) {
    case Sofa:
        break;
    case Chair:
        break;
    case Table:
        break;
    }


    //set different shapes based on furniture
    if (furnitureType == Chair || furnitureType == Sofa) {
        painter->drawRect (rect);
    }
    if (furnitureType == Table){
        painter -> drawEllipse (rect);
    }

    // Draw label in the middle of the furniture icon
    /*QString label;
    switch (furnitureType) {
    case Sofa:
        label = "Sofa";
        break;
    case Chair:
        label = "Chair";
        break;
    case Table:
        label = "Table";
        break;
    }
    painter->drawText(rect, Qt::AlignCenter, label);*/

}

bool Furniture::collidesWithFurnitureOrWalls()
{
    if (!scene())
        return false;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (item == this) continue; // Skip self

        if (dynamic_cast<Furniture*>(item) || dynamic_cast<Wall*>(item)) {
            if (collidesWithItem(item)) {
                return true;
            }
        }
    }

    // Check if the furniture is within scene bounds
    QRectF sceneRect = scene()->sceneRect();
    QRectF itemRect = mapToScene(boundingRect()).boundingRect();

    if (!sceneRect.contains(itemRect)) {
        return true;
    }

    return false;

}

bool Furniture::tryRotate(qreal angle)
{
    // Save original rotation
    qreal originalRotation = rotation();

    // Apply new rotation
    setRotation(rotation() + angle);

    // Check if rotation causes collision
    if (collidesWithFurnitureOrWalls()) {
        // Restore original rotation
        setRotation(originalRotation);
        return false;
    }

    return true;

}


void Furniture::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
        originalPos = pos();
    }
    QGraphicsItem::mousePressEvent(event);

}

void Furniture::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    // Check for collisions during drag
    if (collidesWithFurnitureOrWalls()) {
        // Move back to the original position
        setPos(pos());
    }

}

void Furniture::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    // If position changed and no collision, this is a successful move
    if (pos() != originalPos && !collidesWithFurnitureOrWalls()) {
        // Movement handling is done in mainwindow through commands
    }
}

void Furniture::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // Create context menu for rotation
    QMenu menu;
    QAction *rotateAction = menu.addAction("Rotate 90° Clockwise");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == rotateAction) {
        tryRotate(90);
    }

}

