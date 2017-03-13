// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CellPrinter.cpp"                             |
// +-----------------------------------------------------------------+


#include  <unistd.h>
#include  <algorithm>
#include  <sstream>
#include  <boost/bind.hpp>
#include  <QApplication>
#include  <QPrinter>
#include  "vlsisapd/configuration/Configuration.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/CellPrinter.h"
#include  "hurricane/viewer/PaletteWidget.h"


namespace Hurricane {


  CellPrinter::CellPrinter ( QWidget* parent ) : QMainWindow      (parent)
                                               , _screenCellWidget(NULL)
                                               , _cellWidget      (NULL)
                                               , _palette         (NULL)
                                               , _printer         (NULL)
                                               , _mode            (Cfg::getParamEnumerate("viewer.printer.mode",1)->asInt())
                                               , _paperWidth      (0)
                                               , _paperHeight     (0)
                                               , _frameMargin     (50)
                                               , _cartoucheWidth  (1000)
                                               , _cartoucheHeight (90)   // 40*2
                                               , _titleHeight     (60)   // 30*2
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

    _cellWidget->bindToPalette( _palette );
    _cellWidget->refresh();
  }


  void  CellPrinter::setScreenCellWidget ( CellWidget* cellWidget )
  {
    _screenCellWidget = cellWidget;

    shared_ptr<CellWidget::State> clone ( _screenCellWidget->getStateClone() );
    _cellWidget->setState       ( clone );
    _cellWidget->setLayerVisible("grid"          , _screenCellWidget->isLayerVisible("grid"          ));
    _cellWidget->setLayerVisible("text.instance" , _screenCellWidget->isLayerVisible("text.instance" ));
    _cellWidget->setLayerVisible("text.component", _screenCellWidget->isLayerVisible("text.component"));
    _cellWidget->setLayerVisible("rubber"        , _screenCellWidget->isLayerVisible("rubber"        ));
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
    int  right          = 0;
    int  bottom         = 0;
    int  userFieldWidth = 150 * _mode;
    int  dateFieldWidth = 180 * _mode;
    int  unitFieldWidth = 150 * _mode;
    int  areaFieldWidth = cartoucheWidth() - userFieldWidth - dateFieldWidth - unitFieldWidth;

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
    if (    (_printer->paperSize  () == QPrinter::A4)
        and (_printer->orientation() == QPrinter::Landscape) ) {
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
    cartouchePen.setWidth( 2 );
    painter.setPen  ( cartouchePen );
    painter.drawRect( cartoucheRect );

  // The title & horizontal separator.
    string title = getString(_cellWidget->getCell()->getName());

    cartouchePen.setWidth( 1 );
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
    if (printer == NULL) return;
    if (_cellWidget->getCell() == NULL) return;

    _printer = printer;
  //_printer->setResolution ( resolution() );
    _printer->setResolution ( 150 );
    _printer->setPageMargins( 0.0, 0.0, 0.0, 0.0, QPrinter::DevicePixel );

    _paperWidth    = _printer->width ();
    _paperHeight   = _printer->height ();
  //_drawingWidth  = _paperWidth /4 - (frameMargin()<<1);
  //_drawingHeight = _paperHeight/4 - (frameMargin()<<1);
    _drawingWidth  = _paperWidth  - (frameMargin()<<1);
    _drawingHeight = _paperHeight - (frameMargin()<<1);
    _xpaper        = (imageOnly) ? 0 : frameMargin();
    _ypaper        = (imageOnly) ? 0 : frameMargin();

  // Substract the cartouche size only for A4 format.
    if ( _printer->orientation() == QPrinter::Landscape ) {
      _drawingWidth -= cartoucheHeight();
    } else {
      _drawingHeight -= cartoucheHeight();
    }

  // Compute the delta size between CellPrinter and CellWidget.
  //cerr << "Printer/Screen settings:" << endl;
  //cerr << "  Paper size:              " << _printer->paperSize() << endl;
  //cerr << "  Resolution (DPI):        " << _printer->resolution() << " (screen:" << resolution() << ")" << endl;   
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

    resize ( _drawingWidth, _drawingHeight );

    Box visibleArea = _screenCellWidget->getVisibleArea();
    if ( visibleArea.contains(_screenCellWidget->getCell()->getAbutmentBox()) ) {
    //cerr << "fit to AB" << endl;
      setFitOnAbutmentBox( true );
      _cellWidget->fitToContents();
    } else {
    //cerr << "reframe" << endl;
      _cellWidget->reframe( _screenCellWidget->getVisibleArea() );
    }

  //cerr << "  After resize CellWidget: " << _cellWidget->geometry().width() << "x" << _cellWidget->geometry().height() << endl;
  //cerr << "  VisibleArea(printer):    " << _cellWidget->getVisibleArea() << endl;
  //cerr << "  VisibleArea(screen):     " << _screenCellWidget->getVisibleArea() << endl;
    
  //cerr << "  (xpaper,ypaper) =        (" << _xpaper << "," << _ypaper << ")"  << endl;
  //cerr << "  (w,h) =                  (" << w << "," << h << ")"  << endl;
  //cerr << "  (dw,dh) =                (" << _drawingWidth << "," << _drawingHeight << ")"  << endl;

    CellWidget::PainterCb_t cb = boost::bind( &CellPrinter::pageDecorate, this, _1 );
    _cellWidget->copyToPrinter( _xpaper, _ypaper, _printer, cb );

    _printer = NULL;
  }


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
