#include <math.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "Slice.h"
using namespace H;

#include "Utils.h"
#include "InstanceGraphicsItem.h"
#include "SegmentFigure.h"
#include "CellGraphicsItem.h"

CellGraphicsItem::CellGraphicsItem(Cell* c):
	QGraphicsItem(),
	cell(c) {
	//QTransform transform;
	//transform.setMatrix(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
	//setTransform(transform);
	//for_each_instance(instance, cell->GetInstances()) {
	//	new InstanceGraphicsItem(this, instance);
	//	end_for;
	//}

        unsigned zValue = 10;
	for_each_slice(slice, cell->GetSlices()) {
            for_each_go(go, slice->GetGos()) {
                Segment* segment = dynamic_cast<Segment*>(go);
                if (segment) {
                    SegmentGraphicsItem* segmentItem = new SegmentGraphicsItem(this, segment);
                    segmentItem->setZValue(zValue);
                }
                end_for;
            }
            zValue += 10;
            end_for;
        }
}

CellGraphicsItem::CellGraphicsItem(InstanceGraphicsItem* master, Cell* c) : QGraphicsItem(master), cell(c) {
	//for_each_instance(instance, cell->GetInstances()) {
	//	new InstanceGraphicsItem(this, instance);
	//	end_for;
	//}
	//for_each_slice(slice, cell->GetSlices()) {
	//	new SliceFigure(this, slice);
	//	end_for;
	//}
} 

QRectF CellGraphicsItem::boundingRect() const {
	Box box = cell->GetBoundingBox();
	QRectF rect;
	boxToRectangle(box, rect);
	//rect = transform().mapRect(rect);
	return rect;
}

void CellGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *) {
	painter->setClipRect(option->exposedRect);
	drawBoundary(painter);
	//drawPhantom(painter);
}

void CellGraphicsItem::drawBoundary(QPainter* painter) {
	QPen pen(Qt::black); 
	painter->setPen(pen);
	Box box = cell->GetAbutmentBox();
	QRectF rect;
	boxToRectangle(box, rect);
	painter->drawRect(rect);
}

void CellGraphicsItem::drawPhantom(QPainter* painter) {
	painter->setBrush(Qt::red);
	Box box = cell->GetAbutmentBox();
	QRectF rect;
	boxToRectangle(box, rect);
	painter->drawRect(rect);
}
