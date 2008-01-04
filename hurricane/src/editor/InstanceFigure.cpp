#include <QPainter>
#include <QTransform>
#include <QStyleOptionGraphicsItem>

#include "Cell.h"
#include "Slice.h"
#include "Segment.h"
#include "BasicLayer.h"
using namespace H;

#include "Utils.h"
#include "InstanceFigure.h"

//InstanceFigure::InstanceFigure(Instance* inst)
//    : instance(inst),
//    cell(instance->GetMasterCell()) 
//{
//    Transformation transformation = instance->GetTransformation();
//    QTransform transform;
//    QPoint pos;
//    HurricanePositionToQtPosition(transformation, transform, pos);
//    setTransform(transform);
//    setPos(pos);
//    constructSubInstances();
//}

InstanceFigure::InstanceFigure(Cell* c)
    : instance(NULL),
    cell(c)
{
    constructSubInstances();
}

InstanceFigure::InstanceFigure(InstanceFigure* parent, Instance* inst)
    : QGraphicsItem(parent),
    instance(inst),
    cell(instance->GetMasterCell())
{
    Transformation transformation = instance->GetTransformation();
    QTransform transform;
    QPoint pos;
    HurricanePositionToQtPosition(transformation, transform, pos);
    setTransform(transform);
    setPos(pos);
    constructSubInstances();
}

void InstanceFigure::constructSubInstances() {
    for_each_instance(instance, cell->GetInstances()) {
        new InstanceFigure(this, instance);
        end_for;
    }
}

QRectF InstanceFigure::boundingRect() const {
    Box box = cell->GetAbutmentBox();
    QRectF rect;
    BoxToRectangle(box, rect);
    return rect;
}

void InstanceFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setClipRect(option->exposedRect.adjusted(-1.0, -1.0, 1.0, 1.0));
    if (option->levelOfDetail > 1.0) {
    	drawBoundary(painter);
    	drawElements(painter);
    } else {
    	drawPhantom(painter);
    }
}

void InstanceFigure::drawElements(QPainter* painter) {
    for_each_slice(slice, cell->GetSlices()) {
        painter->save();
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
        painter->restore();
        end_for;
    }
}

void InstanceFigure::drawBoundary(QPainter* painter) {
    QPen pen(Qt::black); 
    painter->setPen(pen);
    Box box = cell->GetAbutmentBox();
    QRectF rect;
    BoxToRectangle(box, rect);
    painter->drawRect(rect);
}

void InstanceFigure::drawPhantom(QPainter* painter) {
    painter->setBrush(Qt::red);
    Box box = cell->GetAbutmentBox();
    QRectF rect;
    BoxToRectangle(box, rect);
    painter->drawRect(rect);
}
