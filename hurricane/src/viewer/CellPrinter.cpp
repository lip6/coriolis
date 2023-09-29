// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2012-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CellPrinter.cpp"                             |
// +-----------------------------------------------------------------+


#include <unistd.h>
#include <algorithm>
#include <functional>
#include <sstream>
#include <QApplication>
#include <QPrinter>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/DataBase.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/ExtensionSlice.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/CellPrinter.h"
#include "hurricane/viewer/PaletteWidget.h"


namespace Hurricane {


  CellPrinter::CellPrinter ( QWidget* parent )
    : QMainWindow      (parent)
    , _screenCellWidget(NULL)
    , _cellWidget      (NULL)
    , _palette         (NULL)
    , _printer         (NULL)
    , _dpi             (Cfg::getParamInt      ("viewer.printer.DPI" ,150)->asInt())
    , _mode            (Cfg::getParamEnumerate("viewer.printer.mode",  1)->asInt())
    , _paperWidth      (0)
    , _paperHeight     (0)
    , _frameMargin     (_scalePixels(  50))  // Dimensions are in pixels,
    , _cartoucheWidth  (_scalePixels(1000))  // computed for a reference DPI of 150.
    , _cartoucheHeight (_scalePixels(  90))
    , _titleHeight     (_scalePixels(  60))
    , _xpaper          (0)
    , _ypaper          (0)
    , _drawingWidth    (0)
    , _drawingHeight   (0)
    , _fitOnAbutmentBox(false)
  {
    setObjectName("viewer.printer");
    setAttribute (Qt::WA_DontShowOnScreen);
    createLayout ();
    show         ();
  }


  CellPrinter::~CellPrinter ()
  {
    delete _palette;
  }


  void  CellPrinter::createLayout ()
  {
    if (_cellWidget) return;

    _cellWidget = new CellWidget   ();
    _palette    = new PaletteWidget();

    setCorner( Qt::TopLeftCorner    , Qt::LeftDockWidgetArea  );
    setCorner( Qt::BottomLeftCorner , Qt::LeftDockWidgetArea  );
    setCorner( Qt::TopRightCorner   , Qt::RightDockWidgetArea );
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

    setCentralWidget( _cellWidget );
    _palette->readGraphics ();

    _cellWidget->setPixelThreshold( 1 );
    _cellWidget->setPrinter( true );
    _cellWidget->bindToPalette( _palette );
    _cellWidget->refresh();
  }


  void  CellPrinter::setScreenCellWidget ( CellWidget* cellWidget )
  {
    _screenCellWidget = cellWidget;

    array<string,14> labels = {{ string("fallback"        )
                               , string("rubber"          )
                               , string("phantom"         )
                               , string("boundaries"      )
                               , string("marker"          )
                               , string("grid"            )
                               , string("spot"            )
                               , string("ghost"           )
                               , string("text.ruler"      )
                               , string("text.cell"       )
                               , string("text.instance"   )
                               , string("text.components" )
                               , string("text.references" )
                               , string("undef"           )
                              }};

    for ( string label : labels )
      _cellWidget->setLayerVisible( label
                                  , _screenCellWidget->isLayerVisible(label) );

    for ( const BasicLayer* layer : DataBase::getDB()->getTechnology()->getBasicLayers() )
      _cellWidget->setLayerVisible( layer->getName()
                                  , _screenCellWidget->isLayerVisible( layer->getName() ));

    shared_ptr<CellWidget::State> clone ( _screenCellWidget->getStateClone() );
    _cellWidget->setState( clone );

    _cellWidget->copyDrawExtensionGos( _screenCellWidget );

    for ( ExtensionSlice* extension : cellWidget->getCell()->getExtensionSlices() )
      _cellWidget->setLayerVisible( extension->getName()
                                  , _screenCellWidget->isLayerVisible( extension->getName() ));
  }


  QString  CellPrinter::getUnitQString ()
  {
    unsigned int   mode;
    DbU::UnitPower p;

    DbU::getStringMode( mode, p );

    QString unit = "unit: ";
    switch( mode ) {
      case DbU::Db:       unit += "db";          break;
      case DbU::Grid:     unit += "grid";        break;
      case DbU::Symbolic: unit += (QChar)0x03BB; break;
      case DbU::Physical: 
        switch( p ) {
          case DbU::Pico:  unit += (QChar)'p';    break;
          case DbU::Nano:  unit += (QChar)'n';    break;
          case DbU::Micro: unit += (QChar)0x00B5; break;
          case DbU::Milli: unit += (QChar)'m';    break;
          case DbU::Unity: break;
          case DbU::Kilo:  unit += (QChar)'k';    break;
          default:         unit += (QChar)'?';    break;
        }
        unit += (QChar)'m';
    }

    return unit;
  }


  QString  CellPrinter::dbuToPrinterString ( DbU::Unit unit )
  {
    QString value = DbU::getValueString(unit).c_str();

    if (not value[value.size()-2].isDigit()) value.chop(2);
    if (not value[value.size()-1].isDigit()) value.chop(1);

    return value;
  }


