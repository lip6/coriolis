#include <math.h>

#include <QPainter>

#include "SliceFigure.h"

#include "Utils.h"
#include "GoFigure.h"

GoFigure::GoFigure(SliceFigure* master):
    QGraphicsItem(master)
{} 

QRectF GoFigure::boundingRect() const {
    Box box = getGo()->GetBoundingBox();
    QRectF rect;
    BoxToRectangle(box, rect);
    return rect;
}
