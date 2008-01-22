#ifndef __CELL_GRAPHICS_ITEM_H
#define __CELL_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QObject>

#include "Cell.h"
using namespace Hurricane;

class InstanceGraphicsItem;

class CellGraphicsItem : public QGraphicsItem {
    public:
        CellGraphicsItem(Cell* cell);
        CellGraphicsItem(InstanceGraphicsItem* master, Cell* cell);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    private:
        Cell* cell;
        void drawBoundary(QPainter* painter);
        void drawPhantom(QPainter* painter);
};

#endif /* __CELL_GRAPHICS_ITEM_H */
