#include <math.h>

#include <QBitmap>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "CompositeLayer.h"
#include "BasicLayer.h"
using namespace H;

#include "Utils.h"
#include "CellGraphicsItem.h"
#include "SegmentFigure.h"

namespace {

QBrush getBrush(const string &pattern, int redValue, int greenValue, int blueValue) {
    if (pattern == "FFFFFFFFFFFFFFFF") {
        return QBrush(QColor(redValue, greenValue, blueValue));
    } else {
        uchar bits[8];
        for (int i = 0; i < 8; i++) {
            int high = pattern[i * 2];
            if (('0' <= high) && (high <= '9')) {
                high = high - '0';
            } else {  
                if (('a' <= high) && (high <= 'f')) {
                    high = 10 + high - 'a';
                } else {  
                    if (('A' <= high) && (high <= 'F')) {
                        high = 10 + high - 'A';
                    } else {
                        high = '0';
                    }
                }
            }
            int low = pattern[(i * 2) + 1];
            if (('0' <= low) && (low <= '9')) {
                low = low - '0';
            } else {  
                if (('a' <= low) && (low <= 'f')) {
                    low = 10 + low - 'a';
                } else {  
                    if (('A' <= low) && (low <= 'F')) {
                        low = 10 + low - 'A';
                    } else {
                        low = '0';
                    }
                }
            }
            bits[i] = (uchar)((high * 16) + low); 
        }
        return QBrush(QColor(redValue, greenValue, blueValue), QBitmap::fromData(QSize(8,8), bits, QImage::Format_Mono));
    }
}

}

SegmentGraphicsItem::SegmentGraphicsItem(CellGraphicsItem* master, Segment* s):
    QGraphicsItem(master),
    segment(s)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);
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
    Box box = segment->GetBoundingBox();
    QRectF rect;
    boxToRectangle(box, rect);
    BasicLayer* blayer = dynamic_cast<BasicLayer*>(segment->GetLayer()); 
    if (blayer) {
        QColor layerColor(blayer->getRedValue(),
            blayer->getGreenValue(),
            blayer->getBlueValue());
        QBrush brush(layerColor);
        brush.setStyle(Qt::Dense1Pattern);
        painter->setBrush(brush);
        //painter->setBrush(getBrush(blayer->GetFillPattern(),
        //            blayer->GetRedValue(),
        //            blayer->GetGreenValue(),
        //            blayer->GetBlueValue()));
        painter->setPen(layerColor);
        painter->drawRect(rect);
    } else {
        CompositeLayer* clayer = dynamic_cast<CompositeLayer*>(segment->GetLayer());
        if (clayer) {
            for_each_basic_layer(basiclayer, clayer->getBasicLayers()) {
                QColor layerColor(basiclayer->getRedValue(),
                            basiclayer->getGreenValue(),
                            basiclayer->getBlueValue());
                QBrush brush(layerColor);
                brush.setStyle(Qt::Dense1Pattern);
                painter->setBrush(brush);
                //painter->setBrush(getBrush(basiclayer->GetFillPattern(),
                //            basiclayer->GetRedValue(),
                //            basiclayer->GetGreenValue(),
                //            basiclayer->GetBlueValue()));
                painter->setPen(layerColor);
                painter->drawRect(rect);
                end_for;
            }
        }
    }
}

void SegmentGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mousePressEvent(event);
}

void SegmentGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
