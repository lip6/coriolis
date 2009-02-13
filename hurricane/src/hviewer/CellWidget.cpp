
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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


#include <sys/resource.h>
#include <ctime>

#include <QApplication>
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
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Contact.h"
#include "hurricane/Pad.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/ExtensionGo.h"

#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/PaletteItem.h"
#include "hurricane/viewer/PaletteWidget.h"
// #include "MapView.h"
#include "hurricane/viewer/Command.h"
#include "hurricane/viewer/CellWidget.h"




namespace Hurricane {


// -------------------------------------------------------------------
// Collections.


  typedef Hurricane::Filter<Occurrence>     OccurrenceHF;
  typedef Hurricane::Locator<Occurrence>    OccurrenceHL;
  typedef Hurricane::Collection<Occurrence> OccurrenceHC;


// -------------------------------------------------------------------
// Class  :  "Occurrences_IsSelectable".


  class Occurrences_IsSelectable : public OccurrenceHF {
    public:
      inline                    Occurrences_IsSelectable ( const CellWidget* );
      inline                    Occurrences_IsSelectable ( const Occurrences_IsSelectable& );
      virtual OccurrenceHF*     getClone                 () const;
      virtual bool              accept                   ( Occurrence ) const;
      virtual string            _getString               () const;
    private:
              const CellWidget* _cellWidget;    
  };


  inline Occurrences_IsSelectable::Occurrences_IsSelectable ( const CellWidget* widget )
    : OccurrenceHF()
    , _cellWidget(widget)
  { }


  inline Occurrences_IsSelectable::Occurrences_IsSelectable ( const Occurrences_IsSelectable& rhs )
    : OccurrenceHF()
    , _cellWidget(rhs._cellWidget)
  { }


  OccurrenceHF* Occurrences_IsSelectable::getClone () const
  {
    return new Occurrences_IsSelectable(_cellWidget);
  }


  bool  Occurrences_IsSelectable::accept ( Occurrence occurrence ) const
  {
    Entity* entity = occurrence.getEntity();
    if ( !entity ) return false;

    ExtensionGo* eGo = dynamic_cast<ExtensionGo*>(entity);
    if ( eGo )
      return _cellWidget->isSelectable ( eGo->getName() );

    Component* component = dynamic_cast<Component*>(entity);
    if ( component ) {
      return _cellWidget->isSelectable ( component->getLayer() );
    }

    return true;
  }


  string  Occurrences_IsSelectable::_getString () const
  {
    return "<Occurrences_IsSelectable>";
  }



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


  QPoint  CellWidget::Spot::computeSpotPoint ( const QPoint& screenPoint )
  {
    Point mousePoint = _cellWidget->screenToDbuPoint ( screenPoint );
    Point  spotPoint = Point ( DbU::getOnSnapGrid(mousePoint.getX())
                             , DbU::getOnSnapGrid(mousePoint.getY())
                             );
    return _cellWidget->dbuToScreenPoint(spotPoint);
  }


