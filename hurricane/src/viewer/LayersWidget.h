#ifndef LAYERS_WIDGET_H
#define LAYERS_WIDGET_H

#include <set>
using namespace std;

#include <QWidget>

class LayersWidget : public QWidget {
    Q_OBJECT

    public :
        LayersWidget(QWidget *parent = 0);
        ~LayersWidget();
    private:
        set<QWidget*> widgets;
};

#endif
