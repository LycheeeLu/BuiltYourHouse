#include "furniture.h"
#include <QBrush>
#include <QUndoStack>
#include "movecommand.h"
#include <QDateTime>
#include "mainwindow.h"
#include <QTimer>

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
//this only works for one item
/* void Furniture::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    // If position changed and no collision, this is a successful move
    if (pos() != originalPos && !collidesWithFurnitureOrWalls()) {
        // Create a move command and add it to the undo stack
        QUndoStack *undoStack = dynamic_cast<QUndoStack*>(scene()->parent()->findChild<QUndoStack*>());
        if (undoStack) {
            MoveCommand *command = new MoveCommand(this, originalPos, pos());
            undoStack->push(command);
        }
    } else if (collidesWithFurnitureOrWalls()) {
        // Move back to original position if there's a collision
        setPos(originalPos);
    }
} */

void Furniture::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    // If position hasn't changed or we have a collision, revert the move
    if (pos() == originalPos || collidesWithFurnitureOrWalls()) {
        setPos(originalPos);
        return;
    }

    QUndoStack *undoStack = dynamic_cast<QUndoStack*>(scene()->parent()->findChild<QUndoStack*>());
    if (!undoStack) return;

    // Check if this is part of a multi-selection
    QList<QGraphicsItem*> selectedItems = scene()->selectedItems();

    // If this is the only selected item, create a command just for it
    if (selectedItems.size() <= 1) {
        MoveCommand *command = new MoveCommand(this, originalPos, pos());
        undoStack->push(command);
        return;
    }

    // For multi-selection, we need to handle this differently
    // We'll use a static variable to track when we've already processed a group
    static QGraphicsScene* lastProcessedScene = nullptr;
    static qint64 lastProcessedTime = 0;

    // Get current time in milliseconds
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    // If we've just processed items from this scene (within 100ms), don't create another command
    if (lastProcessedScene == scene() && (currentTime - lastProcessedTime) < 100) {
        return;
    }

    // Create lists for the move command
    QList<QGraphicsItem*> items;
    QList<QPointF> oldPositions;
    QList<QPointF> newPositions;

    // For each selected item
    foreach (QGraphicsItem* item, selectedItems) {
        Furniture* furniture = dynamic_cast<Furniture*>(item);
        if (furniture) {
            items.append(furniture);
            oldPositions.append(furniture->originalPos);
            newPositions.append(furniture->pos());
        }
    }

    // Create a single command for all items
    if (!items.isEmpty()) {
        MoveCommand *groupCommand = new MoveCommand(items, oldPositions, newPositions);
        undoStack->push(groupCommand);

        // Remember that we've processed this scene
        lastProcessedScene = scene();
        lastProcessedTime = currentTime;
    }


}

void Furniture::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // Create context menu for rotation
    QMenu menu;
    QAction *rotateAction = menu.addAction("Rotate");
    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == rotateAction) {
        tryRotate(90);
    }

}

