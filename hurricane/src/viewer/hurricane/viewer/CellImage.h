
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/CellImage.h"                |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_CELL_IMAGE_H
#define  HURRICANE_CELL_IMAGE_H


#include <list>
using namespace std;

#include <QMainWindow>

class QEvent;
class QKeyEvent;
class QAction;
class QMenu;
class QPainter;
class QImage;

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"
#include  "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  class Cell;
  class PaletteWidget;


  class CellImage : public QMainWindow {
      Q_OBJECT;
                                   
    public:
      enum    Flags                { ShowScale=0x0001 };
    public:
                                   CellImage                 ( QWidget* parent=NULL );
      virtual                     ~CellImage                 ();
      inline  CellWidget*          getCellWidget             ();
      inline  const CellWidget*    getCellWidget             () const;
              void                 setScreenCellWidget       ( CellWidget* );
      inline  void                 setMode                   ( int mode );
      inline  void                 setFitOnAbutmentBox       ( bool );
              void                 createLayout              ();
      inline  bool                 fitOnAbutmentBox          () const;
              void                 pageDecorate              ( QPainter& );
              QImage*              toImage                   ( unsigned int flags=0 );
      virtual std::string          _getString                () const;

    protected:                     
      CellWidget*    _screenCellWidget;
      CellWidget*    _cellWidget;
      PaletteWidget* _palette;
      QImage*        _image;
      int            _mode;
      unsigned int   _flags;
      bool           _fitOnAbutmentBox;
      int            _drawingWidth;
      int            _drawingHeight;
  };


// Inline Functions.
  inline CellWidget*       CellImage::getCellWidget       () { return _cellWidget; }
  inline const CellWidget* CellImage::getCellWidget       () const { return _cellWidget; }
  inline void              CellImage::setMode             ( int mode ) { _mode = mode; }
  inline void              CellImage::setFitOnAbutmentBox ( bool state ) { _fitOnAbutmentBox = state; }
  inline bool              CellImage::fitOnAbutmentBox    () const { return _fitOnAbutmentBox; }


} // Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::CellImage)
IOSTREAM_POINTER_SUPPORT(Hurricane::CellImage)


#endif
