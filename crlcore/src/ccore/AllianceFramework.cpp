
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
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AllianceFramework.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Warning.h"
#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Library.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Instance.h"
#include  "hurricane/viewer/Graphics.h"

#include  "crlcore/Utilities.h"
#include  "crlcore/GraphicsParser.h"
#include  "crlcore/SymbolicTechnologyParser.h"
#include  "crlcore/RealTechnologyParser.h"
#include  "crlcore/CellGauge.h"
#include  "crlcore/RoutingGauge.h"
#include  "crlcore/RoutingLayerGauge.h"
#include  "crlcore/AllianceFramework.h"


namespace CRL {


  using Hurricane::Warning;
  using Hurricane::tab;
  using Hurricane::Graphics;
  using Hurricane::ForEachIterator;
  using Hurricane::Instance;


  AllianceFramework* AllianceFramework::_singleton         = NULL;
  const Name         AllianceFramework::_parentLibraryName = "AllianceFramework";


  AllianceFramework::AllianceFramework ()
    : _environment()
    , _parsers()
    , _drivers()
    , _catalog()
    , _parentLibrary(NULL)
    , _routingGauges()
  {
  //cerr << "AllianceFramework::AllianceFramework()" << endl;

  // Triggers System singleton loading.
    System::get ();

    DataBase* db = DataBase::getDB ();
    if ( !db )
      db = DataBase::create ();

    _environment.loadFromShell ();
    _environment.loadFromXml   ();

    string  userEnvironment = Environment::getEnv ( "HOME", "<HomeDirectory>" );
    _environment.loadFromXml ( userEnvironment+"/.environment.alliance.xml", false );

    SymbolicTechnologyParser::load ( db, _environment.getSYMBOLIC_TECHNOLOGY() );
    RealTechnologyParser::load     ( db, _environment.getREAL_TECHNOLOGY() );
    GraphicsParser::load           ( _environment.getDISPLAY() );

    if ( !_environment.getDisplayStyle().empty() )
      Graphics::setStyle ( _environment.getDisplayStyle() );

    bool        hasCatalog;
    SearchPath& LIBRARIES   = _environment.getLIBRARIES ();
    Library*    rootLibrary = db->getRootLibrary ();

    cmess2 << "  o  Creating Alliance Framework root library." << endl;
    if ( !rootLibrary )
      rootLibrary = Library::create ( db, "RootLibrary" );

    _parentLibrary = rootLibrary->getLibrary ( _parentLibraryName );
    if ( !_parentLibrary )
      _parentLibrary = Library::create ( rootLibrary, _parentLibraryName );

    cmess2 << "  o  Loading libraries (working first)." << endl;
    for ( unsigned i=0 ; i<LIBRARIES.getSize() ; i++ ) {
      Name libraryName = LIBRARIES[i];

      _libraries.push_back ( getAllianceLibrary(libraryName,hasCatalog) );

      cmess2 << "     - \"" << libraryName._getString() << "\"";
      cmess2.flush();

      if ( hasCatalog ) cmess2 << " [have CATAL]." << endl;
      else              cmess2 << " [no CATAL]"    << endl;
    }

  // Temporary: create the SxLib routing gauge.
    Technology* technology = db->getTechnology();

    RoutingGauge* sxlibRg      = RoutingGauge::create ( "sxlib" );
    const Layer*  routingLayer = NULL;

    switch ( 1 ) {
      default:
        routingLayer = technology->getLayer("METAL1");
        if ( routingLayer == NULL ) break;

        sxlibRg->addLayerGauge ( RoutingLayerGauge::create( routingLayer
                                                          , Constant::Vertical
                                                          , Constant::PinOnly
                                                          , 0              // Depth (?).
                                                          , 0              // Density.
                                                          , DbU::lambda(0) // Offset.
                                                          , DbU::lambda(5) // Pitch.
                                                          , DbU::lambda(2) // Wire width.
                                                          , DbU::lambda(3) // Via width.
                                                          ) );

        routingLayer = technology->getLayer("METAL2");
        if ( routingLayer == NULL ) break;

        sxlibRg->addLayerGauge ( RoutingLayerGauge::create( routingLayer
                                                          , Constant::Horizontal
                                                          , Constant::Default
                                                          , 1              // Depth (?).
                                                          , 7.7            // Density.
                                                          , DbU::lambda(0) // Offset.
                                                          , DbU::lambda(5) // Pitch.
                                                          , DbU::lambda(2) // Wire width.
                                                          , DbU::lambda(3) // Via width.
                                                          ) );

        routingLayer = technology->getLayer("METAL3");
        if ( routingLayer == NULL ) break;

        sxlibRg->addLayerGauge ( RoutingLayerGauge::create( routingLayer
                                                          , Constant::Vertical
                                                          , Constant::Default
                                                          , 2              // Depth (?).
                                                          , 0              // Density.
                                                          , DbU::lambda(0) // Offset.
                                                          , DbU::lambda(5) // Pitch.
                                                          , DbU::lambda(2) // Wire width.
                                                          , DbU::lambda(3) // Via width.
                                                          ) );

        routingLayer = technology->getLayer("METAL4");
        if ( routingLayer == NULL ) break;

        sxlibRg->addLayerGauge ( RoutingLayerGauge::create( routingLayer
                                                          , Constant::Horizontal
                                                          , Constant::Default
                                                          , 3              // Depth (?).
                                                          , 0              // Density.
                                                          , DbU::lambda(0) // Offset.
                                                          , DbU::lambda(5) // Pitch.
                                                          , DbU::lambda(2) // Wire width.
                                                          , DbU::lambda(3) // Via width.
                                                          ) );

        routingLayer = technology->getLayer("METAL5");
        if ( routingLayer == NULL ) break;

        sxlibRg->addLayerGauge ( RoutingLayerGauge::create( routingLayer
                                                          , Constant::Vertical
                                                          , Constant::Default
                                                          , 4              // Depth (?).
                                                          , 0              // Density.
                                                          , DbU::lambda(0) // Offset.
                                                          , DbU::lambda(5) // Pitch.
                                                          , DbU::lambda(2) // Wire width.
                                                          , DbU::lambda(3) // Via width.
                                                          ) );
    }
    addRoutingGauge ( sxlibRg );

    CellGauge* sxlibCg = CellGauge::create ( "sxlib"
                                           , "metal2"          // pinLayerName.
                                           , DbU::lambda( 5.0) // pitch.
                                           , DbU::lambda(50.0) // sliceHeight.
                                           , DbU::lambda( 5.0) // sliceStep.
                                           );
    addCellGauge ( sxlibCg );
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


  AllianceFramework* AllianceFramework::create ()
  {
    if ( !_singleton )
      _singleton = new AllianceFramework ();

    return _singleton;
  }


  AllianceFramework* AllianceFramework::get ()
  {
    return create ();
  }


  void AllianceFramework::destroy ()
  {
    delete this;
  }


  string  AllianceFramework::getPrint () const
  {
    return _environment.getPrint ();
  }


  AllianceLibrary* AllianceFramework::getAllianceLibrary ( const Name &path , bool &flag )
  {
    string  spath   = getString ( path );
    size_t  slash   = spath.rfind ( '/' );
    string  sname   = spath.substr ( (slash!=string::npos)?slash+1:0 );

    AllianceLibrary* library = new AllianceLibrary ( path, Library::create(getParentLibrary(),sname) );
    string           catalog = spath + "/" + _environment.getCATALOG();

    flag = false;
    if ( _catalog.loadFromFile(catalog,library->getLibrary()) ) flag = true;

    ParserFormatSlot& parser = _parsers.getParserSlot ( spath, Catalog::State::Physical, _environment );

    if ( !parser.loadByLib() ) return library;

    size_t  base = spath.find_last_of ( '/' );
    if ( base == spath.npos ) return library;

    string  file = spath.substr(base+1,spath.size()-base);

  // Load the whole library.
    if ( ! _readLocate(file,Catalog::State::State::Logical,true) ) return library;

  // Call the parser function.
    (parser.getParsLib())( _environment.getLIBRARIES().getSelected() , library->getLibrary() , _catalog );

    return library;
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
          state->getCell()->destroy();
        throw;
      }
    }

