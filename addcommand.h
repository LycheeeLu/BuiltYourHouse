#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

// Command for adding items to the scene
class AddCommand : public QUndoCommand
{
public:
    AddCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr);
    AddCommand(QGraphicsScene *scene, const QList<QGraphicsItem*> &items, QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    QGraphicsScene *scene;
    QList<QGraphicsItem*> items;
    bool itemsAdded;
};

#endif
