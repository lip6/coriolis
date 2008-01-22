#ifndef __SEGMENT_GRAPHICS_ITEM_H
#define __SEGMENT_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QObject>

#include "Segment.h"
using namespace Hurricane;

class CellGraphicsItem;

class SegmentGraphicsItem : public QGraphicsItem {
    public:
        SegmentGraphicsItem(CellGraphicsItem* parent, Segment* segment);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;
    private:
        Segment* segment;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif /* __SEGMENT_GRAPHICS_ITEM_H */
