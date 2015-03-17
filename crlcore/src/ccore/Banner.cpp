// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Banner.cpp"                             |
// +-----------------------------------------------------------------+


# include <time.h>
# include <cstring>
# include <cstdlib>
# include <iomanip>
# include "hurricane/Error.h"
# include "crlcore/Banner.h"


namespace {

  using std::make_pair;
  using std::cerr;
  using std::endl;
  using std::setw;
  using Hurricane::Error;


  const char* badChar =
    "BannerFont::operator[](): Font \"%s\" doesn't has character \'%c\'.\n";
  const char* unevenCharWidth =
    "BannerFont::BigChar::BigChar():\n"
    "    Character '%c' has an uneven width.\n"
    "    (line %d has a different width)\n\n";
  const char* nullCharLine =
    "BannerFont::BigChar::BigChar():\n"
    "    Character '%c' has an unitialized line.\n"
    "    (line %d has NULL pointer)\n";
  const char* badHeight =
    "BannerFont::addChar():\n"
    "    Character '%c' has not the same height as others in font \"%s\".\n"
    "    (font character height := %d)\n";
  const char* dupChar =
    "BannerFont::addChar(): Attempt to redefine character \'%c\' in font \"%s\".\n";
  const char* badBannerWidth =
    "Banner::_ReDraw() :\n\n"
    "    Banner \"%s\" doesnt't fit in screen width (%d characters).\n";


# include  "UnknownBoldNormal14.h"


} // End of anonymous namespace.


namespace CRL {


  BannerFont::FontMap  BannerFont::_fontMap;



// -------------------------------------------------------------------
// Class  :  "BannerFont::BigChar".


  BannerFont::BigChar::BigChar ( char              c
                               , int               height
                               , const char*       lines[]
                               , const BannerFont* font )
    : _character(c)
    , _height(height)
    , _lines(lines)
    , _font(font)
  {
    if ( lines[0] == NULL )
      throw Error ( nullCharLine, _character, _height );

    unsigned width = strlen ( lines[0] );

    for ( int i=1 ; i<_height ; i++ ) {
      if ( lines[i] == NULL )
        throw Error ( nullCharLine, _character, _height );

      if ( strlen(lines[i]) != width )
        throw Error ( unevenCharWidth, _character, _height );
    }
  }


  BannerFont::BigChar::~BigChar ()
  { }


  const char* BannerFont::BigChar::operator[] ( int line ) const
  {
    return _lines[line];
  }