  string  CellPrinter::getUserName ()
  {
    string user = getenv("USER");
    if (user.empty()) user = "unkown";
    else {
      size_t equal = user.find('=');
      if (equal != string::npos)
        user.erase( 0, equal );
    }
    return user;
  }


  string  CellPrinter::getTime ( const char* format )
  {
    time_t     t  = time(NULL);
    struct tm* bt = localtime(&t);
    char       formatted[1024];
    
    strftime ( formatted, 1024, format, bt );

    return formatted;
  }


  void  CellPrinter::pageDecorate ( QPainter& painter )
  {
    int  thinWidth       = _scalePixels(    1 );
    int  thickWidth      = _scalePixels(    2 );
    int  right           = 0;
    int  bottom          = 0;
    int  userFieldWidth  = _scalePixels( 150 * _mode );
    int  dateFieldWidth  = _scalePixels( 180 * _mode );
    int  unitFieldWidth  = _scalePixels( 150 * _mode );
    int  areaFieldWidth  = cartoucheWidth() - userFieldWidth - dateFieldWidth - unitFieldWidth;

    QFont font ( "Bitstream Vera Sans", 18 );
    font.setWeight ( QFont::Bold );

  // Draw frame.
    QPen framePen = QPen( QBrush(QColor("black")), 1.0 );
    painter.setPen  ( framePen );
    painter.drawRect( frameMargin()
                    , frameMargin()
                    , _paperWidth  - (frameMargin()<<1)
                    , _paperHeight - (frameMargin()<<1)
                    );
  // Draw a rectangle around the copied area from the CellWidget buffer.
  // painter.drawRect( _xpaper
  //                 , _xpaper
  //                 , _drawingWidth
  //                 , _drawingHeight
  //                 );

  // Rotate the painter for the cartouche if in A4+Landscape mode.
    if (    (_printer->pageLayout().pageSize().id() == QPageSize::A4)
       and (_printer->pageLayout().orientation()    == QPageLayout::Landscape) ) {
      painter.translate ( _paperWidth - frameMargin(), frameMargin() );
      painter.rotate ( -90 );
    } else
      painter.translate ( _paperWidth - frameMargin(), _paperHeight - frameMargin() );

  // Draw the cartouche.
    QPen cartouchePen = QPen( QBrush(QColor("black")), 1.0 );
    painter.setPen( cartouchePen );

    QRect cartoucheRect = QRect ( right  - cartoucheWidth()
                                , bottom - cartoucheHeight()
                                , cartoucheWidth()
                                , cartoucheHeight()
                                );
    QRect titleRect = cartoucheRect;
    titleRect.adjust( 0, 0, 0,  titleHeight() - cartoucheHeight() );

  // The cartouche box.
    cartouchePen.setWidth( thickWidth );
    painter.setPen  ( cartouchePen );
    painter.drawRect( cartoucheRect );

  // The title & horizontal separator.
    string title = getString(_cellWidget->getCell()->getName());

    cartouchePen.setWidth( thinWidth );
    painter.setPen  ( cartouchePen );
    painter.drawLine( titleRect.bottomLeft(), titleRect.bottomRight() );
    painter.setFont ( font );
    painter.drawText( titleRect, Qt::AlignVCenter|Qt::AlignHCenter, title.c_str() );

  // The user's name.
    QRect fieldRect = QRect ( cartoucheRect.x()
                            , cartoucheRect.y() + titleHeight()
                            , userFieldWidth
                            , cartoucheHeight() - titleHeight()
                            );
    font.setPointSize( 11 );
    painter.setFont  ( font );
    painter.drawLine ( fieldRect.topRight(), fieldRect.bottomRight() );
    painter.drawText ( fieldRect, Qt::AlignVCenter|Qt::AlignHCenter, getUserName().c_str() );

  // The date.
    string date = getTime ( "%d %b %Y" );
    fieldRect = QRect ( cartoucheRect.x() + userFieldWidth
                      , cartoucheRect.y() + titleHeight()
                      , dateFieldWidth
                      , cartoucheHeight() - titleHeight()
                      );
    font.setWeight  ( QFont::Normal );
    painter.setFont ( font );
    painter.drawLine( fieldRect.topRight(), fieldRect.bottomRight() );
    painter.drawText( fieldRect, Qt::AlignVCenter|Qt::AlignHCenter, date.c_str() );

  // The Unit.
    QString unit = getUnitQString();
    fieldRect = QRect ( cartoucheRect.x() + userFieldWidth + dateFieldWidth
                      , cartoucheRect.y() + titleHeight()
                      , unitFieldWidth
                      , cartoucheHeight() - titleHeight()
                      );
    font.setWeight  ( QFont::Normal );
    painter.setFont ( font );
    painter.drawLine( fieldRect.topRight(), fieldRect.bottomRight() );
    painter.drawText( fieldRect, Qt::AlignVCenter|Qt::AlignHCenter, unit );

  // The Area.
    Box     area;
    QString sarea;
    if (fitOnAbutmentBox()) {
      area   = _cellWidget->getCell()->getAbutmentBox();
      sarea += "A.Box. (";
    } else {
      area   = _cellWidget->getVisibleArea();
      sarea += "area (";
    }

    sarea += dbuToPrinterString( area.getXMin() ) + " "
          +  dbuToPrinterString( area.getYMin() ) + ") ("
          +  dbuToPrinterString( area.getXMax() ) + " "
          +  dbuToPrinterString( area.getYMax() ) + ")";

    fieldRect = QRect ( cartoucheRect.x() + userFieldWidth + dateFieldWidth + unitFieldWidth
                      , cartoucheRect.y() + titleHeight()
                      , areaFieldWidth
                      , cartoucheHeight() - titleHeight()
                      );
  //painter.drawLine( fieldRect.topRight(), fieldRect.bottomRight() );
    painter.drawText( fieldRect, Qt::AlignVCenter|Qt::AlignHCenter, sarea );
  }


