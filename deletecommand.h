
#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include "command.h"
#include <QGraphicsScene>
#include <QGraphicsItem>

// Command for deleting items from the scene
class DeleteCommand : public Command
{
public:
    DeleteCommand(QGraphicsScene *scene, QGraphicsItem *item);
    DeleteCommand(QGraphicsScene *scene, const QList<QGraphicsItem*> &items);
    ~DeleteCommand();

    void execute() override;
    void undo() override;

private:
    QGraphicsScene *scene;
    QList<QGraphicsItem*> items;
    QList<QPointF> positions;
    bool itemsRemoved;
};

#endif // DELETECOMMAND_H
