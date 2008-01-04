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
        QPainter* painter;
        H::Box clipBox;
        double scale;
        int screenDx;
	int screenDy;

        void drawCell(const Cell* cell, const Layer* layer) const;
        void drawPhantoms(const Cell* cell) const;
        void drawBoundaries(const Cell* cell) const;
        void drawRectangle(const H::Box& box);
        int getScreenX(const Unit& x) const;
        int getScreenY(const Unit& y) const;
};


#endif /* __CELL_WIDGET_H */
