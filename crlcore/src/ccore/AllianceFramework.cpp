// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./AllianceFramework.cpp"                       |
// +-----------------------------------------------------------------+

#include <unistd.h>
#include "vlsisapd/utilities/Path.h"
#include "hurricane/Initializer.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/viewer/Graphics.h"
#include "crlcore/Utilities.h"
#include "crlcore/CellGauge.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/RoutingLayerGauge.h"
#include "crlcore/AllianceFramework.h"

#include "hurricane/SharedName.h"


namespace CRL {

  using namespace std;
  using namespace std::placeholders;
  using Hurricane::DataBase;
  using Hurricane::Initializer;
  using Hurricane::JsonTypes;
  using Hurricane::JsonArray;
  using Hurricane::Warning;
  using Hurricane::tab;
  using Hurricane::Graphics;
  using Hurricane::ForEachIterator;
  using Hurricane::getCollection;
  using Hurricane::Instance;
  using Hurricane::PrivateProperty;


// -------------------------------------------------------------------
// Class  :  "CRL::AllianceFrameworkProperty".


  class AllianceFrameworkProperty : public PrivateProperty {
    public:
      static  AllianceFrameworkProperty* create          ( AllianceFramework* );
      static  Name                       getPropertyName ();
      virtual Name                       getName         () const;
      inline  AllianceFramework*         getFramework    () const;
      virtual bool                       hasJson         () const;
      virtual void                       toJson          ( JsonWriter*, const DBo* ) const;
      virtual string                     _getTypeName    () const;
      virtual Record*                    _getRecord      () const;
    private:
      static  Name       _name;
      AllianceFramework* _framework;
    private:
      inline  AllianceFrameworkProperty ( AllianceFramework* );
    public:
      class JsonProperty : public JsonObject {
        public:
          static  void          initialize   ();
                                JsonProperty ( unsigned long flags );
          virtual string        getTypeName  () const;
          virtual JsonProperty* clone        ( unsigned long ) const;
          virtual void          toData       ( JsonStack& ); 
      };
  };


  Name  AllianceFrameworkProperty::_name = "AllianceFrameworkProperty";


  inline AllianceFrameworkProperty::AllianceFrameworkProperty ( AllianceFramework* af )
    : PrivateProperty(), _framework(af)
  { }


  inline AllianceFramework* AllianceFrameworkProperty::getFramework () const
  { return _framework; }


  AllianceFrameworkProperty* AllianceFrameworkProperty::create ( AllianceFramework* af )
  {
    AllianceFrameworkProperty *property = new AllianceFrameworkProperty ( af );

    property->_postCreate ();
    return property;
  }


  Name  AllianceFrameworkProperty::getPropertyName ()
  { return _name; }


  Name  AllianceFrameworkProperty::getName () const
  { return getPropertyName(); }


  string  AllianceFrameworkProperty::_getTypeName () const
  { return _TName ( "AllianceFrameworkProperty" ); }


  Record* AllianceFrameworkProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot("_name"     ,_name     ) );
      record->add( getSlot("_framework",_framework) );
    }
    return record;
  }


  bool  AllianceFrameworkProperty::hasJson () const
  { return true; }


  void  AllianceFrameworkProperty::toJson ( JsonWriter* w, const DBo* ) const
  {
    w->startObject();
    std::string tname = getString(getPropertyName());
    jsonWrite( w, "@typename" , tname      );
    jsonWrite( w, "_framework", _framework );
    w->endObject();
  }


  Initializer<AllianceFrameworkProperty::JsonProperty>  jsonFrameworkPropertyInit ( 20 );


  AllianceFrameworkProperty::JsonProperty::JsonProperty ( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_framework", typeid(AllianceFramework*) );
  }


  string  AllianceFrameworkProperty::JsonProperty::getTypeName () const
  { return getString(AllianceFrameworkProperty::getPropertyName()); }


  void  AllianceFrameworkProperty::JsonProperty::initialize ()
  { JsonTypes::registerType( new JsonProperty (JsonWriter::RegisterMode) ); }


  AllianceFrameworkProperty::JsonProperty* AllianceFrameworkProperty::JsonProperty::clone ( unsigned long flags ) const
  { return new JsonProperty ( flags ); }


  void AllianceFrameworkProperty::JsonProperty::toData ( JsonStack& stack )
  {
    check( stack, "AllianceFrameworkProperty::JsonProperty::toData" );

    DBo*                       dbo       = stack.back_dbo();
    AllianceFramework*         framework = get<AllianceFramework*>(stack,"_framework");
    AllianceFrameworkProperty* property
                 = AllianceFrameworkProperty::create(framework);
    if (dbo) dbo->put( property );
    
    update( stack, property );
  }


