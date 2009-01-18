
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
// |  C++ Module  :       "./BreakpointWidget.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QLabel>
#include  <QPushButton>
#include  <QSpinBox>
#include  <QGridLayout>
#include  <QFrame>

#include  "hurricane/Breakpoint.h"
#include  "hurricane/viewer/BreakpointWidget.h"


namespace Hurricane {


  BreakpointWidget::BreakpointWidget ( QWidget* parent )
    : QDialog   (parent)
    , _message  (new QLabel())
    , _stopLevel(new QSpinBox())
  {
    setModal       ( true );
    setWindowTitle ( "Breakpoint" );
    setToolTip     ( "Crush the Mush to continue..." );

    _message->setTextFormat ( Qt::RichText );
    _message->setText       ( "<b>No Message Yet</b>" );

    QLabel* stopLabel = new QLabel ();
    stopLabel->setText ( "Adjust Stop Level:" );

    QPushButton* ok = new QPushButton ();
    ok->setIcon      ( QIcon(":/images/gnome-gmush.png") );
    ok->setFlat      ( true );
    ok->setIconSize  ( QSize(48,48) );

    QFrame* vLine = new QFrame ();
    vLine->setFrameShape  ( QFrame::VLine );
    vLine->setFrameShadow ( QFrame::Sunken );

    QGridLayout* layout = new QGridLayout ();
    layout->addWidget ( _message  , 0, 0 );
    layout->addWidget ( stopLabel , 1, 0 );
    layout->addWidget ( _stopLevel, 1, 1 );
    layout->addWidget ( vLine     , 0, 2, 2, 1 );
    layout->addWidget ( ok        , 0, 3, 2, 1, Qt::AlignVCenter );
    setLayout ( layout );

    connect ( ok        , SIGNAL(clicked())        , this, SLOT(accept()) );
    connect ( _stopLevel, SIGNAL(valueChanged(int)), this, SLOT(updateStopLevel(int)) );
  }


  void  BreakpointWidget::setMessage ( const QString& message )
  {
    _message->setText ( message );
  }


  void  BreakpointWidget::setStopLevel ( int level )
  {
    _stopLevel->setValue ( level );
  }


  int  BreakpointWidget::getStopLevel () const
  {
    return _stopLevel->value ();
  }


  void  BreakpointWidget::updateStopLevel ( int value )
  {
    unsigned int stopLevel = (value<0) ? 0 : value;
    Breakpoint::setStopLevel ( stopLevel );
  }


}  // End of Hurricane Namespace.
