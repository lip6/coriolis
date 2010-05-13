
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
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ParsersDrivers.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# ifndef __CRL_PARSERS_DRIVERS_H__
#   define __CRL_PARSERS_DRIVERS_H__


# include  <string>
# include  <map>

# include  "hurricane/Name.h"

namespace Hurricane {
  class Library;
  class Cell;
}


namespace CRL {


  using Hurricane::Name;
  using Hurricane::Cell;
  using Hurricane::Library;


  class Environment;
  class Catalog;
  class ParsersMap;


// -------------------------------------------------------------------
// Typedefs.

  typedef  void (  LibraryParser_t  )( const string& , Library*, Catalog& );
  typedef  void (  CellParser_t     )( const string& , Cell* );
  typedef  void (  LibraryDriver_t  )( const string& , Library *, Catalog& );
  typedef  void (  CellDriver_t     )( const string& , Cell*, unsigned int& );


// -------------------------------------------------------------------
// Struct  :  "CRL::ParserSlot".

  struct ParserSlot {
      Name  _tag;
      Name  _ext;
      void* _parser;

            ParserSlot ( const string& t, const string& e, void* p ): _tag(t),_ext(e),_parser(p) {};
  };


  typedef  std::list<ParserSlot>            ParserSlots;
  typedef  std::list<ParserSlot>::iterator  ParserSlotIter;


// -------------------------------------------------------------------
// Class  :  "CRL::ParserFormatSlot".

  class ParserFormatSlot {

    public:
    // Constructors.
      inline                  ParserFormatSlot ();
    // Accessors.
      inline bool             loadByLib        ();
      inline const Name&      getTag           ();
      inline const Name&      getExt           ();
      inline LibraryParser_t* getParsLib       ();
      inline CellParser_t*    getParsCell      ();
    // Modifiers.
             void             registerCell     ( const std::string& tag, CellParser_t*    p, const std::string& ext );
             void             registerLib      ( const std::string& tag, LibraryParser_t* p, const std::string& ext );
             bool             unRegisterCell   ( const Name& ext );
             bool             unRegisterLib    ( const Name& ext );
    // Iterators handling.
      inline void             cbegin           ();
      inline void             lbegin           ();
             bool             cend             ();
             bool             lend             ();
             void             operator++       (int);

    private:
    // Internal - Attributes.
             bool             _loadByLib;
             bool             _itset;
             ParserSlots      _cells;
             ParserSlots      _libs;
             ParserSlotIter   _it;
  };


  inline                  ParserFormatSlot::ParserFormatSlot () : _loadByLib(false), _itset(false) { };
  inline bool             ParserFormatSlot::loadByLib        () { return ( _loadByLib ); }
  inline const Name&      ParserFormatSlot::getTag           () { return ( _it->_tag    ); }
  inline const Name&      ParserFormatSlot::getExt           () { return ( _it->_ext    ); }
  inline LibraryParser_t* ParserFormatSlot::getParsLib       () { return ( (LibraryParser_t*)_it->_parser ); }
  inline CellParser_t*    ParserFormatSlot::getParsCell      () { return ( (CellParser_t*   )_it->_parser ); }
  inline void             ParserFormatSlot::cbegin           () { _it = _cells.begin(); _itset = true; }
  inline void             ParserFormatSlot::lbegin           () { _it = _libs.begin();  _itset = true;  }


// -------------------------------------------------------------------
// Class  :  "CRL::ParsersMap".

  class ParsersMap : public map<Name,ParserFormatSlot> {

    public:
    // Constructor.
                               ParsersMap     ();
    // Methods.
             ParserFormatSlot& getParserSlot  ( const std::string& tag );
             ParserFormatSlot& getParserSlot  ( const std::string& tag, unsigned int mode, const Environment& env );
             void              registerSlot   ( const std::string& tag, LibraryParser_t* p, const std::string& ext );
             void              registerSlot   ( const std::string& tag, CellParser_t*    p, const std::string& ext );
             void              unRegisterSlot ( const Name& tag, const Name& ext, bool lib );
  };


// -------------------------------------------------------------------
// Class  :  "CRL::DriverSlot".

  class DriverSlot {

    public:
    // Constructor.
      inline                  DriverSlot  ( string tag="unknown" );
    // Accessors.
      inline const Name&      getTag      ();
      inline const Name&      getExtLib   ();
      inline const Name&      getExtCell  ();
      inline LibraryDriver_t* getDrivLib  ();
      inline CellDriver_t*    getDrivCell ();

    // Modifiers.
      inline void             setExtLib   ( const std::string &ext );
      inline void             setExtCell  ( const std::string &ext );
      inline void             setDrivLib  ( LibraryDriver_t *driv );
      inline void             setDrivCell ( CellDriver_t *driv );

    private:
    // Internal - Attributes.
             Name             _tag;
             Name             _extLib;
             Name             _extCell;
             LibraryDriver_t* _drivLib;
             CellDriver_t*    _drivCell;
  };


  inline                  DriverSlot::DriverSlot  ( string tag )
                                                  : _tag(tag)
                                                  , _extLib("unknown")
                                                  , _extCell("unknown")
                                                  , _drivLib(NULL)
                                                  , _drivCell(NULL)
                                                  { }
  inline const Name&      DriverSlot::getTag      () { return ( _tag      ); }
  inline const Name&      DriverSlot::getExtLib   () { return ( _extLib   ); }
  inline const Name&      DriverSlot::getExtCell  () { return ( _extCell  ); }
  inline LibraryDriver_t* DriverSlot::getDrivLib  () { return ( _drivLib  ); }
  inline CellDriver_t*    DriverSlot::getDrivCell () { return ( _drivCell ); }
  inline void             DriverSlot::setExtLib   ( const std::string& ext ) { _extLib  = ext; }
  inline void             DriverSlot::setExtCell  ( const std::string& ext ) { _extCell = ext; }
  inline void             DriverSlot::setDrivLib  ( LibraryDriver_t* driv ) { _drivLib  = driv; }
  inline void             DriverSlot::setDrivCell ( CellDriver_t*    driv ) { _drivCell = driv; }




// -------------------------------------------------------------------
// Class  :  "CRL::DriversMap".

  class DriversMap : public map<Name,DriverSlot> {

    public:
    // Constructor.
                         DriversMap      ();
    // Methods.
             DriverSlot& getDriverSlot  ( const std::string& tag );
             DriverSlot& getDriverSlot  ( const std::string& tag, unsigned int mode, const Environment& env );
             void        registerSlot   ( const std::string& tag, CellDriver_t    *d, const std::string& ext );
             void        registerSlot   ( const std::string& tag, LibraryDriver_t *d, const std::string& ext );
             void        unRegisterSlot ( const Name& tag );
  };


// -------------------------------------------------------------------
// Error Messages.

  extern const char* BadParserType;
  extern const char* BadDriverType;
  extern const char* BadInputMode;
  extern const char* BadOutputMode;




}


# endif
