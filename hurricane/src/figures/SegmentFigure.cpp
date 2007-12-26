#include <math.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "BasicLayer.h"
using namespace H;

#include "Utils.h"
#include "SegmentFigure.h"

SegmentFigure::SegmentFigure(SliceFigure* master, Segment* s):
	GoFigure(master),
	segment(s)
	{} 

void SegmentFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	if (option->levelOfDetail < 1.0) {
		painter->setClipRect(option->exposedRect);
		BasicLayer* layer = dynamic_cast<BasicLayer*>(segment->GetLayer()); 
		if (layer) {
			painter->setBrush(QColor(layer->GetRedValue(), layer->GetGreenValue(), layer->GetBlueValue()));
		} else {
			painter->setBrush(Qt::blue);
		}
		Box box = segment->GetBoundingBox();
		QRectF rect;
		BoxToRectangle(box, rect);
		painter->drawRect(rect);
	}
}
