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
    case Medium:
        houseSize = QSize(600, 600);
    case Large:
        houseSize = QSize(800, 600);
        break;
    }

    setSceneRect(0, 0, houseSize.width(), houseSize.height());
}
