#include <iostream>
using namespace std;

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLineEdit>

#include "TechnoRow.h"

TechnoRow::TechnoRow(int id, QWidget *parent) : QWidget(parent), _id(id) {
    _suppress = new QPushButton( tr("-"), this);
    _suppress->setStyleSheet("font: bold 24pt; color: red; border: 0px");
    _suppress->setFixedWidth(_suppress->height());
    _name = new QLineEdit(this);
    _name->setFixedWidth(80);
    _file = new QLineEdit(this);
    _file->setFixedWidth(300);
    _browse = new QPushButton( tr("&browse"), this);

    connect(_suppress, SIGNAL(clicked()), this, SLOT(suppress()));
    connect(_browse  , SIGNAL(clicked()), this, SLOT(chooseFile()));

    //QDialogButtonBox* buttons = new QDialogButtonBox();
    //buttons->addButton ( _execute, QDialogButtonBox::RejectRole );
    //buttons->addButton ( _stop   , QDialogButtonBox::AcceptRole );
    //buttons->addButton ( _show   , QDialogButtonBox::ResetRole  );
    //buttons->addButton ( _clear  , QDialogButtonBox::ResetRole  );

    //connect ( _execute, SIGNAL(clicked()), this    , SLOT(execute())    );
    //connect ( _stop   , SIGNAL(clicked()), this    , SLOT(stop())       );
    //connect ( _show   , SIGNAL(clicked()), this    , SLOT(showOrHide()) );
    //connect ( _clear  , SIGNAL(clicked()), _console, SLOT(clear())      );

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addWidget(_suppress);
    hLayout->addWidget(_name);
    hLayout->addWidget(_file);
    hLayout->addWidget(_browse);
    setLayout(hLayout);
}

QString TechnoRow::getName() {
    return _name->text();
}

QString TechnoRow::getFile() {
    return _file->text();
}

void TechnoRow::setName(QString& name) {
    _name->setText(name);
}

void TechnoRow::setFile(QString& file) {
    _file->setText(file);
}

void TechnoRow::suppress() {
    emit(suppressed(_id));
}

void TechnoRow::chooseFile() {
    _file->setText(QFileDialog::getOpenFileName(this, tr("Select dtr file"), "", tr("Xml file (*.xml)")));
}
