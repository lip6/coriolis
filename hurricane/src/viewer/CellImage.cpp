
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CellImage.cpp"                               |
// +-----------------------------------------------------------------+


#include  <unistd.h>
#include  <algorithm>
#include  <sstream>
#include  <boost/bind.hpp>
#include  <QApplication>
#include  <QImage>
#include  "vlsisapd/configuration/Configuration.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/CellImage.h"
#include  "hurricane/viewer/PaletteWidget.h"


namespace Hurricane {


  CellImage::CellImage ( QWidget* parent ) : QMainWindow      (parent)
                                           , _screenCellWidget(NULL)
                                           , _cellWidget      (NULL)
                                           , _palette         (NULL)
                                           , _image           (NULL)
                                           , _mode            (Cfg::getParamEnumerate("viewer.printer.mode",1)->asInt())
                                           , _flags           (0)
                                           , _fitOnAbutmentBox(false)
                                           , _drawingWidth    (0)
                                           , _drawingHeight   (0)
  {
    setObjectName("viewer.image");
    setAttribute (Qt::WA_DontShowOnScreen);
    createLayout ();
    show         ();
  }


  CellImage::~CellImage ()
  {
    delete _palette;
  }


  void  CellImage::createLayout ()
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


  void  CellImage::setScreenCellWidget ( CellWidget* cellWidget )
  {
    _screenCellWidget = cellWidget;

    shared_ptr<CellWidget::State> clone ( _screenCellWidget->getStateClone() );
    _cellWidget->setState       ( clone );
    _cellWidget->setLayerVisible("grid"          , _screenCellWidget->isLayerVisible("grid"          ));
    _cellWidget->setLayerVisible("text.instance" , _screenCellWidget->isLayerVisible("text.instance" ));
    _cellWidget->setLayerVisible("text.component", _screenCellWidget->isLayerVisible("text.component"));
    _cellWidget->setLayerVisible("rubber"        , _screenCellWidget->isLayerVisible("rubber"        ));
  }


  void  CellImage::pageDecorate ( QPainter& painter )
  {
    if ( _flags & ShowScale ) {
      int xGradient = (_drawingWidth-510)/2;
      painter.setPen  (Qt::white);
      painter.drawRect(xGradient-1, _drawingHeight+9, 512, 31);
      painter.setPen  (Qt::NoPen);

      for ( unsigned i = 0 ; i < 256 ; i++ ) {
        painter.setBrush( Graphics::getColorScale(ColorScale::Fire).getBrush(i,100) );
        painter.drawRect( xGradient+(i*2), _drawingHeight+10, 2, 30 );
        if ( i==0 or i==51 or i==102 or i==153 or i==204 or i==255 ) {
          QRect tArea ( xGradient+(i*2)-15, _drawingHeight+44, 30, 12 );

          std::ostringstream oss;
          oss << (float)(i)/255;

          painter.setPen  ( Qt::white);
          painter.drawLine( xGradient+(i*2)
                          , _drawingHeight+38
                          , xGradient+(i*2)
                          , _drawingHeight+42 );
          painter.drawText( tArea, Qt::AlignCenter, oss.str().c_str() );
          painter.setPen  ( Qt::NoPen );
        }
      }
    }
  }


  QImage* CellImage::toImage ( unsigned int flags )
  {
    if (_image != NULL) return NULL;
    if (_cellWidget->getCell() == NULL) return NULL;

    _flags = flags;

  //int  scale = 80 * Cfg::getParamEnumerate("viewer.printer.mode")->asInt();
    int  scale = (Graphics::isHighDpi()) ? 1 : 2;
    _drawingWidth  = _cellWidget->width () * scale;
    _drawingHeight = _cellWidget->height() * scale;

    _image = new QImage( _drawingWidth
                       , _drawingHeight + ((_flags&ShowScale) ? 60 : 0)
                       , QImage::Format_RGB32 );

// Compute the delta size between CellPrinter and CellWidget.
  cerr << "Image/Screen settings:" << endl;
//cerr << "  Screen Resolution (DPI): " << resolution() << endl;   
  cerr << "  CellImage:               " << geometry().width() << "x" << geometry().height() << endl;
  cerr << "  CellWidget:              " << _cellWidget->geometry().width() << "x" << _cellWidget->geometry().height() << endl;
  cerr << "  ScreenCellWidget:        " << _screenCellWidget->geometry().width() << "x" << _screenCellWidget->geometry().height() << endl;
  cerr << "  resize:                  " << _drawingWidth << "x" << _drawingHeight << endl;
  cerr << "  AbutmentBox:             " << _cellWidget->getCell()->getAbutmentBox() << endl;
  cerr << "  visibleArea:             " << _cellWidget->getVisibleArea() << endl;

  // Compute the delta size between CellImage and CellWidget.
    // cerr << "CellImage:"   << geometry().width() << "x" << geometry().height() << endl;
    // cerr << "CellWidget: " << _cellWidget->geometry().width() << "x" << _cellWidget->geometry().height() << endl;
    // cerr << "resize:     " << _drawingWidth << "x" << _drawingHeight << endl;
    // cerr << "AbutmentBox:" << _cellWidget->getCell()->getAbutmentBox() << endl;
    // cerr << "visibleArea:" << _cellWidget->getVisibleArea() << endl;

    resize( _drawingWidth, _drawingHeight );

    Box visibleArea = _screenCellWidget->getVisibleArea();
    if ( visibleArea.contains(_screenCellWidget->getCell()->getAbutmentBox()) ) {
      setFitOnAbutmentBox( true );
      _cellWidget->fitToContents();
    } else {
      _cellWidget->reframe( _screenCellWidget->getVisibleArea() );
    }

    cerr << "  After resize CellWidget: " << _cellWidget->geometry().width() << "x" << _cellWidget->geometry().height() << endl;
    cerr << "  VisibleArea(printer):    " << _cellWidget->getVisibleArea() << endl;
    cerr << "  VisibleArea(screen):     " << _screenCellWidget->getVisibleArea() << endl;

  //cerr << "(xpaper,ypaper) = (" << xpaper << "," << ypaper << ")"  << endl;
  //cerr << "(w,h) = (" << w << "," << h << ")"  << endl;
  //cerr << "(dw,dh) = (" << drawingWidth << "," << drawingHeight << ")"  << endl;

    CellWidget::PainterCb_t cb = boost::bind( &CellImage::pageDecorate, this, _1 );
    _cellWidget->copyToImage( _image, cb );

    return _image;
  }


  string  CellImage::_getString () const
  {
    ostringstream s;
    s << "<CellImage ";
  //Cell* cell = getCellWidget()->getState()->getCell();
  //if (cell) s << getString(cell->getName());
  //else      s << "No_Cell_Loaded";
    s << ">";
    return s.str();
  }


} // End of Hurricane namespace.
