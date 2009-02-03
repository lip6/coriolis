
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Header  :       "./GraphicsWidget.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_GRAPHICS_WIDGET__
#define  __HURRICANE_GRAPHICS_WIDGET__


#include  <QWidget>

#include  "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  class GraphicsWidget : public QWidget {
      Q_OBJECT;

    public:
            GraphicsWidget ( QWidget* parent=NULL );
      void  setCellWidget  ( CellWidget* );
    public slots:
      void  changeStyle    ();
      void  setStyle       ( int id );

    protected:
      CellWidget*  _cellWidget;
      UpdateState  _updateState;
  };


} // End of Hurricane namespace.


#endif