  string  BannerFont::BigChar::_getPrint () const
  {
    string  s;
    for ( int line=0 ; line<_height ; line++ ) {
      s += _lines[line];
      s += "\n";
    }
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "BannerFont::FontMap".


  BannerFont::FontMap::FontMap (): map<const string,const BannerFont*>()
  {
# define  ADDCHAR(c,t) (font->addChar((c),sizeof(t)/sizeof(char*),(t)))

    BannerFont *font;

    try {
      font = new BannerFont ( "Unknown_Bold_Normal_14" );
    // Digits.
      ADDCHAR ( '0', Unknown_Bold_Normal_14_0 );
      ADDCHAR ( '1', Unknown_Bold_Normal_14_1 );
      ADDCHAR ( '2', Unknown_Bold_Normal_14_2 );
      ADDCHAR ( '3', Unknown_Bold_Normal_14_3 );
      ADDCHAR ( '4', Unknown_Bold_Normal_14_4 );
      ADDCHAR ( '5', Unknown_Bold_Normal_14_5 );
      ADDCHAR ( '6', Unknown_Bold_Normal_14_6 );
      ADDCHAR ( '7', Unknown_Bold_Normal_14_7 );
      ADDCHAR ( '8', Unknown_Bold_Normal_14_8 );
      ADDCHAR ( '9', Unknown_Bold_Normal_14_9 );

    // Alphabetical caps.
      ADDCHAR ( 'A', Unknown_Bold_Normal_14_A );
      ADDCHAR ( 'B', Unknown_Bold_Normal_14_B );
      ADDCHAR ( 'C', Unknown_Bold_Normal_14_C );
      ADDCHAR ( 'D', Unknown_Bold_Normal_14_D );
      ADDCHAR ( 'E', Unknown_Bold_Normal_14_E );
      ADDCHAR ( 'F', Unknown_Bold_Normal_14_F );
      ADDCHAR ( 'G', Unknown_Bold_Normal_14_G );
      ADDCHAR ( 'H', Unknown_Bold_Normal_14_H );
      ADDCHAR ( 'I', Unknown_Bold_Normal_14_I );
      ADDCHAR ( 'J', Unknown_Bold_Normal_14_J );
      ADDCHAR ( 'K', Unknown_Bold_Normal_14_K );
      ADDCHAR ( 'L', Unknown_Bold_Normal_14_L );
      ADDCHAR ( 'M', Unknown_Bold_Normal_14_M );
      ADDCHAR ( 'N', Unknown_Bold_Normal_14_N );
      ADDCHAR ( 'O', Unknown_Bold_Normal_14_O );
      ADDCHAR ( 'P', Unknown_Bold_Normal_14_P );
      ADDCHAR ( 'Q', Unknown_Bold_Normal_14_Q );
      ADDCHAR ( 'R', Unknown_Bold_Normal_14_R );
      ADDCHAR ( 'S', Unknown_Bold_Normal_14_S );
      ADDCHAR ( 'T', Unknown_Bold_Normal_14_T );
      ADDCHAR ( 'U', Unknown_Bold_Normal_14_U );
      ADDCHAR ( 'V', Unknown_Bold_Normal_14_V );
      ADDCHAR ( 'W', Unknown_Bold_Normal_14_W );
      ADDCHAR ( 'X', Unknown_Bold_Normal_14_X );
      ADDCHAR ( 'Y', Unknown_Bold_Normal_14_Y );
      ADDCHAR ( 'Z', Unknown_Bold_Normal_14_Z );

    // Alphabetical.
      ADDCHAR ( 'a', Unknown_Bold_Normal_14_a );
      ADDCHAR ( 'b', Unknown_Bold_Normal_14_b );
      ADDCHAR ( 'c', Unknown_Bold_Normal_14_c );
      ADDCHAR ( 'd', Unknown_Bold_Normal_14_d );
      ADDCHAR ( 'e', Unknown_Bold_Normal_14_e );
      ADDCHAR ( 'f', Unknown_Bold_Normal_14_f );
      ADDCHAR ( 'g', Unknown_Bold_Normal_14_g );
      ADDCHAR ( 'h', Unknown_Bold_Normal_14_h );
      ADDCHAR ( 'i', Unknown_Bold_Normal_14_i );
      ADDCHAR ( 'j', Unknown_Bold_Normal_14_j );
      ADDCHAR ( 'k', Unknown_Bold_Normal_14_k );
      ADDCHAR ( 'l', Unknown_Bold_Normal_14_l );
      ADDCHAR ( 'm', Unknown_Bold_Normal_14_m );
      ADDCHAR ( 'n', Unknown_Bold_Normal_14_n );
      ADDCHAR ( 'o', Unknown_Bold_Normal_14_o );
      ADDCHAR ( 'p', Unknown_Bold_Normal_14_p );
      ADDCHAR ( 'q', Unknown_Bold_Normal_14_q );
      ADDCHAR ( 'r', Unknown_Bold_Normal_14_r );
      ADDCHAR ( 's', Unknown_Bold_Normal_14_s );
      ADDCHAR ( 't', Unknown_Bold_Normal_14_t );
      ADDCHAR ( 'u', Unknown_Bold_Normal_14_u );
      ADDCHAR ( 'v', Unknown_Bold_Normal_14_v );
      ADDCHAR ( 'w', Unknown_Bold_Normal_14_w );
      ADDCHAR ( 'x', Unknown_Bold_Normal_14_x );
      ADDCHAR ( 'y', Unknown_Bold_Normal_14_y );
      ADDCHAR ( 'z', Unknown_Bold_Normal_14_z );

      insert ( make_pair("Unknown_Bold_Normal_14",font) );
    }
    catch ( Error &e ) {
      cerr << e.what() << endl;
      exit (1);
    }

# undef ADDCHAR
  }


  BannerFont::FontMap::~FontMap ()
  {
    for ( iterator it=begin() ; it != end() ; it++ )
      delete it->second;
  }


  const BannerFont* BannerFont::FontMap::operator[] ( const string& name ) const
  {
    const_iterator it = find ( name );
    if ( it != end() )
      return it->second;

    return (*this)[ "Unknown_Bold_Normal_14" ];
  }


// -------------------------------------------------------------------
// Class  :  "BannerFont".


  BannerFont::~BannerFont ()
  {
    CharMap::iterator ichar = _table.begin ();
    for ( ; ichar != _table.end() ; ichar++ )
      delete ichar->second;
  }


  const BannerFont* BannerFont::getBannerFont ( const string& name )
  {
    return _fontMap[name];
  }


  void  BannerFont::addChar ( char character, int height, const char* lines[] )
  {
    CharMap::iterator  it    = _table.find  ( character );
    CharMap::iterator  begin = _table.begin ();
  
    if ( it != _table.end() )
      throw Error ( dupChar, _name.c_str(), character );
  
    _table [ character ] = new BigChar ( character, height, lines, this );

    if ( begin == _table.end() ) _height = height;
    if ( _height != height )
      throw Error ( badHeight, _name.c_str(), _height, character );
  }


  const BannerFont::BigChar& BannerFont::operator[] ( char character ) const
  {
    CharMap::const_iterator  it = _table.find ( character );

    if ( it == _table.end() )
      throw Error ( badChar, _name.c_str(), character );

    return *(it->second);
  }


// -------------------------------------------------------------------
// Class  :  "FontMap".


// -------------------------------------------------------------------
// Class  :  "Banner".


  Banner::Banner ( string name
                 , string version
                 , string purpose
                 , string date
                 , string authors
                 , string contributors )
    : _name          (name)
    , _version       (version)
    , _purpose       (purpose)
    , _date          (date)
    , _authors       (authors)
    , _contributors  (contributors)
    , _screenWidth   (80)
    , _cartoucheWidth(64)
    , _font          (BannerFont::getBannerFont("Unknown_Bold_Normal_14"))
    , _lines         (NULL)
    , _banner        ()
  {
    _redraw ();
  }


  Banner::~Banner()
  {
    if ( _lines ) delete [] _lines;
  }


  void  Banner::_redraw ()
  {
    if ( _lines ) delete [] _lines;

    _lines = new string [ _font->getHeight() ];
    _banner.str ("");
    _banner << "\n";

  // Concat all the caracters.
    for ( unsigned i=0 ; i<_name.size() ; i++ ) {
      for ( int line=0 ; line<_font->getHeight() ; line++ ) {
        _lines[line] += ( *_font )[ _name[i] ][ line ];
        _lines[line] += ' ';
      }
    }

  // Check the banner width.
    if ( _lines[0].size() > _screenWidth )
      throw Error ( badBannerWidth, _name.c_str(), _screenWidth );

    int     half = ( _screenWidth - _lines[0].size() ) / 2;
    string  head ( half, ' ' );

  // Concat & center all the lines.
    for ( int line=0 ; line<_font->getHeight() ; line++ ) {
      if ( _lines[line].find_first_not_of(" ") != string::npos )
        _banner << head << _lines [ line ] << "\n";
    }

  // Draw the cartouche.
  // Purpose.
    if ( _purpose.size() ) {
      half = ( _screenWidth - _purpose.size() ) / 2;
      head.clear ();
      head.append ( half, ' ' );

      _banner << "\n" << head << _purpose << "\n";
    }

    half = ( _screenWidth - _cartoucheWidth ) / 2;
    head.clear ();
    head.append ( half, ' ' );

  // Version.
    if ( _version.size() ) {
      string  nameVersion = _name + " " + _version;
      _banner << "\n" << head << "Coriolis CAD System 1.0,"
              << setw(_cartoucheWidth-25) << nameVersion;
    }
    
  // Date.
    if ( _date.size() ) {
      time_t  t;
      time(&t);
      _banner << "\n" << head << "Copyright (c) " << _date
              << "-" << localtime(&t)->tm_year+1900 << ","
              << setw(_cartoucheWidth-25) << "SoC/LIP6/UPMC";
    }

  // Authors.
    if ( _authors.size() ) {
      string  author;
      size_t  end, token, tail, start;

      end  = _authors.size();
      if ( end > 0 ) {
        for ( token=0 ; token<=end ; token=tail ) {
          tail  = _authors.find ( ',' , token+1 );
          start = (token==0) ? 0 : token+1;

          author = _authors.substr ( start , tail-start );
          if ( !token )
            _banner << "\n" << head << "Author(s):" << setw(_cartoucheWidth-11) << author;
          else
            _banner << "\n" << head << setw(_cartoucheWidth-1) << author;
        }
      }
    }

  // Contributors.
    if ( _contributors.size() ) {
      string  contributor;
      size_t  end, token, tail, start;

      end  = _contributors.size();
      if ( end > 0 ) {
        for ( token=0 ; token<=end ; token=tail ) {
          tail  = _contributors.find ( ',' , token+1 );
          start = (token==0) ? 0 : token+1;

          contributor = _contributors.substr ( start , tail-start );
          if ( !token )
            _banner << "\n" << head << "Contributors(s):" << setw(_cartoucheWidth-17) << contributor;
          else
            _banner << "\n" << head << setw(_cartoucheWidth-1) << contributor;
        }
      }
    }

  // E-mail.
    _banner << "\n" << head << "E-mail:" << setw(_cartoucheWidth-8) << "alliance-users@asim.lip6.fr";

  // Separate the banner from the following text.
    _banner << "\n";
  }


  void  Banner::setFont ( BannerFont* font )
  {
    if ( _font == font ) return;

    _font = font;
    _redraw ();
  }


  void  Banner::setName ( string name )
  {
    if ( _name == name ) return;

    _name = name;
    _redraw ();
  }


  void  Banner::setVersion ( string version )
  {
    if ( _version == version ) return;

    _version = version;
    _redraw ();
  }


  void  Banner::setPurpose ( string purpose )
  {
    if ( _purpose == purpose ) return;

    _purpose = purpose;
    _redraw ();
  }


  void  Banner::setDate ( string date )
  {
    if ( _date == date ) return;

    _date = date;
    _redraw ();
  }


  void  Banner::setAuthors ( string authors )
  {
    if ( _authors == authors ) return;

    _authors = authors;
    _redraw ();
  }


  void  Banner::setContributors ( string contributors )
  {
    if ( _contributors == contributors ) return;
    
    _contributors = contributors;
    _redraw ();
  }


  void  Banner::setScreenWidth ( unsigned int screenWidth )
  {
    if ( _screenWidth == screenWidth ) return;

    _screenWidth = screenWidth;
    _redraw ();
  }




// x-----------------------------------------------------------------x
// |                     Functions Definitions                       |
// x-----------------------------------------------------------------x




} // End of CRL namespace.
