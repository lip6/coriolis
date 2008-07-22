#include <QGridLayout>
#include <QSlider>
#include <QDockWidget>
#include <QComboBox>

#include "Capacitor.h"
#include "hurricane/viewer/CellWidget.h"
#include "AEnv.h"
#include "ATechnology.h"

#include "CapacitorViewer.h"


CapacitorViewer::CapacitorViewer(Library* library) {

    ATechnology* aTechnology = AEnv::getATechnology();
    if (!aTechnology) {
        exit(56);
    }

    DbU::Unit capacitorMinL = 0;
    DbU::Unit capacitorMaxL = 0;
    DbU::Unit capacitorMinW = 0;
    DbU::Unit capacitorMaxW = 0;

    _capacitor = Capacitor::create(library, Name("TEST"));
    //_capacitor->setL(capacitorMinL);
    //_capacitor->setW(capacitorMinW);
    _cellWidget = new CellWidget;
    _cellWidget->setCell(_capacitor);
    _cellWidget->fitToContents();
    setCentralWidget(_cellWidget);
    QSlider* wSlider = new QSlider(Qt::Horizontal);
    wSlider->setRange(capacitorMinW, 4 * capacitorMinW);
    wSlider->setPageStep(DbU::grid(1));
    wSlider->setSliderPosition(capacitorMinW);
    QSlider* lSlider = new QSlider(Qt::Horizontal);
    lSlider->setRange(capacitorMinL, 4 * capacitorMinL);
    lSlider->setPageStep(DbU::grid(1));
    lSlider->setSliderPosition(capacitorMinL);

    QWidget* slidersWidget = new QWidget;
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(wSlider, 0, 0);
    layout->addWidget(lSlider, 1, 0);
    slidersWidget->setLayout(layout);

    QDockWidget *dockWidget = new QDockWidget(tr("Dock Widget"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);
    dockWidget->setWidget(slidersWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    connect(wSlider, SIGNAL(valueChanged(int)), this, SLOT(wvalueChanged(int)));
    connect(lSlider, SIGNAL(valueChanged(int)), this, SLOT(lvalueChanged(int)));
}

void CapacitorViewer::wvalueChanged(int value) {
    //_capacitor->setW(value);
    _cellWidget->redraw();
}

void CapacitorViewer::lvalueChanged(int value) {
    //_capacitor->setL(value);
    _cellWidget->redraw();
}
