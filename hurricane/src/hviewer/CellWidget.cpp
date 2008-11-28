
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
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./CellWidget.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>
#include <QPainter>
#include <QStylePainter>
#include <QBitmap>
#include <QLabel>

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Slice.h"
#include "hurricane/Segment.h"
#include "hurricane/Contact.h"
#include "hurricane/Pad.h"
#include "hurricane/RoutingPad.h"

#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/PaletteItem.h"
#include "hurricane/viewer/PaletteWidget.h"
// #include "MapView.h"
#include "hurricane/viewer/Command.h"
#include "hurricane/viewer/CellWidget.h"




namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::Spot".


  CellWidget::Spot::Spot ( CellWidget* cw )
    : _cellWidget(cw)
    , _spotPoint()
    , _restore(false)
    , _showSpot(true)
  { }


  void  CellWidget::Spot::restore ()
  {
    if ( _restore ) {
      _cellWidget->getDrawingPlanes().copyToScreen ( _spotPoint.x()-5, _spotPoint.y()-5, 10, 10 );
      _restore = false;
    }
  }


  void  CellWidget::Spot::setRestore ( bool state )
  { _restore = state; }


  void  CellWidget::Spot::moveTo ( const QPoint& screenPoint )
  {
    restore ();
    if ( !_showSpot ) return;

    _restore = true;

    QPainter& screenPainter = _cellWidget->getDrawingPlanes().painter(2);

    Point mousePoint = _cellWidget->screenToDbuPoint ( screenPoint );
    Point  spotPoint = Point ( DbU::getOnSnapGrid(mousePoint.getX())
                             , DbU::getOnSnapGrid(mousePoint.getY())
                             );
    QPoint center = _cellWidget->dbuToScreenPoint(spotPoint);

    screenPainter.setPen ( Graphics::getPen("spot") );
    screenPainter.drawRect ( center.x()-3, center.y()-3, 6, 6 );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::Drawingplanes".


  CellWidget::DrawingPlanes::DrawingPlanes ( const QSize& size, CellWidget* cw )
    : _cellWidget(cw)
    , _normalPen()
    , _linePen()
    , _workingPlane(0)
    , _lineMode(false)
  {
    for ( size_t i=0 ; i<2 ; i++ )
      _planes[i] = new QPixmap ( size );
  }


  CellWidget::DrawingPlanes::~DrawingPlanes ()
  {
    for ( size_t i=0 ; i<2 ; i++ ) {
      if ( _painters[i].isActive() ) _painters[i].end();
      delete _planes[i];
    }
  }


  void  CellWidget::DrawingPlanes::setPen ( const QPen& pen )
  {
    _normalPen = pen;
    _linePen   = pen;
    _linePen.setStyle ( Qt::SolidLine );
    _linePen.setWidth ( 1 );
 
    if ( _lineMode ) {
      _painters[0].setPen ( _linePen );
      _painters[1].setPen ( _linePen );
    } else {
      _painters[0].setPen ( _normalPen );
      _painters[1].setPen ( _normalPen );
    }
  }


  void  CellWidget::DrawingPlanes::setBrush ( const QBrush& brush )
  {
    _painters[0].setBrush ( brush );
    _painters[1].setBrush ( brush );
  }


  void  CellWidget::DrawingPlanes::setBackground ( const QBrush& brush )
  {
    _painters[0].setBackground ( brush );
    _painters[1].setBackground ( brush );
  }


  void  CellWidget::DrawingPlanes::setBackgroundMode ( Qt::BGMode mode )
  {
    _painters[0].setBackgroundMode ( mode );
    _painters[1].setBackgroundMode ( mode );
  }


  void  CellWidget::DrawingPlanes::setLineMode ( bool mode )
  {
    if ( _lineMode != mode ) {
      _lineMode = mode;
      if ( _lineMode ) painter().setPen ( _linePen );
      else             painter().setPen ( _normalPen );
    }
  }


  void  CellWidget::DrawingPlanes::resize ( const QSize& size )
  {
    for ( size_t i=0 ; i<2 ; i++ ) {
      bool activePainter = _painters[i].isActive();

      if ( activePainter ) _painters[i].end();
      delete _planes[i];

      _planes[i] = new QPixmap ( size );
      if ( activePainter ) _painters[i].begin ( _planes[i] );
    }
  }


  void  CellWidget::DrawingPlanes::shiftLeft ( int dx )
  {
    paintersBegin ();
    _painters[0].drawPixmap ( dx, 0, *_planes[0], 0, 0, width()-dx, height() );
    _painters[1].drawPixmap ( dx, 0, *_planes[1], 0, 0, width()-dx, height() );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::shiftRight ( int dx )
  {
    paintersBegin ();
    _painters[0].drawPixmap ( 0, 0, *_planes[0], dx, 0, width()-dx, height() );
    _painters[1].drawPixmap ( 0, 0, *_planes[1], dx, 0, width()-dx, height() );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::shiftUp ( int dy )
  {
    paintersBegin ();
    _painters[0].drawPixmap ( 0, dy, *_planes[0], 0, 0, width(), height()-dy );
    _painters[1].drawPixmap ( 0, dy, *_planes[1], 0, 0, width(), height()-dy );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::shiftDown ( int dy )
  {
    paintersBegin ();
    _painters[0].drawPixmap ( 0, 0, *_planes[0], 0, dy, width(), height()-dy );
    _painters[1].drawPixmap ( 0, 0, *_planes[1], 0, dy, width(), height()-dy );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::copyToSelect ( int sx, int sy, int w, int h )
  {
    painterBegin ( 1 );
    _painters[1].setPen        ( Qt::NoPen );
    _painters[1].setBackground ( Graphics::getBrush("background") );
    _painters[1].eraseRect     ( sx, sy, w, h );
  //_painters[1].setOpacity    ( 0.5 );
    _painters[1].drawPixmap    ( sx, sy, *_planes[0], sx, sy, w, h );
    painterEnd ( 1 );
  }


  void  CellWidget::DrawingPlanes::copyToScreen ( int sx, int sy, int w, int h )
  {
    if ( _cellWidget->showSelection() )
      _painters[2].drawPixmap ( sx, sy
                              , *_planes[1]
                              , _cellWidget->getOffsetVA().rx()+sx, _cellWidget->getOffsetVA().ry()+sy
                              , w, h
                              );
    else
      _painters[2].drawPixmap ( sx, sy
                              , *_planes[0]
                              , _cellWidget->getOffsetVA().rx()+sx, _cellWidget->getOffsetVA().ry()+sy
                              , w, h
                              );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::DrawingQuery".


  CellWidget::DrawingQuery::DrawingQuery ( CellWidget* widget )
    : Query()
    , _cellWidget(widget)
    , _drawExtensionGo(NULL)
  { }


  void  CellWidget::DrawingQuery::setDrawExtensionGo ( const Name& name )
  {
    map<Name,pair<InitExtensionGo_t*,DrawExtensionGo_t*> >::iterator idraw
      = _drawExtensionGos.find ( name );

    if ( idraw != _drawExtensionGos.end() ) {
      _drawExtensionGo = idraw->second.second;
      if ( idraw->second.first )
        idraw->second.first ( _cellWidget );
    } else
      _drawExtensionGo = NULL;
  }


  bool  CellWidget::DrawingQuery::hasMasterCellCallback () const
  {
    return true;
  }


  void  CellWidget::DrawingQuery::masterCellCallback ()
  {
    Box bbox = getTransformation().getBox(getMasterCell()->getAbutmentBox());
    _cellWidget->drawBox ( bbox );
  }


  bool  CellWidget::DrawingQuery::hasGoCallback () const
  {
    return true;
  }


  void  CellWidget::DrawingQuery::goCallback ( Go* go )
  {
    drawGo ( go, getBasicLayer(), getArea(), getTransformation() );
  }


  void  CellWidget::DrawingQuery::drawGo ( const Go*              go
                                         , const BasicLayer*      basicLayer
                                         , const Box&             area
                                         , const Transformation&  transformation
                                         )
  {
  //cerr << "DrawingQuery::drawGo() - " << go << endl;

    const Segment* segment = dynamic_cast<const Segment*>(go);
    if ( segment ) {
      if ( 1 < _cellWidget->dbuToDisplayLength(segment->getWidth()) ) {
        _cellWidget->drawBox ( transformation.getBox(segment->getBoundingBox(basicLayer)) );
      } else {
        _cellWidget->drawLine ( transformation.getPoint(segment->getSourcePosition())
                              , transformation.getPoint(segment->getTargetPosition()) );
      }
      return;
    }

    const Contact* contact = dynamic_cast<const Contact*>(go);
    if ( contact ) {
      _cellWidget->drawBox ( transformation.getBox(contact->getBoundingBox(basicLayer)) );
      return;
    }

    const Pad* pad = dynamic_cast<const Pad*>(go);
    if ( pad ) {
      _cellWidget->drawBox ( transformation.getBox(pad->getBoundingBox(basicLayer)) );
      return;
    }

    const RoutingPad* rp = dynamic_cast<const RoutingPad*>(go);
    if ( rp ) {
      _cellWidget->drawBox ( transformation.getBox(rp->getBoundingBox(basicLayer)) );
      return;
    }
  }


  bool  CellWidget::DrawingQuery::hasExtensionGoCallback () const
  {
    return true;
  }


  void  CellWidget::DrawingQuery::extensionGoCallback ( Go* go )
  {
    if ( _drawExtensionGo )
      _drawExtensionGo ( _cellWidget, go, getBasicLayer(), getArea(), getTransformation() );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::TextDrawingQuery".


  CellWidget::TextDrawingQuery::TextDrawingQuery ( CellWidget* widget )
    : Query()
    ,_cellWidget(widget)
  {
    setBasicLayer ( NULL );
    setFilter     ( Query::DoMasterCells|Query::DoTerminalCells );
    setStartLevel ( 0 );
    setStopLevel  ( 1 );
  }


  bool  CellWidget::TextDrawingQuery::hasMasterCellCallback () const
  { return true; }


  void  CellWidget::TextDrawingQuery::masterCellCallback ()
  {
    Box bbox = getTransformation().getBox(getMasterCell()->getAbutmentBox());
    if ( getDepth() == 2 )
      _cellWidget->drawText ( Point(bbox.getXMin(),bbox.getYMin())
                            , getInstance()->getName()
                            , -90
                            , true
                            );
  }


  bool  CellWidget::TextDrawingQuery::hasGoCallback () const
  { return false; }


  void  CellWidget::TextDrawingQuery::goCallback ( Go* go )
  { }


  bool  CellWidget::TextDrawingQuery::hasExtensionGoCallback () const
  { return false; }


  void  CellWidget::TextDrawingQuery::extensionGoCallback ( Go* go )
  { }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::SelectorCriterions".


  CellWidget::SelectorCriterions::SelectorCriterions ( CellWidget* cw )
    : _cellWidget(cw)
    , _criterions()
  { }


  CellWidget::SelectorCriterions::~SelectorCriterions ()
  {
    clear ();
  }


  bool  CellWidget::SelectorCriterions::add ( const Net* net, bool delayRedraw )
  {
    if ( !_cellWidget->isSelected(Occurrence(net)) ) {
      _criterions.push_back ( new NetSelectorCriterion(net) );
      _criterions.back()->doSelection ( _cellWidget, delayRedraw );
      return true;
    }
    return false;
  }


  bool  CellWidget::SelectorCriterions::add ( Box area )
  {
    _criterions.push_back ( new AreaSelectorCriterion(area) );
    _criterions.back()->doSelection ( _cellWidget, true );
    return true;
  }


  bool  CellWidget::SelectorCriterions::remove (  const Net* net, bool delayRedraw )
  {
    if ( !_cellWidget->isSelected(Occurrence(net)) ) return false;

    size_t i=0;
    for ( ; i<_criterions.size() ; i++ )
      if ( _criterions[i]->getNet() == net ) break;

    if ( i < _criterions.size() ) {
      swap ( _criterions[i], *(_criterions.end()-1) );
      _criterions.back()->undoSelection ( _cellWidget, delayRedraw );
      _criterions.pop_back ();
    } else
      return false;

    return true;
  }


  void  CellWidget::SelectorCriterions::clear ()
  {
    for ( size_t i=0 ; i<_criterions.size() ; i++ )
      delete _criterions[i];
    _criterions.clear ();
  }


  void  CellWidget::SelectorCriterions::revalidate ()
  {
    size_t i    = 0;
    size_t last = _criterions.size ();
    while ( i < last ) {
      if ( _criterions[i]->isValid(_cellWidget) ) {
        _criterions[i]->doSelection ( _cellWidget, true );
        ++i;
      } else
        swap ( _criterions[i], _criterions[--last] );
    }
    _criterions.erase ( _criterions.begin()+last, _criterions.end() );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget".


  const int  CellWidget::_stripWidth = 100;


  CellWidget::CellWidget ( QWidget* parent ) : QWidget(parent)
                                             , _technology(NULL)
                                             , _palette(NULL)
                                             , _displayArea(0,0,6*_stripWidth,6*_stripWidth)
                                             , _visibleArea(_stripWidth,_stripWidth,4*_stripWidth,4*_stripWidth)
                                             , _scale(1.0)
                                             , _offsetVA(_stripWidth,_stripWidth)
                                             , _drawingPlanes(QSize(6*_stripWidth,6*_stripWidth),this)
                                             , _drawingQuery(this)
                                             , _textDrawingQuery(this)
                                             , _queryFilter(Query::DoAll)
                                             , _mousePosition(0,0)
                                             , _spot(this)
                                             , _cell(NULL)
                                             , _showBoundaries(true)
                                             , _showSelection(false)
                                             , _cumulativeSelection(false)
                                             , _selectionHasChanged(false)
                                             , _delaySelectionChanged(0)
                                             , _cellModificated(true)
                                             , _selectors()
                                             , _selection(this)
                                             , _commands()
                                             , _redrawRectCount(0)
                                             , _textFontHeight(20)
  {
  //setBackgroundRole ( QPalette::Dark );
  //setAutoFillBackground ( false );
    setAttribute     ( Qt::WA_OpaquePaintEvent );
  //setAttribute     ( Qt::WA_NoSystemBackground );
  //setAttribute     ( Qt::WA_PaintOnScreen );
  //setAttribute     ( Qt::WA_StaticContents );
    setSizePolicy    ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy   ( Qt::StrongFocus );
    setMouseTracking ( true );

  //_mapView = new MapView ( this );
    DataBase* database = DataBase::getDB();
    if ( database )
      _technology = database->getTechnology ();

    QFont font = Graphics::getNormalFont();
    _textFontHeight = QFontMetrics(font).ascent();

    fitToContents ();
  }


  CellWidget::~CellWidget ()
  {
    cerr << "CellWidget::~CellWidget()" << endl;

    for ( size_t i=0 ; i<_commands.size() ; i++ )
      unbindCommand ( _commands[i] );
  }


  void  CellWidget::updatePalette ()
  {
    emit updatePalette(getCell());
  }


  void  CellWidget::bindToPalette ( PaletteWidget* palette )
  {
    detachFromPalette ();
    _palette = palette;

    connect ( _palette, SIGNAL(paletteChanged())    , this    , SLOT(redraw()) );
    connect ( this    , SIGNAL(cellChanged(Cell*))  , _palette, SLOT(updateExtensions(Cell*)) );
    connect ( this    , SIGNAL(updatePalette(Cell*)), _palette, SLOT(updateExtensions(Cell*)) );
  }


  void  CellWidget::detachFromPalette ()
  {
    if ( _palette ) {
      disconnect ( _palette, SIGNAL(paletteChanged()), this,  SLOT(redraw()) );
      disconnect ( this    , SIGNAL(cellChanged(Cell*))  , _palette, SLOT(updateExtensions(Cell*)) );
      disconnect ( this    , SIGNAL(updatePalette(Cell*)), _palette, SLOT(updateExtensions(Cell*)) );
      _palette = NULL;
    }
  }


  void  CellWidget::bindCommand ( Command* command )
  {
    for ( size_t i=0 ; i<_commands.size() ; i++ )
      if ( _commands[i] == command ) return;

    _commands.push_back ( command );
  }


  void  CellWidget::unbindCommand ( Command* command )
  {
    size_t i = 0;
    for ( ; i<_commands.size() ; i++ )
      if ( _commands[i] == command ) break;

    for ( ; i+1<_commands.size() ; i++ )
      _commands[i] = _commands[i+1];

    _commands.pop_back ();
  }


  void  CellWidget::pushCursor ( Qt::CursorShape cursor )
  {
    setCursor ( cursor );
    _cursors.push_back ( cursor );
  }


  void  CellWidget::popCursor ()
  {
    _cursors.pop_back ();
    if ( !_cursors.empty() )
      setCursor ( _cursors.back() );
    else
      unsetCursor ();
  }


  QSize  CellWidget::minimumSizeHint () const
  {
    return QSize(_stripWidth*4,_stripWidth*4);
  }


  void  CellWidget::setShowSelection ( bool state )
  {
    if ( state != _showSelection ) {
      _showSelection       = state;
      _selectionHasChanged = false;
      redraw ();

      emit showSelectionToggled ( state );
    }
  }


  void  CellWidget::setCumulativeSelection ( bool state )
  {
    _cumulativeSelection = state;
  }


  void  CellWidget::redraw ( QRect redrawArea )
  {
  //cerr << "CellWidget::redraw() - "
  //     << _selectionHasChanged << " filter:"
  //     << _queryFilter << endl;

    _redrawRectCount = 0;

    pushCursor ( Qt::BusyCursor );

    if ( ! ( _selectionHasChanged && _showSelection ) || _cellModificated ) {
      _spot.setRestore ( false );
      _drawingPlanes.select ( 0 );
      _drawingPlanes.painterBegin ();

      _drawingPlanes.painter().setPen        ( Qt::NoPen );
      _drawingPlanes.painter().setBackground ( Graphics::getBrush("background") );
      _drawingPlanes.painter().setClipRect   ( redrawArea );
      _drawingPlanes.painter().eraseRect     ( redrawArea );

      int darkening = (_showSelection) ? Graphics::getDarkening() : 100;

      if ( _cell ) {

        Box redrawBox = displayToDbuBox ( redrawArea );

        _drawingQuery.setExtensionMask  ( 0 );
        _drawingQuery.setArea           ( redrawBox );
        _drawingQuery.setTransformation ( Transformation() );

        forEach ( BasicLayer*, iLayer, _technology->getBasicLayers() ) {
          _drawingPlanes.setPen   ( Graphics::getPen  ((*iLayer)->getName(),darkening) );
          _drawingPlanes.setBrush ( Graphics::getBrush((*iLayer)->getName(),darkening) );

          if ( isDrawableLayer((*iLayer)->getName()) ) {
            _drawingQuery.setBasicLayer ( *iLayer );
            _drawingQuery.setFilter     ( _queryFilter & ~Query::DoMasterCells );
            _drawingQuery.doQuery       ();
          }
        }

        if ( isDrawableLayer("boundaries") ) {
          _drawingPlanes.setPen   ( Graphics::getPen  ("boundaries",darkening) );
          _drawingPlanes.setBrush ( Graphics::getBrush("boundaries",darkening) );

          _drawingQuery.setBasicLayer ( NULL );
          _drawingQuery.setFilter     ( _queryFilter & ~Query::DoComponents );
          _drawingQuery.doQuery       ();
        }

        if ( isDrawableLayer("text.instance") ) {
          _drawingPlanes.setPen               ( Graphics::getPen  ("text.instance",darkening) );
          _drawingPlanes.setBrush             ( Graphics::getBrush("text.instance",darkening) );
          _drawingPlanes.setBackground        ( Graphics::getBrush("boundaries"   ,darkening) );
          _textDrawingQuery.setArea           ( redrawBox );
          _textDrawingQuery.setTransformation ( Transformation() );
          _textDrawingQuery.doQuery           ();
        }

      //_drawingQuery.setFilter        ( _queryFilter & ~Query::DoMasterCells );
        forEach ( ExtensionSlice*, islice, _cell->getExtensionSlices() ) {
          if ( isDrawableExtension((*islice)->getName()) ) {
            _drawingQuery.setExtensionMask   ( (*islice)->getMask() );
            _drawingQuery.setDrawExtensionGo ( (*islice)->getName() );
            _drawingQuery.setFilter          ( Query::DoExtensionGos );
            _drawingQuery.doQuery            ();
          }
        }
      }

      _drawingPlanes.painterEnd ();
      _cellModificated = false;
    }

    if ( _showSelection )
      redrawSelection ( redrawArea );


    update ();
    popCursor ();

  //cerr << "Redrawed objects: " << _redrawRectCount << endl;
  //cerr << "CellWidget::redraw() - finished." << endl;
  }


  void  CellWidget::redrawSelection ( QRect redrawArea )
  {
    _drawingPlanes.copyToSelect ( redrawArea.x()
                                , redrawArea.y()
                                , redrawArea.width()
                                , redrawArea.height()
                                );

    _drawingPlanes.select ( 1 );
    _drawingPlanes.painterBegin ();
    _drawingPlanes.painter().setPen        ( Qt::NoPen );
    _drawingPlanes.painter().setBackground ( Graphics::getBrush("background") );
    _drawingPlanes.painter().setClipRect   ( redrawArea );

    if ( _cell ) {
      Box  redrawBox = displayToDbuBox ( redrawArea );

      for_each_basic_layer ( basicLayer, _technology->getBasicLayers() ) {
      //if ( !isDrawableLayer(basicLayer->getName()) ) continue;

        _drawingPlanes.setPen   ( Graphics::getPen  (basicLayer->getName()) );
        _drawingPlanes.setBrush ( Graphics::getBrush(basicLayer->getName()) );

        set<Selector*>::iterator  iselector = _selectors.begin ();
        for ( ; iselector != _selectors.end() ; iselector++ ) {
          Occurrence      occurrence     = (*iselector)->getOccurrence();
          Transformation  transformation = occurrence.getPath().getTransformation();

          Instance* instance = dynamic_cast<Instance*>(occurrence.getEntity());
          if ( instance ) {
          // Temporary.
          //drawInstance ( instance, basicLayer, redrawBox, transformation );
            continue;
          }

          Component* component = dynamic_cast<Component*>(occurrence.getEntity());
          if ( !component ) continue;
          if ( !component->getLayer() ) continue;
          if ( !component->getLayer()->contains(basicLayer) ) continue;

          _drawingQuery.drawGo ( dynamic_cast<Go*>(occurrence.getEntity())
                               , basicLayer
                               , redrawBox
                               , transformation
                               );
        }
        end_for;
      }
    }

    _drawingPlanes.painterEnd ();
    _selectionHasChanged = false;
  }


  bool  CellWidget::isDrawableLayer ( const Name& layerName )
  {
    PaletteItem* item = (_palette) ? _palette->find(layerName) : NULL;

    return (!item || item->isChecked())
      && ( Graphics::getThreshold(layerName)/DbU::lambda(1.0) < _scale );
  }


  bool  CellWidget::isDrawableExtension ( const Name& extensionName )
  {
    PaletteItem* item = (_palette) ? _palette->find(extensionName) : NULL;

    return (!item || item->isChecked());
  }


  void  CellWidget::drawBox ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2 )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( false );
    _drawingPlanes.painter().drawRect ( dbuToDisplayRect(x1,y1,x2,y2) );
  }


  void  CellWidget::drawBox ( const Box& box )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( false );
    _drawingPlanes.painter().drawRect ( dbuToDisplayRect(box) );
  }


  void  CellWidget::drawText ( const Point& point, const Name& text, int angle, bool reverse )
  {
    _drawingPlanes.painter().save();
    if ( reverse ) {
      _drawingPlanes.painter().setPen            ( Graphics::getPen  ("background") );
      _drawingPlanes.painter().setBackgroundMode ( Qt::OpaqueMode );
    }
    _drawingPlanes.painter().translate( dbuToDisplayPoint(point) );
    _drawingPlanes.painter().rotate( angle );
    _drawingPlanes.painter().drawText ( 0, _textFontHeight, getString(text).c_str() );
    _drawingPlanes.painter().restore();
  }


  void  CellWidget::drawLine ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2 )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( true );
    _drawingPlanes.painter().drawLine ( dbuToDisplayPoint(x1,y1), dbuToDisplayPoint(x2,y2) );
  }


  void  CellWidget::drawLine ( const Point& p1, const Point& p2 )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( true );
    _drawingPlanes.painter().drawLine ( dbuToDisplayPoint(p1), dbuToDisplayPoint(p2) );
  }


  void  CellWidget::drawScreenPolyline ( const QPoint* points, int count, int width )
  {
    QPen pen = Graphics::getPen("grid");
    pen.setWidth ( width );

    _drawingPlanes.painter(2).setPen ( pen );
    _drawingPlanes.painter(2).drawPolyline ( points, count );
  }


  void  CellWidget::drawScreenRect ( const QPoint& p1, const QPoint& p2 )
  {
    _drawingPlanes.painter(2).setPen ( Graphics::getPen("grid") );
    _drawingPlanes.painter(2).drawRect ( QRect(p1,p2) );
  }


  void  CellWidget::drawGrid ()
  {
    _drawingPlanes.painter(2).setPen ( Graphics::getPen("grid") );

    bool lambdaGrid = false;
    if ( Graphics::getThreshold("grid")/DbU::lambda(1.0) < _scale/5 )
      lambdaGrid = true;

    DbU::Unit  gridStep      = DbU::getSnapGridStep();
    DbU::Unit  superGridStep = gridStep*5;
    DbU::Unit  xGrid;
    DbU::Unit  yGrid;
    QPoint     center;

    for ( yGrid = DbU::getOnSnapGrid(_visibleArea.getYMin())
        ; yGrid < _visibleArea.getYMax()
        ; yGrid += gridStep
        ) {
      for ( xGrid = DbU::getOnSnapGrid(_visibleArea.getXMin())
          ; xGrid < _visibleArea.getXMax()
          ; xGrid += gridStep
          ) {
        center = dbuToScreenPoint(xGrid,yGrid);
        if ( (xGrid % superGridStep) || (yGrid % superGridStep) ) {
          if ( lambdaGrid )
            _drawingPlanes.painter(2).drawPoint ( center );
        } else {
          if ( lambdaGrid ) {
            _drawingPlanes.painter(2).drawLine ( center.x()-3, center.y()  , center.x()+3, center.y()   );
            _drawingPlanes.painter(2).drawLine ( center.x()  , center.y()-3, center.x()  , center.y()+3 );
          } else {
            _drawingPlanes.painter(2).drawPoint ( center );
          }
        }
      }
    }
  }


  void  CellWidget::goLeft ( int dx )
  {
    if ( !dx ) dx = geometry().size().width() / 4;

    _visibleArea.translate ( - (DbU::Unit)( dx / _scale ) , 0 );

    if ( _offsetVA.rx() - dx >= 0 ) {
      _offsetVA.rx() -= dx;
      update ();
    } else {
      shiftLeft ( dx );
    }
  }


  void  CellWidget::goRight ( int dx )
  {
    if ( !dx ) dx = geometry().size().width() / 4;

  //cerr << "CellWidget::goRight() - dx: " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;
    _visibleArea.translate ( (DbU::Unit)( dx / _scale ) , 0 );

    if ( _offsetVA.rx() + dx < 2*_stripWidth ) {
      _offsetVA.rx() += dx;
      update ();
    } else {
      shiftRight ( dx );
    }
  }


  void  CellWidget::goUp ( int dy )
  {
    if ( !dy ) dy = geometry().size().height() / 4;

    _visibleArea.translate ( 0, (DbU::Unit)( dy / _scale ) );

    if ( _offsetVA.ry() - dy >= 0 ) {
      _offsetVA.ry() -= dy;
      update ();
    } else {
      shiftUp ( dy );
    }
  }


  void  CellWidget::goDown ( int dy )
  {
    if ( !dy ) dy = geometry().size().height() / 4;

    _visibleArea.translate ( 0, - (DbU::Unit)( dy / _scale ) );

    if ( _offsetVA.ry() + dy < 2*_stripWidth ) {
      _offsetVA.ry() += dy;
      update ();
    } else {
      shiftDown ( dy );
    }
  }


  void  CellWidget::displayReframe ()
  {
    _offsetVA.rx() = _stripWidth;
    _offsetVA.ry() = _stripWidth;

    DbU::Unit xmin = (DbU::Unit)( _visibleArea.getXMin() - ((float)_offsetVA.x()/_scale) );
    DbU::Unit xmax = (DbU::Unit)( xmin + ((float)_drawingPlanes.width()/_scale) ) ;
    DbU::Unit ymax = (DbU::Unit)( _visibleArea.getYMax() + ((float)_offsetVA.y()/_scale) );
    DbU::Unit ymin = (DbU::Unit)( ymax - ((float)_drawingPlanes.height()/_scale) ) ;

    _displayArea = Box ( xmin, ymin, xmax, ymax );

    redraw ();
  }


  void  CellWidget::setScale ( float scale )
  {
    _scale = scale;

    Point center = _visibleArea.getCenter();

    _visibleArea.makeEmpty ();
    _visibleArea.merge ( (DbU::Unit)( center.getX() - width () / (_scale*2) ) 
                       , (DbU::Unit)( center.getY() - height() / (_scale*2) ) 
                       , (DbU::Unit)( center.getX() + width () / (_scale*2) )
                       , (DbU::Unit)( center.getY() + height() / (_scale*2) )
                       );

  //cerr << "_visibleArea: " << _visibleArea << " (offset: " << _offsetVA.x() << ")" << endl;
  //cerr << "              " << center << endl;

    displayReframe ();
  }


  void  CellWidget::reframe ( const Box& box )
  {
  //cerr << "CellWidget::reframe() - " << box << endl;
  //cerr << "  widget size := " << _drawingPlanes.width() << "x" << _drawingPlanes.height() << endl;

    int  width  = this->width  ();
    int  height = this->height ();

    float scaleX = width  / (float)box.getWidth ();
    float scaleY = height / (float)box.getHeight();
    _scale = min ( scaleX, scaleY );

    Point center = box.getCenter();

    width  /= 2;
    height /= 2;

    _visibleArea = Box ( (DbU::Unit)( center.getX() - width  / _scale ) 
                       , (DbU::Unit)( center.getY() - height / _scale ) 
                       , (DbU::Unit)( center.getX() + width  / _scale )
                       , (DbU::Unit)( center.getY() + height / _scale )
                       );
    displayReframe ();

  //cerr << "  _displayArea: " << _displayArea << " (offset: " << _offsetVA.x() << ")" << endl;
  }


  void  CellWidget::fitToContents ()
  {
    if ( _cell ) reframe ( _cell->getBoundingBox() );
  }


  void  CellWidget::shiftLeft ( int dx )
  {
  //cerr << "CellWidget::shiftLeft() - " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;

    int  leftShift  = ( 1 + ( dx - _offsetVA.rx() ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( - (DbU::Unit)( leftShift / _scale ) , 0 );
    _offsetVA.rx() -= dx - leftShift;

    if ( leftShift >= _drawingPlanes.width() ) {
      redraw ();
    } else {
      _drawingPlanes.shiftLeft ( leftShift );

      redraw ( QRect ( QPoint ( 0, 0 )
                     , QSize  ( leftShift, _drawingPlanes.height() )) );
    }

    assert ( _offsetVA.rx() >= 0 );
  }


  void  CellWidget::shiftRight ( int dx )
  {
  //cerr << "CellWidget::shiftRight() - " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;

    int  rightShift  = ( ( _offsetVA.rx() + dx ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( (DbU::Unit)( rightShift / _scale ) , 0 );
    _offsetVA.rx() += dx - rightShift;

  //cerr << "  _displayArea: " << _displayArea << endl;

    if ( rightShift >= _drawingPlanes.width() ) {
      redraw ();
    } else {
      _drawingPlanes.shiftRight ( rightShift );

      redraw ( QRect ( QPoint ( _drawingPlanes.width()-rightShift, 0 )
                     , QSize  ( rightShift, _drawingPlanes.height() )) );
    }

    assert ( _offsetVA.rx() >= 0 );
  }


  void  CellWidget::shiftUp ( int dy )
  {
  //cerr << "CellWidget::shiftUp() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

    int  upShift  = ( 1 + ( dy - _offsetVA.ry() ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( 0, (DbU::Unit)( upShift / _scale ) );
    _offsetVA.ry() -= dy - upShift;

    if ( upShift >= _drawingPlanes.height() ) {
      redraw ();
    } else {
      _drawingPlanes.shiftUp ( upShift );

      redraw ( QRect ( QPoint ( 0, 0 )
                     , QSize  ( _drawingPlanes.width(), upShift )) );
    }

    assert ( _offsetVA.ry() >= 0 );
  }


  void  CellWidget::shiftDown ( int dy )
  {
  //cerr << "CellWidget::shiftDown() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

    int  downShift  = (  ( _offsetVA.ry() + dy ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( 0, - (DbU::Unit)( downShift / _scale ) );
    _offsetVA.ry() += dy - downShift;

    if ( downShift >= _drawingPlanes.height() ) {
      redraw ();
    } else {
      _drawingPlanes.shiftDown ( downShift );

      redraw ( QRect ( QPoint ( 0, _drawingPlanes.height()-downShift )
                     , QSize  ( _drawingPlanes.width(), downShift )) );
    }

    assert ( _offsetVA.ry() >= 0 );
  }


  void  CellWidget::paintEvent ( QPaintEvent* event )
  {
    _drawingPlanes.painterBegin ( 2 );
    _drawingPlanes.copyToScreen ();

    for ( size_t i=0 ; i<_commands.size() ; i++ )
      _commands[i]->draw ( this );

    if ( isDrawableLayer("grid") ) drawGrid ();
    if ( isDrawableLayer("spot") ) _spot.moveTo ( _mousePosition );

    _drawingPlanes.painterEnd ( 2 );
  }


  void  CellWidget::resizeEvent ( QResizeEvent* )
  {
    QSize uaDelta ( 0, 0 );
    QSize uaSize  = geometry().size();

    uaSize.rwidth () += 2*_stripWidth;
    uaSize.rheight() += 2*_stripWidth;

    if ( uaSize.width () > _drawingPlanes.width () )
      uaDelta.rwidth () = uaSize.width () - _drawingPlanes.width ();

    if ( uaSize.height() > _drawingPlanes.height() )
      uaDelta.rheight() = uaSize.height() - _drawingPlanes.height();

  //cerr << "New UA widget size: " << uaSize.width() << "x" << uaSize.height() << endl;

    if ( uaDelta.width() || uaDelta.height() ) {
      _displayArea.inflate ( 0, 0, (DbU::Unit)(uaDelta.width()/_scale), (DbU::Unit)(uaDelta.height()/_scale) );
      _visibleArea.inflate ( 0, 0, (DbU::Unit)(uaDelta.width()/_scale), (DbU::Unit)(uaDelta.height()/_scale) );
    //cerr << "new      " << _displayArea << endl;

    //cerr << "Previous buffer size: " << _drawingPlanes.width () << "x"
    //                                 << _drawingPlanes.height() << endl;

      QSize bufferSize ( ( ( uaSize.width () / _stripWidth ) + 1 ) * _stripWidth
                       , ( ( uaSize.height() / _stripWidth ) + 1 ) * _stripWidth );
      _drawingPlanes.resize ( bufferSize );

    //cerr << "Effective buffer resize to: " << bufferSize.width() << "x"
    //                                       << bufferSize.height() << endl;
    }
    redraw ();
  }


  void  CellWidget::keyPressEvent ( QKeyEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->keyPressEvent ( this, event );

    if ( !commandActive ) QWidget::keyPressEvent ( event );
  }


  void  CellWidget::mouseMoveEvent ( QMouseEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->mouseMoveEvent ( this, event );

    if ( !commandActive ) {
      _mousePosition = event->pos();
      emit mousePositionChanged ( screenToDbuPoint(event->pos()) );

      update ();
    }
  }


  void  CellWidget::mousePressEvent ( QMouseEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->mousePressEvent ( this, event );

    _spot.setShowSpot ( !commandActive );
  }


  void  CellWidget::mouseReleaseEvent ( QMouseEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->mouseReleaseEvent ( this, event );

    _spot.setShowSpot ( true );
  }


  QPoint  CellWidget::dbuToDisplayPoint ( DbU::Unit x, DbU::Unit y ) const
  {
    return QPoint ( dbuToDisplayX(x), dbuToDisplayY(y) );
  }


  QPoint  CellWidget::dbuToDisplayPoint ( const Point& point ) const
  {
    return dbuToDisplayPoint ( point.getX(), point.getY() );
  }


  QRect  CellWidget::dbuToDisplayRect ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2 ) const
  {
    return QRect ( dbuToDisplayX(x1)
                 , dbuToDisplayY(y2)
                 , dbuToDisplayX(x2) - dbuToDisplayX(x1)
                 , dbuToDisplayY(y1) - dbuToDisplayY(y2)
                 );
  }


  QRect  CellWidget::dbuToDisplayRect ( const Box& box ) const
  {
    return dbuToDisplayRect ( box.getXMin()
                            , box.getYMin()
                            , box.getXMax()
                            , box.getYMax()
                            );
  }


  QPoint  CellWidget::dbuToScreenPoint ( DbU::Unit x, DbU::Unit y ) const
  {
    return QPoint ( dbuToScreenX(x), dbuToScreenY(y) );
  }


  void  CellWidget::setShowBoundaries ( bool state )
  {
    if ( _showBoundaries != state ) {
      _showBoundaries = state;
      redraw ();
    }
  }


  void  CellWidget::setCell ( Cell* cell )
  {
    cellPreModificate ();

    _cell = cell;
    _drawingQuery.setCell ( cell );
    _textDrawingQuery.setCell ( cell );

    emit cellChanged ( cell );

    fitToContents ();

    cellPostModificate ();
  }


  void  CellWidget::select ( const Net* net, bool delayRedraw )
  {
    ++_delaySelectionChanged;

    if ( !_cumulativeSelection ) unselectAll ( true );
    bool added = _selection.add ( net, delayRedraw );

    if ( !--_delaySelectionChanged && added ) emit selectionChanged(_selectors,_cell);
  }


  bool  CellWidget::isSelected ( Occurrence occurrence )
  {
	if ( !occurrence.isValid() )
      throw Error ( "Can't select occurrence : invalid occurrence" );

	if ( occurrence.getOwnerCell() != getCell() )
      throw Error ( "Can't select occurrence : incompatible occurrence" );

	Property* property = occurrence.getProperty ( Selector::getPropertyName() );
	if ( !property )
      return false;

    Selector* selector = dynamic_cast<Selector*>(property);
    if ( !selector )
      throw Error ( "Abnormal property named " + getString(Selector::getPropertyName()) );

	return selector->isAttachedTo(this);
  }


  void  CellWidget::select ( Occurrence occurrence )
  {
	if ( !occurrence.isValid() )
      throw Error ( "Can't select occurrence : invalid occurrence" );

	if ( occurrence.getOwnerCell() != getCell() )
      throw Error ( "Can't select occurrence : incompatible occurrence" );

  //if ( !_cumulativeSelection ) unselectAll ( true );

	Property* property = occurrence.getProperty ( Selector::getPropertyName() );
    Selector* selector = NULL;
	if ( !property )
      selector = Selector::create ( occurrence );
	else {
      selector = dynamic_cast<Selector*>(property);
      if ( !selector )
        throw Error ( "Abnormal property named " + getString(Selector::getPropertyName()) );
    }

	selector->attachTo(this);

    _selectionHasChanged = true;
    if ( !_delaySelectionChanged ) emit selectionChanged(_selectors,_cell);
  }


  void  CellWidget::selectOccurrencesUnder ( Box selectArea )
  {
    ++_delaySelectionChanged;

    if ( !_cumulativeSelection ) unselectAll ( true );
    bool added = _selection.add ( selectArea );

    if ( !--_delaySelectionChanged && added ) emit selectionChanged(_selectors,_cell);
  }


  void  CellWidget::unselect ( const Net* net, bool delayRedraw )
  {
    ++_delaySelectionChanged;

    bool removed = _selection.remove ( net, delayRedraw );
    if ( !--_delaySelectionChanged && removed ) emit selectionChanged(_selectors,_cell);
  }


  void  CellWidget::unselect ( Occurrence occurrence )
  {
	if ( !occurrence.isValid() )
		throw Error ( "Can't unselect occurrence : invalid occurrence" );

	if ( occurrence.getOwnerCell() != getCell() )
		throw Error ( "Can't unselect occurrence : incompatible occurrence" );

	Property* property = occurrence.getProperty ( Selector::getPropertyName() );
	if ( property ) {
      Selector* selector = dynamic_cast<Selector*>(property);
      if ( !selector )
        throw Error ( "Abnormal property named " + getString(Selector::getPropertyName()) );

      selector->detachFrom(this);
    }

    _selectionHasChanged = true;
    if ( !_delaySelectionChanged ) emit selectionChanged(_selectors,_cell);
  }


  void  CellWidget::unselectAll ( bool delayRedraw )
  {
    ++_delaySelectionChanged;

    _selection.clear ();
    _unselectAll ( delayRedraw );

    if ( !--_delaySelectionChanged ) emit selectionChanged(_selectors,_cell);
  }


  void  CellWidget::toggleSelect ( Occurrence occurrence, bool fromPopup )
  {
	if ( !occurrence.isValid() )
      throw Error ( "Can't select occurrence : invalid occurrence" );

	if ( occurrence.getOwnerCell() != getCell() )
      throw Error ( "Can't select occurrence : incompatible occurrence" );

	Property* property = occurrence.getProperty ( Selector::getPropertyName() );
    Selector* selector = NULL;
	if ( !property ) {
      selector = Selector::create ( occurrence );
      selector->attachTo ( this );
	} else {
      selector = dynamic_cast<Selector*>(property);
      if ( !selector )
        throw Error ( "Abnormal property named " + getString(Selector::getPropertyName()) );
      selector->detachFrom ( this );
    }

    _selectionHasChanged = true;
    if ( _showSelection ) redraw ();

    if ( fromPopup ) emit occurrenceToggled ( occurrence );
  }


  void  CellWidget::_select ( const Net* net, bool delayRedraw )
  {
    select ( Occurrence(net) );
    forEach ( Component*, component, net->getComponents() ) {
      Occurrence occurrence ( *component );
      select ( occurrence );
    }
    if ( !delayRedraw && _showSelection ) redraw ();
  }


  void  CellWidget::_unselect ( const Net* net, bool delayRedraw )
  {
    unselect ( Occurrence(net) );
    forEach ( Component*, component, net->getComponents() ) {
      Occurrence occurrence ( *component );
      unselect ( occurrence );
    }
    if ( !delayRedraw && _showSelection ) redraw ();
  }


  void  CellWidget::_selectOccurrencesUnder ( Box selectArea )
  {
    forEach ( Occurrence, ioccurrence, _cell->getOccurrencesUnder(selectArea) )
      select ( *ioccurrence );
  }


  void  CellWidget::_unselectAll ( bool delayRedraw )
  {
    set<Selector*>::iterator iselector;
    while ( !_selectors.empty() )
      (*_selectors.begin())->detachFrom ( this );

    if ( !_selectionHasChanged ) _selectionHasChanged = true;
    if ( !delayRedraw && _showSelection ) redraw ();
  }


  void  CellWidget::cellPreModificate ()
  {
    _unselectAll ( true );
    
    emit selectionChanged(_selectors,_cell);
    emit cellPreModificated ();
  }


  void  CellWidget::cellPostModificate ()
  {
    _cellModificated = true;

    ++_delaySelectionChanged;
    _selection.revalidate ();

    updatePalette ();
    redraw ();

    --_delaySelectionChanged;
    emit selectionChanged(_selectors,_cell);
    emit cellPostModificated ();
  }


} // End of Hurricane namespace.
