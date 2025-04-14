#include "addcommand.h"


AddCommand::AddCommand(QGraphicsScene *scene, QGraphicsItem *item)
    : scene(scene), itemsAdded(false)
{
     items.append(item);
}

AddCommand::AddCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &itemList)
    :  scene(scene), items(itemList), itemsAdded(false)
{

}

AddCommand::~AddCommand()
{
    if (!itemsAdded) {
        // Clean up items if they were never added to the scene
        qDeleteAll(items);
    }

}

void AddCommand::execute()
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
