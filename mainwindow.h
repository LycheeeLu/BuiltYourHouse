#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>
#include <QStack>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>


#include "housescene.h"
#include "wall.h"
#include "furniture.h"

#include "command.h"
#include "movecommand.h"
#include "addcommand.h"
#include "deletecommand.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //adding QGraphicsView and QGraphicsScene (houseScene is its subclass)
    // to create a drawing canvas
    QGraphicsView *view;
    HouseScene *scene;

    Ui::MainWindow *ui;
    // House creation buttons
    QPushButton *smallHouseBtn;
    QPushButton *mediumHouseBtn;
    QPushButton *largeHouseBtn;


    // Furniture buttons
    QPushButton *sofaBtn;
    QPushButton *chairBtn;
    QPushButton *tableBtn;
    QPushButton *wallBtn;

    // Wall drawing
    QPointF wallStartPoint;

    // Mode tracking
    enum Mode { Normal, DrawingWall, PlacingFurniture };
    Mode currentMode;
    QString furnitureToAdd;


    // Clipboard for copy-paste
    QList<QGraphicsItem*> clipboardItems;

    // Command stacks for undo-redo
    QStack<Command*> undoStack;
    QStack<Command*> redoStack;


    void setupUI();
    void createMenus();

    // Event handlers for scene
    bool eventFilter(QObject *obj, QEvent *event) override;
    void handleMousePress(QGraphicsSceneMouseEvent *event);
    void handleMouseMove(QGraphicsSceneMouseEvent *event);
    void handleMouseRelease(QGraphicsSceneMouseEvent *event);


    // Helper methods
    void executeCommand(Command* command);
    QList<QGraphicsItem*> getSelectedItems();


private slots:
    void setSmallHouse();
    void setMediumHouse();
    void setLargeHouse();


    void addSofa();
    void addChair();
    void addTable();
    void startDrawingWall();


    void cutSelection();
    void copySelection();
    void pasteSelection();
    void deleteSelection();
    void selectAll();

    void undo();
    void redo();


};
#endif // MAINWINDOW_H
