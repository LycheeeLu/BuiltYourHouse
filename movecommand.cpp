#include "movecommand.h"



MoveCommand::MoveCommand(QGraphicsItem *item, const QPointF &oldPos, const QPointF &newPos)
{
    items.append(item);
    oldPositions.append(oldPos);
    newPositions.append(newPos);

}

MoveCommand::MoveCommand(const QList<QGraphicsItem *> &itemList, const QList<QPointF> &oldPosList, const QList<QPointF> &newPosList)
 : items(itemList), oldPositions(oldPosList), newPositions(newPosList)
{

}

MoveCommand::~MoveCommand()
{

}

void MoveCommand::execute()
{
    for (int i = 0; i < items.size(); ++i) {
        items[i]->setPos(newPositions[i]);
    }

}

void MoveCommand::undo()
{
    for (int i = 0; i < items.size(); ++i) {
        items[i]->setPos(oldPositions[i]);
    }

}
