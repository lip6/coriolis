// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ExportCellDialog.cpp"                   |
// +-----------------------------------------------------------------+


#include  <iostream>
using namespace std;

#include  <QLabel>
#include  <QPushButton>
#include  <QLineEdit>
#include  <QComboBox>
#include  <QCheckBox>
#include  <QHBoxLayout>
#include  <QVBoxLayout>

#include  "hurricane/Warning.h"
#include  "hurricane/viewer/Graphics.h"

#include  "unicorn/ExportCellDialog.h"


namespace Unicorn {


  using Hurricane::Warning;
  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "ExportCellDialog".


  ExportCellDialog::ExportCellDialog ( QWidget* parent )
    : QDialog        (parent)
    , _lineEdit      (new QLineEdit())
    , _formatComboBox(new QComboBox())
  {
    setWindowTitle  ( tr("Export Cell") );

    QLabel* label = new QLabel ();
    label->setText ( tr("Enter Cell name (without extention)") );
    label->setFont ( Graphics::getNormalFont(true) );

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

    QHBoxLayout* hLayout2 = new QHBoxLayout ();

    QLabel* formatLabel = new QLabel ();
    formatLabel->setText ( tr("Export Format") );
    formatLabel->setFont ( Graphics::getNormalFont(true) );
    hLayout2->addWidget ( formatLabel  );

    _formatComboBox->addItem ( tr("Alliance compliant DEF"), AllianceDef );
    _formatComboBox->addItem ( tr("ASCII/GDSII (AGDS)")    , AsciiGds    );
    hLayout2->addWidget ( _formatComboBox );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->setSizeConstraint  ( QLayout::SetFixedSize );
    vLayout->addWidget ( label );
    vLayout->addWidget ( _lineEdit );
    vLayout->addLayout ( hLayout2 );
    vLayout->addWidget ( separator );
    vLayout->addLayout ( hLayout1 );

    setLayout ( vLayout );
  //setModal  ( true );

    connect (     okButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect ( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }


  const QString  ExportCellDialog::getCellName () const
  { return _lineEdit->text(); }


  void  ExportCellDialog::setCellName ( const QString& name )
  { _lineEdit->setText ( name ); }


  bool  ExportCellDialog::runDialog ( QString& name, int& format )
  {
    setCellName ( name );
    bool dialogResult = (exec() == Accepted);

    name   = getCellName ();
    format = getFormat   ();

    return dialogResult;
  }


  int  ExportCellDialog::getFormat () const
  {
    int index = _formatComboBox->currentIndex();
    if ( index < 0 ) return 0;

    return _formatComboBox->itemData(index).toInt();
  }


} // End of Unicorn namespace.
