#include "deletecommand.h"


DeleteCommand::DeleteCommand(QGraphicsScene *scene, QGraphicsItem *item)
    : scene(scene), itemsRemoved(false)
{
    items.append(item);
    positions.append(item->pos());
}

DeleteCommand::DeleteCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &itemList)
    : scene(scene), itemsRemoved(false)
{
    foreach (QGraphicsItem *item, itemList) {
        items.append(item);
        positions.append(item->pos());
    }
}

DeleteCommand::~DeleteCommand()
{
    if (itemsRemoved) {
        // Clean up items if they are not in the scene
        qDeleteAll(items);
    }
}

void DeleteCommand::execute()
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