  void  CellPrinter::toPdf ( QPrinter* printer, bool imageOnly )
  {
  //int screenResolution = resolution();

    if (printer == NULL) return;
    if (_cellWidget->getCell() == NULL) return;

    _printer = printer;
    _printer->setResolution ( _dpi );
    _printer->setPageMargins( QMarginsF(0.0, 0.0, 0.0, 0.0), QPageLayout::Millimeter );

    _paperWidth    = _printer->width ();
    _paperHeight   = _printer->height ();
    _drawingWidth  = _paperWidth  - (frameMargin()<<1);
    _drawingHeight = _paperHeight - (frameMargin()<<1);
    _xpaper        = (imageOnly) ? 0 : frameMargin();
    _ypaper        = (imageOnly) ? 0 : frameMargin();

  // Substract the cartouche size only for A4 format.
    if ( _printer->pageLayout().orientation() == QPageLayout::Landscape ) {
      _drawingWidth -= cartoucheHeight();
    } else {
      _drawingHeight -= cartoucheHeight();
    }

  // Compute the delta size between CellPrinter and CellWidget.
  //cerr << "Printer/Screen settings:" << endl;
  //cerr << "  Paper size:              " << _printer->paperSize() << endl;
  //cerr << "  Resolution (DPI):        " << _printer->resolution() << " (screen:" << screenResolution << ")" << endl;   
  //cerr << "  (paperw,paperh) =        (" << _paperWidth << "," << _paperHeight << ")"  << endl;
  //cerr << "  CellPrinter:             " << geometry().width() << "x" << geometry().height() << endl;
  //cerr << "  CellWidget:              " << _cellWidget->geometry().width() << "x" << _cellWidget->geometry().height() << endl;
  //cerr << "  ScreenCellWidget:        " << _screenCellWidget->geometry().width() << "x" << _screenCellWidget->geometry().height() << endl;
  //cerr << "  resize:                  " << _drawingWidth << "x" << _drawingHeight << endl;
  //cerr << "  AbutmentBox:             " << _cellWidget->getCell()->getAbutmentBox() << endl;
  //cerr << "  visibleArea:             " << _cellWidget->getVisibleArea() << endl;

  // Resize the pixel window to the size of the paper usable area.
  // Then redraw the same visible area (in Hurricane coordinates).
  // if (_printer->orientation() == QPrinter::Landscape) {
  //   swap( _drawingHeight, _drawingWidth );
  //   swap( _xpaper       , _ypaper       );
  // }

    resize( _drawingWidth, _drawingHeight );

    Box visibleArea = _screenCellWidget->getVisibleArea();
    if ( visibleArea.contains(_screenCellWidget->getCell()->getAbutmentBox()) ) {
      setFitOnAbutmentBox( true );
      _cellWidget->fitToContents();
    } else {
      _cellWidget->reframe( _screenCellWidget->getVisibleArea() );
    }

  //cerr << "  After resize CellWidget: " << _cellWidget->geometry().width() << "x" << _cellWidget->geometry().height() << endl;
  //cerr << "  VisibleArea(printer):    " << _cellWidget->getVisibleArea() << endl;
  //cerr << "  VisibleArea(screen):     " << _screenCellWidget->getVisibleArea() << endl;
  //
  //cerr << "  (xpaper,ypaper) =        (" << _xpaper << "," << _ypaper << ")"  << endl;
  //cerr << "  (dw,dh) =                (" << _drawingWidth << "," << _drawingHeight << ")"  << endl;

    CellWidget::PainterCb_t cb = std::bind( &CellPrinter::pageDecorate, this, std::placeholders::_1 );
    _cellWidget->copyToPrinter( _xpaper, _ypaper, _printer, cb );

    _printer = NULL;
  }


  int  CellPrinter::_scalePixels ( int pixels )
  { return (int)( (float)_dpi/150.0 * (float)pixels ); }


  string  CellPrinter::_getString () const
  {
    ostringstream s;
    s << "<CellPrinter ";
  //Cell* cell = getCellWidget()->getState()->getCell();
  //if (cell) s << getString(cell->getName());
  //else      s << "No_Cell_Loaded";
    s << ">";
    return s.str();
  }


} // End of Hurricane namespace.
