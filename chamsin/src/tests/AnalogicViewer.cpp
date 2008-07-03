#include <QGridLayout>
#include <QSlider>
#include <QDockWidget>
#include <QComboBox>
#include "AnalogicViewer.h"

#include "Transistor.h"
#include "hurricane/viewer/CellWidget.h"
#include "AEnv.h"
#include "ATechnology.h"


AnalogicViewer::AnalogicViewer(Library* library) {

    ATechnology* aTechnology = AEnv::getATechnology();
    if (!aTechnology) {
        exit(56);
    }

    DbU::Unit transistorMinL = aTechnology->getPhysicalRule("transistorMinL")->getValue();
    DbU::Unit transistorMaxL = aTechnology->getPhysicalRule("transistorMaxL")->getValue();
    DbU::Unit transistorMinW = aTechnology->getPhysicalRule("transistorMinW")->getValue();
    DbU::Unit transistorMaxW = aTechnology->getPhysicalRule("transistorMaxW")->getValue();

    _transistor = Transistor::create(library, Name("TEST"), Transistor::Polarity::N, transistorMinL, transistorMinW);
    _transistor->createLayout();
    _cellWidget = new CellWidget;
    _cellWidget->setCell(_transistor);
    _cellWidget->fitToContents();
    setCentralWidget(_cellWidget);
    QSlider* wSlider = new QSlider(Qt::Horizontal);
    wSlider->setRange(transistorMinW, 4 * transistorMinW);
    wSlider->setPageStep(DbU::grid(1));
    wSlider->setSliderPosition(transistorMinW);
    QSlider* lSlider = new QSlider(Qt::Horizontal);
    lSlider->setRange(transistorMinL, 4 * transistorMinL);
    lSlider->setPageStep(DbU::grid(1));
    lSlider->setSliderPosition(transistorMinL);
    QStringList choices;
    choices << "N" << "P";
    QComboBox* choiceBox = new QComboBox;
    choiceBox->addItems(choices);

    QWidget* slidersWidget = new QWidget;
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(wSlider, 0, 0);
    layout->addWidget(lSlider, 1, 0);
    layout->addWidget(choiceBox, 2, 0);
    slidersWidget->setLayout(layout);

    QDockWidget *dockWidget = new QDockWidget(tr("Dock Widget"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
            Qt::RightDockWidgetArea);
    dockWidget->setWidget(slidersWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    connect(wSlider, SIGNAL(valueChanged(int)), this, SLOT(wvalueChanged(int)));
    connect(lSlider, SIGNAL(valueChanged(int)), this, SLOT(lvalueChanged(int)));
    connect(choiceBox, SIGNAL(currentIndexChanged(int)), this, SLOT(transistorTypeChanged(int)));
}

void AnalogicViewer::wvalueChanged(int value) {
    _transistor->setW(value);
    _cellWidget->redraw();
}

void AnalogicViewer::lvalueChanged(int value) {
    _transistor->setL(value);
    _cellWidget->redraw();
}

void AnalogicViewer::transistorTypeChanged(int value) {
    if (value == 0) {
        _transistor->setPolarity(Transistor::Polarity::N);
        _cellWidget->redraw();
    } else if (value == 1) {
        _transistor->setPolarity(Transistor::Polarity::P);
        _cellWidget->redraw();
    }
}
