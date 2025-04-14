
#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include "command.h"
#include <QGraphicsScene>
#include <QGraphicsItem>

// Command for adding items to the scene
class AddCommand : public Command
{
public:
    AddCommand(QGraphicsScene *scene, QGraphicsItem *item);
    AddCommand(QGraphicsScene *scene, const QList<QGraphicsItem*> &itemList);
    ~AddCommand();

    void execute() override;
    void undo() override;

private:
    QGraphicsScene *scene;
    QList<QGraphicsItem*> items;
    bool itemsAdded;
};

#endif // ADDCOMMAND_H
