// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ImportCellDialog.cpp"                   |
// +-----------------------------------------------------------------+


#include <iostream>
using namespace std;

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "hurricane/Warning.h"
#include "hurricane/viewer/Graphics.h"
#include "unicorn/ImportCell.h"
#include "unicorn/ImportCellDialog.h"


namespace Unicorn {


  using Hurricane::Warning;
  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "ImportCellDialog".


  ImportCellDialog::ImportCellDialog ( QWidget* parent )
    : QDialog        (parent)
    , _lineEdit      (new QLineEdit())
    , _viewerCheckBox(new QCheckBox())
    , _formatComboBox(new QComboBox())
  {
    setWindowTitle  ( tr("Import Cell") );

    QLabel* label = new QLabel ();
    label->setText ( tr("Enter Cell name (without extention)") );
    label->setFont ( Graphics::getNormalFont(true) );

    _lineEdit->setMinimumWidth ( 400 );
    _viewerCheckBox->setText ( tr("Import in new Viewer") );

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
    formatLabel->setText ( tr("Import Format") );
    formatLabel->setFont ( Graphics::getNormalFont(true) );
    hLayout2->addWidget ( formatLabel  );

#if 0
    _formatComboBox->addItem ( tr("ACM/SIGDA (aka MCNC, .bench)"), ImportCell::AcmSigda    );
    _formatComboBox->addItem ( tr("ISPD'04 (Bookshelf)")         , ImportCell::Ispd04      );
    _formatComboBox->addItem ( tr("ISPD'05 (Bookshelf)")         , ImportCell::Ispd05      );
    _formatComboBox->addItem ( tr("ICCAD'04 (LEF/DEF)")          , ImportCell::Iccad04     );
    _formatComboBox->addItem ( tr("Alliance compliant DEF")      , ImportCell::AllianceDef );
#endif
    hLayout2->addWidget ( _formatComboBox );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->setSizeConstraint  ( QLayout::SetFixedSize );
    vLayout->addWidget ( label );
    vLayout->addWidget ( _lineEdit );
    vLayout->addLayout ( hLayout2 );
    vLayout->addWidget ( _viewerCheckBox );
    vLayout->addWidget ( separator );
    vLayout->addLayout ( hLayout1 );

    setLayout ( vLayout );
  //setModal  ( true );

    connect (     okButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect ( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }


  void  ImportCellDialog::addFormat ( QString menuName, int index )
  {
    _formatComboBox->addItem( menuName, index );
  }


  const QString  ImportCellDialog::getCellName () const
  { return _lineEdit->text(); }


  bool  ImportCellDialog::useNewViewer () const
  { return _viewerCheckBox->isChecked(); }


  bool  ImportCellDialog::runDialog ( QString& name, int& format, bool& newViewerRequest )
  {
    bool dialogResult = (exec() == Accepted);

    name             = getCellName  ();
    newViewerRequest = useNewViewer ();
    format           = getFormat    ();

    return dialogResult;
  }


  int  ImportCellDialog::getFormat () const
  {
    int index = _formatComboBox->currentIndex();
    if ( index < 0 ) return 0;

    return _formatComboBox->itemData(index).toInt();
  }


} // End of Unicorn namespace.
