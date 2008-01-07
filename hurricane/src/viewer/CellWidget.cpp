#include <math.h>

#include <QPaintEvent>
#include <QPainter>
#include <QPen>

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

}

CellWidget::CellWidget(Cell* c, QWidget* parent)
    : QWidget(parent),
    cell(c),
    invalidRegion(),
    clipBox(),
    painter(NULL),
    scale(1),
    screenDx(0),
    screenDy(0) {
        painter = new QPainter();
}

void CellWidget::paintEvent(QPaintEvent* event) {
    invalidate(event->rect());
    redraw();
}

void CellWidget::invalidate() {
    invalidRegion = QRegion(rect());
}


void CellWidget::invalidate(const QRect& screenRect) {
    QRect visibleScreenRect = screenRect.intersect(rect());

    if (!visibleScreenRect.isEmpty()) {
        invalidRegion = invalidRegion.unite(QRegion(visibleScreenRect));
    }
}



void CellWidget::redraw() {
    if (!invalidRegion.isEmpty()) {
        QRect invalidRect = invalidRegion.boundingRect();

        H::Box area = getBox(invalidRect).inflate(getSize(1));

        QPaintDevice* device = this;

	QPainter newPainter(device);
	QPainter* oldPainter = painter;
	painter = &newPainter;

	painter->fillRect(QRectF(0, 0, 100, 100), QBrush(QColor(255, 255, 255)));

        painter->setPen(QPen(QColor(255, 255, 0)));
        painter->setBrush(QBrush(QColor(255, 255, 0)));

        drawPhantoms(cell, area, Transformation());
        drawBoundaries(cell, area, Transformation());
        for_each_layer(layer, getTechnology()->GetLayers()) {
            drawCell(cell, layer, area, Transformation());
            end_for;
        }

        invalidRegion = QRegion();
    }
}

void CellWidget::drawCell(const Cell* cell, const Layer* layer, const H::Box& updateArea, const Transformation& transformation) const {
}

void CellWidget::drawPhantoms(const Cell* cell, const H::Box& updateArea, const Transformation& transformation) const {
    for_each_instance(instance, cell->GetInstancesUnder(updateArea)) {
        drawPhantoms(instance, updateArea, transformation);
        end_for;
    }
}

void CellWidget::drawPhantoms(const Instance* instance, const H::Box& updateArea, const Transformation& transformation) const {
    H::Box masterArea = updateArea;
    Transformation masterTransformation = instance->GetTransformation();
    instance->GetTransformation().getInvert().applyOn(masterArea);
    transformation.applyOn(masterTransformation);
    drawPhantoms(instance->GetMasterCell(), masterArea, masterTransformation);
}

void CellWidget::drawBoundaries(const Cell* cell, const H::Box& updateArea, const Transformation& transformation) const {
    drawRectangle(cell->GetAbutmentBox());
    for_each_instance(instance, cell->GetInstances()) {
        drawBoundaries(instance, updateArea, transformation);
        end_for;
    }
}

void CellWidget::drawBoundaries(const Instance* instance, const H::Box& updateArea, const Transformation& transformation) const {
    H::Box masterArea = updateArea;
    Transformation masterTransformation = instance->GetTransformation();
    instance->GetTransformation().getInvert().applyOn(masterArea);
    transformation.applyOn(masterTransformation);
    drawBoundaries(instance->GetMasterCell(), masterArea, masterTransformation);
}

void CellWidget::drawRectangle(const H::Box& box) const {
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

Unit CellWidget::getX(int screenX) const {
    return GetUnit((screenX - screenDx) / scale);
}

Unit CellWidget::getY(int screenY) const {
    return GetUnit(((height() - screenY) - screenDy) / scale);
}

Unit CellWidget::getSize(int screenSize) const {
    return GetUnit(screenSize / scale);
}

H::Box CellWidget::getBox(const QRect& screenRect) const {
    if (screenRect.isEmpty()) {
        return H::Box();
    }

    return H::Box(getX(screenRect.left()),
            getY(screenRect.bottom()),
            getX(screenRect.right()),
            getY(screenRect.top()));
}

int CellWidget::getScreenX(const Unit& x) const {
    return screenDx + (int)rint(GetValue(x) * scale);
}

int CellWidget::getScreenY(const Unit& y) const {
    return height() - (int)rint(screenDy + (GetValue(y) * scale));
}
