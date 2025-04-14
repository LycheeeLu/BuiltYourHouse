#ifndef COMMAND_H
#define COMMAND_H
#include <QGraphicsScene>
#include <QGraphicsItem>

// Base command class for undo/redo functionality
class Command
{
public:
    Command();
    virtual ~Command() {}

    virtual void execute() = 0;
    virtual void undo() = 0;
};

#endif // COMMAND_H
