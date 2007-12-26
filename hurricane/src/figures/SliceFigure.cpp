#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "BasicLayer.h"
using namespace H;

#include "SegmentFigure.h"
#include "Utils.h"
#include "SliceFigure.h"

SliceFigure::SliceFigure(CellFigure* parent, Slice* sli):
    QGraphicsItem(parent),
    slice(sli)
{
//    constructGoFigures();
}

QRectF SliceFigure::boundingRect() const {
   Box box = slice->GetBoundingBox();
   return QRectF(box.GetXMin(), box.GetXMax(), box.GetYMin(), box.GetYMax());
}

void SliceFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	if (option->levelOfDetail > 1.0) {
//			painter->setClipRect(option->exposedRect);
			BasicLayer* layer = dynamic_cast<BasicLayer*>(slice->GetLayer()); 
			if (layer) {
				painter->setBrush(QColor(layer->GetRedValue(), layer->GetGreenValue(), layer->GetBlueValue()));
			} else {
				painter->setBrush(Qt::blue);
			}
			for_each_go(go, slice->GetGos()) {
				Segment* segment = dynamic_cast<Segment*>(go);
				if (segment) {
					Box box = segment->GetBoundingBox();
					QRectF rect;
					BoxToRectangle(box, rect);
					painter->drawRect(rect);
				}
				end_for;
			}
	}
}

void SliceFigure::constructGoFigures() {
    for_each_go(go, slice->GetGos()) {
        Segment* segment = dynamic_cast<Segment*>(go);
        if (segment) {
            new SegmentFigure(this, segment);
        }
        end_for;
    }
}
