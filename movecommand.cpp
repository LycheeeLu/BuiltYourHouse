#include "movecommand.h"

MoveCommand::MoveCommand(QGraphicsItem *item, const QPointF &oldPos, const QPointF &newPos, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    items.append(item);
    oldPositions.append(oldPos);
    newPositions.append(newPos);
    setText("Move Item");
}

MoveCommand::MoveCommand(const QList<QGraphicsItem*> &itemList, const QList<QPointF> &oldPosList, const QList<QPointF> &newPosList, QUndoCommand *parent)
    : QUndoCommand(parent), items(itemList), oldPositions(oldPosList), newPositions(newPosList)
{
    setText(QString("Move %1 Items").arg(items.size()));
}

MoveCommand::~MoveCommand()
{
}

void MoveCommand::redo()
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
