#ifndef __CELL_FIGURE_H
#define __CELL_FIGURE_H

#include <QGraphicsItem>
#include <QObject>

#include "Cell.h"
using namespace Hurricane;

class InstanceFigure;

class CellFigure : public QGraphicsItem {
    public:
        CellFigure(Cell* cell);
        CellFigure(InstanceFigure* master, Cell* cell);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    private:
        Cell* cell;
        void drawBoundary(QPainter* painter);
        void drawPhantom(QPainter* painter);
};

#endif /* __CELL_FIGURE_H */