  void  CellWidget::Spot::moveTo ( const QPoint& screenPoint )
  {
    restore ();
    if ( !_showSpot ) return;

    _restore = true;

    QPainter& screenPainter = _cellWidget->getDrawingPlanes().painter(PlaneId::Widget);
    _spotPoint = computeSpotPoint ( screenPoint );

    screenPainter.setPen ( Graphics::getPen("spot") );
    screenPainter.drawRect ( _spotPoint.x()-3, _spotPoint.y()-3, 6, 6 );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::RedrawEvent".


  CellWidget::RedrawEvent::RedrawEvent ( EventType type, int shift, CellWidget* widget )
    : _type(type)
    , _shift(shift)
  { }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::RedrawManager".


  CellWidget::RedrawManager::RedrawManager ( CellWidget* widget )
    : _widget        (widget)
    , _events        ()
    , _refreshSession(0)
    , _processing    (false)
    , _interrupted   (false)
  { }


  CellWidget::RedrawManager::~RedrawManager ()
  {
    while ( !_events.empty() ) {
      delete _events.front ();
      _events.pop_front ();
    }
  }


  void  CellWidget::RedrawManager::goLeft ( int shift )
  {
#ifdef ALLOW_REQUEST_INTERRUPT
    list<RedrawEvent*>::iterator ievent = _events.end();
    if ( !_events.empty() && (_events.back()->getType() == RedrawEvent::Refresh) )
      --ievent;
    _events.insert ( ievent, new RedrawEvent(RedrawEvent::GoLeft,shift,_widget) );
#else
    _events.push_back ( new RedrawEvent(RedrawEvent::GoLeft,shift,_widget) );
#endif

    if ( !_processing ) process ();
  }


  void  CellWidget::RedrawManager::goRight ( int shift )
  {
#ifdef ALLOW_REQUEST_INTERRUPT
    list<RedrawEvent*>::iterator ievent = _events.end();
    if ( !_events.empty() && (_events.back()->getType() == RedrawEvent::Refresh) )
      --ievent;
    _events.insert ( ievent, new RedrawEvent(RedrawEvent::GoRight,shift,_widget) );
#else
    _events.push_back ( new RedrawEvent(RedrawEvent::GoRight,shift,_widget) );
#endif

    if ( !_processing ) process ();
  }


  void  CellWidget::RedrawManager::goUp ( int shift )
  {
#ifdef ALLOW_REQUEST_INTERRUPT
    list<RedrawEvent*>::iterator ievent = _events.end();
    if ( !_events.empty() && (_events.back()->getType() == RedrawEvent::Refresh) )
      --ievent;
    _events.insert ( ievent, new RedrawEvent(RedrawEvent::GoUp,shift,_widget) );
#else
    _events.push_back ( new RedrawEvent(RedrawEvent::GoUp,shift,_widget) );
#endif

    if ( !_processing ) process ();
  }


  void  CellWidget::RedrawManager::goDown ( int shift )
  {
#ifdef ALLOW_REQUEST_INTERRUPT
    list<RedrawEvent*>::iterator ievent = _events.end();
    if ( !_events.empty() && (_events.back()->getType() == RedrawEvent::Refresh) )
      --ievent;
    _events.insert ( ievent, new RedrawEvent(RedrawEvent::GoDown,shift,_widget) );
#else
    _events.push_back ( new RedrawEvent(RedrawEvent::GoDown,shift,_widget) );
#endif

    if ( !_processing ) process ();
  }


  void  CellWidget::RedrawManager::refresh ()
  {
#ifdef ALLOW_REQUEST_INTERRUPT
    if ( !_events.empty() && (_events.back()->getType() == RedrawEvent::Refresh) ) {
      _interrupted = true;
    } else if ( _events.empty() ) {
      _events.push_back ( new RedrawEvent(RedrawEvent::Refresh,0,_widget) );

      if ( !_processing ) process ();
    }
#else
    bool addRefresh = true;
    if ( _refreshSession ) {
      list<RedrawEvent*>::iterator ievent = _events.begin();
      for ( ; ievent != _events.end() ; ievent++ ) {
        if ( (_events.back()->getType() == RedrawEvent::Refresh) ) {
          addRefresh = false;
          break;
        }
      }
    }
    if ( addRefresh )
      _events.push_back ( new RedrawEvent(RedrawEvent::Refresh,0,_widget) );

    if ( !_processing && (_refreshSession == 0) ) process ();
#endif
  }


  void  CellWidget::RedrawManager::process ()
  {
    _processing = true;

    while ( !_events.empty() ) {
      RedrawEvent* event = _events.front ();

      switch ( event->getType() ) {
        case RedrawEvent::GoLeft:  _widget->_goLeft  ( event->getShift() ); break;
        case RedrawEvent::GoRight: _widget->_goRight ( event->getShift() ); break;
        case RedrawEvent::GoUp:    _widget->_goUp    ( event->getShift() ); break;
        case RedrawEvent::GoDown:  _widget->_goDown  ( event->getShift() ); break;
        case RedrawEvent::Refresh: _widget->_refresh (); break;
      }

#ifdef ALLOW_REQUEST_INTERRUPT
      if ( event->getType() == RedrawEvent::Refresh ) {
        _events.pop_back ();
      } else
        _events.pop_front ();

      delete event;

      if ( _events.empty() && _interrupted ) {
        _events.push_back ( new RedrawEvent(RedrawEvent::Refresh,0,_widget) );
        _interrupted = false;
      }
#else
      delete _events.front ();
      _events.pop_front ();
#endif
    }

    _processing = false;
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::DrawingPlanes".


  CellWidget::DrawingPlanes::DrawingPlanes ( const QSize& size, CellWidget* cw )
    : _cellWidget(cw)
    , _printer(NULL)
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
    _painters[PlaneId::Normal   ].setBrush ( brush );
    _painters[PlaneId::Selection].setBrush ( brush );
  }


  void  CellWidget::DrawingPlanes::setBackground ( const QBrush& brush )
  {
    _painters[PlaneId::Normal   ].setBackground ( brush );
    _painters[PlaneId::Selection].setBackground ( brush );
  }


  void  CellWidget::DrawingPlanes::setBackgroundMode ( Qt::BGMode mode )
  {
    _painters[PlaneId::Normal   ].setBackgroundMode ( mode );
    _painters[PlaneId::Selection].setBackgroundMode ( mode );
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
    _painters[PlaneId::Normal   ].drawPixmap ( dx, 0, *_planes[0], 0, 0, width()-dx, height() );
    _painters[PlaneId::Selection].drawPixmap ( dx, 0, *_planes[1], 0, 0, width()-dx, height() );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::shiftRight ( int dx )
  {
    paintersBegin ();
    _painters[PlaneId::Normal   ].drawPixmap ( 0, 0, *_planes[0], dx, 0, width()-dx, height() );
    _painters[PlaneId::Selection].drawPixmap ( 0, 0, *_planes[1], dx, 0, width()-dx, height() );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::shiftUp ( int dy )
  {
    paintersBegin ();
    _painters[PlaneId::Normal   ].drawPixmap ( 0, dy, *_planes[0], 0, 0, width(), height()-dy );
    _painters[PlaneId::Selection].drawPixmap ( 0, dy, *_planes[1], 0, 0, width(), height()-dy );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::shiftDown ( int dy )
  {
    paintersBegin ();
    _painters[PlaneId::Normal   ].drawPixmap ( 0, 0, *_planes[0], 0, dy, width(), height()-dy );
    _painters[PlaneId::Selection].drawPixmap ( 0, 0, *_planes[1], 0, dy, width(), height()-dy );
    paintersEnd ();
  }


  void  CellWidget::DrawingPlanes::copyToSelect ( int sx, int sy, int w, int h )
  {
    painterBegin ( PlaneId::Selection );
    _painters[PlaneId::Selection].setPen        ( Qt::NoPen );
    _painters[PlaneId::Selection].setBackground ( Graphics::getBrush("background") );
    _painters[PlaneId::Selection].eraseRect     ( sx, sy, w, h );
  //_painters[PlaneId::Selection].setOpacity    ( 0.5 );
    _painters[PlaneId::Selection].drawPixmap    ( sx, sy, *_planes[0], sx, sy, w, h );
    painterEnd ( PlaneId::Selection );
  }


  void  CellWidget::DrawingPlanes::copyToScreen ( int sx, int sy, int w, int h )
  {
    if ( _cellWidget->showSelection() )
      _painters[PlaneId::Widget].drawPixmap
        ( sx, sy
        , *_planes[PlaneId::Selection]
        , _cellWidget->getOffsetVA().rx()+sx, _cellWidget->getOffsetVA().ry()+sy
        , w, h
        );
    else
      _painters[PlaneId::Widget].drawPixmap
        ( sx, sy
        , *_planes[PlaneId::Normal]
        , _cellWidget->getOffsetVA().rx()+sx, _cellWidget->getOffsetVA().ry()+sy
        , w, h
        );
  }


  void  CellWidget::DrawingPlanes::copyToPrinter ( int sx, int sy, int w, int h, QPrinter* printer, bool imageOnly )
  {
    int   ximage    = 0;
    int   yimage    = 0;

    if ( !printer ) return;
    if ( imageOnly ) {
      printer->setPaperSize   ( QSizeF(w,h),        QPrinter::DevicePixel );
      printer->setPageMargins ( 0.0, 0.0, 0.0, 0.0, QPrinter::DevicePixel );
    }

    _printer = printer;
    painterBegin ( PlaneId::Printer );

    if ( !imageOnly ) {
      QFont font ( "Bitstream Vera Sans", 12 );
      font.setWeight ( QFont::Bold );

      DbU::Unit x1 = _cellWidget->displayToDbuX ( sx );
      DbU::Unit x2 = _cellWidget->displayToDbuX ( sx+w );
      DbU::Unit y1 = _cellWidget->displayToDbuY ( sy );
      DbU::Unit y2 = _cellWidget->displayToDbuY ( sy+h );

      string title = "Unicorn:" + getString(_cellWidget->getCell())
                   + " area [ " + DbU::getValueString(x1)
                   + " "        + DbU::getValueString(y1)
                   + " ] [ "    + DbU::getValueString(x2)
                   + " "        + DbU::getValueString(y2)
                   + " ]";

      QRect titleArea = QRect ( 0, 0, _printer->width(), 50 );

      _painters[PlaneId::Printer].setFont ( font );
      _painters[PlaneId::Printer].drawText ( titleArea, Qt::AlignVCenter|Qt::AlignHCenter, title.c_str() );

      ximage = (_printer->width() > w) ? (_printer->width()-w)/2 : 0;
      yimage = 100;
    }

    cerr << "sy: " << sy << " offsetVA.ry(): " << _cellWidget->getOffsetVA().ry() << endl;
    cerr << "w: " << w << " h:" << h << endl;

    if ( _cellWidget->showSelection() )
      _painters[PlaneId::Printer].drawPixmap
        ( ximage, yimage
        , *_planes[PlaneId::Selection]
        , _cellWidget->getOffsetVA().rx()+sx, _cellWidget->getOffsetVA().ry()+sy
        , w, h
        );
    else
      _painters[PlaneId::Printer].drawPixmap
        ( ximage, yimage
        , *_planes[PlaneId::Normal]
        , _cellWidget->getOffsetVA().rx()+sx, _cellWidget->getOffsetVA().ry()+sy
        , w, h
        );

    if ( !imageOnly ) {
      _painters[PlaneId::Printer].setPen ( QPen(QBrush(QColor("black")), 1.0) );
      _painters[PlaneId::Printer].drawRect ( ximage-2, 98, w+4, h+4 );
    }

    painterEnd ( PlaneId::Printer );
    _printer = NULL;
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::DrawingQuery".


  CellWidget::DrawingQuery::DrawingQuery ( CellWidget* widget )
    : Query()
    , _cellWidget(widget)
    , _drawExtensionGo(NULL)
    , _goCount(0)
    , _extensionGoCount(0)
    , _instanceCount(0)
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
    _instanceCount++;

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

    static QRect         rectangle;
    static unsigned int  state;

    const Component* component = dynamic_cast<const Component*>(go);
    if ( component ) {
      _goCount++;
      rectangle = _cellWidget->dbuToDisplayRect ( transformation.getBox(component->getBoundingBox(basicLayer)) );
      state     = ( (rectangle.width() > 2) ? 1:0) | ( (rectangle.height() > 2) ? 2:0); 
      switch ( state ) {
        case 0: break;
        case 1: _cellWidget->drawScreenLine ( rectangle.bottomLeft(), rectangle.bottomRight() ); break;
        case 2: _cellWidget->drawScreenLine ( rectangle.bottomLeft(), rectangle.topLeft    () ); break;
        case 3: _cellWidget->drawScreenRect ( rectangle ); break;
      }
    }
  }


  bool  CellWidget::DrawingQuery::hasExtensionGoCallback () const
  {
    return true;
  }


  void  CellWidget::DrawingQuery::extensionGoCallback ( Go* go )
  {
    drawExtensionGo ( _cellWidget, go, getBasicLayer(), getArea(), getTransformation() );
  }


  void  CellWidget::DrawingQuery::drawExtensionGo ( CellWidget*           widget
                                                  , const Go*             go
                                                  , const BasicLayer*     basicLayer
                                                  , const Box&            area
                                                  , const Transformation& transformation
                                                  )
  {
    if ( _drawExtensionGo ) {
      _extensionGoCount++;
      _drawExtensionGo ( widget, go, basicLayer, area, transformation );
    }
  }


  bool  CellWidget::DrawingQuery::hasRubberCallback () const
  {
    return true;
  }


  void  CellWidget::DrawingQuery::rubberCallback ( Rubber* rubber )
  {
    drawRubber ( rubber, getArea(), getTransformation() );
  }


  void  CellWidget::DrawingQuery::drawRubber ( const Rubber*          rubber
                                             , const Box&             area
                                             , const Transformation&  transformation
                                             )
  {
    static QPoint center;
    static QPoint extremity;
    static QPoint steiner;

    switch ( _cellWidget->getRubberShape() ) {
      case CellWidget::Steiner:
        center = _cellWidget->dbuToDisplayPoint(transformation.getPoint(rubber->getBarycenter()));
        forEach ( Hook*, hook, rubber->getHooks() ) {
          extremity = _cellWidget->dbuToDisplayPoint
            ( transformation.getPoint(hook->getComponent()->getCenter()) );
          steiner = QPoint ( extremity.x(), center.y() );
          _cellWidget->drawScreenLine ( center , steiner  , PlaneId::Working, false );
          _cellWidget->drawScreenLine ( steiner, extremity, PlaneId::Working, false );
        }
        break;
      case CellWidget::Barycentric:
        center = _cellWidget->dbuToDisplayPoint(transformation.getPoint(rubber->getBarycenter()));
        forEach ( Hook*, hook, rubber->getHooks() ) {
          extremity = _cellWidget->dbuToDisplayPoint
            ( transformation.getPoint(hook->getComponent()->getCenter()) );
          _cellWidget->drawScreenLine ( center, extremity, PlaneId::Working, false );
        }
        break;
      case CellWidget::Centric:
      default:
        center = _cellWidget->dbuToDisplayPoint(transformation.getPoint(rubber->getCenter()));
        forEach ( Hook*, hook, rubber->getHooks() ) {
          extremity = _cellWidget->dbuToDisplayPoint
            ( transformation.getPoint(hook->getComponent()->getCenter()) );
          _cellWidget->drawScreenLine ( center, extremity, PlaneId::Working, false );
        }
        break;
    }
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


  void  CellWidget::TextDrawingQuery::goCallback ( Go* )
  { }


  bool  CellWidget::TextDrawingQuery::hasRubberCallback () const
  { return false; }


  void  CellWidget::TextDrawingQuery::rubberCallback ( Rubber* )
  { }


  bool  CellWidget::TextDrawingQuery::hasExtensionGoCallback () const
  { return false; }


  void  CellWidget::TextDrawingQuery::extensionGoCallback ( Go* )
  { }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::SelectorCriterions".


  CellWidget::SelectorCriterions::SelectorCriterions ()
    : _cellWidget(NULL)
    , _criterions()
  { }


  CellWidget::SelectorCriterions::~SelectorCriterions ()
  {
    clear ();
  }


  bool  CellWidget::SelectorCriterions::add ( const Net* net )
  {
    if ( !_cellWidget ) return false;
    if ( !_cellWidget->isSelected(Occurrence(net)) ) {
      _criterions.push_back ( new NetSelectorCriterion(net) );
      _criterions.back()->doSelection ( _cellWidget );
      return true;
    }
    return false;
  }


  bool  CellWidget::SelectorCriterions::add ( Box area )
  {
    if ( !_cellWidget ) return false;
    _criterions.push_back ( new AreaSelectorCriterion(area) );
    _criterions.back()->doSelection ( _cellWidget );
    return true;
  }


  bool  CellWidget::SelectorCriterions::remove (  const Net* net )
  {
    if ( !_cellWidget ) return false;
    if ( !_cellWidget->isSelected(Occurrence(net)) ) return false;

    size_t i=0;
    for ( ; i<_criterions.size() ; i++ )
      if ( _criterions[i]->getNet() == net ) break;

    if ( i < _criterions.size() ) {
      swap ( _criterions[i], *(_criterions.end()-1) );
      _criterions.back()->undoSelection ( _cellWidget );
      _criterions.pop_back ();
    } else
      return false;

    return true;
  }


  void  CellWidget::SelectorCriterions::clear ()
  {
    for ( size_t i=0 ; i<_criterions.size() ; i++ ) {
      delete _criterions[i];
    }
    _criterions.clear ();
  }


  void  CellWidget::SelectorCriterions::revalidate ()
  {
    if ( !_cellWidget ) return;

    size_t i    = 0;
    size_t last = _criterions.size ();
    while ( i < last ) {
      if ( _criterions[i]->isValid(_cellWidget) ) {
        _criterions[i]->doSelection ( _cellWidget );
        ++i;
      } else
        swap ( _criterions[i], _criterions[--last] );
    }
    _criterions.erase ( _criterions.begin()+last, _criterions.end() );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::State".


  void  CellWidget::State::setScale ( float scale )
  {
    _scaleHistory.erase ( _scaleHistory.begin()+_ihistory+1,_scaleHistory.end() );
    if ( _historyEnable ) {
      _scaleHistory.push_back ( ScaleEntry(scale,Point(0,0)) );
      _ihistory++;
    } else {
      _scaleHistory[_ihistory]._scale   = scale;
      _scaleHistory[_ihistory]._topLeft = Point(0,0);
    }
  }


  bool  CellWidget::State::scaleHistoryUp ()
  {
    if ( _ihistory == 0 ) return false;

    _ihistory--;

    return true;
  }


  bool  CellWidget::State::scaleHistoryDown ()
  {
    if ( _ihistory+2 > _scaleHistory.size() ) return false;

    _ihistory++;

    return true;
  }


  const Name& CellWidget::State::getName () const
  {
    static const Name noCell = "None";

    if ( !_cell ) return noCell;
    return _cell->getName();
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget".


  const int  CellWidget::_stripWidth  = 10;
  const int  CellWidget::_initialSide = (400/_stripWidth)*_stripWidth;


  CellWidget::CellWidget ( QWidget* parent )
    : QWidget               (parent)
    , _technology           (NULL)
    , _palette              (NULL)
    , _displayArea          (0,0,_initialSide+2*_stripWidth,_initialSide+2*_stripWidth)
    , _visibleArea          (_stripWidth,_stripWidth,_initialSide,_initialSide)
    , _offsetVA             (_stripWidth,_stripWidth)
    , _redrawManager        (this)
    , _drawingPlanes        (QSize(_initialSide+2*_stripWidth,_initialSide+2*_stripWidth),this)
    , _drawingQuery         (this)
    , _textDrawingQuery     (this)
    , _darkening            (100)
    , _mousePosition        (0,0)
    , _spot                 (this)
    , _state                (new State(NULL))
    , _cellChanged          (true)
    , _selectionHasChanged  (false)
    , _delaySelectionChanged(0)
    , _cellModificated      (true)
    , _enableRedrawInterrupt(false)
    , _selectors            ()
    , _commands             ()
    , _redrawRectCount      (0)
    , _textFontHeight       (20)
  {
  //setBackgroundRole ( QPalette::Dark );
  //setAutoFillBackground ( false );
    setAttribute     ( Qt::WA_OpaquePaintEvent );
    setAttribute     ( Qt::WA_NoSystemBackground );
    setAttribute     ( Qt::WA_PaintOnScreen );
    setAttribute     ( Qt::WA_StaticContents );
    setSizePolicy    ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy   ( Qt::StrongFocus );
    setMouseTracking ( true );

  //_mapView = new MapView ( this );
    DataBase* database = DataBase::getDB();
    if ( database )
      _technology = database->getTechnology ();

    QFont font = Graphics::getNormalFont();
    _textFontHeight = QFontMetrics(font).ascent();
  }


  CellWidget::~CellWidget ()
  {
    cerr << "CellWidget::~CellWidget()" << endl;

    for ( size_t i=0 ; i<_commands.size() ; i++ )
      unbindCommand ( _commands[i] );
  }


  void  CellWidget::setStyle ( int id )
  {
    Graphics::setStyle ( (size_t)id );
    refresh ();

    emit styleChanged ();
  }


  void  CellWidget::updatePalette ()
  {
    emit updatePalette(getCell());
  }


  void  CellWidget::bindToPalette ( PaletteWidget* palette )
  {
    detachFromPalette ();
    _palette = palette;

    connect ( _palette, SIGNAL(paletteChanged())    , this    , SLOT(refresh()) );
    connect ( this    , SIGNAL(cellChanged(Cell*))  , _palette, SLOT(updateExtensions(Cell*)) );
    connect ( this    , SIGNAL(updatePalette(Cell*)), _palette, SLOT(updateExtensions(Cell*)) );
    connect ( this    , SIGNAL(styleChanged())      , _palette, SLOT(changeStyle()) );
  }


  void  CellWidget::detachFromPalette ()
  {
    if ( _palette ) {
      disconnect ( _palette, SIGNAL(paletteChanged())    , this    ,  SLOT(refresh()) );
      disconnect ( this    , SIGNAL(cellChanged(Cell*))  , _palette, SLOT(updateExtensions(Cell*)) );
      disconnect ( this    , SIGNAL(updatePalette(Cell*)), _palette, SLOT(updateExtensions(Cell*)) );
      disconnect ( this    , SIGNAL(styleChanged(void*)) , _palette, SLOT(styleChange(void*)) );
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
    return QSize(_initialSide,_initialSide);
  }


  void  CellWidget::rubberChange ()
  {
    switch ( getRubberShape() ) {
      case Centric:     setRubberShape(Barycentric); break;
      case Barycentric: setRubberShape(Steiner    ); break;
      case Steiner:     setRubberShape(Centric    ); break;
    }
  }


  void  CellWidget::setShowSelection ( bool state )
  {
    if ( state != _state->showSelection() ) {
      _state->setShowSelection ( state );
      _selectionHasChanged = false;
      refresh ();

      emit selectionModeChanged ();
    }
  }


  void  CellWidget::setCumulativeSelection ( bool state )
  {
    if ( state != _state->cumulativeSelection() ) {
      _state->setCumulativeSelection ( state );
      emit selectionModeChanged ();
    }
  }


  void  CellWidget::setShowBoundaries ( bool state )
  {
    if ( _state->showBoundaries() != state ) {
      _state->setShowBoundaries ( state );
      _redrawManager.refresh ();

      emit showBoundariesToggled ( state );
    }
  }


  void  CellWidget::changeQueryFilter ()
  {
    _redrawManager.refresh ();
    emit queryFilterChanged ();
  }


  void  CellWidget::_redraw ( QRect redrawArea )
  {
//     cerr << "CellWidget::redraw() - start "
//          << _selectionHasChanged << " filter:"
//          << _state->getQueryFilter() << endl;

//     static bool  timedout;
//     static Timer timer;

    if ( !isVisible() ) return;

//     timer.start ();
//     timedout         = false;
    _cellChanged     = false;
    _redrawRectCount = 0;

    pushCursor ( Qt::BusyCursor );

    if ( ! ( _selectionHasChanged && _state->showSelection() ) || _cellModificated ) {
      _spot.setRestore ( false );
    //_drawingPlanes.copyToSelect ( redrawArea );
      _drawingPlanes.select ( PlaneId::Normal );
      _drawingPlanes.painterBegin ();

      _drawingPlanes.painter().setPen        ( Qt::NoPen );
      _drawingPlanes.painter().setBackground ( Graphics::getBrush("background") );
      _drawingPlanes.painter().setClipRect   ( redrawArea );
      _drawingPlanes.painter().eraseRect     ( redrawArea );

      setDarkening ( (_state->showSelection()) ? Graphics::getDarkening() : 100 );

      if ( getCell() ) {

        Box redrawBox = displayToDbuBox ( redrawArea );

        _drawingQuery.resetGoCount      ();
        _drawingQuery.resetExtensionGoCount();
        _drawingQuery.resetInstanceCount();
        _drawingQuery.setExtensionMask  ( 0 );
        _drawingQuery.setArea           ( redrawBox );
        _drawingQuery.setTransformation ( Transformation() );

        forEach ( BasicLayer*, iLayer, _technology->getBasicLayers() ) {
          _drawingPlanes.setPen   ( Graphics::getPen  ((*iLayer)->getName(),getDarkening()) );
          _drawingPlanes.setBrush ( Graphics::getBrush((*iLayer)->getName(),getDarkening()) );

          if ( isDrawable((*iLayer)->getName()) ) {
            _drawingQuery.setBasicLayer ( *iLayer );
            _drawingQuery.setFilter     ( getQueryFilter().unset(Query::DoMasterCells|Query::DoRubbers) );
            _drawingQuery.doQuery       ();
          }
          if ( _enableRedrawInterrupt ) QApplication::processEvents();
          if ( _redrawManager.interrupted() ) {
          //cerr << "CellWidget::redraw() - interrupt after " << (*iLayer)->getName() << endl;
            break;
          }
        //if ( timeout("redraw [layer]",timer,10.0,timedout) ) break;
        }

        if ( /*!timeout("redraw [boundaries]",timer,10.0,timedout) &&*/ (!_redrawManager.interrupted()) ) {
          if ( isDrawable("boundaries") ) {
             _drawingPlanes.setPen   ( Graphics::getPen  ("boundaries",getDarkening()) );
             _drawingPlanes.setBrush ( Graphics::getBrush("boundaries",getDarkening()) );

             _drawingQuery.setBasicLayer ( NULL );
             _drawingQuery.setFilter     ( getQueryFilter().unset(Query::DoComponents|Query::DoRubbers) );
             _drawingQuery.doQuery       ();
          }
        }

        if ( /*!timeout("redraw [rubbers]",timer,10.0,timedout) &&*/ (!_redrawManager.interrupted()) ) {
          if ( isDrawable("rubber") ) {
             _drawingPlanes.setPen   ( Graphics::getPen  ("rubber",getDarkening()) );
             _drawingPlanes.setBrush ( Graphics::getBrush("rubber",getDarkening()) );

             _drawingQuery.setBasicLayer ( NULL );
             _drawingQuery.setFilter     ( getQueryFilter().unset(Query::DoComponents|Query::DoMasterCells) );
             _drawingQuery.doQuery       ();
          }
        }

        if ( _enableRedrawInterrupt ) QApplication::processEvents();
        if ( /*!timeout("redraw [text.instances]",timer,10.0,timedout) &&*/ (!_redrawManager.interrupted()) ) {
          if ( isDrawable("text.instance") ) {
            _drawingPlanes.setPen               ( Graphics::getPen  ("text.instance",getDarkening()) );
            _drawingPlanes.setBrush             ( Graphics::getBrush("text.instance",getDarkening()) );
            _drawingPlanes.setBackground        ( Graphics::getBrush("boundaries"   ,getDarkening()) );
            _textDrawingQuery.setArea           ( redrawBox );
            _textDrawingQuery.setTransformation ( Transformation() );
            _textDrawingQuery.doQuery           ();
          }
        }

      //_drawingQuery.setFilter ( getQueryFilter() & ~Query::DoMasterCells );
        forEach ( ExtensionSlice*, islice, getCell()->getExtensionSlices() ) {
          if ( _enableRedrawInterrupt ) QApplication::processEvents();
          if ( /*timeout("redraw [extension]",timer,10.0,timedout) ||*/ (_redrawManager.interrupted()) ) break;

          if ( isDrawableExtension((*islice)->getName()) ) {
            _drawingQuery.setExtensionMask   ( (*islice)->getMask() );
            _drawingQuery.setDrawExtensionGo ( (*islice)->getName() );
            _drawingQuery.setFilter          ( Query::DoExtensionGos );
            _drawingQuery.doQuery            ();
          }
        }
        repaint ();
      }

      _drawingPlanes.painterEnd ();
      _cellModificated = false;
    }

    if ( isDrawable("grid") ) drawGrid ( redrawArea );

    setDarkening ( 100 );
    if ( _state->showSelection() )
      redrawSelection ( redrawArea );

    popCursor ();

//     timer.stop ();
//     cerr << "CellWidget::redraw() - " << _redrawRectCount
//          << " in " << timer.getCombTime() << "s ("
//          << setprecision(3) << (timer.getCombTime()/_redrawRectCount) << " s/r)";
//     if ( _drawingQuery.getGoCount() )
//       cerr << " " << _drawingQuery.getGoCount()
//            << " (" << setprecision(3) << (timer.getCombTime()/_drawingQuery.getGoCount()) << " s/go)";
//     else
//       cerr << " 0 Gos";
//     if ( _drawingQuery.getExtensionGoCount() )
//       cerr << " " << _drawingQuery.getExtensionGoCount()
//            << " (" << setprecision(3) << (timer.getCombTime()/_drawingQuery.getExtensionGoCount()) << " s/ego)";
//     else
//       cerr << " 0 eGos";
//     if ( _drawingQuery.getInstanceCount() )
//       cerr << " " << _drawingQuery.getInstanceCount()
//            << " (" << setprecision(3) << (timer.getCombTime()/_drawingQuery.getInstanceCount()) << " s/inst)";
//     else
//       cerr << " 0 Instances";
//     cerr << endl;
  }


  void  CellWidget::redrawSelection ( QRect redrawArea )
  {
    _drawingPlanes.copyToSelect ( redrawArea.x()
                                , redrawArea.y()
                                , redrawArea.width()
                                , redrawArea.height()
                                );

    _drawingPlanes.select ( PlaneId::Selection );
    _drawingPlanes.painterBegin ();
    _drawingPlanes.painter().setPen        ( Qt::NoPen );
    _drawingPlanes.painter().setBackground ( Graphics::getBrush("background") );
    _drawingPlanes.painter().setClipRect   ( redrawArea );

    if ( getCell() ) {
      Box                    redrawBox = displayToDbuBox ( redrawArea );
      SelectorSet::iterator  iselector;

      forEach ( BasicLayer*, basicLayer, _technology->getBasicLayers() ) {
      //if ( !isDrawableLayer(basicLayer->getName()) ) continue;

        _drawingPlanes.setPen   ( Graphics::getPen  (basicLayer->getName()) );
        _drawingPlanes.setBrush ( Graphics::getBrush(basicLayer->getName()) );

        iselector = _selectors.begin ();
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
          if ( !component ) break;
          if ( !component->getLayer() ) continue;
          if ( !component->getLayer()->contains(*basicLayer) ) continue;

          _drawingQuery.drawGo ( dynamic_cast<Go*>(occurrence.getEntity())
                               , *basicLayer
                               , redrawBox
                               , transformation
                               );
        }
      }

      _drawingPlanes.setPen   ( Graphics::getPen  ("rubber") );
      _drawingPlanes.setBrush ( Graphics::getBrush("rubber") );

      for ( ; iselector != _selectors.end() ; iselector++ ) {
        Occurrence      occurrence     = (*iselector)->getOccurrence();
        Transformation  transformation = occurrence.getPath().getTransformation();

        Rubber* rubber = dynamic_cast<Rubber*>(occurrence.getEntity());
        if ( !rubber ) break;

        _drawingQuery.drawRubber ( rubber, redrawBox, transformation );
      }

      Name extensionName = "";
      for ( ; iselector != _selectors.end() ; iselector++ ) {
        Occurrence      occurrence     = (*iselector)->getOccurrence();
        Transformation  transformation = occurrence.getPath().getTransformation();

        ExtensionGo* eGo = dynamic_cast<ExtensionGo*>(occurrence.getEntity());
        if ( !eGo ) break;

        if ( eGo->getName() != extensionName ) {
          extensionName = eGo->getName();
          _drawingQuery.setDrawExtensionGo ( extensionName );
        }

        if ( isDrawable(extensionName) )
          _drawingQuery.drawExtensionGo ( this, eGo, NULL, redrawBox, transformation );
      }

      repaint ();
    }

    _drawingPlanes.painterEnd ();
    _selectionHasChanged = false;
  }


  void  CellWidget::setLayerVisible ( const Name& layer, bool visible )
  {
    if ( !_palette ) return;
    _palette->setItemVisible ( layer, visible ); 
  }


  bool  CellWidget::isDrawable ( const Name& name )
  {
    PaletteItem* item = (_palette) ? _palette->find(name) : NULL;

    return (!item || item->isItemVisible())
      && ( Graphics::getThreshold(name)/DbU::lambda(1.0) < getScale() );
  }


  bool  CellWidget::isDrawableLayer ( const Name& layerName )
  {
    PaletteItem* item = (_palette) ? _palette->find(layerName) : NULL;

    return !item || item->isItemVisible();
  }


  bool  CellWidget::isDrawableExtension ( const Name& extensionName )
  {
    PaletteItem* item = (_palette) ? _palette->find(extensionName) : NULL;

    return (!item || item->isItemVisible());
  }


  bool  CellWidget::isSelectable ( const Name& name ) const
  {
    PaletteItem* item = (_palette) ? _palette->find(name) : NULL;

    return (!item || item->isItemSelectable());
  }


  bool  CellWidget::isSelectable ( const Layer* layer ) const
  {
    PaletteItem* item = NULL;
    forEach ( BasicLayer*, ilayer, layer->getBasicLayers() ) {
      item = (_palette) ? _palette->find(ilayer->getName()) : NULL;
      if ( item && item->isItemSelectable() )
        return true;
    }
    return false;
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


  void  CellWidget::drawLine ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2, bool mode )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( mode );
    _drawingPlanes.painter().drawLine ( dbuToDisplayPoint(x1,y1), dbuToDisplayPoint(x2,y2) );
  }


  void  CellWidget::drawLine ( const Point& p1, const Point& p2, bool mode )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( mode );
    _drawingPlanes.painter().drawLine ( dbuToDisplayPoint(p1), dbuToDisplayPoint(p2) );
  }


  void  CellWidget::drawScreenLine ( const QPoint& p1, const QPoint& p2, size_t plane, bool mode )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( mode );
    _drawingPlanes.painter(plane).drawLine ( p1, p2 );
  }


  void  CellWidget::drawScreenPolyline ( const QPoint* points, int count, int width, size_t plane )
  {
    QPen pen = _drawingPlanes.painter(plane).pen ();
    pen.setWidth ( width );

    _drawingPlanes.painter(plane).setPen ( pen );
    _drawingPlanes.painter(plane).drawPolyline ( points, count );
  }


  void  CellWidget::drawScreenRect ( const QPoint& p1, const QPoint& p2, size_t plane )
  {
    _drawingPlanes.setLineMode ( false );
    _drawingPlanes.painter(plane).drawRect ( QRect(p1,p2) );
  }


  void  CellWidget::drawScreenRect ( const QRect& r, size_t plane )
  {
    _redrawRectCount++;
    _drawingPlanes.setLineMode ( false );
    _drawingPlanes.painter(plane).drawRect ( r );
  }


  void  CellWidget::drawGrid ( QRect redrawArea )
  {
    _drawingPlanes.select       ( PlaneId::Normal );
    _drawingPlanes.painterBegin ();
    _drawingPlanes.painter      ().setClipRect   ( redrawArea );
    _drawingPlanes.painter      ( PlaneId::Normal ).setPen ( Graphics::getPen("grid") );

    Box redrawBox = displayToDbuBox ( redrawArea ).inflate ( DbU::lambda(1.0) );

    bool lambdaGrid = false;
    if ( Graphics::getThreshold("grid")/DbU::lambda(1.0) < getScale()/5 )
      lambdaGrid = true;

    DbU::Unit  gridStep      = DbU::getSnapGridStep();
    DbU::Unit  superGridStep = gridStep*5;
    DbU::Unit  xGrid;
    DbU::Unit  yGrid;
    QPoint     center;

    for ( yGrid = DbU::getOnSnapGrid(redrawBox.getYMin())
        ; yGrid < redrawBox.getYMax()
        ; yGrid += gridStep
        ) {
      for ( xGrid = DbU::getOnSnapGrid(redrawBox.getXMin())
          ; xGrid < redrawBox.getXMax()
          ; xGrid += gridStep
          ) {
        center = dbuToDisplayPoint(xGrid,yGrid);
        if ( (xGrid % superGridStep) || (yGrid % superGridStep) ) {
          if ( lambdaGrid ) {
            _drawingPlanes.painter(PlaneId::Normal).drawPoint ( center );
          }
        } else {
          if ( lambdaGrid ) {
            _drawingPlanes.painter(PlaneId::Normal).drawLine ( center.x()-3, center.y()  , center.x()+3, center.y()   );
            _drawingPlanes.painter(PlaneId::Normal).drawLine ( center.x()  , center.y()-3, center.x()  , center.y()+3 );
          } else {
            _drawingPlanes.painter(PlaneId::Normal).drawPoint ( center );
          }
        }
      }
    }

    _drawingPlanes.copyToSelect ( redrawArea );
    _drawingPlanes.painterEnd   ();
    repaint ();
  }


  void  CellWidget::goLeft ( int dx )
  {
    if ( !dx ) dx = geometry().size().width() / 4;

    _redrawManager.goLeft ( dx );
  }


  void  CellWidget::goRight ( int dx )
  {
    if ( !dx ) dx = geometry().size().width() / 4;

    _redrawManager.goRight ( dx );
  }


  void  CellWidget::goUp ( int dy )
  {
    if ( !dy ) dy = geometry().size().height() / 4;

    _redrawManager.goUp ( dy );
  }


  void  CellWidget::goDown ( int dy )
  {
    if ( !dy ) dy = geometry().size().height() / 4;

    _redrawManager.goDown ( dy );
  }


  void  CellWidget::displayReframe ()
  {
    _offsetVA.rx() = _stripWidth;
    _offsetVA.ry() = _stripWidth;

    DbU::Unit xmin = (DbU::Unit)( _visibleArea.getXMin() - ((float)_offsetVA.x()/getScale()) );
    DbU::Unit xmax = (DbU::Unit)( xmin + ((float)_drawingPlanes.width()/getScale()) ) ;
    DbU::Unit ymax = (DbU::Unit)( _visibleArea.getYMax() + ((float)_offsetVA.y()/getScale()) );
    DbU::Unit ymin = (DbU::Unit)( ymax - ((float)_drawingPlanes.height()/getScale()) ) ;

    _displayArea = Box ( xmin, ymin, xmax, ymax );

    _redrawManager.refresh ();
  }


  Box  CellWidget::computeVisibleArea ( float scale ) const
  {
    Point center = _visibleArea.getCenter();

    return Box ( (DbU::Unit)( center.getX() - width () / (scale*2) ) 
               , (DbU::Unit)( center.getY() - height() / (scale*2) ) 
               , (DbU::Unit)( center.getX() + width () / (scale*2) )
               , (DbU::Unit)( center.getY() + height() / (scale*2) )
               );
  }


  Box  CellWidget::computeVisibleArea ( float scale, const Point& topLeft ) const
  {
    return Box ( topLeft.getX()
               , (DbU::Unit)( topLeft.getY() - height() / scale )
               , (DbU::Unit)( topLeft.getX() + width () / scale )
               , topLeft.getY() 
               );
  }


  Box  CellWidget::computeVisibleArea ( const Box& area, float& scale ) const
  {
    int  widgetWidth  = width  ();
    int  widgetHeight = height ();

    float scaleX = widgetWidth  / (float)area.getWidth ();
    float scaleY = widgetHeight / (float)area.getHeight();
    scale = min ( scaleX, scaleY );

    Point center = area.getCenter();

    widgetWidth  /= 2;
    widgetHeight /= 2;

    return Box ( (DbU::Unit)( center.getX() - widgetWidth  / scale ) 
               , (DbU::Unit)( center.getY() - widgetHeight / scale ) 
               , (DbU::Unit)( center.getX() + widgetWidth  / scale )
               , (DbU::Unit)( center.getY() + widgetHeight / scale )
               );
  }


  void  CellWidget::setScale ( float scale )
  {
  //cerr << "CellWidget::setScale() - " << scale << endl;

    _state->setTopLeft ( getTopLeft() );
    _visibleArea = computeVisibleArea ( scale );
    _state->setScale ( scale );

  //cerr << "_visibleArea: " << _visibleArea << " (offset: " << _offsetVA.x() << ")" << endl;
  //cerr << "              " << center << endl;

    displayReframe ();
  }


  void  CellWidget::scaleHistoryUp ()
  {
    _state->setTopLeft ( getTopLeft() );
    if ( _state->scaleHistoryUp () ) {
      _visibleArea = computeVisibleArea ( _state->getScale(), _state->getTopLeft() );
      displayReframe ();
    }
  }


  void  CellWidget::scaleHistoryDown ()
  {
    _state->setTopLeft ( getTopLeft() );
    if ( _state->scaleHistoryDown () ) {
      _visibleArea = computeVisibleArea ( _state->getScale(), _state->getTopLeft() );
      displayReframe ();
    }
  }


  void  CellWidget::reframe ()
  {
  //cerr << "CellWidget::reframe() - scale:" << _state->getScale()
  //     << " topLeft:" << _state->getTopLeft() << endl;

    _visibleArea = computeVisibleArea ( _state->getScale(), _state->getTopLeft() );
    displayReframe ();
  }


  void  CellWidget::reframe ( const Box& box, bool historyEnable )
  {
  //cerr << "CellWidget::reframe() - " << box << endl;
  //cerr << "  widget size := " << _drawingPlanes.width() << "x" << _drawingPlanes.height() << endl;

  //cerr << "  CellWidget::reframe() - widget size := " << width() << "x" << height() << endl;

    bool backupHistoryEnable = _state->getHistoryEnable ();
    _state->setHistoryEnable ( historyEnable );
    _state->setTopLeft       ( getTopLeft() );

    float scale;
    _visibleArea = computeVisibleArea ( box, scale );
    _state->setScale ( scale );
    displayReframe ();

    _state->setHistoryEnable ( backupHistoryEnable );

  //cerr << "  _displayArea: " << _displayArea << " (offset: " << _offsetVA.x() << ")" << endl;
  }


  void  CellWidget::fitToContents ( bool historyEnable )
  {
  //cerr << "CellWidget::fitToContents()" << endl;

    Box boundingBox = Box ( DbU::lambda(0)
                          , DbU::lambda(0)
                          , DbU::lambda(10)
                          , DbU::lambda(50)
                          );

    if ( getCell() ) boundingBox = getCell()->getBoundingBox();
    reframe ( boundingBox, historyEnable );
  }


  void  CellWidget::fitToNet ( const Net* net, bool historyEnable )
  {
    if ( !net )
      throw Error ( "<b>CellWidget::fitToNet()</b>: NULL net argument." );

    if ( net->getCell() != getCell() )
      throw Error ( "<b>CellWidget::fitToNet()</b>:<br>"
                    "Net %s (cell: %s) do not belong to Cell %s"
                  , Graphics::toHtml(getString(net->getName())).c_str()
                  , Graphics::toHtml(getString(net->getCell()->getName())).c_str()
                  , Graphics::toHtml(getString(getCell()->getName())).c_str()
                  );

    Box boundingBox = net->getBoundingBox ();
    if ( !boundingBox.isEmpty() )
      reframe ( boundingBox, historyEnable );
  }


  void  CellWidget::_goLeft ( int dx )
  {
    _visibleArea.translate ( - (DbU::Unit)( dx / getScale() ) , 0 );

    if ( _offsetVA.rx() - dx >= 0 ) {
      _offsetVA.rx() -= dx;
      repaint ();
      return;
    }

    int shift = ( 1 + ( dx - _offsetVA.rx() ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( - (DbU::Unit)( shift / getScale() ) , 0 );
    _offsetVA.rx() -= dx - shift;

    if ( shift >= _drawingPlanes.width() )
      _redrawManager.refresh ();
    else {
      _drawingPlanes.shiftLeft ( shift );
      _redraw ( QRect ( QPoint(0,0), QSize(shift,_drawingPlanes.height()) ) );
    }

    assert ( _offsetVA.rx() >= 0 );
  }


  void  CellWidget::_goRight ( int dx )
  {
  //cerr << "CellWidget::goRight() - dx: " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;

    _visibleArea.translate ( (DbU::Unit)( dx / getScale() ) , 0 );

    if ( _offsetVA.rx() + dx < 2*_stripWidth ) {
      _offsetVA.rx() += dx;
      repaint ();
      return;
    }

    int shift = ( ( _offsetVA.rx() + dx ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( (DbU::Unit)( shift / getScale() ) , 0 );
    _offsetVA.rx() += dx - shift;

    if ( shift >= _drawingPlanes.width() )
      _redrawManager.refresh ();
    else {
      _drawingPlanes.shiftRight ( shift );
      _redraw ( QRect ( QPoint(_drawingPlanes.width()-shift,0)
                      , QSize (shift,_drawingPlanes.height()) ) );
    }

    assert ( _offsetVA.rx() >= 0 );
  }


  void  CellWidget::_goUp ( int dy )
  {
  //cerr << "CellWidget::shiftUp() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

    _visibleArea.translate ( 0, (DbU::Unit)( dy / getScale() ) );

    if ( _offsetVA.ry() - dy >= 0 ) {
      _offsetVA.ry() -= dy;
      repaint ();
      return;
    }

    int shift = ( 1 + ( dy - _offsetVA.ry() ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( 0, (DbU::Unit)( shift / getScale() ) );
    _offsetVA.ry() -= dy - shift;

    if ( shift >= _drawingPlanes.height() )
      _redrawManager.refresh ();
    else {
      _drawingPlanes.shiftUp ( shift );
      _redraw ( QRect ( QPoint(0,0), QSize(_drawingPlanes.width(),shift) ) );
    }

    assert ( _offsetVA.ry() >= 0 );
  }


  void  CellWidget::_goDown ( int dy )
  {
  //cerr << "CellWidget::shiftDown() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

    _visibleArea.translate ( 0, - (DbU::Unit)( dy / getScale() ) );

    if ( _offsetVA.ry() + dy < 2*_stripWidth ) {
      _offsetVA.ry() += dy;
      repaint ();
      return;
    }

    int shift = ( ( _offsetVA.ry() + dy ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( 0, - (DbU::Unit)( shift / getScale() ) );
    _offsetVA.ry() += dy - shift;

    if ( shift >= _drawingPlanes.height() )
      _redrawManager.refresh ();
    else {
      _drawingPlanes.shiftDown ( shift );
      _redraw ( QRect ( QPoint (0,_drawingPlanes.height()-shift)
                      , QSize  (_drawingPlanes.width(), shift) ) );
    }

    assert ( _offsetVA.ry() >= 0 );
  }


  void  CellWidget::_refresh ()
  {
    _redraw ( QRect(QPoint(0,0),_drawingPlanes.size()) );
  }


  void  CellWidget::showEvent ( QShowEvent* )
  {
  //cerr << "CellWidget::showEvent() - size: " << geometry().width() << "x" << geometry().height() << endl;
    if ( _cellChanged )
      fitToContents ( false );
  }


  void  CellWidget::paintEvent ( QPaintEvent* event )
  {
//  static Timer   timer;
//  static time_t  prevTime = 0;
//  static time_t  currTime = 0;

//  timer.start ();
    _drawingPlanes.painterBegin ( PlaneId::Widget );
    _drawingPlanes.copyToScreen ();
    for ( size_t i=0 ; i<_commands.size() ; i++ )
      _commands[i]->draw ( this );

    if ( isDrawable("spot") ) _spot.moveTo ( _mousePosition );
    _drawingPlanes.painterEnd ( PlaneId::Widget );
//  timer.stop ();

//  time ( &currTime );
//  double delta = difftime ( currTime, prevTime );

//  if ( delta )
//    cerr << "CellWidget::paintEvent() - lagging: " << delta << endl;

//  prevTime = currTime;
  }


  void  CellWidget::resizeEvent ( QResizeEvent* )
  {
//  cerr << "CellWidget::resizeEvent() - size: " << geometry().width() << "x" << geometry().height() << endl;

    QSize uaDelta ( 0, 0 );
    QSize uaSize  = geometry().size();

    uaSize.rwidth () += 2*_stripWidth;
    uaSize.rheight() += 2*_stripWidth;

    if ( uaSize.width () > _drawingPlanes.width () )
      uaDelta.rwidth () = uaSize.width () - _drawingPlanes.width ();

    if ( uaSize.height() > _drawingPlanes.height() )
      uaDelta.rheight() = uaSize.height() - _drawingPlanes.height();

    if ( uaDelta.width() || uaDelta.height() ) {
      _displayArea.inflate ( 0, 0, (DbU::Unit)(uaDelta.width()/getScale()), (DbU::Unit)(uaDelta.height()/getScale()) );
      _visibleArea.inflate ( 0, 0, (DbU::Unit)(uaDelta.width()/getScale()), (DbU::Unit)(uaDelta.height()/getScale()) );

      QSize bufferSize ( ( ( uaSize.width () / _stripWidth ) + 1 ) * _stripWidth
                       , ( ( uaSize.height() / _stripWidth ) + 1 ) * _stripWidth );
      _drawingPlanes.resize ( bufferSize );
    }

    _redrawManager.refresh ();
  }


  void  CellWidget::wheelEvent ( QWheelEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->wheelEvent ( this, event );

    if ( !commandActive ) QWidget::wheelEvent ( event );
  }


  void  CellWidget::keyPressEvent ( QKeyEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->keyPressEvent ( this, event );

    if ( !commandActive ) QWidget::keyPressEvent ( event );
  }


  void  CellWidget::keyReleaseEvent ( QKeyEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->keyReleaseEvent ( this, event );

    if ( !commandActive ) QWidget::keyReleaseEvent ( event );
  }


  void  CellWidget::mouseMoveEvent ( QMouseEvent* event )
  {
    bool commandActive = false;
    for ( size_t i=0 ; i<_commands.size() && !commandActive; i++ )
      commandActive = _commands[i]->mouseMoveEvent ( this, event );

    if ( !commandActive ) {
      Point mousePoint = screenToDbuPoint ( _mousePosition = event->pos() );
      emit mousePositionChanged ( Point ( DbU::getOnSnapGrid(mousePoint.getX())
                                        , DbU::getOnSnapGrid(mousePoint.getY())
                                        ) );

      repaint ();
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


  QRect  CellWidget::dbuToDisplayRect ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2, bool usePoint ) const
  {
    int width, height;

    if ( usePoint ) {
      width  = dbuToDisplayX(x2) - dbuToDisplayX(x1);
      height = dbuToDisplayY(y1) - dbuToDisplayY(y2);
    } else {
      width  = dbuToDisplayLength ( x2 - x1 );
      height = dbuToDisplayLength ( y2 - y1 );
    }

    return QRect ( dbuToDisplayX(x1)
                 , dbuToDisplayY(y2)
                 , width  ? width  : 1
                 , height ? height : 1
                 );
  }


  QRect  CellWidget::dbuToDisplayRect ( const Box& box, bool usePoint ) const
  {
    return dbuToDisplayRect ( box.getXMin()
                            , box.getYMin()
                            , box.getXMax()
                            , box.getYMax()
                            , usePoint
                            );
  }


  QPoint  CellWidget::dbuToScreenPoint ( DbU::Unit x, DbU::Unit y ) const
  {
    return QPoint ( dbuToScreenX(x), dbuToScreenY(y) );
  }


  void  CellWidget::setCell ( Cell* cell )
  {
  //cerr << "CellWidget::setCell() - " << cell << endl;

    if ( cell == getCell() ) return;

    openRefreshSession ();

    shared_ptr<State>  state ( new State(cell) );
    setState ( state );

    fitToContents ( false );

    _state->setHistoryEnable ( true );

    closeRefreshSession ();
  }


  void  CellWidget::setState ( shared_ptr<State>& state )
  {
  //cerr << "CellWidget::setState() - " << state->getName() << endl;

    if ( state == _state ) return;

    openRefreshSession ();

    cellPreModificate ();
    _state->getSelection  ().clear ();
    _state->setCellWidget ( NULL );
    _state->setTopLeft    ( getTopLeft() ); 

    _cellChanged = true;
    _state       = state;

//     cerr << "  about to restore " << (void*)_state.get()
//          << " " << _state->getName()
//          << ": _state->setTopLeft("
//          << DbU::getValueString(_state->getTopLeft().getX()) << ","
//          << DbU::getValueString(_state->getTopLeft().getY()) << ")" << endl;

    _state->setHistoryEnable ( false );
    _state->setCellWidget ( this );
    _drawingQuery    .setCell       ( getCell() );
    _drawingQuery    .setStartLevel ( _state->getStartLevel() );
    _drawingQuery    .setStopLevel  ( _state->getStopLevel() );
    _textDrawingQuery.setCell       ( getCell() );

    reframe ();
    _state->setHistoryEnable ( true );

    emit cellChanged        ( getCell() );
    emit stateChanged       ( _state );
    emit queryFilterChanged ();

    cellPostModificate ();

    closeRefreshSession ();
  }


  Occurrences  CellWidget::getOccurrencesUnder ( const Box& area ) const
  {
    return getCell()->getOccurrencesUnder(area).getSubSet(Occurrences_IsSelectable(this));
  }


  void  CellWidget::select ( const Net* net )
  {
    ++_delaySelectionChanged;

    bool added = _state->getSelection().add ( net );

    if ( !--_delaySelectionChanged && added ) emit selectionChanged(_selectors);
  }


  bool  CellWidget::isSelected ( Occurrence occurrence )
  {
	if ( !occurrence.isValid() )
      throw Error ( "Can't select occurrence : invalid occurrence" );

	if ( occurrence.getOwnerCell() != getCell() ) {
      string s1 = Graphics::toHtml ( getString(occurrence.getOwnerCell()) );
      string s2 = Graphics::toHtml ( getString(getCell()) );
      throw Error ( "Can't select occurrence : incompatible occurrence %s vs. %s"
                  , s1.c_str(), s2.c_str() );
    }

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

	if ( occurrence.getOwnerCell() != getCell() ) {
      string s1 = Graphics::toHtml ( getString(occurrence.getOwnerCell()) );
      string s2 = Graphics::toHtml ( getString(getCell()) );
      throw Error ( "Can't select occurrence : incompatible occurrence %s vs. %s" 
                  , s1.c_str(), s2.c_str() );
    }

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
    if ( !_delaySelectionChanged ) emit selectionChanged(_selectors);
  }


  void  CellWidget::selectOccurrencesUnder ( Box selectArea )
  {
    ++_delaySelectionChanged;

    if ( !_state->cumulativeSelection() ) {
      openRefreshSession ();
      unselectAll ();
      closeRefreshSession ();
    }
    bool added = _state->getSelection().add ( selectArea );

    if ( !--_delaySelectionChanged && added ) emit selectionChanged(_selectors);
  }


  void  CellWidget::unselect ( const Net* net )
  {
    ++_delaySelectionChanged;

    bool removed = _state->getSelection().remove ( net );
    if ( !--_delaySelectionChanged && removed ) emit selectionChanged(_selectors);
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
    if ( !_delaySelectionChanged ) emit selectionChanged(_selectors);
  }


  void  CellWidget::unselectAll ()
  {
    ++_delaySelectionChanged;

    _state->getSelection().clear ();
    _unselectAll ();

    if ( !--_delaySelectionChanged ) emit selectionChanged(_selectors);
  }


  void  CellWidget::toggleSelection ( Occurrence occurrence )
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
    if ( _state->showSelection() ) _redrawManager.refresh ();

    emit selectionToggled ( occurrence );
  }


  void  CellWidget::_select ( const Net* net )
  {
    select ( Occurrence(net) );
    forEach ( Component*, component, net->getComponents() ) {
      Occurrence occurrence ( *component );
      select ( occurrence );
    }
    forEach ( Rubber*, rubber, net->getRubbers() ) {
      Occurrence occurrence ( *rubber );
      select ( occurrence );
    }
    if ( _state->showSelection() ) _redrawManager.refresh ();
  }


  void  CellWidget::_unselect ( const Net* net )
  {
    unselect ( Occurrence(net) );
    forEach ( Component*, component, net->getComponents() ) {
      Occurrence occurrence ( *component );
      unselect ( occurrence );
    }
    forEach ( Rubber*, rubber, net->getRubbers() ) {
      Occurrence occurrence ( *rubber );
      unselect ( occurrence );
    }
    if ( _state->showSelection() ) _redrawManager.refresh ();
  }


  void  CellWidget::_selectOccurrencesUnder ( Box selectArea )
  {
    forEach ( Occurrence, ioccurrence, getOccurrencesUnder(selectArea) )
      select ( *ioccurrence );
  }


  void  CellWidget::_unselectAll ()
  {
    SelectorSet::iterator iselector;
    while ( !_selectors.empty() )
      (*_selectors.begin())->detachFrom ( this );

    if ( !_selectionHasChanged ) _selectionHasChanged = true;
    if ( _state->showSelection() ) _redrawManager.refresh ();
  }


  void  CellWidget::cellPreModificate ()
  {
    openRefreshSession ();
    _unselectAll ();
    
    emit selectionChanged(_selectors);
    emit cellPreModificated ();

    closeRefreshSession ();
  }


  void  CellWidget::cellPostModificate ()
  {
    openRefreshSession ();
    _cellModificated = true;

    ++_delaySelectionChanged;
    _state->getSelection().revalidate ();

    updatePalette ();
    _redrawManager.refresh ();

    --_delaySelectionChanged;
    emit selectionChanged(_selectors);
    emit cellPostModificated ();

    closeRefreshSession ();
  }


} // End of Hurricane namespace.
