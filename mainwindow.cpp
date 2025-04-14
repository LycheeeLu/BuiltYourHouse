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
    // add file menu
    QMenu *fileMenu = menuBar()->addMenu("&File");

    //add Edit menu
    QMenu *editMenu = menuBar()->addMenu("&Edit");

    // Create toolbar
    QToolBar *toolBar = addToolBar("Main Toolbar");


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
