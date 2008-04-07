
// -*- C++ -*-


# ifndef  __CELLWIDGET_H__
#   define  __CELLWIDGET_H__

# include  <math.h>

# include  <vector>
# include  <map>

# include  <QWidget>
# include  <QPixmap>
# include  <QPainter>
# include  <QRect>
# include  <QPoint>

class QCursor;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;
class QAction;


# include  "Commons.h"
# include  "Point.h"
# include  "Box.h"
# include  "Transformation.h"

# include  "DisplayStyle.h"




namespace Hurricane {

  class BasicLayer;
  class Go;
  class Cell;
  class Instance;
  class Slice;
  class Segment;
  class Contact;

  class ScreenLayer;
  class LayersList;
//class MapView;


  class CellWidget : public QWidget {
      Q_OBJECT;

    // Attributes.
    protected:
      static const int                       _stripWidth;
                   map<string,DisplayStyle>  _styles;
                   DisplayStyle*             _activeStyle;
                   vector<ScreenLayer*>      _screenLayers;
                   vector<Qt::CursorShape>   _cursors;
//                 MapView*                  _mapView;
                   LayersList*               _layersList;
                   Box                       _displayArea;
                   Box                       _visibleArea;
                   float                     _scale;
                   QPoint                    _offsetVA;
                   QPixmap                   _drawingBuffer;
                   QPainter                  _painter;
                   QPoint                    _lastMousePosition;
                   Cell*                     _cell;
                   bool                      _mouseGo;
                   bool                      _openCell;
                   bool                      _showBoundaries;
                   size_t                    _redrawRectCount;

    public:
                                    CellWidget        ( Cell* cell );
      virtual                      ~CellWidget        ();

    public slots:
              void                  redraw            ( QRect redrawArea );
      inline  void                  redraw            ();
              void                  goLeft            ( int dx = 0 );
              void                  goRight           ( int dx = 0 );
              void                  goUp              ( int dy = 0 );
              void                  goDown            ( int dy = 0 );
              void                  fitToContents     ();
              void                  setScale          ( float scale );
              void                  setShowBoundaries ( bool state );
              void                  reframe           ( const Box& box );
              void                  screenReframe     ();
              void                  shiftLeft         ( int dx );
              void                  shiftRight        ( int dx );
              void                  shiftUp           ( int dy );
              void                  shiftDown         ( int dy );

    // Qt QWidget Overloads.
    public:
              void                  pushCursor        ( Qt::CursorShape cursor );
              void                  popCursor         ();
              QSize                 minimumSizeHint   () const;
              void                  paintEvent        ( QPaintEvent* );
              void                  resizeEvent       ( QResizeEvent* );
              void                  keyPressEvent     ( QKeyEvent* );
              void                  mouseMoveEvent    ( QMouseEvent* );
              void                  mousePressEvent   ( QMouseEvent* );
              void                  mouseReleaseEvent ( QMouseEvent* );

    // Geometric conversions.
    public:
              QRect                 getScreenRect     ( Unit x1, Unit y1, Unit x2, Unit y2 ) const;
              QRect                 getScreenRect     ( const Box& box ) const;
              QPoint                getScreenPoint    ( Unit x, Unit y ) const;
              QPoint                getScreenPoint    ( const Point& point ) const;
      inline  int                   getScreenX        ( Unit x ) const;
      inline  int                   getScreenY        ( Unit y ) const;
      inline  int                   getScreenLength   ( Unit length ) const;
      inline  Unit                  getX              ( int  x ) const;
      inline  Unit                  getY              ( int  y ) const;
      inline  Unit                  getLength         ( int  length ) const;
      inline  Box                   getBox            ( const QRect& rect ) const;

    // Painter control & Hurricane objects drawing primitives.
    public:
              void                  addStyle          ( const string& key, DisplayStyle& displayStyle );
              void                  setStyle          ( const string& key );
      inline  const QPen&           getPen            ( const string& key );
      inline  const QBrush&         getBrush          ( const string& key );
              void                  drawBoundaries    ( const Cell*    , const Box&, const Transformation& );
              void                  drawBoundaries    ( const Instance*, const Box&, const Transformation& );
              bool                  isDrawable        ( const ScreenLayer* screenLayer );
              void                  drawCell          ( const Cell*    , const BasicLayer*, const Box&, const Transformation& );
              void                  drawInstance      ( const Instance*, const BasicLayer*, const Box&, const Transformation& );
              void                  drawSlice         ( const Slice*   , const BasicLayer*, const Box&, const Transformation& );
              void                  drawGo            ( const Go*      , const BasicLayer*, const Box&, const Transformation& );
              void                  drawSegment       ( const Segment* , const BasicLayer*, const Box&, const Transformation& );
              void                  drawContact       ( const Contact* , const BasicLayer*, const Box&, const Transformation& );
              void                  drawBox           ( const Box& );
              void                  drawLine          ( const Point&, const Point& );

    // Accessors.
    public:
//            MapView*              getMapView        () { return _mapView; };
              LayersList*           getLayersList     () { return _layersList; };
              vector<ScreenLayer*>& getScreenLayers   () { return _screenLayers; };
      inline  Cell*                 getCell           () const;
      inline  bool                  showBoundaries    () const;

    // Modifiers.
    public:
  };




inline void  CellWidget::redraw ()
{
  redraw ( QRect(QPoint(0,0),_drawingBuffer.size()) );
}


inline int  CellWidget::getScreenX ( Unit x ) const
{
  return (int)rint ( (float)( x - _displayArea.getXMin() ) * _scale );
}


inline int  CellWidget::getScreenY ( Unit y ) const
{
  return (int)rint ( (float)( _displayArea.getYMax() - y ) * _scale );
}


inline int  CellWidget::getScreenLength ( Unit length ) const
{
  return (int)rint ( (float)length * _scale );
}


inline Unit  CellWidget::getX ( int x ) const
{
  return (Unit)(x/_scale) + _displayArea.getXMin();
}


inline Unit  CellWidget::getY ( int y ) const
{
  return _displayArea.getYMax() - (Unit)(y/_scale);
}


inline Unit  CellWidget::getLength ( int length ) const
{
  return (int)( (float)length / _scale );
}


inline Box  CellWidget::getBox ( const QRect& rect ) const
{
  return Box ( getX(rect.x())
             , getY(rect.y())
             , getX(rect.x()+rect.width ())
             , getY(rect.y()+rect.height()));
}


inline const QPen& CellWidget::getPen ( const string& key )
{
  return _activeStyle->getPen ( key );
}


inline const QBrush& CellWidget::getBrush ( const string& key )
{
  return _activeStyle->getBrush ( key );
}


inline Cell* CellWidget::getCell () const
{
  return _cell;
}


inline bool  CellWidget::showBoundaries () const
{
  return _showBoundaries;
}




} // End of Hurricane namespace.


# endif
