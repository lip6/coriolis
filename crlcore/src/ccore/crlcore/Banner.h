

// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Banner.h"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x






# ifndef  __CRL_BANNER_H__
#   define  __CRL_BANNER_H__

# include  <map>
# include  <string>
# include  <vector>
# include  <ostream>
# include  <sstream>

# ifdef  __CCORE_INTERNAL__
#   include  "hurricane/CConfig.h"
# endif
# include  "crlcore/Utilities.h"


namespace CRL {


  using std::map;
  using std::vector;
  using std::ostream;
  using std::ostringstream;


// -------------------------------------------------------------------
// Class  :  "CRL::BannerFont".

  
  class BannerFont {

    public:
    // BigChar subclass.
      class BigChar {
        public:
        // Constructor & Destructor.
                                   BigChar       ( char c, int height, const char* lines[], const BannerFont* font );
                                  ~BigChar       ();
        // Methods.
          inline int               getHeight     () const;
          inline const BannerFont* getFont       () const;
                 string            _getPrint     () const;
                 const char*       operator[]    ( int line ) const;

        private:
        // Internal: Attributes.
                 char              _character;
                 int               _height;
                 const char**      _lines;
                 const BannerFont* _font;

        // Internal: Constructors.
                 BigChar&          operator=   ( const BigChar& );
                                   BigChar     ( const BigChar& );
      };

    // Static Methods.
      static  const BannerFont* getBannerFont ( const string& name );
    // Constructor & Destructor.
      inline                    BannerFont    ( const string& name );
    // Methods.
      inline  void              addChar       ( char  character, int height, const char *lines[] );
      inline  int               getHeight     () const;
      inline  const BigChar&    operator[]    ( char character ) const;

    private:
    // Internal: FontMap subclass.
      class FontMap : public map<const string,const BannerFont*> {
        public:
                            FontMap    ();
                           ~FontMap    ();
          const BannerFont* operator[] ( const string& name ) const;
      };

    // Internal: Types.
      typedef map<char,BigChar*>  CharMap;

    // Internal: Static Attributes.
      static  FontMap           _fontMap;
    // Internal: Attributes.
              CharMap           _table;
              string            _name;
              int               _height;

    // Internal: Constructors.
                                BannerFont    ( const BannerFont& );
                               ~BannerFont    ();
      inline  BannerFont&       operator=     ( const BannerFont& );

  };




// -------------------------------------------------------------------
// Class  :  "CRL::Banner".


  class Banner {
    public:
    // Constructor & Destructor.
                                Banner          ( string name=""
                                                , string version=""
                                                , string purpose=""
                                                , string date=""
                                                , string authors=""
                                                , string contributors="" );
      virtual                  ~Banner          ();
    // Accessors.
      inline  const BannerFont* getFont         () const;
      inline  const string&     getName         () const;
      inline  const string&     getVersion      () const;
      inline  const string&     getPurpose      () const;
      inline  const string&     getDate         () const;
      inline  const string&     getAuthors      () const;
      inline  const string&     getContributors () const;
      inline  unsigned int      getScreenWidth  () const;
    // Modifiers.
              void              setFont         ( BannerFont* font );
              void              setName         ( string name );
              void              setVersion      ( string version );
              void              setPurpose      ( string purpose );
              void              setDate         ( string date );
              void              setAuthors      ( string authors );
              void              setContributors ( string contributors );
              void              setScreenWidth  ( unsigned screenWidth );
    // Hurricane Management.
      virtual const string      _getPrint       () const { return ( _banner.str() ); };

    // Internal: Attributes.
    private:
              string            _name;
              string            _version;
              string            _purpose;
              string            _date;
              string            _authors;
              string            _contributors;
              unsigned int      _screenWidth;
              const BannerFont* _font;
              string*           _lines;
              ostringstream     _banner;

    // Internal: Constructors.
                                Banner          ( const Banner & );
              Banner&           operator=       ( const Banner & );
    // Internal: Methods.
              void              _redraw         ();
  };


// Inline Functions.
  inline int               BannerFont::BigChar::getHeight () const { return _height; }
  inline const BannerFont* BannerFont::BigChar::getFont   () const { return _font; }
  inline                   BannerFont::BannerFont         ( const string& name ): _table(), _name(name) {}
  inline int               BannerFont::getHeight          () const { return _height; }
  inline const BannerFont* Banner::getFont                () const { return _font; }
  inline const string&     Banner::getName                () const { return _name; }
  inline const string&     Banner::getVersion             () const { return _version; }
  inline const string&     Banner::getPurpose             () const { return _purpose; }
  inline const string&     Banner::getDate                () const { return _date; }
  inline const string&     Banner::getAuthors             () const { return _authors; }
  inline const string&     Banner::getContributors        () const { return _contributors; }
  inline unsigned int      Banner::getScreenWidth         () const { return _screenWidth; }


}  // End of CRL namespace.


// Hurricane Management.

inline std::string   getPrint   ( const CRL::BannerFont::BigChar& c ) { return c._getPrint(); }
inline std::string   getPrint   ( const CRL::Banner& b ) { return b._getPrint(); }
inline std::ostream& operator<< ( std::ostream& o, const CRL::BannerFont::BigChar& c ) { return o << getPrint(c); }
inline std::ostream& operator<< ( std::ostream& o, const CRL::Banner& b ) { return o << getPrint(b); }

MSTREAM_R_SUPPORT(CRL::Banner);


# endif
