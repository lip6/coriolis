#include "DataBase.h"
#include "Technology.h"
#include "BasicLayer.h"
using namespace H;

#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>

#include "LayersWidget.h"

LayersWidget::LayersWidget(QWidget* parent)
    : QWidget(parent),
    widgets()
{
    DataBase* db = getDataBase();
    Technology* techno = db->getTechnology();
    QGridLayout* mainLayout = new QGridLayout;

    int line = 0;
    for_each_basic_layer(basicLayer, techno->getBasicLayers()) {
        QCheckBox* checkBox = new QCheckBox();
        widgets.insert(checkBox);
        mainLayout->addWidget(checkBox, line, 0, Qt::AlignRight);
        //connect(checkBox, SIGNAL(toggled(bool)),
        //        renderArea, SLOT(setAntialiased(bool)));

        string layerName = getString(basicLayer->getName());
        QLabel* label = new QLabel(tr(layerName.c_str()));
        widgets.insert(label);
        mainLayout->addWidget(label, line, 1, Qt::AlignRight);
        ++line;
        end_for;
    }
    setLayout(mainLayout);
    setWindowTitle(tr("Layers"));
}

LayersWidget::~LayersWidget() {
    for (set<QWidget*>::iterator wsit = widgets.begin();
            wsit != widgets.end();
            wsit++) {
        delete *wsit;
    }
}
