#ifndef __INSTANCE_GRAPHICS_ITEM_H
#define __INSTANCE_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QObject>

#include "Instance.h"
#include "Cell.h"
using namespace Hurricane;

class InstanceGraphicsItem : public QGraphicsItem {
    public:
        //InstanceGraphicsItem(InstanceGraphicsItem* parent, Instance* instance);
        InstanceGraphicsItem(Cell* cell);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    private:
        Instance* instance;
        Cell* cell;
	void drawPhantom(QPainter* painter);
	void drawBoundary(QPainter* painter);
	void drawElements(QPainter* painter);
        void constructSubInstances();

};

#endif /* __INSTANCE_GRAPHICS_ITEM_H */
