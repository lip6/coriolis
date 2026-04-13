// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicsWidget.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include <QWidget>
class QGridLayout;
class QButtonGroup;

#include  "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  class GraphicsWidget : public QWidget {
      Q_OBJECT;

    public:
            GraphicsWidget ( QWidget* parent=NULL );
      void  setCellWidget  ( CellWidget* );
      void  readGraphics   ();
      void  resetGraphics  ();
      void  rereadGraphics ();
      void  saveQtSettings ( size_t viewerId ) const;
      void  readQtSettings ( size_t viewerId );
    public slots:
      void  changeStyle    ();
      void  setStyle       ( int id );

    protected:
      CellWidget*   _cellWidget;
      QGridLayout*  _stylesGrid;
      QButtonGroup* _stylesGroup;
  };


} // Hurricane namespace.