// -------------------------------------------------------------------
// Class  :  "CRL::AllianceFramework".


  AllianceFramework* AllianceFramework::_singleton         = NULL;
  const Name         AllianceFramework::_parentLibraryName = "Alliance";



  AllianceFramework::AllianceFramework ()
    : _observers          ()
    , _environment        ()
    , _parsers            ()
    , _drivers            ()
    , _catalog            ()
    , _parentLibrary      (NULL)
    , _routingGauges      ()
    , _defaultRoutingGauge(NULL)
    , _cellGauges         ()
    , _defaultCellGauge   (NULL)
  {
    DataBase* db = DataBase::getDB ();
    if ( not db )
      db = DataBase::create ();

    db->put ( AllianceFrameworkProperty::create(this) );
    db->_setCellLoader( bind(&AllianceFramework::cellLoader,this,_1) );

  //cmess1 << "  o  Reading Alliance Environment." << endl;

  //_environment.loadFromShell ();
  }


  AllianceFramework::~AllianceFramework ()
  {
    for ( size_t i=0 ; i<_libraries.size() ; i++ )
      delete _libraries[i];

    while ( !_routingGauges.empty() ) {
      _routingGauges.begin()->second->destroy();
      _routingGauges.erase ( _routingGauges.begin() );
    }

    while ( !_cellGauges.empty() ) {
      _cellGauges.begin()->second->destroy();
      _cellGauges.erase ( _cellGauges.begin() );
    }
  }


  void  AllianceFramework::_bindLibraries ()
  {
    DataBase*     db          = DataBase::getDB ();
    unsigned int  flags       = AppendLibrary;
    SearchPath&   LIBRARIES   = _environment.getLIBRARIES ();
    Library*      rootLibrary = db->getRootLibrary ();

  //cmess2 << "  o  Creating Alliance Framework root library." << endl;
    if ( !rootLibrary )
      rootLibrary = Library::create ( db, "Root" );

    _parentLibrary = rootLibrary->getLibrary ( _parentLibraryName );
    if ( !_parentLibrary )
      _parentLibrary = Library::create ( rootLibrary, _parentLibraryName );

  //cmess2 << "  o  Loading libraries (working first)." << endl;
    for ( unsigned i=0 ; i<LIBRARIES.getSize() ; i++ ) {
      createLibrary ( LIBRARIES[i].getPath(), flags, LIBRARIES[i].getName() );

    //cmess2 << "     - " << LIBRARIES[i].getName()
    //       << " \"" << LIBRARIES[i].getPath() << "\"" << endl;
    //cmess2.flush();

    //if ( flags&HasCatalog ) cmess2 << " [have CATAL]." << endl;
    //else                    cmess2 << " [no CATAL]"    << endl;
    }
  }


  AllianceFramework* AllianceFramework::create ( unsigned long flags )
  {
    if (not _singleton) {
    // Triggers System singleton creation.
      System::get ();
      _singleton = new AllianceFramework ();
      if (not (flags & NoPythonInit))
        System::runPythonInit();
      _singleton->_bindLibraries();
    }

    return _singleton;
  }


  AllianceFramework* AllianceFramework::get ()
  {
    return create();
  }


  void AllianceFramework::destroy ()
  {
    delete this;
  }


  string  AllianceFramework::getPrint () const
  {
    return _environment.getPrint ();
  }


  void AllianceFramework::addObserver ( BaseObserver* observer )
  { _observers.addObserver( observer ); }


  void AllianceFramework::removeObserver ( BaseObserver* observer )
  { _observers.removeObserver( observer ); }


  void AllianceFramework::notify ( unsigned flags )
  { _observers.notify( flags ); }


  Catalog::State* AllianceFramework::isInCatalog ( const Name& name )
  { return getCatalog()->getState( name, false ); }


  Catalog::State* AllianceFramework::isInCatalog ( string name )
  { return getCatalog()->getState( Name(name), false ); }


  AllianceLibrary* AllianceFramework::getAllianceLibrary ( unsigned index )
  {
    if ( index >= _libraries.size() )
      return NULL;

    return _libraries[index];
  }


  AllianceLibrary* AllianceFramework::getAllianceLibrary ( const Name &libName, unsigned int flags )
  {
    for ( size_t ilib=0 ; ilib<_libraries.size() ; ++ilib ) {
      if ( _libraries[ilib]->getLibrary()->getName() == libName )
        return _libraries[ilib];
    }

    return (flags&CreateLibrary) ? createLibrary ( getString(libName), flags ) : NULL;
  }


  AllianceLibrary* AllianceFramework::getAllianceLibrary ( Library* library )
  {
    for ( size_t ilib=0 ; ilib<_libraries.size() ; ++ilib ) {
      if ( _libraries[ilib]->getLibrary() == library ) return _libraries[ilib];
    }
    return NULL;
  }


  Cell* AllianceFramework::cellLoader ( const string& rpath )
  {
    size_t   dot      = rpath.rfind('.');
    string   cellName = rpath.substr(dot+1);

    return getCell( cellName, Catalog::State::Views );
  }


  Cell* AllianceFramework::getCell ( const string& name, unsigned int mode, unsigned int depth )
  {
    bool              createCell = false;
    Catalog::State*   state      = _catalog.getState ( name );
    ParserFormatSlot* parser;

  // The cell is not even in the Catalog : add an entry.
    if ( state == NULL ) state = _catalog.getState ( name, true );

    if ( state->isFlattenLeaf() ) depth = 0;
    state->setDepth ( depth );

  // Do not try to load.
    if ( mode & Catalog::State::InMemory ) return state->getCell();

    unsigned int loadMode;
    for ( int i=0 ; i<2 ; i++ ) {
    // Check is the view is requested for loading or already loaded.
      switch ( i ) {
        case 0: loadMode = mode & Catalog::State::Logical;  break;
        case 1: loadMode = mode & Catalog::State::Physical; break;
      }
      if ( loadMode == 0 ) continue;
      if ( state->getFlags(loadMode) != 0 ) continue;

    // Transmit all flags except thoses related to views.
      loadMode |= (mode & (!Catalog::State::Views));
      parser    = & ( _parsers.getParserSlot ( name, loadMode, _environment ) );

    // Try to open cell file (file extention is supplied by the parser).
      if ( !_readLocate(name,loadMode) ) continue;

      if ( state->getCell() == NULL ) {
        state->setCell ( Cell::create ( _libraries[ _environment.getLIBRARIES().getIndex() ]->getLibrary() , name ) );
        state->getCell ()->put ( CatalogProperty::create(state) );
        state->getCell ()->setFlattenLeaf ( false );
        createCell = true;
      }

      try {
      // Call the parser function.
        (parser->getParsCell())( _environment.getLIBRARIES().getSelected() , state->getCell() );
      } catch ( ... ) {
        if ( createCell ) 
        //state->getCell()->destroy();
        throw;
      }
    }

  // At least one view must have been loaded.
    if ( state->getFlags(Catalog::State::Views) != 0 ) {
      state->setFlags( Catalog::State::InMemory, true );
      return state->getCell();
    }

  // Delete the empty cell.
    if ( state->getCell() ) state->getCell()->destroy ();
    _catalog.deleteState ( name );

  // Last resort, search through all Hurricane libraries.
    if (mode & Catalog::State::Foreign)
      return DataBase::getDB()->getCell( name );

    return NULL;
  }


  AllianceLibrary* AllianceFramework::createLibrary ( const string& path, unsigned int flags, string libName )
  {
    if ( libName.empty() ) libName = SearchPath::extractLibName(path);

    flags &= ~HasCatalog;

    string dupLibName = libName;
    for ( size_t duplicate=1 ; true ; ++duplicate ) {
      AllianceLibrary* library = getAllianceLibrary ( dupLibName, flags & ~CreateLibrary );
      if (library == NULL) break;

      ostringstream oss;
      oss << libName << "." << duplicate;
      dupLibName = oss.str();
    }

    // AllianceLibrary* library = getAllianceLibrary ( libName, flags );
    // if ( library != NULL ) {
    //   cerr << Warning("AllianceFramework::createLibrary(): Attempt to re-create <%s>, using already existing."
    //                  ,libName.c_str()) << endl;
    //   return library;
    // }

    SearchPath& LIBRARIES = _environment.getLIBRARIES ();
    if ( not (flags & AppendLibrary) ) LIBRARIES.prepend ( path, dupLibName );
    else                               LIBRARIES.select  ( path );

    Library* hlibrary = getParentLibrary()->getLibrary( dupLibName );
    if (not hlibrary)
      hlibrary = Library::create( getParentLibrary(), dupLibName );
    
    AllianceLibrary* alibrary = new AllianceLibrary ( path, hlibrary );

    AllianceLibraries::iterator ilib = _libraries.begin();
    if (LIBRARIES.getIndex() != SearchPath::npos)
      for ( size_t i=0 ; i<LIBRARIES.getIndex() ; ++i, ++ilib );
    else
      ilib = _libraries.end();

    _libraries.insert ( ilib, alibrary );

    string catalog = path + "/" + _environment.getCATALOG();

    if ( _catalog.loadFromFile(catalog,alibrary->getLibrary()) ) flags |= HasCatalog;

    ParserFormatSlot& parser = _parsers.getParserSlot ( path, Catalog::State::Physical, _environment );

    if ( not parser.loadByLib() ) {
      notify ( AddedLibrary );
      return alibrary;
    }

  // Load the whole library.
    if ( ! _readLocate(dupLibName,Catalog::State::State::Logical,true) ) return alibrary;

  // Call the parser function.
    (parser.getParsLib())( _environment.getLIBRARIES().getSelected() , alibrary->getLibrary() , _catalog );

    notify ( AddedLibrary );
    return alibrary;
  }
  

  void  AllianceFramework::saveLibrary ( Library* library )
  {
    if ( library == NULL ) return;

    AllianceLibrary* alibrary = getAllianceLibrary ( library );
    if ( alibrary == NULL ) {
      throw Error("AllianceFramework::saveLibrary(): Cannot save non-Alliance library <%s>."
                 ,getString(library->getName()).c_str());
    }

    saveLibrary ( alibrary );
  }
  

  void  AllianceFramework::saveLibrary ( AllianceLibrary* library )
  {
    if ( library == NULL ) return;

    Utilities::Path libPath ( getString(library->getPath()) );
    if ( not libPath.exists() ) {
      libPath.mkdir();
    }

    forEach ( Cell*, icell, library->getLibrary()->getCells() ) {
      saveCell ( *icell, Catalog::State::Views );
    }
  }


  Cell* AllianceFramework::createCell ( const string& name, AllianceLibrary* library )
  {
    Catalog::State* state = _catalog.getState ( name );

  // The cell is not in the CATAL : add an entry.
    if ( state == NULL ) state = _catalog.getState ( name, true );

    if ( library == NULL )
      library = _libraries[0];

    if ( !state->getCell() ) {
      state->setPhysical ( true );
      state->setLogical  ( true );
      state->setDepth    ( 1 );

      state->setCell ( Cell::create ( library->getLibrary() , name ) );
      state->getCell ()->put ( CatalogProperty::create(state) );
      state->getCell ()->setFlattenLeaf ( false );
    }

    return state->getCell ();
  }


  void  AllianceFramework::saveCell ( Cell* cell, unsigned int mode )
  {
    if ( cell == NULL ) return;

    string           name       = getString(cell->getName());
    DriverSlot*      driver;
    unsigned int     saveMode   = 0;
    unsigned int     savedViews = mode & (~Catalog::State::Views);
    AllianceLibrary* library    = getAllianceLibrary ( cell->getLibrary() );

    for ( int i=0 ; i<2 ; i++ ) {
    // Check is the view is requested for saving or already saved.
      switch ( i ) {
        case 0: saveMode = mode & Catalog::State::Logical;  break;
        case 1: saveMode = mode & Catalog::State::Physical; break;
      }
      if ( saveMode == 0 ) continue;
      if ( ( savedViews & saveMode ) != 0 ) continue;

    // Transmit all flags except thoses related to views.
      saveMode |= (mode & (~Catalog::State::Views));

      driver = & ( _drivers.getDriverSlot ( name, saveMode, _environment ) );

    // Try to open cell file (file extention is supplied by the parser).
      if ( library != NULL )
        _environment.getLIBRARIES().select ( getString(library->getPath()) );
      if ( !_writeLocate(name,saveMode,false) ) continue;

    // Call the driver function.
      (driver->getDrivCell())( _environment.getLIBRARIES().getSelected(), cell, savedViews );
    }
  }


  Library* AllianceFramework::getLibrary ( unsigned index )
  {
    if ( index >= _libraries.size() )
      return NULL;

    return _libraries[ index ]->getLibrary();
  }


  Library* AllianceFramework::getLibrary ( const Name &libName )
  {
    for ( size_t ilib=0 ; ilib<_libraries.size() ; ++ilib ) {
      if ( _libraries[ilib]->getLibrary()->getName() == libName )
        return _libraries[ilib]->getLibrary();
    }

    return NULL;
  }


  unsigned int  AllianceFramework::loadLibraryCells ( Library *library )
  {
    cmess2 << "      " << tab++ << "+ Library: " << getString(library->getName()) << endl;

    map<Name,Catalog::State*>*           states = _catalog.getStates ();
    map<Name,Catalog::State*>::iterator  istate = states->begin ();

    unsigned int count = 0;
    for ( ; istate != states->end() ; istate++ ) {
      if ( istate->second->getLibrary() == library ) {
        getCell ( getString(istate->first), Catalog::State::Views );
        count++; 
      }
    }
    tab--;
    
    return count;
  }
    

  unsigned int  AllianceFramework::loadLibraryCells ( const Name& name )
  {
    for ( size_t i=0 ; i<_libraries.size() ; i++ ) {
      string  spath       = getString ( _libraries[i]->getPath() );
      size_t  slash       = spath.rfind ('/');
      Name    libraryName = spath.substr ( (slash!=string::npos)?slash+1:0 );
      if ( libraryName == name )
        return loadLibraryCells ( _libraries[i]->getLibrary() );
    }

    return 0;
  }


  bool  AllianceFramework::_readLocate ( const string& file, unsigned int mode, bool isLib )
  {
    string  name;

    SearchPath&       LIBRARIES = _environment.getLIBRARIES ();
    ParserFormatSlot& format    = _parsers.getParserSlot ( file, mode, _environment );

    if ( isLib ) {
    // Try to open using the library parsers.
      for ( format.lbegin() ; !format.lend() ; format++ ) {
        name = file + "." + getString(format.getExt());

        LIBRARIES.locate ( name );
        if ( LIBRARIES.hasSelected() ) return true;
      }
    } else {
    // Try to open using the cell parsers.
      for ( format.cbegin() ; !format.cend() ; format++ ) {
        name = file + "." + getString(format.getExt());

        LIBRARIES.locate ( name );
        if ( LIBRARIES.hasSelected() ) return true;
      }
    }

    return false;
  }


  bool  AllianceFramework::_writeLocate ( const string& file, unsigned int mode, bool isLib )
  {
    SearchPath& LIBRARIES = _environment.getLIBRARIES ();
    DriverSlot& format    = _drivers.getDriverSlot ( file, mode, _environment );
    string      name;

    if ( isLib ) {
      if ( !format.getDrivLib() ) return false;

    // Try to open using the library driver.
      name = file + "." + getString(format.getExtLib());

      LIBRARIES.locate ( name, ios::out|ios::trunc );
      if ( LIBRARIES.hasSelected() ) return true;
    } else {
      if ( !format.getDrivCell() ) return false;

    // Try to open using the cell driver.
      name = file + "." + getString(format.getExtCell());

      LIBRARIES.locate ( name, ios::out|ios::trunc );
      if ( LIBRARIES.hasSelected() ) return true;
    }
    return false;
  }


  RoutingGauge* AllianceFramework::setRoutingGauge ( const Name& name )
  {
    RoutingGauge* gauge = getRoutingGauge( name );
    if (gauge) _defaultRoutingGauge = gauge;
    else
      cerr << Error( "AllianceFramework::setRoutingGauge(): No gauge named \"%s\"."
                   , getString(name).c_str() ) << endl;

    return _defaultRoutingGauge;
  }


  RoutingGauge* AllianceFramework::getRoutingGauge ( const Name& name )
  {
    if ( name.isEmpty() ) return _defaultRoutingGauge;

    map<Name,RoutingGauge*>::iterator igauge = _routingGauges.find ( name );
    if ( igauge != _routingGauges.end() )
      return igauge->second;

    return NULL;
  }


  void  AllianceFramework::addRoutingGauge ( RoutingGauge* gauge )
  {
    if ( !gauge ) {
      cerr << Warning("AllianceFramework::addRoutingGauge(): NULL gauge argument.") << endl;
      return;
    }

    if ( getRoutingGauge(gauge->getName()) ) {
      cerr << Warning("AllianceFramework::addRoutingGauge(): gauge %s already exists."
                     ,getString(gauge->getName()).c_str())
           << endl;
      return;
    }

    _routingGauges [ gauge->getName() ] = gauge;
    _defaultRoutingGauge                = gauge;
  }


  CellGauge* AllianceFramework::setCellGauge ( const Name& name )
  {
    CellGauge* gauge = getCellGauge( name );
    if (gauge) _defaultCellGauge = gauge;
    else
      cerr << Error( "AllianceFramework::setCellGauge(): No gauge named \"%s\"."
                   , getString(name).c_str() ) << endl;

    return _defaultCellGauge;
  }


  CellGauge* AllianceFramework::getCellGauge ( const Name& name )
  {
    if ( name.isEmpty() ) return _defaultCellGauge;

    map<Name,CellGauge*>::iterator igauge = _cellGauges.find ( name );
    if ( igauge != _cellGauges.end() )
      return igauge->second;

    return NULL;
  }


  void  AllianceFramework::addCellGauge ( CellGauge* gauge )
  {
    if ( !gauge ) {
      cerr << Warning("AllianceFramework::addCellGauge(): NULL gauge argument.") << endl;
      return;
    }

    if ( getCellGauge(gauge->getName()) ) {
      cerr << Warning("AllianceFramework::addCellGauge(): gauge %s already exists."
                     ,getString(gauge->getName()).c_str())
           << endl;
      return;
    }

    _cellGauges [ gauge->getName() ] = gauge;
    if (not _defaultCellGauge) _defaultCellGauge = gauge;
  }


  CellGauge* AllianceFramework::matchCellGauge ( DbU::Unit width, DbU::Unit height ) const
  {
    for ( const auto item : _cellGauges ) {
      CellGauge* cg       = item.second;
      DbU::Unit  hcount   = width  / cg->getSliceStep  ();
      DbU::Unit  hremains = width  % cg->getSliceStep  ();
      DbU::Unit  vcount   = height / cg->getSliceHeight();
      DbU::Unit  vremains = height % cg->getSliceHeight();

      if ( (hcount) and (not hremains) and (vcount == 1) and (not vremains) )
        return cg;
    }

    return NULL;
  }


  CellGauge* AllianceFramework::matchCellGaugeByHeight ( DbU::Unit height ) const
  {
    for ( const auto item : _cellGauges ) {
      CellGauge* cg       = item.second;
      DbU::Unit  vcount   = height / cg->getSliceHeight();
      DbU::Unit  vremains = height % cg->getSliceHeight();

      if ( (vcount == 1) and (not vremains) ) return cg;
    }

    return NULL;
  }


  size_t  AllianceFramework::getInstancesCount ( Cell* cell, unsigned int flags )
  {
    size_t gates = 0;

    forEach ( Instance*, iinstance,  cell->getInstances() ) {
      CatalogProperty *catalogProperty = static_cast<CatalogProperty*>
        ((*iinstance)->getMasterCell()->getProperty ( CatalogProperty::getPropertyName()) );

      if ( catalogProperty != NULL ) {
        Catalog::State* state = catalogProperty->getState ();
        if ( (flags and IgnoreFeeds) and state->isFeed() ) continue;
      }
      ++gates;

      if ( flags & Recursive ) {
        gates += getInstancesCount ( iinstance->getMasterCell(), flags );
      }
    }

    return gates;
  }


  string  AllianceFramework::_getString () const
  { return "<AllianceFramework>"; }


  Record *AllianceFramework::_getRecord () const
  {
    Record* record = new Record ( "<AllianceFramework>" );
    record->add ( getSlot ( "_environment"        , &_environment         ) );
    record->add ( getSlot ( "_libraries"          , &_libraries           ) );
    record->add ( getSlot ( "_catalog"            , &_catalog             ) );
    record->add ( getSlot ( "_defaultRoutingGauge",  _defaultRoutingGauge ) );
    record->add ( getSlot ( "_routingGauges"      , &_routingGauges       ) );
    record->add ( getSlot ( "_defaultCellGauge"   ,  _defaultCellGauge    ) );
    record->add ( getSlot ( "_cellGauges"         , &_cellGauges          ) );
    return record;
  }


  void  AllianceFramework::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename"           ,  _getTypeName() );
    jsonWrite( w, "_environment"        , &_environment   );
    jsonWrite( w, "_defaultRoutingGauge",  _defaultRoutingGauge->getName() );
    jsonWrite( w, "_defaultCellGauge"   ,  _defaultCellGauge->getName() );
    jsonWrite( w, "+libraries"          ,  getAllianceLibraries() );
    jsonWrite( w, "+routingGauges"      ,  _routingGauges );
    jsonWrite( w, "+cellGauges"         ,  _cellGauges );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "CRL::JsonAllianceFramework".

  Initializer<JsonAllianceFramework>  jsonFrameworkInit ( 20 );


  JsonAllianceFramework::JsonAllianceFramework ( unsigned long flags )
    : JsonObject          (flags)
    , _defaultRoutingGauge()
    , _defaultCellGauge   ()
  {
    add( "_environment"        , typeid(Environment*) );
    add( "_defaultRoutingGauge", typeid(string)       );
    add( "_defaultCellGauge"   , typeid(string)       );
    add( "+libraries"          , typeid(JsonArray)    );
    add( "+routingGauges"      , typeid(JsonArray)    );
    add( "+cellGauges"         , typeid(JsonArray)    );
  }


  JsonAllianceFramework::~JsonAllianceFramework ()
  {
    AllianceFramework* framework = AllianceFramework::get(); 
    framework->setRoutingGauge( _defaultRoutingGauge );
    framework->setCellGauge   ( _defaultCellGauge    );
  }


  string JsonAllianceFramework::getTypeName () const
  { return "AllianceFramework"; }


  void  JsonAllianceFramework::initialize ()
  { JsonTypes::registerType( new JsonAllianceFramework (JsonWriter::RegisterMode) ); }


  JsonAllianceFramework* JsonAllianceFramework::clone ( unsigned long flags ) const
  { return new JsonAllianceFramework ( flags ); }


  void JsonAllianceFramework::toData ( JsonStack& stack )
  {
    check( stack, "JsonAllianceFramework::toData" );

  // It's a singleton. Do not create it...
    AllianceFramework* framework = AllianceFramework::get(); 

    string _defaultRoutingGauge = get<string>( stack, "_defaultRoutingGauge" );
    string _defaultCellGauge    = get<string>( stack, "_defaultCellGauge"    );

    update( stack, framework );
  }


} // End of CRL namespace.
