#include <QPainter>
#include <QTransform>

#include "Utils.h"
#include "InstanceFigure.h"

InstanceFigure::InstanceFigure(CellFigure* parent, Instance* inst):
	QGraphicsItem(parent),
	instance(inst) {
	Cell* masterCell = inst->GetMasterCell();
	
	Transformation transformation = instance->GetTransformation();
	QTransform transform;
	QPoint pos;
	HurricanePositionToQtPosition(transformation, transform, pos);
	setTransform(transform);
	setPos(pos);
	new CellFigure(this, masterCell);
}

QRectF InstanceFigure::boundingRect() const {
	Cell* masterCell = instance->GetMasterCell();
	Box box = masterCell->GetAbutmentBox();
	QRectF rect;
	BoxToRectangle(box, rect);
	return rect;
}

void InstanceFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{}
