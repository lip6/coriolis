// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SaveCellDialog.cpp"                     |
// +-----------------------------------------------------------------+


#include  <iostream>
using namespace std;

#include  <QLabel>
#include  <QLineEdit>
#include  <QPushButton>
#include  <QRadioButton>
#include  <QButtonGroup>
#include  <QCheckBox>
#include  <QHBoxLayout>
#include  <QVBoxLayout>

#include  "hurricane/Warning.h"
#include  "hurricane/viewer/Graphics.h"

#include  "crlcore/Environment.h"
#include  "crlcore/AllianceFramework.h"
#include  "unicorn/SaveCellDialog.h"


namespace Unicorn {


  using Hurricane::Warning;
  using Hurricane::Graphics;
  using CRL::Environment;
  using CRL::AllianceFramework;


// -------------------------------------------------------------------
// Class  :  "SaveCellDialog".


  SaveCellDialog::SaveCellDialog ( QWidget* parent )
    : QDialog  (parent)
    , _lineEdit(NULL)
  {
    setWindowTitle ( tr("Save Cell") );

    QLabel* label = new QLabel ();
    label->setText ( tr("Enter Cell name (without extention)") );
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

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

#if DEPRECATED
    QHBoxLayout*  hLayout2     = new QHBoxLayout ();
    QButtonGroup* formatGroup  = new QButtonGroup ();
    QRadioButton* formatButton = new QRadioButton ();
    formatButton->setText ( tr("vst/ap") );
    formatButton->setChecked ( true );
    formatGroup->setId     ( formatButton, 0 );
    formatGroup->addButton ( formatButton );
    hLayout2->addWidget    ( formatButton  );

    formatButton = new QRadioButton ();
    formatButton->setText ( tr("DEF") );
    formatButton->setChecked ( false );
    formatGroup->setId     ( formatButton, 1 );
    formatGroup->addButton ( formatButton );
    hLayout2->addWidget    ( formatButton  );
#endif

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->setSizeConstraint  ( QLayout::SetFixedSize );
    vLayout->addWidget ( label );
    vLayout->addWidget ( _lineEdit );
    vLayout->addLayout ( hLayout1 );
  //vLayout->addWidget ( separator );
  //vLayout->addLayout ( hLayout2 );

    setLayout ( vLayout );
  //setModal  ( true );

    connect (     okButton, SIGNAL(clicked())         , this, SLOT(accept()) );
    connect ( cancelButton, SIGNAL(clicked())         , this, SLOT(reject()) );
  //connect ( formatGroup , SIGNAL(buttonClicked(int)), this, SLOT(formatChanged(int)) );
  }


  const QString  SaveCellDialog::getCellName () const
  {
    return _lineEdit->text();
  }


  void  SaveCellDialog::setCellName ( const QString& name )
  {
    return _lineEdit->setText ( name );
  }


  bool  SaveCellDialog::runDialog ( QWidget* parent, QString& name )
  {
    unique_ptr<SaveCellDialog> dialog ( new SaveCellDialog(parent) );
    dialog->setCellName ( name );
    bool dialogResult = (dialog->exec() == Accepted);

    name = dialog->getCellName ();

    return dialogResult;
  }


#if DEPRECATED
  void  SaveCellDialog::formatChanged ( int index )
  {
    Environment* environment = AllianceFramework::get()->getEnvironment();
    switch ( index ) {
      case 0:
        environment->setOUT_LO ( "vst" );
        environment->setOUT_PH ( "ap"  );
        break;
      case 1:
        environment->setOUT_LO ( "def" );
        environment->setOUT_PH ( "def" );
        break;
      default:
        cerr << Warning("Unknown output format: %d.",index) << endl;
    }
  }
#endif


} // End of Unicorn namespace.
