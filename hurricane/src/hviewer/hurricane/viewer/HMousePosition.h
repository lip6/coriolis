
// -*- C++ -*-


# ifndef  __HMOUSE_POSITION_H__
#   define  __HMOUSE_POSITION_H__

# include  <QWidget>

class QLabel;

# include  "hurricane/Commons.h"
# include  "hurricane/Point.h"


namespace Hurricane {


  class DynamicLabel;


  class HMousePosition : public QWidget {
      Q_OBJECT;

    public:
    // Constructor.
                              HMousePosition ( QWidget* parent=NULL );
    public slots:
              void            setPosition    ( const Point& position );

    protected:
    // Internal - Attributes.
              DynamicLabel*   _xPosition;
              DynamicLabel*   _yPosition;

    // Internal - Constructor.
                              HMousePosition ( const DynamicLabel& );
              HMousePosition& operator=      ( const DynamicLabel& );
  };


} // End of Hurricane namespace.


# endif
