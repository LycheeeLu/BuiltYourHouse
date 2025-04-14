#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

// Command for deleting items from the scene
class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr);
    DeleteCommand(QGraphicsScene *scene, const QList<QGraphicsItem*> &items, QUndoCommand *parent = nullptr);
    ~DeleteCommand();

    void undo() override;
    void redo() override;

private:
    QGraphicsScene *scene;
    QList<QGraphicsItem*> items;
    QList<QPointF> positions;
    bool itemsRemoved;
};

#endif
