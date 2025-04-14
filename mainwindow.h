#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include <QMainWindow>
#include "housescene.h"

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



    void setupUI();
    void createMenus();



private slots:
    void setSmallHouse();
    void setMediumHouse();
    void setLargeHouse();

};
#endif // MAINWINDOW_H
