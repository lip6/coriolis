
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ExceptionWidget.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QCloseEvent>
#include  <QLabel>
#include  <QPushButton>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QFrame>

#include  "hurricane/Exception.h"
#include  "hurricane/viewer/ExceptionWidget.h"


namespace Hurricane {


  ExceptionWidget::ExceptionWidget ( QWidget* parent )
    : QDialog   (parent)
    , _message  (new QLabel())
  {
    setAttribute       ( Qt::WA_DeleteOnClose );
    setModal           ( true );
    setWindowTitle     ( tr("<An Exception was Caught>") );
    setToolTip         ( tr("Ben mon cochon, t'es dans le bouillon") );

    QLabel* header = new QLabel ();
    header->setMinimumWidth ( 200 );
    header->setTextFormat   ( Qt::RichText );
    header->setText         ( "<b>[ERROR]</b>" );

    _message->setTextFormat ( Qt::RichText );
    _message->setText       ( "<b>Oups! I did it again!</b>" );

    QLabel* ok = new QLabel ();
    ok->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    ok->setPixmap     ( QPixmap(":/images/gnome-core.png") );
    ok->setStyleSheet ( "QLabel { background-color: #FF9999;"
                        "         padding:          5px }" );

    QPushButton* abortButton = new QPushButton ();
    abortButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    abortButton->setText       ( tr("Abort") );

    QPushButton* contButton = new QPushButton ();
    contButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    contButton->setText       ( tr("Try to Continue") );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addStretch ( 1 );
    hLayout2->addWidget  ( contButton, Qt::AlignCenter );
    hLayout2->addStretch ( 4 );
    hLayout2->addWidget  ( abortButton, Qt::AlignCenter );
    hLayout2->addStretch ( 1 );

    QVBoxLayout* vLayout1 = new QVBoxLayout ();
    vLayout1->setContentsMargins ( 10, 10, 10, 10 );
    vLayout1->setSpacing ( 0 );
    vLayout1->addWidget  ( header  , Qt::AlignCenter );
    vLayout1->addWidget  ( _message, Qt::AlignCenter );
    vLayout1->addSpacing ( 10 );
    vLayout1->addLayout  ( hLayout2, Qt::AlignCenter );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->setSizeConstraint  ( QLayout::SetFixedSize );
    hLayout1->setContentsMargins ( 0, 0, 0, 0 );
    hLayout1->addWidget          ( ok );
    hLayout1->addLayout          ( vLayout1 );

    setLayout ( hLayout1 );

    connect ( contButton , SIGNAL(clicked()), this, SLOT(accept()) );
    connect ( abortButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }


  void  ExceptionWidget::closeEvent ( QCloseEvent* event )
  {
    event->ignore ();
  }


  void  ExceptionWidget::setMessage ( const QString& message )
  {
    _message->setText ( message );
  }


}  // End of Hurricane Namespace.
