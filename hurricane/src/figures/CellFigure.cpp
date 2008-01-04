#include <math.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "InstanceFigure.h"
#include "SliceFigure.h"

#include "Utils.h"
#include "CellFigure.h"

CellFigure::CellFigure(Cell* c):
	QGraphicsItem(),
	cell(c) {
	QTransform transform;
	transform.setMatrix(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
	setTransform(transform);
	for_each_instance(instance, cell->GetInstances()) {
		new InstanceFigure(this, instance);
		end_for;
	}
	//for_each_slice(slice, cell->GetSlices()) {
	//	new SliceFigure(this, slice);
	//	end_for;
	//}
}

CellFigure::CellFigure(InstanceFigure* master, Cell* c) : QGraphicsItem(master), cell(c) {
	for_each_instance(instance, cell->GetInstances()) {
		new InstanceFigure(this, instance);
		end_for;
	}
	for_each_slice(slice, cell->GetSlices()) {
		new SliceFigure(this, slice);
		end_for;
	}
} 

QRectF CellFigure::boundingRect() const {
	Box box = cell->GetBoundingBox();
	QRectF rect;
	BoxToRectangle(box, rect);
	rect = transform().mapRect(rect);
	return rect;
}

void CellFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *) {
	//	QPen pen(Qt::blue);
	//	pen.setWidth(10);
	//	painter->setPen(pen);
	//	painter->drawLine(0, 0, 50, 0);
	//
	//	painter->drawLine(0, 0, 0, 50);
	//	pen.setColor(Qt::red);
	//	painter->setPen(pen);

	//painter->setClipRect(option->exposedRect);
	//if (option->levelOfDetail > 1.0) {
	//	drawBoundary(painter);
	//} else {
	//	drawPhantom(painter);
	//}
}

void CellFigure::drawBoundary(QPainter* painter) {
	QPen pen(Qt::black); 
	painter->setPen(pen);
	Box box = cell->GetAbutmentBox();
	QRectF rect;
	BoxToRectangle(box, rect);
	painter->drawRect(rect);
}

void CellFigure::drawPhantom(QPainter* painter) {
	painter->setBrush(Qt::red);
	Box box = cell->GetAbutmentBox();
	QRectF rect;
	BoxToRectangle(box, rect);
	painter->drawRect(rect);
}
