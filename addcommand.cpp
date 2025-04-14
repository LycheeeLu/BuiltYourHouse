#include "addcommand.h"

AddCommand::AddCommand(QGraphicsScene *s, QGraphicsItem *item, QUndoCommand *parent)
    : QUndoCommand(parent), scene(s), itemsAdded(false)
{
    items.append(item);
    setText("Add Item");
}

AddCommand::AddCommand(QGraphicsScene *s, const QList<QGraphicsItem*> &itemList, QUndoCommand *parent)
    : QUndoCommand(parent), scene(s), items(itemList), itemsAdded(false)
{
    setText(QString("Add %1 Items").arg(items.size()));
}

AddCommand::~AddCommand()
{
    if (!itemsAdded) {
        // Clean up items if they were never added to the scene
        qDeleteAll(items);
    }
}

void AddCommand::redo()
{
    foreach (QGraphicsItem *item, items) {
        scene->addItem(item);
    }
    itemsAdded = true;
}

void AddCommand::undo()
{
    foreach (QGraphicsItem *item, items) {
        scene->removeItem(item);
    }
    itemsAdded = false;
}
