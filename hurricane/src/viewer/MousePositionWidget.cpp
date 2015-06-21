
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./MousePositionWidget.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QHBoxLayout>

#include  "hurricane/viewer/DynamicLabel.h"
#include  "hurricane/viewer/MousePositionWidget.h"


namespace Hurricane {


  MousePositionWidget::MousePositionWidget ( QWidget* parent )
    : QWidget(parent)
    , _xPosition(new DynamicLabel())
    , _yPosition(new DynamicLabel())
  {
    _xPosition->setStaticText  ( "X:" );
    _xPosition->setDynamicText ( "N/A" );
    
    _yPosition->setStaticText  ( "Y:" );
    _yPosition->setDynamicText ( "N/A" );

    QHBoxLayout* layout = new QHBoxLayout ();
    layout->addWidget ( _xPosition );
    layout->addWidget ( _yPosition );
    layout->setContentsMargins ( 0, 0, 0, 0 );

    setLayout ( layout );
  }


  void  MousePositionWidget::setPosition ( const Point& position )
  {
    _xPosition->setDynamicText ( position.getX() );
    _yPosition->setDynamicText ( position.getY() );
  }


} // End of Hurricane namespace.
