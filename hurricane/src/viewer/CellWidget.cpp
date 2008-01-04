#include <math.h>

#include <QPaintEvent>
#include <QPainter>

#include "DataBase.h"
#include "Technology.h"
#include "Box.h"
using namespace H;

#include "CellWidget.h"

namespace {

Technology* getTechnology() {
    DataBase* database = GetDataBase();
    if (database) {
        return database->GetTechnology();
    }
    return NULL;
}

CellWidget::CellWidget(Cell* c, QWidget* parent)
    : QAbstractScrollArea(parent),
    cell(c) {
}

void CellWidget::paintEvent(QPaintEvent* event) {
    //invalidate(event->rect());
    redraw();
}

void CellWidget::redraw() {
    drawPhantoms(cell);
    drawBoundaries(cell);
    for_each_layer(layer, getTechnology()->GetLayers()) {
        drawCell(cell, layer);
        end_for;
    }
}

void CellWidget::drawCell(const Cell* cell, const Layer* layer) const {
}

void CellWidget::drawPhantoms(const Cell* cell) const {
}

void CellWidget::drawBoundaries(const Cell* cell) const {
}

void CellWidget::drawRectangle(const H::Box& box) {
    if (painter) {
        H::Box ibox = box.getIntersection(clipBox);
        if (!ibox.isEmpty()) {
            int x = getScreenX(ibox.getXMin());
            int y = getScreenY(ibox.getYMax());
            int w = getScreenX(ibox.getXMax()) - x + 1;
            int h = getScreenY(ibox.getYMin()) - y + 1;

            painter->drawRect(x, y, w, h);
        }
    }
}

int CellWidget::getScreenX(const Unit& x) const {
    return screenDx + (int)rint(GetValue(x) * scale);
}

int CellWidget::getScreenY(const Unit& y) const {
    return height() - (int)rint(screenDy + (GetValue(y) * scale));
}

}
