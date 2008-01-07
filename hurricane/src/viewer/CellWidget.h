#ifndef __CELL_WIDGET_H
#define __CELL_WIDGET_H

#include <QAbstractScrollArea>

#include "Box.h"
#include "Cell.h"
using namespace H;


class CellWidget : public QAbstractScrollArea {
    public:
        CellWidget(Cell* cell, QWidget* parent=0);
        void redraw();
    protected:
        void paintEvent(QPaintEvent* event);
    private:
        Cell* cell;
        QRegion invalidRegion;
        H::Box clipBox;
        QPainter* painter;
        double scale;
        int screenDx;
	int screenDy;

        void drawCell(const Cell* cell, const Layer* layer, const H::Box& updateArea, const Transformation& transformation) const;
        void drawPhantoms(const Cell* cell, const H::Box& updateArea, const Transformation& transformation) const;
        void drawPhantoms(const Instance* instance, const H::Box& updateArea, const Transformation& transformation) const;
        void drawBoundaries(const Cell* cell, const H::Box& updateArea, const Transformation& transformation) const;
        void drawBoundaries(const Instance* instance, const H::Box& updateArea, const Transformation& transformation) const;
        void drawRectangle(const H::Box& box) const;
        Unit getX(int screenX) const;
        Unit getY(int screenY) const;
        Unit getSize(int screenSize) const;
        H::Box getBox(const QRect& screenRect) const;
        int getScreenX(const Unit& x) const;
        int getScreenY(const Unit& y) const;
        void invalidate();
        void invalidate(const QRect& screenRect);
};


#endif /* __CELL_WIDGET_H */
