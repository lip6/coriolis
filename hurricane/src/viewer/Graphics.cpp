
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Graphics.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <assert.h>

#include  <Qt>
#include  <QBrush>
#include  <QPen>
#include  <QApplication>

#include  "hurricane/Name.h"
#include  "hurricane/Exception.h"

#include  "hurricane/viewer/DisplayStyle.h"
#include  "hurricane/viewer/Graphics.h"


namespace Hurricane {


  Graphics* Graphics::_singleton = NULL;


  Graphics::Graphics ()
    : _htmlTranslator       ()
    , _styles               ()
    , _active               (NULL)
    , _fireColorScale       ()
    , _rainbowColorScale    ()
    , _temperatureColorScale()
    , _qtEnabled            (false)
  {
    _htmlTranslator.addTranslation ( "<br>"     , "<br>"      );
    _htmlTranslator.addTranslation ( "<em>"     , "<em>"      );
    _htmlTranslator.addTranslation ( "</em>"    , "</em>"     );
    _htmlTranslator.addTranslation ( "<strong>" , "<strong>"  );
    _htmlTranslator.addTranslation ( "</strong>", "</strong>" );
    _htmlTranslator.addTranslation ( "<tt>"     , "<tt>"      );
    _htmlTranslator.addTranslation ( "</tt>"    , "</tt>"     );
    _htmlTranslator.addTranslation ( "<b>"      , "<b>"       );
    _htmlTranslator.addTranslation ( "</b>"     , "</b>"      );
    _htmlTranslator.addTranslation ( "<i>"      , "<i>"       );
    _htmlTranslator.addTranslation ( "</i>"     , "</i>"      );
    _htmlTranslator.addTranslation ( "<big>"    , "<big>"     );
    _htmlTranslator.addTranslation ( "</big>"   , "</big>"    );
    _htmlTranslator.addTranslation ( "<small>"  , "<small>"   );
    _htmlTranslator.addTranslation ( "</small>" , "</small>"  );
    _htmlTranslator.addTranslation ( "<"        , "&lt;"      );
    _htmlTranslator.addTranslation ( ">"        , "&gt;"      );
    _htmlTranslator.addTranslation ( "\n"       , "<br>"      );
  }


  Graphics::~Graphics ()
  {
    delete _singleton;
  }


  Graphics* Graphics::getGraphics ()
  {
    if ( !_singleton ) {
      _singleton = new Graphics ();
      DisplayStyle* fallback = new DisplayStyle("Fallback");
      fallback->setDescription ( "Builtin fallback style" );
      _singleton->_addStyle ( fallback );

      Exception::setHtmlTranslator ( _singleton->_getHtmlTranslator() );
    }

    return _singleton;
  }


  const QFont  Graphics::getFixedFont ( int weight, bool italic, bool underline, int scale )
  {
    const QFont defaultFont = QApplication::font ();

    QFont fixedFont ( "Bitstream Vera Sans Mono", defaultFont.pointSize()+scale );
    fixedFont.setWeight    ( weight );
    fixedFont.setItalic    ( italic );
    fixedFont.setUnderline ( underline );

    return fixedFont;
  }


  const QFont  Graphics::getNormalFont ( bool bold, bool italic, bool underline )
  {
    QFont defaultFont = QApplication::font ();

    defaultFont.setBold      ( bold );
    defaultFont.setItalic    ( italic );
    defaultFont.setUnderline ( underline );

    return defaultFont;
  }


  void  Graphics::_enable ()
  {
    _qtEnabled = true;
    for ( size_t si=0 ; si < _styles.size() ; si++ )
      _styles[si]->qtAllocate ();

    _fireColorScale.qtAllocate ();
    _rainbowColorScale.qtAllocate();
    _temperatureColorScale.qtAllocate();

    Breakpoint::setStopCb ( Graphics::breakpointStopCb );
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

    if ( _qtEnabled )
      _active->qtAllocate ();
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


  void  Graphics::_setStyle ( size_t id )
  {
    if ( id >= _styles.size() ) {
      cerr << "[WARNING] Graphics::setStyle(): no style id \"" << id << "\"." << endl;
      return;
    }

    _active = _styles [ id ];
  }


  DisplayStyle* Graphics::_getStyle ( const Name& key )
  {
//  if ( !_qtEnabled )
//    cerr << "[ERROR] Graphics has not been enabled, you are likely to crash..." << endl;

    size_t si = _findStyle(key);
    if ( si == _styles.size() ) 
      return NULL;

    return _styles [ si ];
  }


  bool  Graphics::isEnabled ()
  {
    return getGraphics()->_qtEnabled;
  }


  void  Graphics::enable ()
  {
    return getGraphics()->_enable();
  }


  void  Graphics::addStyle ( DisplayStyle* displayStyle )
  {
    getGraphics()->_addStyle ( displayStyle );
  }


  void  Graphics::setStyle ( const Name& key )
  {
    getGraphics()->_setStyle ( key );
  }


  void  Graphics::setStyle ( size_t id )
  {
    getGraphics()->_setStyle ( id );
  }


  DisplayStyle* Graphics::getStyle ( const Name& key )
  {
    return getGraphics()->_getStyle ( key );
  }


  DisplayStyle* Graphics::getStyle ()
  {
    return getGraphics()->_getStyle ();
  }


  const vector<DisplayStyle*>& Graphics::getStyles ()
  {
    return getGraphics()->_getStyles ();
  }


  const Name& Graphics::getGroup ( const Name& key )
  {
    return getGraphics()->_getGroup ( key );
  }


  QColor  Graphics::getColor ( const Name& key, int darkening )
  {
    return getGraphics()->_getColor ( key, darkening );
  }


  QPen  Graphics::getPen ( const Name& key, int darkening )
  {
    return getGraphics()->_getPen ( key, darkening );
  }


  QBrush  Graphics::getBrush ( const Name& key, int darkening )
  {
    return getGraphics()->_getBrush ( key, darkening );
  }


  const string& Graphics::getPattern ( const Name& key )
  {
    return getGraphics()->_getPattern ( key );
  }


  float  Graphics::getThreshold ( const Name& key )
  {
    return getGraphics()->_getThreshold ( key );
  }


  int  Graphics::getDarkening ()
  {
    return getGraphics()->_getDarkening ();
  }


  const ColorScale& Graphics::getColorScale ( ColorScale::ScaleType id )
  {
    return getGraphics()->_getColorScale ( id );
  }


  const TextTranslator& Graphics::getHtmlTranslator ()
  {
    return getGraphics()->_getHtmlTranslator();
  }


  string  Graphics::toHtml ( const string& s )
  {
    return getGraphics()->getHtmlTranslator().translate(s);

    // string protect = s;

    // if ( !isEnabled() ) return protect;

    // unsigned int  pos = protect.find ( '<' );
    // while ( pos < protect.size() ) {
    //   protect.replace ( pos, 1, "&lt;" );
    //   pos = protect.find ( '<', pos );
    // }
    // pos = protect.find ( '>' );
    // while ( pos < protect.size() ) {
    //   protect.replace ( pos, 1, "&gt;" );
    //   pos = protect.find ( '>', pos );
    // }
    // return protect;
  }


  bool  Graphics::breakpointStopCb ( const string& message )
  {
    static BreakpointWidget* bpw = NULL;

    if ( !bpw )
      bpw = new BreakpointWidget ();
    bpw->setMessage ( message.c_str() );
    bpw->execNoModal ();

    return ( bpw->result() == QDialog::Accepted );
  }




} // End of Hurricane namespace.
