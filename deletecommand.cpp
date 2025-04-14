#include "deletecommand.h"

DeleteCommand::DeleteCommand(QGraphicsScene *s, QGraphicsItem *item, QUndoCommand *parent)
    : QUndoCommand(parent), scene(s), itemsRemoved(false)
{
    items.append(item);
    positions.append(item->pos());
    setText("Delete Item");
}

DeleteCommand::DeleteCommand(QGraphicsScene *s, const QList<QGraphicsItem*> &itemList, QUndoCommand *parent)
    : QUndoCommand(parent), scene(s), itemsRemoved(false)
{
    foreach (QGraphicsItem *item, itemList) {
        items.append(item);
        positions.append(item->pos());
    }
    setText(QString("Delete %1 Items").arg(items.size()));
}

DeleteCommand::~DeleteCommand()
{
    if (itemsRemoved) {
        // Clean up items if they are not in the scene
        qDeleteAll(items);
    }
}

void DeleteCommand::redo()
{
    foreach (QGraphicsItem *item, items) {
        scene->removeItem(item);
    }
    itemsRemoved = true;
}

void DeleteCommand::undo()
{
    for (int i = 0; i < items.size(); ++i) {
        items[i]->setPos(positions[i]);
        scene->addItem(items[i]);
    }
    itemsRemoved = false;
}
