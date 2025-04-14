#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <QUndoCommand>
#include <QGraphicsItem>

// Command for moving furniture items
class MoveCommand : public QUndoCommand
{
public:
    MoveCommand(QGraphicsItem *item, const QPointF &oldPos, const QPointF &newPos, QUndoCommand *parent = nullptr);
    MoveCommand(const QList<QGraphicsItem*> &items, const QList<QPointF> &oldPositions, const QList<QPointF> &newPositions, QUndoCommand *parent = nullptr);
    ~MoveCommand();

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem*> items;
    QList<QPointF> oldPositions;
    QList<QPointF> newPositions;
};

#endif
