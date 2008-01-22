#include <math.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "BasicLayer.h"
using namespace H;

#include "Utils.h"
#include "CellGraphicsItem.h"
#include "SegmentFigure.h"

SegmentGraphicsItem::SegmentGraphicsItem(CellGraphicsItem* master, Segment* s):
    QGraphicsItem(master),
    segment(s)
{
    setFlag(ItemIsMovable);
}

QRectF SegmentGraphicsItem::boundingRect() const {
    Box box = segment->GetBoundingBox();
    QRectF rect;
    boxToRectangle(box, rect);
    return rect;
}

void SegmentGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	//if (option->levelOfDetail > 1.0) {
		painter->setClipRect(option->exposedRect);
		BasicLayer* layer = dynamic_cast<BasicLayer*>(segment->GetLayer()); 
		if (layer) {
			painter->setBrush(QColor(layer->GetRedValue(), layer->GetGreenValue(), layer->GetBlueValue()));
		} else {
			painter->setBrush(Qt::blue);
		}
		Box box = segment->GetBoundingBox();
		QRectF rect;
		boxToRectangle(box, rect);
		painter->drawRect(rect);
	//}
}


void SegmentGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mousePressEvent(event);
}

void SegmentGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
