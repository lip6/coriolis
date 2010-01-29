#include <QGridLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>

#include "configDialog.h"

#define LINEEDITWIDTH 300

ConfigDialog::ConfigDialog() {
    // First GroupBox : Directories
    // Create Widgets
    QLabel* compileL = new QLabel(tr("Compilation script:"), this);
    QLabel* sourceL  = new QLabel(tr("Chams source root directory:"), this);
    _compileE = new QLineEdit ("", this);
    _sourceE  = new QLineEdit ("", this);
    _compileE->setFixedWidth(LINEEDITWIDTH);
    _sourceE->setFixedWidth (LINEEDITWIDTH);
    QPushButton* compileB = new QPushButton(tr("&Browse"));
    QPushButton* sourceB  = new QPushButton(tr("&Browse"));
    QDialogButtonBox* bBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    // Connect SIGNAL/SLOTS
    connect(compileB, SIGNAL(clicked()) , this, SLOT(chooseCProgram()));
    connect(sourceB , SIGNAL(clicked()) , this, SLOT(chooseSourceRD()));
    connect(bBox    , SIGNAL(accepted()), this, SLOT(doConfig()));
    // Create GroupBox
    QGroupBox* directories = new QGroupBox("Directories", this);
    QGridLayout* gLayout = new QGridLayout(this);
    gLayout->addWidget( compileL, 0, 0, 1, 1);
    gLayout->addWidget(_compileE, 0, 1, 1, 2);
    gLayout->addWidget( compileB, 0, 3, 1, 1);
    gLayout->addWidget( sourceL , 1, 0, 1, 1);
    gLayout->addWidget(_sourceE , 1, 1, 1, 2);
    gLayout->addWidget( sourceB , 1, 3, 1, 1);
    directories->setLayout(gLayout);
    // Second GroupBox : Tools
    // Create Widgets
    _hurCB    = new QCheckBox("Hurricane"        , this);
    _ioCB     = new QCheckBox("Io"               , this); 
    _testCB   = new QCheckBox("Test (not in svn)", this);
    _crlCB    = new QCheckBox("Crlcore"          , this);
    _chamsCB  = new QCheckBox("Chams"            , this);
    _hurAMSCB = new QCheckBox("HurricaneAMS"     , this);
    // Create GroupBox
    QGroupBox* tools = new QGroupBox("Available Tools", this);
    QGridLayout* grLayout = new QGridLayout(this);
    grLayout->addWidget(_hurCB   , 0, 0, 1, 1);
    grLayout->addWidget(_ioCB    , 1, 0, 1, 1);
    grLayout->addWidget(_testCB  , 2, 0, 1, 1);
    grLayout->addWidget(_crlCB   , 0, 1, 1, 1);
    grLayout->addWidget(_chamsCB , 1, 1, 1, 1);
    grLayout->addWidget(_hurAMSCB, 2, 1, 1, 1);
    tools->setLayout(grLayout);
    // ConfigDialog Layout
    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(directories);
    vLayout->addWidget(tools);
    vLayout->addWidget(bBox);
    setLayout(vLayout);
}

void ConfigDialog::chooseCProgram() {
    _compileE->setText(QFileDialog::getOpenFileName(this, tr("Select chams.sh"), "", tr("Shell script (*.sh)")));
}

void ConfigDialog::chooseSourceRD() {
    _sourceE->setText(QFileDialog::getExistingDirectory(this, tr("Select the chams source root directory")));
}

void ConfigDialog::doConfig() {
    QStringList list;
    QString hur    = (_hurCB->isChecked())    ? "ON" : "OFF";
    QString io     = (_ioCB->isChecked())     ? "ON" : "OFF";
    QString test   = (_testCB->isChecked())   ? "ON" : "OFF";
    QString crl    = (_crlCB->isChecked())    ? "ON" : "OFF";
    QString chams  = (_chamsCB->isChecked())  ? "ON" : "OFF";
    QString hurAMS = (_hurAMSCB->isChecked()) ? "ON" : "OFF";
    list << _compileE->text() << _sourceE->text() << hur << io << test << crl << chams << hurAMS;
    emit configDone(list);
    accept();
}
