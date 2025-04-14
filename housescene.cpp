#include "housescene.h"



HouseScene::HouseScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setHouseSize(Medium);
    //default to medium canvas

}

void HouseScene::setHouseSize(SizeType sizetype)
{
    switch (sizetype){
    case Small:
        houseSize = QSize(300, 300);
        break;
    case Medium:
        houseSize = QSize(600, 600);
        break;
    case Large:
        houseSize = QSize(800, 600);
        break;
    }

    setSceneRect(0, 0, houseSize.width(), houseSize.height());


    // Clear previous items
    clear();

    // Draw yellow background under the house area
    QGraphicsRectItem *houseRect = addRect(0, 0, houseSize.width(), houseSize.height(),
                                           QPen(Qt::black), QBrush(QColor("#fff9cc")));
    houseRect->setZValue(-1);  // Make sure it's behind other items
    // Add a visible border (optional)
    addRect(sceneRect(), QPen(Qt::black));
}
