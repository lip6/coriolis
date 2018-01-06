
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
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
// |  C++ Header  :       "./MousePositionWidget.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_MOUSE_POSITION__
#define  __HURRICANE_MOUSE_POSITION__

#include  <QWidget>

class QLabel;

#include  "hurricane/Commons.h"
#include  "hurricane/Point.h"


namespace Hurricane {


  class DynamicLabel;


  class MousePositionWidget : public QWidget {
      Q_OBJECT;

    public:
                                   MousePositionWidget ( QWidget* parent=NULL );
    public slots:                  
              void                 setPosition         ( const Point& );
                                   
    protected:                     
              DynamicLabel*        _xPosition;
              DynamicLabel*        _yPosition;
    protected:
                                   MousePositionWidget ( const DynamicLabel& );
              MousePositionWidget& operator=           ( const DynamicLabel& );
  };


} // End of Hurricane namespace.


# endif
