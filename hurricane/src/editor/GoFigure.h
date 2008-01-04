#ifndef __GO_FIGURE_H
#define __GO_FIGURE_H

#include <QGraphicsItem>
#include <QObject>

#include "Go.h"
using namespace H;

class SliceFigure;

class GoFigure : public QGraphicsItem {
    public:
        GoFigure(SliceFigure* parent);
        QRectF boundingRect() const;
    protected:
        virtual Go* getGo() const = 0;
};

#endif /* __GO_FIGURE_H */
