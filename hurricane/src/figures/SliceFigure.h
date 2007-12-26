#ifndef __SLICE_FIGURE_H
#define __SLICE_FIGURE_H

#include <QGraphicsItem>
#include <QObject>

#include "CellFigure.h"

#include "Slice.h"
using namespace Hurricane;

class SliceFigure : public QGraphicsItem {
    public:
        SliceFigure(CellFigure* parent, Slice* slice);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    private:
        Slice* slice;
        void constructGoFigures();
};

#endif /* __SLICE_FIGURE_H */
