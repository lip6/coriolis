
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
// |  C++ Header  :  "./hurricane/viewer/CellPrinter.h"              |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_CELL_PRINTER_H
#define  HURRICANE_CELL_PRINTER_H


#include   <list>

using namespace std;

#include  <QMainWindow>

class QEvent;
class QKeyEvent;
class QAction;
class QMenu;
class QPainter;
class QPrinter;

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"
#include  "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  class Cell;
  class PaletteWidget;


  class CellPrinter : public QMainWindow {
      Q_OBJECT;
                                   
    public:
      static  std::string          getUserName               ();
      static  std::string          getTime                   ( const char* format );
      static  QString              getUnitQString            ();
      static  QString              dbuToPrinterString        ( DbU::Unit unit );
    public:
                                   CellPrinter               ( QWidget* parent=NULL );
      virtual                     ~CellPrinter               ();
      inline  CellWidget*          getCellWidget             ();
      inline  const CellWidget*    getCellWidget             () const;
              void                 setScreenCellWidget       ( CellWidget* );
      inline  void                 setMode                   ( int mode );
      inline  void                 setFitOnAbutmentBox       ( bool );
              void                 createLayout              ();
      inline  bool                 fitOnAbutmentBox          () const;
      inline  int                  resolution                () const;
      inline  int                  frameMargin               () const;
      inline  int                  cartoucheWidth            () const;
      inline  int                  cartoucheHeight           () const;
      inline  int                  titleHeight               () const;
              void                 pageDecorate              ( QPainter& );
              void                 toPdf                     ( QPrinter* , bool imageOnly=false );
      virtual std::string          _getString                () const;
    private:
              int                  _scalePixels              ( int pixels );

    protected:                     
      CellWidget*    _screenCellWidget;
      CellWidget*    _cellWidget;
      PaletteWidget* _palette;
      QPrinter*      _printer;
      int            _dpi;
      int            _mode;
      int            _paperWidth;
      int            _paperHeight;
      int            _frameMargin;
      int            _cartoucheWidth;
      int            _cartoucheHeight;
      int            _titleHeight;
      int            _xpaper;
      int            _ypaper;
      int            _drawingWidth;
      int            _drawingHeight;
      bool           _fitOnAbutmentBox;
  };


// Inline Functions.
  inline CellWidget*       CellPrinter::getCellWidget       () { return _cellWidget; }
  inline const CellWidget* CellPrinter::getCellWidget       () const { return _cellWidget; }
  inline void              CellPrinter::setMode             ( int mode ) { _mode = mode; }
  inline void              CellPrinter::setFitOnAbutmentBox ( bool state ) { _fitOnAbutmentBox = state; }
  inline int               CellPrinter::resolution          () const { return (_mode == CellWidget::Res_CellMode) ? 150 : 300; }
  inline bool              CellPrinter::fitOnAbutmentBox    () const { return _fitOnAbutmentBox; }
  inline int               CellPrinter::frameMargin         () const { return _frameMargin*_mode; }
  inline int               CellPrinter::cartoucheWidth      () const { return _cartoucheWidth*_mode; }
  inline int               CellPrinter::cartoucheHeight     () const { return _cartoucheHeight*_mode; }
  inline int               CellPrinter::titleHeight         () const { return _titleHeight*_mode; }


} // Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::CellPrinter)
IOSTREAM_POINTER_SUPPORT(Hurricane::CellPrinter)


#endif
