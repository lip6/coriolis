
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./StratusWidget.cpp"                      |
// +-----------------------------------------------------------------+


#include  <Python.h>
#include  <iostream>
#include  <memory>
using namespace std;

#include  <QLabel>
#include  <QLineEdit>
#include  <QPushButton>
#include  <QRadioButton>
#include  <QButtonGroup>
#include  <QCheckBox>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  "hurricane/utilities/Path.h"
#include  "hurricane/Warning.h"
#include  "hurricane/isobar/Script.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/StratusScript.h"
#include  "hurricane/viewer/StratusWidget.h"
#include  "hurricane/viewer/CellViewer.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "StratusWidget".


  StratusWidget::StratusWidget ( QWidget* parent )
    : QDialog  (parent)
    , _lineEdit(NULL)
  {
    setWindowTitle ( tr("Execute Stratus Script") );

    QLabel* label = new QLabel ();
    label->setText ( tr("Stratus script to execute") );
    label->setFont ( Graphics::getNormalFont(true) );

    _lineEdit = new QLineEdit ();
    _lineEdit->setMinimumWidth ( 400 );

    QPushButton* okButton = new QPushButton ();
    okButton->setText    ( "OK" );
    okButton->setDefault ( true );

    QPushButton* cancelButton = new QPushButton ();
    cancelButton->setText ( "Cancel" );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->addStretch ();
    hLayout1->addWidget ( okButton );
    hLayout1->addStretch ();
    hLayout1->addWidget ( cancelButton );
    hLayout1->addStretch ();

  //QFrame* separator = new QFrame ();
  //separator->setFrameShape  ( QFrame::HLine );
  //separator->setFrameShadow ( QFrame::Sunken );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->setSizeConstraint  ( QLayout::SetFixedSize );
    vLayout->addWidget ( label );
    vLayout->addWidget ( _lineEdit );
    vLayout->addLayout ( hLayout1 );
  //vLayout->addWidget ( separator );

    setLayout ( vLayout );
  //setModal  ( true );

    connect (     okButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect ( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }


  const QString  StratusWidget::getScriptName () const
  {
    return _lineEdit->text();
  }


  bool  StratusWidget::runScript ( QWidget* parent )
  {
    StratusWidget* dialog       = new StratusWidget ( parent );
    bool           doRunStratus = (dialog->exec() == Accepted);
    QString        scriptName   = dialog->getScriptName ();

    delete dialog;
    if ( not doRunStratus ) return false;

    dbo_ptr<StratusScript> script
      = StratusScript::create ( scriptName.toStdString(), qobject_cast<CellViewer*>(parent) );
    
    bool returnCode = script->run ();

#if DEPRECATED
    if ( scriptName.endsWith(".py",Qt::CaseInsensitive) )
      scriptName.truncate ( scriptName.size()-3 );

    Utilities::Path userStratus   ( scriptName.toStdString() );
    Utilities::Path userDirectory ( userStratus.branch_path() );

    if ( not userDirectory.absolute() )
      userDirectory = Utilities::Path:cwd() / userDirectory;

    Isobar::Script::addPath ( userDirectory.string() );

    dbo_ptr<Isobar::Script> script = Isobar::Script::create(userStratus.leaf());
    script->setEditor ( qobject_cast<CellViewer*>(parent) );

    bool returnCode = script->runFunction ( "StratusScript", NULL, Isobar::Script::NoScriptArgs );

    Isobar::Script::removePath ( userDirectory.string() );
#endif

    return returnCode;
  }


} // End of Hurricane namespace.
