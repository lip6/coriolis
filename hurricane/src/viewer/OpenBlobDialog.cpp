// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./OpenBlobDialog.cpp"                     |
// +-----------------------------------------------------------------+


#include <iostream>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "hurricane/Warning.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/OpenBlobDialog.h"


namespace Hurricane {

  using namespace std;


// -------------------------------------------------------------------
// Class  :  "OpenBlobDialog".


  OpenBlobDialog::OpenBlobDialog ( QWidget* parent )
    : QDialog  (parent)
    , _lineEdit(NULL)
  {
    setWindowTitle( tr("Open Design Blob") );

    QLabel* label = new QLabel ();
    label->setText( tr("Enter the Blob name (without extention \".blob.json.bz2\")") );
    label->setFont( Graphics::getNormalFont(true) );

    _lineEdit = new QLineEdit ();
    _lineEdit->setMinimumWidth( 400 );

    QPushButton* okButton = new QPushButton ();
    okButton->setText   ( "OK" );
    okButton->setDefault( true );

    QPushButton* cancelButton = new QPushButton ();
    cancelButton->setText( "Cancel" );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->addStretch();
    hLayout1->addWidget ( okButton );
    hLayout1->addStretch();
    hLayout1->addWidget ( cancelButton );
    hLayout1->addStretch();

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->setSizeConstraint  ( QLayout::SetFixedSize );
    vLayout->addWidget( label );
    vLayout->addWidget( _lineEdit );
    vLayout->addLayout( hLayout1 );

    setLayout ( vLayout );

    connect (     okButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect ( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }


  const QString  OpenBlobDialog::getBlobName () const
  { return _lineEdit->text(); }


  bool  OpenBlobDialog::runDialog ( QWidget* parent, QString& name )
  {
    OpenBlobDialog* dialog       = new OpenBlobDialog ( parent );
    bool            dialogResult = (dialog->exec() == Accepted);

    name = dialog->getBlobName();

    delete dialog;

    return dialogResult;
  }


} // Hurricane namespace.
