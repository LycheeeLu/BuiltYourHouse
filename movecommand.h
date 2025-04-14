#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "command.h"
#include <QGraphicsItem>

class MoveCommand : public Command
{
public:
    MoveCommand(QGraphicsItem *item, const QPointF &oldPos, const QPointF &newPos);
    MoveCommand(const QList<QGraphicsItem*> &itemList, const QList<QPointF> &oldPosList, const QList<QPointF> &newPosList);
    ~MoveCommand();

    void execute() override;
    void undo() override;
private:
    QList<QGraphicsItem*> items;
    QList<QPointF> oldPositions;
    QList<QPointF> newPositions;
};

#endif // MOVECOMMAND_H
