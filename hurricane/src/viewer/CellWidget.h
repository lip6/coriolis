#ifndef __CELL_WIDGET_H
#define __CELL_WIDGET_H

#include <QWidget>
#include <QPen>

#include "Box.h"
#include "Cell.h"
using namespace H;


class CellWidget : public QWidget {
        Q_OBJECT

    public:
        CellWidget(Cell* cell, QWidget* parent=0);
        void redraw();
    protected:
        void paintEvent(QPaintEvent* event);
    private:
        Cell* cell;
        QRegion invalidRegion;
        Box clipBox;
        double clipX[4];
        double clipY[4];
        QPainter* painter;
        QColor backgroundColor;
        QColor foregroundColor;
        double scale;
        int screenDx;
        int screenDy;
        int brushDx;
        int brushDy;
        map<BasicLayer*, QBrush> basicLayersBrush;
        map<BasicLayer*, QPen> basicLayersPen;

        void drawContent(const Cell* cell, const BasicLayer* basicLayer, const H::Box& updateArea, const Transformation& transformation) const;
        void drawContent(const Instance* instance, const BasicLayer* basicLayer, const H::Box& updateArea, const Transformation& transformation) const;
        void drawSlice(const Slice* slice, const BasicLayer* basicLayer, const H::Box& updateArea, const Transformation& transformation) const;
        void drawPhantoms(const Cell* cell, const Box& updateArea, const Transformation& transformation) const;
        void drawPhantoms(const Instance* instance, const Box& updateArea, const Transformation& transformation) const;
        void drawBoundaries(const Cell* cell, const Box& updateArea, const Transformation& transformation) const;
        void drawBoundaries(const Instance* instance, const Box& updateArea, const Transformation& transformation) const;
        void drawGo(const Go* go, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const;
        void drawSegment(const Segment* segment, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const;
        void drawContact(const Contact* contact, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const;
        void drawRectangle(const Box& box) const;
        void drawLine(const Point& po, const Point& pe) const;
        void drawLine(const Unit& xo, const Unit& yo, const Unit& xe, const Unit& ye) const;
        Unit getX(int screenX) const;
        Unit getY(int screenY) const;
        Unit getSize(int screenSize) const;
        H::Box getBox(const QRect& screenRect) const;
        int getScreenX(const Unit& x) const;
        int getScreenY(const Unit& y) const;
        int getScreenSize(const Unit& size) const;
        void invalidate();
        void invalidate(const QRect& screenRect);
        void setClipBox(const Box& area);
        int getClipCode(double x, double y) const;
        bool clipLine(double& xo, double& yo, int co, double& xe, double& ye, int ce, int u=0) const;
        void setFont(const QFont& font);
        void setPen(const QPen& pen, double brightness = 1.0);
        void setBrush(const QBrush& brush, double brightness = 1.0);
        void setBrushOrigin(const QPoint& origin);
        const QColor& getBackgroundColor() const;
        bool isDrawable(BasicLayer* layer) const;
};



inline const QColor& CellWidget::getBackgroundColor() const {
    return backgroundColor;
}


#endif /* __CELL_WIDGET_H */
