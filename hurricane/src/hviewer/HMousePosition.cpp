
// -*- C++ -*-


# include  <QHBoxLayout>

# include  "hurricane/viewer/DynamicLabel.h"
# include  "hurricane/viewer/HMousePosition.h"


namespace Hurricane {


  HMousePosition::HMousePosition ( QWidget* parent )
    : QWidget(parent)
    , _xPosition(NULL)
    , _yPosition(NULL)
  {
    _xPosition = new DynamicLabel ();
    _xPosition->setStaticText  ( "X:" );
    _xPosition->setDynamicText ( "N/A" );
    
    _yPosition = new DynamicLabel ();
    _yPosition->setStaticText  ( "Y:" );
    _yPosition->setDynamicText ( "N/A" );

    QHBoxLayout* layout = new QHBoxLayout ();
    layout->addWidget ( _xPosition );
    layout->addWidget ( _yPosition );
    layout->setContentsMargins ( 0, 0, 0, 0 );

    setLayout ( layout );
  }


  void  HMousePosition::setPosition ( const Point& position )
  {
    _xPosition->setDynamicText ( position.getX() );
    _yPosition->setDynamicText ( position.getY() );
  }


} // End of Hurricane namespace.
