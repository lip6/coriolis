
// -*- C++ -*-


# ifndef  __SCREENLAYER_H__
#   define  __SCREENLAYER_H__

# include  <string>
# include  <QBrush>
# include  <QPixmap>
# include  <QWidget>

class QPaintEvent;

# include  "Commons.h"


namespace Hurricane {


  class Name;
  class BasicLayer;
  class ScreenLayer;


  class LayerSample : public QWidget {
      Q_OBJECT;

    protected:
      QPixmap       _sample;
      ScreenLayer*  _layer;

    public:
                   LayerSample  ( ScreenLayer* layer );

    protected:
             void  redraw       ();
             void  paintEvent   ( QPaintEvent* );

    public:

      inline void  setVisible   ( bool state );
  };


  class ScreenLayer {

    protected:
      BasicLayer* _layer;
      size_t      _index;
      bool        _isVisible;
      QBrush      _brush;

    // Constructor.
    public:
                          ScreenLayer   ( BasicLayer* layer, size_t index, bool isVisible );

    // Predicates.
    public:
             bool         isVisible     () const { return _isVisible; };
    // Accessors.
    public:
             size_t       getIndex      () const { return _index; };
             BasicLayer*  getBasicLayer () const { return _layer; };
      const  Name&        getName       () const;

    // Methods.
    public:
      const  QBrush&      getBrush      () const { return _brush; };
             LayerSample* getSample     ();

    // Modifiers.
    public:
      inline void         setVisible    ( bool state ) { _isVisible=state; };
  };




  inline void  LayerSample::setVisible ( bool state )
  {
    _layer->setVisible ( state );
  }


} // End of Hurricane namespace.


# endif
