#include <iostream>
using namespace std;

#include <QGridLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMessageBox>

#include "ConfigDialog.h"
#include "TechnoRow.h"

#define LINEEDITWIDTH 300

int ConfigDialog::_rowID = 0;

ConfigDialog::ConfigDialog() {
    setStyleSheet("font-weight: normal;");
    // First GroupBox : Directories
    // Create widgets
    QLabel* programL  = new QLabel(tr("Pharos binary")   , this);
    QLabel* libraryL  = new QLabel(tr("LD_LIBRARY_PATH") , this);
    _programE  = new QLineEdit ("", this);
    _libraryE  = new QLineEdit ("", this); 
    _programE->setFixedWidth(LINEEDITWIDTH);
    _libraryE->setFixedWidth(LINEEDITWIDTH);
    _programE->setToolTip(tr("Select <b>Pharos</b> binary file."));
    _libraryE->setToolTip(tr("Select the directory containing c++ dynamic libraries used by <b>Pharos</b>."));
    QPushButton* programB  = new QPushButton(tr("&Browse"));
    QPushButton* libraryB  = new QPushButton(tr("&Browse"));
    // Connect SIGNAL/SLOTS
    connect(programB , SIGNAL(clicked()), this, SLOT(chooseProgram()));
    connect(libraryB , SIGNAL(clicked()), this, SLOT(chooseLibrary()));
    // Create GroupBox
    QGroupBox* directories = new QGroupBox("Directories", this);
    QGridLayout* gLayout = new QGridLayout(this);
    gLayout->addWidget( programL , 0, 0, 1, 1);
    gLayout->addWidget(_programE , 0, 1, 1, 1);
    gLayout->addWidget( programB , 0, 2, 1, 1);
    gLayout->addWidget( libraryL , 1, 0, 1, 1);
    gLayout->addWidget(_libraryE , 1, 1, 1, 1);
    gLayout->addWidget( libraryB , 1, 2, 1, 1);
    directories->setLayout(gLayout);
    // Second GroupBox : Technos
    // Create Widgets / Layout
    QLabel* name = new QLabel(tr("Name"));
    QLabel* file = new QLabel(tr("XML Config File"));
    name->setFixedWidth(80);
    file->setFixedWidth(LINEEDITWIDTH);
    _rowLayout = new QVBoxLayout();
    _rowLayout->setContentsMargins(0,0,0,0);
    _rowLayout->setSpacing(0);
    addRow();
    // Create GroupBox
    QGroupBox* technos = new QGroupBox("Technologies", this);
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->addSpacing(20);
    headerLayout->addWidget(name);
    headerLayout->addWidget(file);
    QVBoxLayout* techLayout = new QVBoxLayout();
    techLayout->addLayout(headerLayout);
    techLayout->addLayout(_rowLayout);
    technos->setLayout(techLayout);

    QDialogButtonBox* dbBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    _add = new QPushButton("&Add technology", this);
    dbBox->addButton(_add, QDialogButtonBox::ActionRole);
    connect(_add , SIGNAL(clicked()) , this, SLOT(addRow()));
    connect(dbBox, SIGNAL(accepted()), this, SLOT(doConfig()));

    QVBoxLayout* tLayout = new QVBoxLayout();
    tLayout->setContentsMargins(0,0,0,0);
    tLayout->addWidget(directories);
    tLayout->addWidget(technos);
    tLayout->addWidget(dbBox);
    setLayout(tLayout);
    setWindowTitle(tr("Configure runPharos"));
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}


void ConfigDialog::addRow() {
    TechnoRow* row = new TechnoRow(_rowID++, this);
    connect(row, SIGNAL(suppressed(int)), this, SLOT(removeRow(int)));
    row->setContentsMargins(0,0,0,0);
    _rows.push_back(row);
    _rowLayout->addWidget(row);
}

void ConfigDialog::addRow(QString& name, QString& file) {
    TechnoRow* row = new TechnoRow(_rowID++, this);
    row->setName(name);
    row->setFile(file);
    connect(row, SIGNAL(suppressed(int)), this, SLOT(removeRow(int)));
    row->setContentsMargins(0,0,0,0);
    _rows.push_back(row);
    _rowLayout->addWidget(row);
}

void ConfigDialog::removeRow(int id) {
    TechnoRow* row = _rows[id];
    _rowLayout->removeWidget(row);
    row->hide();
}

void ConfigDialog::doConfig() {
    QStringList list;
    if ( _programE->text().isEmpty() || _libraryE->text().isEmpty() ) {
        QMessageBox::warning(this, tr("runPharos warning"), tr("You must set all directories."));
        return;
    }
    list << _programE->text() << _libraryE->text();
    vector<pair<QString, QString> > technos;
    for ( size_t i = 0 ; i < _rows.size() ; i++ ) {
        TechnoRow* row = _rows[i];
        if (row->isVisible()) {
            if((row->getName() != "") && (row->getFile() != "")) {
                technos.push_back(pair<QString, QString>(row->getName(), row->getFile()));
            } else {
                QMessageBox::warning(this, tr("runPharos warning"), tr("You must set a valid name and file for each technology."));
                return;
            } 
        }
    }
    if (technos.size() > 0) {
        emit(configDone(list, technos));
        accept();
    } else {
        QMessageBox::warning(this, tr("runPharos warning"), tr("You must set at least one valid technology."));
    }
}

void ConfigDialog::chooseProgram() {
    _programE->setText(QFileDialog::getOpenFileName(this, tr("Select Pharos binary file"), "", tr("Executable file (*)")));
}

void ConfigDialog::chooseLibrary() {
    _libraryE->setText(QFileDialog::getExistingDirectory(this, tr("Select directory containing c++ dynamic libraries")));
}

void ConfigDialog::setProgram(QString program) {
    _programE->setText(program);
}

void ConfigDialog::setLibrary(QString library) {
    _libraryE->setText(library);
}

