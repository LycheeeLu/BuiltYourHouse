#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), currentMode(Normal)
{
    ui->setupUi(this);

    setWindowTitle("Decorate your House!");
    resize(1000,800);

    //setup UI
    setupUI();
    createMenus();

    //set up a default medium house
    HouseScene();

    // Setup event filter for the scene
    scene->installEventFilter(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupUI(){
    // Create central widget and main layout
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    //Canvas
    view = new QGraphicsView(this);
    scene = new HouseScene(this);
    scene -> setHouseSize(HouseScene::Medium);
    view-> setScene (scene);


    // Create house size buttons
    QGroupBox *houseGroup = new QGroupBox("House Size");
    QHBoxLayout *houseLayout = new QHBoxLayout(houseGroup);

    smallHouseBtn = new QPushButton("Small (300x300)");
    mediumHouseBtn = new QPushButton("Medium (600x600)");
    largeHouseBtn = new QPushButton("Large (800x600)");

    houseLayout->addWidget(smallHouseBtn);
    houseLayout->addWidget(mediumHouseBtn);
    houseLayout->addWidget(largeHouseBtn);

    // Connect buttons
    connect(smallHouseBtn, &QPushButton::clicked, this, &MainWindow::setSmallHouse);
    connect(mediumHouseBtn, &QPushButton::clicked, this, &MainWindow::setMediumHouse);
    connect(largeHouseBtn, &QPushButton::clicked, this, &MainWindow::setLargeHouse);

    // Add widgets to  layout
    layout->addWidget(houseGroup);



    //Create add furniture buttons and adding the group to layout
    QGroupBox *furnitureGroup = new QGroupBox("Add Furniture");
    QHBoxLayout *furnitureLayout = new QHBoxLayout(furnitureGroup);
    sofaBtn = new QPushButton("Sofa (60x20)");
    chairBtn = new QPushButton("Chair (30x30)");
    tableBtn = new QPushButton("Table (30x30)");
    wallBtn = new QPushButton("Draw Wall");
    furnitureLayout->addWidget(sofaBtn);
    furnitureLayout->addWidget(chairBtn);
    furnitureLayout->addWidget(tableBtn);
    furnitureLayout->addWidget(wallBtn);
    layout->addWidget(furnitureGroup);
    connect(sofaBtn, &QPushButton::clicked, this, &MainWindow::addSofa);
    connect(chairBtn, &QPushButton::clicked, this, &MainWindow::addChair);
    connect(tableBtn, &QPushButton::clicked, this, &MainWindow::addTable);
    connect(wallBtn, &QPushButton::clicked, this, &MainWindow::startDrawingWall);




    layout->addWidget(view);
    // Set central widget
    setCentralWidget(central);

}

void MainWindow::createMenus(){

    //edit menus
    QMenu *editMenu = menuBar()->addMenu("&Edit");

    QAction *undoAction = editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    QAction *redoAction = editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

    editMenu->addSeparator();

    QAction *cutAction = editMenu->addAction("Cu&t");
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, this, &MainWindow::cutSelection);

    QAction *copyAction = editMenu->addAction("&Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copySelection);

    QAction *pasteAction = editMenu->addAction("&Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::pasteSelection);

    editMenu->addSeparator();

    QAction *deleteAction = editMenu->addAction("&Delete");
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelection);

    QAction *selectAllAction = editMenu->addAction("Select All");
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, &QAction::triggered, this, &MainWindow::selectAll);

    // Create toolbar
    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->addAction(undoAction);
    toolBar->addAction(redoAction);
    toolBar->addSeparator();
    toolBar->addAction(cutAction);
    toolBar->addAction(copyAction);
    toolBar->addAction(pasteAction);
    toolBar->addAction(deleteAction);


}

//implementing slots for setting house canvas size
void MainWindow::setSmallHouse()
{
    scene-> setHouseSize(HouseScene::Small);
}
void MainWindow::setMediumHouse() {
    scene->setHouseSize(HouseScene::Medium);
}

void MainWindow::setLargeHouse() {
    scene->setHouseSize(HouseScene::Large);
}

//implementing slots for adding furnitures, including walls
void MainWindow::addSofa()
{
    currentMode = PlacingFurniture;
    furnitureToAdd = "Sofa";
    statusBar()->showMessage("Click to place a sofa");
}

void MainWindow::addChair()
{
    currentMode = PlacingFurniture;
    furnitureToAdd = "Chair";
    statusBar()->showMessage("Click to place a chair");
}

void MainWindow::addTable()
{
    currentMode = PlacingFurniture;
    furnitureToAdd = "Table";
    statusBar()->showMessage("Click to place a table");
}

void MainWindow::startDrawingWall()
{
    currentMode = DrawingWall;
    statusBar()->showMessage("Click and drag to draw a wall");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == scene) {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            handleMousePress(static_cast<QGraphicsSceneMouseEvent*>(event));
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            handleMouseMove(static_cast<QGraphicsSceneMouseEvent*>(event));
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            handleMouseRelease(static_cast<QGraphicsSceneMouseEvent*>(event));
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::handleMousePress(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();

    // Check if position is within scene bounds
    if (!scene->sceneRect().contains(pos)) {
        return;
    }

    if (currentMode == DrawingWall) {
        wallStartPoint = pos;
    }
    else if (currentMode == PlacingFurniture) {
        Furniture *furniture = nullptr;

        if (furnitureToAdd == "Sofa") {
            furniture = new Furniture(60, 20, Furniture::Sofa);
        }
        else if (furnitureToAdd == "Chair") {
            furniture = new Furniture(30, 30, Furniture::Chair);
        }
        else if (furnitureToAdd == "Table") {
            furniture = new Furniture(30, 30, Furniture::Table);
        }

        if (furniture) {
            furniture->setPos(pos);

            // Check for collisions before adding
            scene->addItem(furniture);
            if (furniture->collidesWithFurnitureOrWalls()) {
                scene->removeItem(furniture);
                delete furniture;
                statusBar()->showMessage("Cannot place furniture: collision detected");
            }
            else {
                // Create and execute add command
                AddCommand *command = new AddCommand(scene, furniture);
                executeCommand(command);

                statusBar()->showMessage("Furniture added");
                currentMode = Normal;
            }
        }
    }
}

void MainWindow::handleMouseMove(QGraphicsSceneMouseEvent *event)
{
    // Only needed for wall drawing
    if (currentMode == DrawingWall) {
        // Could add preview wall here if needed
    }
}

void MainWindow::handleMouseRelease(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();

    if (currentMode == DrawingWall) {
        // Determine if wall is horizontal or vertical
        QPointF start = wallStartPoint;
        QPointF end = pos;

        // Calculate angle to determine if closer to horizontal or vertical
        qreal dx = end.x() - start.x();
        qreal dy = end.y() - start.y();

        // Snap to horizontal or vertical based on angle
        if (qAbs(dx) > qAbs(dy)) {
            // Horizontal wall
            end.setY(start.y());
        } else {
            // Vertical wall
            end.setX(start.x());
        }

        // Create wall if it has some length
        qreal length = QLineF(start, end).length();
        if (length > 5) {
            Wall *wall = new Wall(start, end);
            scene->addItem(wall);

            // Create and execute add command
            AddCommand *command = new AddCommand(scene, wall);
            executeCommand(command);

            statusBar()->showMessage("Wall added");
        }

        currentMode = Normal;
    }
}

void MainWindow::executeCommand(Command *command)
{
    command->execute();
    undoStack.push(command);

    // Clear redo stack
    qDeleteAll(redoStack);
    redoStack.clear();


}

QList<QGraphicsItem *> MainWindow::getSelectedItems()
{
    QList<QGraphicsItem*> items;
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (dynamic_cast<Furniture*>(item)) {
            items.append(item);
        }
    }
    return items;

}

void MainWindow::undo()
{
    if (!undoStack.isEmpty()) {
        Command *command = undoStack.pop();
        command->undo();
        redoStack.push(command);
        statusBar()->showMessage("Undo");
    } else {
        statusBar()->showMessage("Nothing to undo");
    }
}

void MainWindow::redo()
{
    if (!redoStack.isEmpty()) {
        Command *command = redoStack.pop();
        command->execute();
        undoStack.push(command);
        statusBar()->showMessage("Redo");
    } else {
        statusBar()->showMessage("Nothing to redo");
    }
}

void MainWindow::cutSelection()
{
    copySelection();
    deleteSelection();
}

void MainWindow::copySelection()
{
    clipboardItems.clear();

    foreach (QGraphicsItem *item, getSelectedItems()) {
        if (Furniture *furniture = dynamic_cast<Furniture*>(item)) {
            clipboardItems.append(furniture);
        }
    }

    if (!clipboardItems.isEmpty()) {
        statusBar()->showMessage(QString("%1 item(s) copied").arg(clipboardItems.size()));
    } else {
        statusBar()->showMessage("No furniture selected to copy");
    }
}

void MainWindow::pasteSelection()
{
    if (clipboardItems.isEmpty()) {
        statusBar()->showMessage("Nothing to paste");
        return;
    }

    // Deselect all items
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        item->setSelected(false);
    }

    QList<QGraphicsItem*> newItems;

    // Create copies of clipboard items with slight offset
    foreach (QGraphicsItem *item, clipboardItems) {
        if (Furniture *furniture = dynamic_cast<Furniture*>(item)) {
            Furniture *copy = new Furniture(*furniture); // Use copy constructor
            copy->moveBy(20, 20); // Offset by small amount
            scene->addItem(copy);
            copy->setSelected(true);
            newItems.append(copy);

            // Check for collisions
            if (copy->collidesWithFurnitureOrWalls()) {
                // Try different positions
                bool foundPosition = false;
                for (int offsetX = 30; offsetX <= 100; offsetX += 20) {
                    for (int offsetY = 30; offsetY <= 100; offsetY += 20) {
                        copy->setPos(furniture->pos() + QPointF(offsetX, offsetY));
                        if (!copy->collidesWithFurnitureOrWalls()) {
                            foundPosition = true;
                            break;
                        }
                    }
                    if (foundPosition) break;
                }

                // If still colliding, remove it
                if (!foundPosition) {
                    scene->removeItem(copy);
                    delete copy;
                    newItems.removeLast();
                }
            }
        }
    }

    if (!newItems.isEmpty()) {
        // Create add command for all new items
        AddCommand *command = new AddCommand(scene, newItems);
        executeCommand(command);
        statusBar()->showMessage(QString("%1 item(s) pasted").arg(newItems.size()));
    } else {
        statusBar()->showMessage("Could not paste items due to collisions");
    }
}

void MainWindow::deleteSelection()
{
    QList<QGraphicsItem*> items = getSelectedItems();

    if (items.isEmpty()) {
        statusBar()->showMessage("No furniture selected to delete");
        return;
    }

    DeleteCommand *command = new DeleteCommand(scene, items);
    executeCommand(command);

    statusBar()->showMessage(QString("%1 item(s) deleted").arg(items.size()));
}

void MainWindow::selectAll()
{
    // Select all furniture items
    foreach (QGraphicsItem *item, scene->items()) {
        if (dynamic_cast<Furniture*>(item)) {
            item->setSelected(true);
        }
    }

    statusBar()->showMessage("All furniture selected");
}

