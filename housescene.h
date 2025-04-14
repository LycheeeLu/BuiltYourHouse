#ifndef HOUSESCENE_H
#define HOUSESCENE_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
class HouseScene : public QGraphicsScene
{
    Q_OBJECT

public:
    HouseScene(QObject *parent = nullptr);

    enum SizeType{ Small, Medium, Large};
    void setHouseSize(SizeType sizetype);
private:
    QSize houseSize;

};

#endif // HOUSESCENE_H