  // At least one view must have been loaded.
    if ( state->getFlags(Catalog::State::Views) != 0 ) return state->getCell();

  // Delete the empty cell.
    if ( state->getCell() ) state->getCell()->destroy ();
    _catalog.deleteState ( name );

    return NULL;
  }


  Cell* AllianceFramework::createCell ( const string& name )
  {
    Catalog::State* state = _catalog.getState ( name );

  // The cell is not in the CATAL : add an entry.
    if ( state == NULL ) state = _catalog.getState ( name, true );

    if ( !state->getCell() ) {
      state->setPhysical ( true );
      state->setLogical  ( true );
      state->setDepth    ( 1 );

      state->setCell ( Cell::create ( _libraries[0]->getLibrary() , name ) );
      state->getCell ()->put ( CatalogProperty::create(state) );
      state->getCell ()->setFlattenLeaf ( false );
    }

    return state->getCell ();
  }


  void  AllianceFramework::saveCell ( Cell* cell , unsigned int mode )
  {
    if ( !cell ) return;

    string        name   = getString(cell->getName());
    DriverSlot*   driver;
    unsigned int  saveMode;
    unsigned int  savedViews = 0;

    for ( int i=0 ; i<2 ; i++ ) {
    // Check is the view is requested for saving or already saved.
      switch ( i ) {
        case 0: saveMode = mode & Catalog::State::Logical;  break;
        case 1: saveMode = mode & Catalog::State::Physical; break;
      }
      if ( saveMode == 0 ) continue;
      if ( ( savedViews & saveMode ) != 0 ) continue;

    // Transmit all flags except thoses related to views.
      saveMode |= (mode & (!Catalog::State::Views));

      driver = & ( _drivers.getDriverSlot ( name, saveMode, _environment ) );

    // Try to open cell file (file extention is supplied by the parser).
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

      LIBRARIES.locate ( name, ios::out|ios::trunc, 0, 1 );
      if ( LIBRARIES.hasSelected() ) return true;
    } else {
      if ( !format.getDrivCell() ) return false;

    // Try to open using the cell driver.
      name = file + "." + getString(format.getExtCell());

      LIBRARIES.locate ( name, ios::out|ios::trunc, 0, 1 );
      if ( LIBRARIES.hasSelected() ) return true;
    }
    return false;
  }


  RoutingGauge* AllianceFramework::getRoutingGauge ( const Name& name )
  {
    if ( name.isEmpty() ) return _defaultRoutingGauge;

    map<const Name,RoutingGauge*>::iterator igauge = _routingGauges.find ( name );
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


  CellGauge* AllianceFramework::getCellGauge ( const Name& name )
  {
    if ( name.isEmpty() ) return _defaultCellGauge;

    map<const Name,CellGauge*>::iterator igauge = _cellGauges.find ( name );
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
    _defaultCellGauge                = gauge;
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


} // End of CRL namespace.
