#ifndef __SEGMENT_FIGURE_H
#define __SEGMENT_FIGURE_H

#include <QGraphicsItem>
#include <QObject>

#include "GoFigure.h"

#include "Segment.h"
using namespace Hurricane;

class SegmentFigure : public GoFigure {
    public:
        SegmentFigure(SliceFigure* parent, Segment* segment);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:
        Segment* segment;
    protected:
        Go* getGo() const { return segment; }
};

#endif /* __SEGMENT_FIGURE_H */
