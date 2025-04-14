#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Decorate your House!");
    resize(1000,800);

    //setup UI
    setupUI();

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

    layout->addWidget(view);
    setCentralWidget(central);


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
