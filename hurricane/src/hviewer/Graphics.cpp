

// -*- C++ -*-


# include  <assert.h>

# include  <QBrush>
# include  <QPen>
# include  <QFont>
# include  <QApplication>

# include  "Name.h"

# include  "DisplayStyle.h"
# include  "Graphics.h"




namespace Hurricane {


  Graphics* Graphics::_singleton = NULL;


  Graphics::Graphics ()
    : _styles()
    , _active(NULL)
  {
    _styles.push_back ( new DisplayStyle("Fallback") );
    _active = _styles[0];
  }


  Graphics::~Graphics ()
  {
    delete _singleton;
  }


  Graphics* Graphics::getGraphics ()
  {
    if ( !_singleton )
      _singleton = new Graphics ();

    return _singleton;
  }


  const QFont  Graphics::getFixedFont ( bool bold, bool underline )
  {
    const QFont defaultFont = QApplication::font ();

    QFont fixedFont ( "Bitstream Vera Sans Mono", defaultFont.pointSize() );
    if ( bold      ) fixedFont.setBold      ( true );
    if ( underline ) fixedFont.setUnderline ( true );

    return fixedFont;
  }


  size_t  Graphics::_findStyle ( const Name& name ) const
  {
    size_t si = 0;
    for ( ; si < _styles.size() ; si++ )
      if ( _styles[si]->getName() == name )
        break;

    return si;
  }



  void  Graphics::_addStyle ( DisplayStyle* displayStyle )
  {
    size_t si = _findStyle(displayStyle->getName());
    if ( si != _styles.size() ) {
      cerr << "[WARNING] Graphics::addStyle(): overriding style \""
           << displayStyle->getName() << "\"." << endl;

      delete _styles [ si ];
    } else {
      _styles.push_back ( NULL );
    }

    _styles [ si ] = displayStyle;
    _active = _styles [ si ];
  }


  void  Graphics::_setStyle ( const Name& key )
  {
    size_t si = _findStyle(key);
    if ( si == _styles.size() ) {
      cerr << "[WARNING] Graphics::setStyle(): no style named \"" << key << "\"." << endl;
      return;
    }

    _active = _styles [ si ];
  }


  DisplayStyle* Graphics::_getStyle ( const Name& key )
  {
    size_t si = _findStyle(key);
    if ( si == _styles.size() ) 
      return NULL;

    return _styles [ si ];
  }


  void  Graphics::addStyle ( DisplayStyle* displayStyle )
  {
    getGraphics()->_addStyle ( displayStyle );
  }


  void  Graphics::setStyle ( const Name& key )
  {
    getGraphics()->_setStyle ( key );
  }


  DisplayStyle* Graphics::getStyle ( const Name& key )
  {
    return getGraphics()->_getStyle ( key );
  }


  DisplayStyle* Graphics::getStyle ()
  {
    return getGraphics()->_getStyle ();
  }


  const Name& Graphics::getGroup ( const Name& key )
  {
    return getGraphics()->_getGroup ( key );
  }


  const QColor& Graphics::getColor ( const Name& key )
  {
    return getGraphics()->_getColor ( key );
  }


  const QPen& Graphics::getPen ( const Name& key )
  {
    return getGraphics()->_getPen ( key );
  }


  const QBrush& Graphics::getBrush ( const Name& key )
  {
    return getGraphics()->_getBrush ( key );
  }


  const string& Graphics::getPattern ( const Name& key )
  {
    return getGraphics()->_getPattern ( key );
  }


  float  Graphics::getThreshold ( const Name& key )
  {
    return getGraphics()->_getThreshold ( key );
  }




} // End of Hurricane namespace.
