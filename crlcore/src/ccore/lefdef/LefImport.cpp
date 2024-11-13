// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        C a d e n c e   L E F   I m p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LefImport.cpp"                          |
// +-----------------------------------------------------------------+


#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <boost/algorithm/string.hpp>
#if defined(HAVE_LEFDEF)
#  include  "lefrReader.hpp"
#endif
#include "hurricane/configuration/Configuration.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Pad.h"
#include "hurricane/Contact.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Cell.h"
#include "hurricane/Library.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DebugSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/ToolBox.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/CellGauge.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/LefImport.h"
#include "crlcore/Gds.h"


#if defined(HAVE_LEFDEF)

namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;


#if THIS_IS_DISABLED
  void  addSupplyNets ( Cell* cell )
  {
    Net* vss = Net::create( cell, "vss" );
    vss->setExternal( true );
    vss->setGlobal  ( true );
    vss->setType    ( Net::Type::GROUND );

    Net* vdd = Net::create( cell, "vdd" );
    vdd->setExternal( true );
    vdd->setGlobal  ( true );
    vdd->setType    ( Net::Type::POWER );
  }
#endif
  

  class PinRectilinearFilter {
    public:
                        PinRectilinearFilter ( DbU::Unit xThreshold=0
                                             , DbU::Unit yThreshold=0
                                             , uint32_t  flags=LefImport::PinFilter_NOFLAGS );
             bool       match                ( const Box& candidate, const Box& best );
      inline DbU::Unit  getXThreshold        () const;
      inline DbU::Unit  getYThreshold        () const;
      inline uint32_t   getFlags             () const;
      inline void       setXThreshold        ( DbU::Unit );
      inline void       setYThreshold        ( DbU::Unit );
      inline void       setFlags             ( uint32_t );
    private:
      DbU::Unit  _xThreshold;
      DbU::Unit  _yThreshold;
      uint32_t   _flags;
  };

  
  PinRectilinearFilter::PinRectilinearFilter ( DbU::Unit xThreshold, DbU::Unit yThreshold, uint32_t flags )
    : _xThreshold(xThreshold)
    , _yThreshold(yThreshold)
    , _flags     (flags)
  { }


  inline DbU::Unit  PinRectilinearFilter::getXThreshold () const                 { return _xThreshold; }
  inline DbU::Unit  PinRectilinearFilter::getYThreshold () const                 { return _yThreshold; }
  inline uint32_t   PinRectilinearFilter::getFlags      () const                 { return _flags; }
  inline void       PinRectilinearFilter::setXThreshold ( DbU::Unit xThreshold ) { _xThreshold=xThreshold; }
  inline void       PinRectilinearFilter::setYThreshold ( DbU::Unit yThreshold ) { _yThreshold=yThreshold; }
  inline void       PinRectilinearFilter::setFlags      ( uint32_t flags )       { _flags=flags; }


  bool  PinRectilinearFilter::match ( const Box& candidate, const Box& best )
  {
    if (candidate.getWidth () < _xThreshold) return false;
    if (candidate.getHeight() < _yThreshold) return false;
    if (_flags & LefImport::PinFilter_TALLEST) return (candidate.getHeight() > best.getHeight());
    if (_flags & LefImport::PinFilter_WIDEST ) return (candidate.getWidth () > best.getWidth ());
    if (_flags & LefImport::PinFilter_LARGEST) {
      float candidateArea = (float)candidate.getWidth() * (float)candidate.getHeight();
      float bestArea      = (float)best     .getWidth() * (float)best     .getHeight();
      return (candidateArea > bestArea);
    }
    return false;
  }


  class LefParser {
    public:
      static       void               setMergeLibrary          ( Library* );
      static       void               setGdsForeignDirectory   ( string );
      static       void               setGdsForeignLibrary     ( Library* );
      static       Library*           getGdsForeignLibrary     ();
      static       void               setPinFilter             ( DbU::Unit xThreshold, DbU::Unit yThreshold, uint32_t flags );
      static       DbU::Unit          fromLefUnits             ( int );
      static       Layer*             getLayer                 ( string );
      static       void               addLayer                 ( string, Layer* );
      static       void               clearLayer               ( string );
      static       void               reset                    ();
      static       Library*           parse                    ( string file );
                                      LefParser                ( string file, string libraryName );
                                     ~LefParser                ();
      inline       bool               isVH                     () const;
                   bool               isUnmatchedLayer         ( string );
                   Library*           createLibrary            ();
                   Cell*              earlyGetCell             ( bool& created, string name="" );
                   Net*               earlyGetNet              ( string name );
      inline       string             getLibraryName           () const;
      inline       Library*           getLibrary               ( bool create=false );
      inline const Point&             getOrigin                () const;
      inline       DbU::Unit          getOriginX               () const;
      inline       DbU::Unit          getOriginY               () const;
      inline       string             getForeignPath           () const;
      inline       void               setForeignPath           ( string );
      inline const Point&             getForeignPosition       () const;
      inline       void               setForeignPosition       ( const Point&  );
      inline       Net*               getGdsPower              () const;
      inline       void               setGdsPower              ( Net* );
      inline       Net*               getGdsGround             () const;
      inline       void               setGdsGround             ( Net* );
      inline       Cell*              getCell                  () const;
      inline       void               setCell                  ( Cell* );
      inline       CellGauge*         getCellGauge             () const;
      inline       void               setCellGauge             ( CellGauge* );
      inline       Net*               getNet                   () const;
      inline       void               setNet                   ( Net* );
      inline       void               setOrigin                ( DbU::Unit x, DbU::Unit y );
      static       void               setCoreSite              ( DbU::Unit x, DbU::Unit y );
      static       DbU::Unit          getCoreSiteX             ();
      static       DbU::Unit          getCoreSiteY             ();
      inline       DbU::Unit          getMinTerminalWidth      () const;
      inline       double             getUnitsMicrons          () const;
      inline       DbU::Unit          fromUnitsMicrons         ( double ) const;
      inline       void               setUnitsMicrons          ( double );
      inline       bool               hasErrors                () const;
      inline const vector<string>&    getErrors                () const;
      inline       void               pushError                ( const string& );
                   int                flushErrors              ();
      inline       void               clearErrors              ();
      inline       int                getNthMetal              () const;
      inline       void               incNthMetal              ();
      inline       int                getNthCut                () const;
      inline       void               incNthCut                ();
      inline       int                getNthRouting            () const;
      inline       void               incNthRouting            ();
      inline       RoutingGauge*      getRoutingGauge          () const;
      inline       void               addPinComponent          ( string name, Component* );
      inline       void               clearPinComponents       ();
    private:                                               
      static       int                _unitsCbk                ( lefrCallbackType_e,       lefiUnits*       , lefiUserData );
      static       int                _layerCbk                ( lefrCallbackType_e,       lefiLayer*       , lefiUserData );
      static       int                _siteCbk                 ( lefrCallbackType_e,       lefiSite*        , lefiUserData );
      static       int                _obstructionCbk          ( lefrCallbackType_e,       lefiObstruction* , lefiUserData );
      static       int                _macroCbk                ( lefrCallbackType_e,       lefiMacro*       , lefiUserData );
      static       int                _macroSiteCbk            ( lefrCallbackType_e, const lefiMacroSite*   , lefiUserData );
      static       int                _macroForeignCbk         ( lefrCallbackType_e, const lefiMacroForeign*, lefiUserData );
      static       int                _pinCbk                  ( lefrCallbackType_e,       lefiPin*         , lefiUserData );
                   void               _pinStdPostProcess       ();
                   void               _pinPadPostProcess       ();
    private:                                               
      static       string                _gdsForeignDirectory;
      static       Library*              _mergeLibrary;
      static       Library*              _gdsForeignLibrary;
      static       PinRectilinearFilter  _pinFilter;
                   string                _file;
                   string                _libraryName;
                   Library*              _library;
                   string                _foreignPath;
                   Point                 _foreignPosition;
                   Net*                  _gdsPower;
                   Net*                  _gdsGround;
                   Cell*                 _cell;
                   Point                 _origin;
                   Net*                  _net;
                   string                _busBits;
                   double                _unitsMicrons;
                   map< string, vector<Component*> >  _pinComponents;
      static       map<string,Layer*>    _layerLut;
                   vector<string>        _unmatchedLayers;
                   vector<string>        _errors;
                   int                   _nthMetal;
                   int                   _nthCut;
                   int                   _nthRouting;
                   RoutingGauge*         _routingGauge;
                   CellGauge*            _cellGauge;
                   DbU::Unit             _minTerminalWidth;
      static       DbU::Unit             _coreSiteX;
      static       DbU::Unit             _coreSiteY;
  };


  inline       bool              LefParser::isVH                     () const { return _routingGauge->isVH(); }
  inline       DbU::Unit         LefParser::getMinTerminalWidth      () const { return _minTerminalWidth; }
  inline       string            LefParser::getLibraryName           () const { return _libraryName; }
  inline       Library*          LefParser::getLibrary               ( bool create ) { if (not _library and create) createLibrary(); return _library; }
  inline       Cell*             LefParser::getCell                  () const { return _cell; }
  inline       void              LefParser::setCell                  ( Cell* cell ) { _cell=cell; }
  inline const Point&            LefParser::getOrigin                () const { return _origin; }
  inline       DbU::Unit         LefParser::getOriginX               () const { return _origin.getX(); }
  inline       DbU::Unit         LefParser::getOriginY               () const { return _origin.getY(); }
  inline       void              LefParser::setOrigin                ( DbU::Unit x, DbU::Unit y ) { _origin=Point(x,y); }
  inline       string            LefParser::getForeignPath           () const { return _foreignPath; }
  inline       void              LefParser::setForeignPath           ( string path ) { _foreignPath=path; }
  inline const Point&            LefParser::getForeignPosition       () const { return _foreignPosition; }
  inline       void              LefParser::setForeignPosition       ( const Point& position ) { _foreignPosition=position; }
  inline       Net*              LefParser::getGdsPower              () const { return _gdsPower; }
  inline       void              LefParser::setGdsPower              ( Net* net ) { _gdsPower=net; }
  inline       Net*              LefParser::getGdsGround             () const { return _gdsGround; }
  inline       void              LefParser::setGdsGround             ( Net* net ) { _gdsGround=net; }
  inline       void              LefParser::setCellGauge             ( CellGauge* gauge ) { _cellGauge=gauge; }
  inline       Net*              LefParser::getNet                   () const { return _net; }
  inline       void              LefParser::setNet                   ( Net* net ) { _net=net; }
  inline       double            LefParser::getUnitsMicrons          () const { return _unitsMicrons; }
  inline       void              LefParser::setUnitsMicrons          ( double precision ) { _unitsMicrons=precision; }
  inline       int               LefParser::getNthMetal              () const { return _nthMetal; }
  inline       void              LefParser::incNthMetal              () { ++_nthMetal; }
  inline       int               LefParser::getNthCut                () const { return _nthCut; }
  inline       void              LefParser::incNthCut                () { ++_nthCut; }
  inline       int               LefParser::getNthRouting            () const { return _nthRouting; }
  inline       void              LefParser::incNthRouting            () { ++_nthRouting; }
  inline       RoutingGauge*     LefParser::getRoutingGauge          () const { return _routingGauge; }
  inline       CellGauge*        LefParser::getCellGauge             () const { return _cellGauge; }
  inline       void              LefParser::setCoreSite              ( DbU::Unit x, DbU::Unit y ) { _coreSiteX=x; _coreSiteY=y; }
  inline       DbU::Unit         LefParser::getCoreSiteX             () { return _coreSiteX; }
  inline       DbU::Unit         LefParser::getCoreSiteY             () { return _coreSiteY; }
  inline       bool              LefParser::hasErrors                () const { return not _errors.empty(); }
  inline const vector<string>&   LefParser::getErrors                () const { return _errors; }
  inline       void              LefParser::pushError                ( const string& error ) { _errors.push_back(error); }
  inline       void              LefParser::clearErrors              () { return _errors.clear(); }
  inline       void              LefParser::addPinComponent          ( string name, Component* comp ) { _pinComponents[name].push_back(comp); }
  inline       void              LefParser::clearPinComponents       () { _pinComponents.clear(); }

  inline DbU::Unit  LefParser::fromUnitsMicrons ( double d ) const
  {
    DbU::Unit u = DbU::fromPhysical(d,DbU::Micro);
    if (u % DbU::oneGrid) {
      cerr << Error( "LefParser::fromUnitsMicrons(): Offgrid value %s (DbU=%d), grid %s (DbU=%d)."
                   , DbU::getValueString(u).c_str(), u
                   , DbU::getValueString(DbU::oneGrid).c_str(), DbU::oneGrid )
           << endl;
    }
    return u;
  }


  string                LefParser::_gdsForeignDirectory = "";
  Library*              LefParser::_gdsForeignLibrary   = nullptr;
  Library*              LefParser::_mergeLibrary        = nullptr;
  PinRectilinearFilter  LefParser::_pinFilter;
  map<string,Layer*>    LefParser::_layerLut;
  DbU::Unit             LefParser::_coreSiteX = 0;
  DbU::Unit             LefParser::_coreSiteY = 0;


  void  LefParser::setMergeLibrary ( Library* library )
  { _mergeLibrary = library; }
  

  void  LefParser::setGdsForeignDirectory ( string path )
  { _gdsForeignDirectory = path; }


  void  LefParser::setGdsForeignLibrary ( Library* library )
  { _gdsForeignLibrary = library; }


  Library* LefParser::getGdsForeignLibrary ()
  { return _gdsForeignLibrary; }


  void  LefParser::setPinFilter ( DbU::Unit xThreshold, DbU::Unit yThreshold, uint32_t flags )
  {
    _pinFilter.setXThreshold( xThreshold );
    _pinFilter.setYThreshold( yThreshold );
    _pinFilter.setFlags     ( flags );
  }


  void  LefParser::reset ()
  {
    _layerLut.clear();
    _coreSiteX = 0;
    _coreSiteY = 0;
  }
  

  Layer* LefParser::getLayer ( string layerName )
  {
    auto item = _layerLut.find( layerName );
    if (item != _layerLut.end()) return (*item).second;
    return NULL;
  }

  void  LefParser::addLayer ( string layerName, Layer* layer )
  {
    if (getLayer(layerName)) {
      cerr << Warning( "LefParser::addLayer(): Duplicated layer name \"%s\" (ignored).", layerName.c_str() );
      return;
    }

    _layerLut[ layerName ] = layer;
  }

  void LefParser::clearLayer ( string layerName )
  {
    auto item = _layerLut.find( layerName );
    if (item != _layerLut.end()) _layerLut.erase(item);
  }

  bool  LefParser::isUnmatchedLayer ( string layerName )
  {
    for ( string layer : _unmatchedLayers ) {
      if (layer == layerName) return true;
    }
    return false;
  }


  LefParser::LefParser ( string file, string libraryName )
    : _file            (file)
    , _libraryName     (libraryName)
    , _library         (nullptr)
    , _foreignPath     ()
    , _foreignPosition (Point(0,0))
    , _gdsPower        (nullptr)
    , _gdsGround       (nullptr)
    , _cell            (nullptr)
    , _origin          ()
    , _net             (nullptr)
    , _busBits         ("()")
    , _unitsMicrons    (0.01)
    , _unmatchedLayers ()
    , _errors          ()
    , _nthMetal        (0)
    , _nthCut          (0)
    , _nthRouting      (0)
    , _routingGauge    (nullptr)
    , _cellGauge       (nullptr)
    , _minTerminalWidth(DbU::fromMicrons( Cfg::getParamDouble("lefImport.minTerminalWidth",0)->asDouble() ))
  {
    _routingGauge = AllianceFramework::get()->getRoutingGauge();
    _cellGauge    = AllianceFramework::get()->getCellGauge();

    if (not _routingGauge)
      throw Error( "LefParser::LefParser(): No default routing gauge defined in Alliance framework." );

    if (not _cellGauge)
      throw Error( "LefParser::LefParser(): No default cell gauge defined in Alliance framework." );

    string unmatcheds = Cfg::getParamString("lefImport.unmatchedLayers","")->asString();
    if (not unmatcheds.empty()) {
      size_t ibegin = 0;
      size_t iend   = unmatcheds.find( ',', ibegin );
      while (iend != string::npos) {
        _unmatchedLayers.push_back( unmatcheds.substr(ibegin,iend-ibegin) );
        ibegin = iend+1;
        iend   = unmatcheds.find( ',', ibegin );
      }
      _unmatchedLayers.push_back( unmatcheds.substr(ibegin) );
    }
    
    lefrInit();
    lefrSetUnitsCbk       ( _unitsCbk        );
    lefrSetLayerCbk       ( _layerCbk        );
    lefrSetSiteCbk        ( _siteCbk         );
    lefrSetObstructionCbk ( _obstructionCbk  );
    lefrSetMacroCbk       ( _macroCbk        );
    lefrSetMacroSiteCbk   ( _macroSiteCbk    );
    lefrSetMacroForeignCbk( _macroForeignCbk );
    lefrSetPinCbk         ( _pinCbk          );
  }


  LefParser::~LefParser ()
  {
    lefrReset();
  }


  Library* LefParser::createLibrary ()
  {
    if (_mergeLibrary) {
      _library = _mergeLibrary;
      return _library;
    }

    DataBase* db          = DataBase::getDB();
    Library*  rootLibrary = db->getRootLibrary();
    if (not rootLibrary) rootLibrary = Library::create( db, "RootLibrary" );

    Library*  lefRootLibrary = rootLibrary->getLibrary( "LEF" );
    if (not lefRootLibrary) lefRootLibrary = Library::create( rootLibrary, "LEF" );

    _library = lefRootLibrary->getLibrary( _libraryName );
    if (_library) 
      cerr << Error( "LefParser::createLibrary(): attempt to reload \"%s\"."
                   , _libraryName.c_str() ) << endl;
    else
      _library = Library::create( lefRootLibrary, _libraryName );
 
    return _library;
  }


  Cell* LefParser::earlyGetCell ( bool& created, string name )
  {
    if (not _cell) {
      if (name.empty())
        name = "EarlyLEFCell";
      _cell = getLibrary(true)->getCell( name );
      if (not _cell) {
        created = true;
        _cell = Cell::create( getLibrary(true), name );
      }
    }
    return _cell;
  }


  Net* LefParser::earlyGetNet ( string name )
  {
    bool created = false;
    if (not _cell) earlyGetCell( created );
    Net* net = _cell->getNet( name );
    if (not net)
      net = Net::create( _cell, name );
    return net;
  }

  
  int  LefParser::_unitsCbk ( lefrCallbackType_e c, lefiUnits* units, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

    if (units->hasDatabase()) {
      parser->_unitsMicrons = 1.0 / units->databaseNumber();
      cerr << "     - Precision: " << parser->_unitsMicrons
           << " (LEF MICRONS scale factor:" << units->databaseNumber() << ")" << endl;
    }
    return 0;
  }

  
  int  LefParser::_layerCbk ( lefrCallbackType_e c, lefiLayer* lefLayer, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

    if (not lefLayer->hasType()) {
      cerr << Warning( "LefParser::_layerCbk(): layer \"%s\" has no TYPE (ignored).", lefLayer->name() );
      return 0;
    }

    Technology* techno = DataBase::getDB()->getTechnology();
    
    string lefType = lefLayer->type();
    boost::to_upper( lefType );

    if (lefType == "CUT") {
      Layer* layer = techno->getNthCut( parser->getNthCut() );
      while (parser->isUnmatchedLayer(getString(layer->getName()))) {
        parser->incNthCut();
        cerr << "     - Unmapped techno layer \"" << layer->getName() << "\"" << endl;
        layer = techno->getNthCut( parser->getNthCut() );
      }
      if (layer) {
        parser->addLayer( lefLayer->name(), layer );
        parser->incNthCut();

        cerr << "     - \"" << lefLayer->name() << "\" map to \"" << layer->getName() << "\"" << endl;
      }
    }

    if (lefType == "ROUTING") {
      Layer* layer = techno->getNthMetal( parser->getNthMetal() );
      while (parser->isUnmatchedLayer(getString(layer->getName()))) {
        parser->incNthMetal();
        cerr << "     - Unmapped techno layer \"" << layer->getName() << "\"" << endl;
        layer = techno->getNthMetal( parser->getNthMetal() );
      }
      
      if (layer) {
        BasicLayer* basicLayer = layer->getBasicLayers().getFirst();
        parser->addLayer( lefLayer->name(), basicLayer );

        cerr << "     - \"" << lefLayer->name() << "\" map to \"" << basicLayer->getName() << "\"" << endl;

        RoutingLayerGauge* gauge = parser->getRoutingGauge()->getLayerGauge( parser->getNthRouting() );

        if (gauge and (layer == gauge->getLayer())) {
          if (lefLayer->hasPitch()) {
            double lefPitch = lefLayer->pitch();
            double crlPitch = DbU::toPhysical(gauge->getPitch(),DbU::Micro);
            if (lefPitch > crlPitch)
              cerr << Warning( "LefParser::_layerCbk(): CRL Routing pitch for \"%s\" of %fum is less than %fum."
                             , getString( basicLayer->getName() ).c_str() , crlPitch , lefPitch ) << endl;
          }

          if (lefLayer->hasWidth()) {
            double lefWidth = lefLayer->width();
            double crlWidth = DbU::toPhysical(gauge->getWireWidth(),DbU::Micro);
            if (lefWidth > crlWidth)
              cerr << Warning( "LefParser::_layerCbk(): CRL Routing wire width for \"%s\" of %fum is less than %fum."
                             , getString( basicLayer->getName() ).c_str() , crlWidth , lefWidth ) << endl;
          }

          if (lefLayer->hasDirection()) {
            string lefDirection = lefLayer->direction();
            boost::to_upper( lefDirection );

            if ( (lefDirection == "HORIZONTAL") and gauge->isVertical() )
              cerr << Warning( "LefParser::_layerCbk(): CRL Routing direction discrepency for \"%s\", LEF is HORIZONTAL."
                             , getString( basicLayer->getName() ).c_str() ) << endl;

            if ( (lefDirection == "VERTICAL") and gauge->isHorizontal() )
              cerr << Warning( "LefParser::_layerCbk(): CRL Routing direction discrepency for \"%s\", LEF is VERTICAL."
                             , getString( basicLayer->getName() ).c_str() ) << endl;
          }
          parser->incNthRouting();
        } else {
          cerr << Warning( "LefParser::_layerCbk(): No CRL routing gauge defined for \"%s\"."
                         , getString( basicLayer->getName() ).c_str()
                         ) << endl;
        }

        parser->incNthMetal();
      }
    }

    return 0;
  }

  
  int  LefParser::_siteCbk ( lefrCallbackType_e c, lefiSite* site, lefiUserData ud )
  {
    LefParser*         parser = (LefParser*)ud;
    AllianceFramework* af     = AllianceFramework::get();

    if (site->hasClass()) {
      string siteClass = site->siteClass();
      boost::to_upper( siteClass );

      DbU::Unit lefSiteWidth  = DbU::fromPhysical( site->sizeX(), DbU::Micro );
      DbU::Unit lefSiteHeight = DbU::fromPhysical( site->sizeY(), DbU::Micro );

      if (siteClass == "CORE") {
        CellGauge* gauge = parser->getCellGauge();
        if (not gauge)
          throw Error( "LefParser::_siteCbk(): Default gauge is not defined. Aborting." );
        
        DbU::Unit  crlSliceStep   = gauge->getSliceStep  ();
        DbU::Unit  crlSliceHeight = gauge->getSliceHeight();

        if (not parser->getCoreSiteX()
           or ((parser->getCoreSiteX() != crlSliceStep) and (parser->getCoreSiteY() != crlSliceHeight)) ) {
          parser->setCoreSite( lefSiteWidth, lefSiteHeight );

          if ( (crlSliceStep == lefSiteWidth) and (crlSliceHeight == lefSiteHeight) )
            cerr << "     - Site \"" << site->name() << "\" of class CORE match the Coriolis Cell gauge." << endl;
        }
      } else if (siteClass == "PAD") {
        string     name = string("LEF.") + site->name();
        CellGauge* cg   = af->getCellGauge( name );

        if (cg) {
          if ( (cg->getSliceStep() != lefSiteWidth) or (cg->getSliceHeight() != lefSiteHeight)) {
            cerr << "     - Site \"" << site->name() << "\" of class PAD has mismatched redefinition OVERWRITING." << endl;
            cerr << "       width: "  << DbU::getValueString(cg->getSliceStep  ()) << " vs. " <<  DbU::getValueString(lefSiteWidth)
                 <<       " height: " << DbU::getValueString(cg->getSliceHeight()) << " vs. " <<  DbU::getValueString(lefSiteHeight)
                 << endl;
          //cg->setPitch      ( lefSiteWidth  );
            cg->setSliceStep  ( lefSiteWidth  );
            cg->setSliceHeight( lefSiteHeight );
          }
          cg->setFlags( CellGauge::Flags::Pad );
        } else {
          cg = CellGauge::create( name.c_str(), "unknown", lefSiteWidth, lefSiteHeight, lefSiteWidth );
          cg->setFlags( CellGauge::Flags::Pad );
          af->addCellGauge( cg );
        }
      }
    }

    return 0;
  }


  int  LefParser::_macroForeignCbk ( lefrCallbackType_e c, const lefiMacroForeign* foreign, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

    bool   created  = false;
    string cellName = string(foreign->cellName()) + "_lef_foreign";
    Cell*  cell     = parser->earlyGetCell( created, cellName );

    // parser->setForeignPosition( Point( parser->fromUnitsMicrons( - foreign->px() )
    //                                  , parser->fromUnitsMicrons( - foreign->py() )));

    if (not created) {
      if (_gdsForeignDirectory.empty()) {
        cerr << Warning( "LefParser::_macroForeignCbk(): GDS directory *not* set, ignoring FOREIGN statement." ) << endl;
        return 0;
      }

      string gdsPath = _gdsForeignDirectory + "/" + foreign->cellName() + ".gds";
      parser->setForeignPath( gdsPath );

      Gds::setTopCellName( foreign->cellName() );
      Gds::load( parser->getLibrary(), parser->getForeignPath()
               , Gds::NoBlockages|Gds::Layer_0_IsBoundary);

      for ( Net* net : cell->getNets() ) {
        if (net->isPower ()) parser->setGdsPower ( net );
        if (net->isGround()) parser->setGdsGround( net );
        if (parser->getForeignPosition() != Point(0,0)) {
          for ( Component* component : net->getComponents() ) {
            component->translate( parser->getForeignPosition().getX()
                                , parser->getForeignPosition().getY() );
          }
        }
      }

      return 0;
    }

    if (not _gdsForeignLibrary) {
      cerr << Warning( "LefParser::_macroForeignCbk(): GDS foreign library *not* set, ignoring FOREIGN statement." ) << endl;
      return 0;
    }
    Cell* gdsCell = _gdsForeignLibrary->getCell( cellName );
    if (not gdsCell) {
      cerr << Warning( "LefParser::_macroForeignCbk(): GDS foreign cell *not* found in library." ) << endl;
      return 0;
    }

    Instance::create( cell, "foreign"
                    , gdsCell
                    , Transformation( parser->getForeignPosition() )
                    , Instance::PlacementStatus::FIXED );

    return 0;
  }
  

  int  LefParser::_obstructionCbk ( lefrCallbackType_e c, lefiObstruction* obstruction, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

    const Layer*     layer         = NULL;
    const Layer*     blockageLayer = NULL;
          Cell*      cell          = parser->getCell();
          Net*       blockageNet   = cell->getNet( "blockage" );

    if (not blockageNet) {
      blockageNet = Net::create( cell, "blockage" );
      blockageNet->setType( Net::Type::BLOCKAGE );
    }

  //cerr << "       @ _obstructionCbk: " << blockageNet->getName() << endl;
      
    lefiGeometries* geoms = obstruction->geometries();
    for ( int igeom=0 ; igeom < geoms->numItems() ; ++ igeom ) {
      if (geoms->itemType(igeom) == lefiGeomLayerE) {
        layer = parser->getLayer( geoms->getLayer(igeom) );
        if (layer)
          blockageLayer = layer->getBlockageLayer();
      }
      if (not blockageLayer) {
        cerr << Error( "DefImport::_obstructionCbk(): No blockage layer associated to \"%s\".\n"
                       "        (while parsing \"%s\")"
                     , getString( layer->getName() ).c_str()
                     , getString( cell ).c_str()
                     ) << endl;
        continue;
      }
      
      if (geoms->itemType(igeom) == lefiGeomRectE) {
        lefiGeomRect* r         = geoms->getRect(igeom);
        double        w         = r->xh - r->xl;
        double        h         = r->yh - r->yl;
        Segment*      segment   = NULL;
        if (w >= h) {
          DbU::Unit yl = parser->fromUnitsMicrons( r->yl );
          DbU::Unit yh = parser->fromUnitsMicrons( r->yh );
          if ((yl % DbU::twoGrid) xor (yh % DbU::twoGrid)) {
            segment = Horizontal::create( blockageNet, blockageLayer
                                        , yh - DbU::oneGrid
                                        , DbU::twoGrid
                                        , parser->fromUnitsMicrons( r->xl )
                                        , parser->fromUnitsMicrons( r->xh )
                                        );
            segment = Horizontal::create( blockageNet, blockageLayer
                                        , (yh - DbU::oneGrid + yl) / 2
                                        ,  yh - DbU::oneGrid - yl
                                        , parser->fromUnitsMicrons( r->xl )
                                        , parser->fromUnitsMicrons( r->xh )
                                        );
          } else {
            segment = Horizontal::create( blockageNet, blockageLayer
                                        , (yh + yl) / 2
                                        ,  yh - yl 
                                        , parser->fromUnitsMicrons( r->xl )
                                        , parser->fromUnitsMicrons( r->xh )
                                        );
          }
        } else {
          DbU::Unit xl = parser->fromUnitsMicrons( r->xl );
          DbU::Unit xh = parser->fromUnitsMicrons( r->xh );
          if ((xl % DbU::twoGrid) xor (xh % DbU::twoGrid)) {
            segment = Vertical::create( blockageNet, blockageLayer
                                      , xh - DbU::oneGrid
                                      , DbU::twoGrid
                                      , parser->fromUnitsMicrons( r->yl )
                                      , parser->fromUnitsMicrons( r->yh )
                                      );
            segment = Vertical::create( blockageNet, blockageLayer
                                      , (xh - DbU::oneGrid + xl) / 2
                                      ,  xh - DbU::oneGrid - xl
                                      , parser->fromUnitsMicrons( r->yl )
                                      , parser->fromUnitsMicrons( r->yh )
                                      );
          } else {
            segment = Vertical::create( blockageNet, blockageLayer
                                      , (xh + xl) / 2
                                      ,  xh - xl
                                      , parser->fromUnitsMicrons( r->yl )
                                      , parser->fromUnitsMicrons( r->yh )
                                      );
          }
        }
        cdebug_log(100,0) << "| " << segment << endl;
      }

      if (geoms->itemType(igeom) == lefiGeomPolygonE) {
        lefiGeomPolygon* polygon = geoms->getPolygon(igeom);
        vector<Point>    points;
        for ( int ipoint=0 ; ipoint<polygon->numPoints ; ++ipoint ) {
          points.push_back( Point( parser->fromUnitsMicrons(polygon->x[ipoint])
                                 , parser->fromUnitsMicrons(polygon->y[ipoint]) ));
        }
        points.push_back( Point( parser->fromUnitsMicrons(polygon->x[0])
                               , parser->fromUnitsMicrons(polygon->y[0]) ));
        Rectilinear::create( blockageNet, blockageLayer, points );
        continue;
      }
    }

    return 0;
  }

  
  int  LefParser::_macroCbk ( lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud )
  {
    AllianceFramework* af     = AllianceFramework::get();
    LefParser*         parser = (LefParser*)ud;

    parser->setCellGauge( nullptr );

    bool       created  = false;
    string     cellName = macro->name();
    DbU::Unit  width    = 0;
    DbU::Unit  height   = 0;
    Cell*      cell     = parser->earlyGetCell( created );

    parser->setOrigin( parser->fromUnitsMicrons( - macro->originX() )
                     , parser->fromUnitsMicrons( - macro->originY() ));

    if (cell->getName() != Name(cellName)) {
      cell->setName( cellName );
    }

    if (macro->hasSize()) {
      width  = parser->fromUnitsMicrons( macro->sizeX() );
      height = parser->fromUnitsMicrons( macro->sizeY() );
      cell->setAbutmentBox( Box( 0, 0, width, height ).translate( parser->getOrigin() ));
    }

    bool   isPad     = false;
    string gaugeName = "Unknown SITE";
    if (macro->hasSiteName()) {
      gaugeName = string("LEF.") + macro->siteName();
      CellGauge* cg = af->getCellGauge( gaugeName );
      if (cg) {
        isPad = cg->isPad();
        if (cg->getSliceHeight() != height) {
          cerr << Warning( "LefParser::_macroCbk(): Cell height %s do not match CellGauge/SITE \"%s\" of %s."
                         , DbU::getValueString(height).c_str()
                         , getString(cg->getName()).c_str()
                         , DbU::getValueString(cg->getSliceHeight()).c_str()
                         ) << endl;
        }
        parser->setCellGauge( cg );
      } else {
        cerr << Warning( "LefParser::_macroCbk(): No CellGauge associated to SITE \"%s\"."
                       , macro->siteName() ) << endl;
      }
    }

    if (not isPad) parser->_pinStdPostProcess();
    else           parser->_pinPadPostProcess();
    parser->clearPinComponents();

    cmess2 << "     o " << cellName
           << " " << DbU::getValueString(width) << " " << DbU::getValueString(height)
           << " " << gaugeName; 
    if (isPad) cerr << " (PAD)";
    cerr << endl; 

    Catalog::State* state = af->getCatalog()->getState( cellName );
    if (not state) state = af->getCatalog()->getState ( cellName, true );
    state->setFlags( Catalog::State::Logical
                   | Catalog::State::Physical
                   | Catalog::State::InMemory
                   | Catalog::State::TerminalNetlist, true );
    cell->setTerminalNetlist( true );
    parser->setCell     ( nullptr );
    parser->setGdsPower ( nullptr );
    parser->setGdsGround( nullptr );

    return 0;
  }

  
  int  LefParser::_macroSiteCbk ( lefrCallbackType_e c, const lefiMacroSite* site, lefiUserData ud )
  { return 0; }

  
  int  LefParser::_pinCbk ( lefrCallbackType_e c, lefiPin* pin, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

  //if (parser->getCell()->getName() == "NAND2B_XL_GF6T_1P5")
  //if (parser->getCell()->getName() == "MXI2_X1_GF6T_1P5")
  //if (parser->getCell()->getName() == "NAND2B_X2_GF6T_1P5")
    if (parser->getCell()->getName() == "AOI222_X2_GF6T_1P5")
      DebugSession::open( 100, 110 );
    cdebug_log(100,1) << "@ _pinCbk()" << endl;

    bool  created = false;
    parser->earlyGetCell( created );

    Net*      net     = nullptr;
    Net::Type netType = Net::Type::UNDEFINED;
    if (pin->hasUse()) {
      string lefUse = pin->use();
      boost::to_upper( lefUse );
      
      if (lefUse == "SIGNAL") netType = Net::Type::LOGICAL;
    //if (lefUse == "ANALOG") netType = Net::Type::ANALOG;
      if (lefUse == "CLOCK" ) netType = Net::Type::CLOCK;
      if (lefUse == "POWER" ) netType = Net::Type::POWER;
      if (lefUse == "GROUND") netType = Net::Type::GROUND;
    }

    if ((netType == Net::Type::POWER) and parser->getGdsPower()) {
      net = parser->getGdsPower();
      cerr << "       - Renaming GDS power net \"" << net->getName() << "\""
           << " to LEF name \"" << pin->name() << "\"." << endl;
      net->setName( pin->name() );
      parser->setGdsPower( nullptr );
    } else {
      if ((netType == Net::Type::GROUND) and parser->getGdsGround()) {
        net = parser->getGdsGround();
        cerr << "       - Renaming GDS ground net \"" << net->getName() << "\""
             << " to LEF name \"" << pin->name() << "\"." << endl;
        net->setName( pin->name() );
        parser->setGdsGround( nullptr );
      } else {
        net = parser->earlyGetNet( pin->name() );
      }
    }
    net->setExternal( true );
    net->setType    ( netType );

    if (pin->hasDirection()) {
      string lefDir = pin->direction();
      boost::to_upper( lefDir );
      
      if (lefDir == "INPUT"          ) net->setDirection( Net::Direction::IN       );
      if (lefDir == "OUTPUT"         ) net->setDirection( Net::Direction::OUT      );
      if (lefDir == "OUTPUT TRISTATE") net->setDirection( Net::Direction::TRISTATE );
      if (lefDir == "INOUT"          ) net->setDirection( Net::Direction::INOUT    );
    }
    if (net->isSupply())                             net->setGlobal( true );
    if (pin->name()[ strlen(pin->name())-1 ] == '!') net->setGlobal( true );

    for ( int iport=0 ; iport < pin->numPorts() ; ++iport ) {
      Layer* layer = NULL;
      
      lefiGeometries* geoms = pin->port( iport );
      for ( int igeom=0 ; igeom < geoms->numItems() ; ++igeom ) {
        if (geoms->itemType(igeom) == lefiGeomLayerE) {
          layer = parser->getLayer( geoms->getLayer(igeom) );
          continue;
        }
        if (geoms->itemType(igeom) == lefiGeomRectE) {
          lefiGeomRect* r          = geoms->getRect(igeom);
          DbU::Unit     xl         = parser->fromUnitsMicrons( r->xl );
          DbU::Unit     xh         = parser->fromUnitsMicrons( r->xh );
          DbU::Unit     yl         = parser->fromUnitsMicrons( r->yl );
          DbU::Unit     yh         = parser->fromUnitsMicrons( r->yh );
          DbU::Unit     w          = xh - xl;
          DbU::Unit     h          = yh - yl;
          Segment*      segment    = NULL;
          float         formFactor = (float)w / (float)h;
          const RoutingLayerGauge* gauge = parser->getRoutingGauge()->getLayerGauge( layer );
          
          cdebug_log(100,0) << "formFactor=" << formFactor
                            << " h=" << DbU::getValueString(h)
                            << " (> " << DbU::getValueString(parser->getMinTerminalWidth()) << ")"
                            << endl;
          if (formFactor > 1.0) {
            if ((yl % DbU::twoGrid) xor (yh % DbU::twoGrid)) {
              Pad::create( net, layer, Box( xl, yl, xh, yh).translate( parser->getOrigin() ));
              yh -= DbU::oneGrid;
            }
            segment = Horizontal::create( net, layer
                                        , (yh + yl) / 2
                                        ,  yh - yl
                                        ,  xl
                                        ,  xh
                                        );
          } else {
            if ((xl % DbU::twoGrid) xor (xh % DbU::twoGrid)) {
              Pad::create( net, layer, Box( xl, yl, xh, yh).translate( parser->getOrigin() ));
              xh -= DbU::oneGrid;
            }
            segment = Vertical::create( net, layer
                                      , (xh + xl) / 2 + parser->getOriginX()
                                      ,  xh - xl
                                      ,  yl + parser->getOriginY()
                                      ,  yh + parser->getOriginY()
                                      );
          }
          cdebug_log(100,0) << "| " << segment << endl;
          if (segment) parser->addPinComponent( pin->name(), segment );
        //cerr << "       | " << segment << endl;
          continue;
        }
        if (geoms->itemType(igeom) == lefiGeomPolygonE) {
          lefiGeomPolygon* polygon = geoms->getPolygon(igeom);
          vector<Point>    points;
          for ( int ipoint=0 ; ipoint<polygon->numPoints ; ++ipoint ) {
            points.push_back( Point( parser->fromUnitsMicrons(polygon->x[ipoint])
                                   , parser->fromUnitsMicrons(polygon->y[ipoint]) ).translate( parser->getOrigin() ));
          }
          points.push_back( Point( parser->fromUnitsMicrons(polygon->x[0])
                                 , parser->fromUnitsMicrons(polygon->y[0]) ).translate( parser->getOrigin() ));
          Rectilinear* rectilinear = Rectilinear::create( net, layer, points );
          if (rectilinear) parser->addPinComponent( pin->name(), rectilinear );
          continue;
        }
        if (geoms->itemType(igeom) == lefiGeomClassE) {
        // Ignore CLASS <site>. Deduced from segments positions.
          continue;
        }

        string geomTypeName;
        switch ( geoms->itemType(igeom) ) {
          case lefiGeomUnknown:           geomTypeName = "lefiGeomUnknown"          ; break;
          case lefiGeomLayerE:            geomTypeName = "lefiGeomLayerE"           ; break;
          case lefiGeomLayerMinSpacingE:  geomTypeName = "lefiGeomMinSpacingE"      ; break;
          case lefiGeomLayerRuleWidthE:   geomTypeName = "lefiGeomRuleWidthE"       ; break;
          case lefiGeomWidthE:            geomTypeName = "lefiGeomWidthE"           ; break;
          case lefiGeomPathE:             geomTypeName = "lefiGeomPathE"            ; break;
          case lefiGeomPathIterE:         geomTypeName = "lefiGeomPathIterE"        ; break;
          case lefiGeomRectE:             geomTypeName = "lefiGeomRectE"            ; break;
          case lefiGeomRectIterE:         geomTypeName = "lefiGeomRectIterE"        ; break;
          case lefiGeomPolygonE:          geomTypeName = "lefiGeomPolygonE"         ; break;
          case lefiGeomPolygonIterE:      geomTypeName = "lefiGeomPolygonIterE"     ; break;
          case lefiGeomViaE:              geomTypeName = "lefiGeomViaE"             ; break;
          case lefiGeomViaIterE:          geomTypeName = "lefiGeomViaIterE"         ; break;
          case lefiGeomClassE:            geomTypeName = "lefiGeomClassE"           ; break;
          case lefiGeomLayerExceptPgNetE: geomTypeName = "lefiGeomLayerExceptPgNetE"; break;
          case lefiGeomEnd:               geomTypeName = "lefiGeomEnd"              ; break;
        }

        if (not geomTypeName.empty())
          cerr << Warning( "LefParser::_pinCkb(): In PIN \"%s\", unsupported geometry \"%s\"."
                         , pin->name(), geomTypeName.c_str() ) << endl;
      }
    }

    cdebug_tabw(100,-1);
  //if (parser->getCell()->getName() == "NAND2B_XL_GF6T_1P5")
  //if (parser->getCell()->getName() == "MXI2_X1_GF6T_1P5")
  //if (parser->getCell()->getName() == "NAND2B_X2_GF6T_1P5")
    if (parser->getCell()->getName() == "AOI222_X2_GF6T_1P5")
      DebugSession::close();
    return 0;
  }


  void  LefParser::_pinStdPostProcess ()
  {
    const Layer*              metal1      = _routingGauge->getLayerGauge( (size_t)0 )->getLayer();
    const RoutingLayerGauge*  gaugeMetal1 = _routingGauge->getLayerGauge( (size_t)0 );
    const RoutingLayerGauge*  gaugeMetal2 = _routingGauge->getLayerGauge( 1 );
          Box                 ab          = _cell->getAbutmentBox();

  //if (_cell->getName() == "gf180mcu_fd_sc_mcu9t5v0__inv_1")
  //if (_cell->getName() == "ENDCAPTIE16_GF6T_1P5")
  //if (_cell->getName() == "NAND4_XL_GF6T_1P5")
  //if (_cell->getName() == "AND3_X12_GF6T_1P5")
  //if (_cell->getName() == "NAND2B_XL_GF6T_1P5")
  //if (_cell->getName() == "MXI2_X1_GF6T_1P5")
    if (_cell->getName() == "AOI222_X2_GF6T_1P5")
      DebugSession::open( 100, 110 );
    cdebug_log(100,1) << "@ _pinStdPostProcess" << endl;

    for ( auto element : _pinComponents ) {
      string              pinName    = element.first;
      vector<Component*>& components = element.second;
      vector<Segment*>    ongrids;
      bool                isSupply = false;

      for ( Component* component : components ) {
        if (component->getNet()->isSupply()) {
          isSupply = true;
          break;
        }

        Segment* segment = dynamic_cast<Segment*>( component );
        if (segment) {
          bool isWide = (segment->getWidth () >= getMinTerminalWidth())
                    and (segment->getLength() >= getMinTerminalWidth());

          cdebug_log(100,0) << "> " << segment << endl;
          if (isVH() and (segment->getLayer()->getMask() == metal1->getMask())) {
            cdebug_log(100,0) << "isVH()=true" << endl;
            DbU::Unit metal1Width = _routingGauge->getLayerGauge((size_t)0)->getWireWidth() / 2;
            Vertical* v           = dynamic_cast<Vertical*>( segment );
            if (v) {
              if (v->getLength() < getMinTerminalWidth())
                continue;
              DbU::Unit nearestX = gaugeMetal2->getTrackPosition( ab.getXMin()
                                                                , ab.getXMax()
                                                                , v->getX()
                                                                , Constant::Nearest );

              if (nearestX == v->getX()) {
                ongrids.push_back( v );
              } else {
                DbU::Unit neighbor = nearestX;
                // DbU::Unit neighbor = nearestX
                //   + ((nearestX > v->getX()) ? 1 : -1) * gaugeMetal2->getPitch();

                cdebug_log(100,0) <<       "| X:" << DbU::getValueString(v->getX())
                                  << " nearestX:" << DbU::getValueString(nearestX)
                                  << " neighbor:" << DbU::getValueString(neighbor)
                                  << endl;
                Interval   segSpan     ( v->getX() - v->getHalfWidth(), v->getX() + v->getHalfWidth() );
                Interval   ongridSpan  ( neighbor - metal1Width, neighbor + metal1Width );
                cdebug_log(100,0) <<       "| M1 half width:" << DbU::getValueString(metal1Width) << endl;
                cdebug_log(100,0) <<       "| " << segSpan << " include? " << ongridSpan << endl;
                if (not segSpan.contains(ongridSpan))
                  continue;
                ongrids.push_back( v );
                // ongrids.push_back( Vertical::create( v->getNet()
                //                                    , v->getLayer()
                //                                    , nearestX
                //                                    , _routingGauge->getLayerGauge((size_t)0)->getWireWidth()
                //                                    , v->getDySource()
                //                                    , v->getDyTarget()
                //                                    )
                //                  );
              }
              cdebug_log(100,0) << "+ " << ongrids[ongrids.size()-1] << endl;
              continue;
            } else {
              Horizontal* h = dynamic_cast<Horizontal*>( segment );
              if (not h) continue;
              if (h->getWidth() < getMinTerminalWidth())
                continue;

              DbU::Unit nearestX = gaugeMetal2->getTrackPosition( ab.getXMin()
                                                                , ab.getXMax()
                                                                , (h->getSourceX() + h->getTargetX()) / 2
                                                                , Constant::Nearest );
              Interval   segSpan     ( h->getSourceX(), h->getTargetX() );
              Interval   ongridSpan  ( nearestX - metal1Width, nearestX + metal1Width );
              cdebug_log(100,0) <<       "| M1 half width:" << DbU::getValueString(metal1Width) << endl;
              cdebug_log(100,0) <<       "| " << segSpan << " include? " << ongridSpan << endl;
              if (not segSpan.contains(ongridSpan))
                continue;
              ongrids.push_back( h );
              cdebug_log(100,0) << "+ " << ongrids[ongrids.size()-1] << endl;
            }
          }
      
          if (isWide) ongrids.push_back( segment );
        }
        Rectilinear* rectilinear = dynamic_cast<Rectilinear*>( component );
        if (rectilinear) {
          cdebug_log(100,0) << "+ " << rectilinear << endl;
          if (rectilinear->getLayer()->getMask() != metal1->getMask())
            continue;

          vector<Box> boxes;
          if (component->getNet()->isSupply()) {
            rectilinear->getAsRectangles( boxes );
            ongrids.push_back( Horizontal::create( rectilinear->getNet()
                                                 , rectilinear->getLayer()
                                                 , boxes.front().getYCenter()
                                                 , boxes.front().getHeight()
                                                 , _cell->getAbutmentBox().getXMin()
                                                 , _cell->getAbutmentBox().getXMax()
                                                 )
                                 );
          } else {
            rectilinear->getAsBiggestRectangles( boxes
                                               , _pinFilter.getXThreshold()
                                               , _pinFilter.getYThreshold() );
            Box best;
            for ( const Box& candidate : boxes ) {
              if (_pinFilter.match(candidate,best))
                best = candidate;
            }
            if (not best.isEmpty()) {
#if EXTENDED_COVERAGE
              DbU::Unit ymin = gaugeMetal2->getTrackPosition( ab.getYMin()
                                                            , ab.getXMax()
                                                            , best.getYMin() + gaugeMetal2->getViaWidth()/2
                                                            , Constant::Superior );
              DbU::Unit ymax = gaugeMetal2->getTrackPosition( ab.getYMin()
                                                            , ab.getXMax()
                                                            , best.getYMax() - gaugeMetal2->getViaWidth()/2
                                                            , Constant::Inferior );
              ongrids.push_back( Vertical::create( rectilinear->getNet()
                                                 , rectilinear->getLayer()
                                                 , best.getXCenter()
                                                 , best.getWidth()
                                                 , ymin - gaugeMetal2->getViaWidth()/2
                                                 , ymax + gaugeMetal2->getViaWidth()/2
                                                 )
                                 );
              cdebug_log(100,0) << "| -> " << ongrids.back() << endl;
#endif
            //if (gaugeMetal2->isHorizontal()) {
              if (best.getWidth() < best.getHeight()) {
                ongrids.push_back( Vertical::create( rectilinear->getNet()
                                                   , rectilinear->getLayer()
                                                   , best.getXCenter()
                                                   , best.getWidth()
                                                   , best.getYMin() 
                                                   , best.getYMax()
                                                   )
                                 );
              } else {
                ongrids.push_back( Horizontal::create( rectilinear->getNet()
                                                     , rectilinear->getLayer()
                                                     , best.getYCenter()
                                                     , best.getHeight()
                                                     , best.getXMin() 
                                                     , best.getXMax()
                                                     )
                                 );
              }
              cdebug_log(100,0) << "| -> " << ongrids.back() << endl;
            }
#if THIS_IS_DISABLED
            for ( const Box& box : boxes ) {
              cdebug_log(100,0) << "| " << box << endl;
            // Assume M2 is horizontal.
              DbU::Unit ymin = gaugeMetal2->getTrackPosition( ab.getYMin()
                                                            , ab.getXMax()
                                                            , box.getYMin() + gaugeMetal2->getViaWidth()/2
                                                            , Constant::Superior );
              DbU::Unit ymax = gaugeMetal2->getTrackPosition( ab.getYMin()
                                                            , ab.getXMax()
                                                            , box.getYMax() - gaugeMetal2->getViaWidth()/2
                                                            , Constant::Inferior );
              ongrids.push_back( Vertical::create( rectilinear->getNet()
                                                 , rectilinear->getLayer()
                                                 , box.getXCenter()
                                                 , box.getWidth()
                                                 , ymin - gaugeMetal2->getViaWidth()/2
                                                 , ymax + gaugeMetal2->getViaWidth()/2
                                                 )
                                 );
              cdebug_log(100,0) << "| -> " << ongrids.back() << endl;
              // DbU::Unit neighbor = nearestY
              //   + ((nearestY > box.getYCenter()) ? 1 : -1) * gaugeMetal2->getPitch();
              
              // if (  (box.getYMin() > neighbor)
              //    or (box.getYMax() < neighbor) ) {
              //   ongrids.push_back( Vertical::create( rectilinear->getNet()
              //                                      , rectilinear->getLayer()
              //                                      , box.getXCenter()
              //                                      , box.getWidth()
              //                                      , box.getYMin()
              //                                      , box.getYMax()
              //                                      )
              //                    );
              // }
            }
#endif
          }
        }
      }

      if (ongrids.empty()) {
        if (not isSupply)
          cerr << Warning( "LefParser::_pinStdPostProcess(): Pin \"%s\" has no terminal ongrid."
                         , pinName.c_str() ) << endl;
        for ( Component* component : components ) {
          NetExternalComponents::setExternal( component );
        }
      } else {
        for ( Segment* segment : ongrids ) {
          NetExternalComponents::setExternal( segment );
        }
      }
    }

    cdebug_tabw(100,-1);
  //if (_cell->getName() == "gf180mcu_fd_sc_mcu9t5v0__inv_1")
  //if (_cell->getName() == "ENDCAPTIE16_GF6T_1P5")
  //if (_cell->getName() == "NAND4_XL_GF6T_1P5")
  //if (_cell->getName() == "AND3_X12_GF6T_1P5")
  //if (_cell->getName() == "NAND2B_XL_GF6T_1P5")
  //if (_cell->getName() == "MXI2_X1_GF6T_1P5")
    if (_cell->getName() == "AOI222_X2_GF6T_1P5")
      DebugSession::close();
  }


  void  LefParser::_pinPadPostProcess ()
  {
    Box  ab          = getCell()->getAbutmentBox();
    bool isCornerPad = (_cellGauge) and (_cellGauge->getSliceHeight() == _cellGauge->getSliceStep());

    for ( auto element : _pinComponents ) {
      string              pinName  = element.first;
      vector<Component*>& segments = element.second;
      vector<Segment*>    ongrids;

      if (segments.empty()) continue;
      
      Net* net = segments[0]->getNet();

      for ( size_t i=0 ; i<segments.size() ; ++i ) {
        Box      bb         = segments[i]->getBoundingBox();
        Interval hspan      = Interval( bb.getXMin(), bb.getXMax() );
        Interval vspan      = Interval( bb.getYMin(), bb.getYMax() );
        Segment* capSegment = NULL;

        if (segments[i]->getLayer()->isBlockage()) continue;

        if (net->isSupply()) {
          if (hspan.contains(ab.getXMin())) {
            capSegment = Horizontal::create( net
                                           , segments[i]->getLayer()
                                           , vspan.getCenter()
                                           , vspan.getSize()
                                           , ab.getXMin()
                                           , hspan.getVMax()
                                           );
          } else if (hspan.contains(ab.getXMax())) {
            capSegment = Horizontal::create( net
                                           , segments[i]->getLayer()
                                           , vspan.getCenter()
                                           , vspan.getSize()
                                           , hspan.getVMin()
                                           , ab.getXMax()
                                           );
          }
        }

        if (not capSegment) {
          vector<DbU::Unit> distanceToSide;
          distanceToSide.push_back( std::abs(bb.getXMin() - ab.getXMin()) );  // West.
          distanceToSide.push_back( std::abs(ab.getXMax() - bb.getXMax()) );  // East.
          distanceToSide.push_back( std::abs(bb.getYMin() - ab.getYMin()) );  // South.
          distanceToSide.push_back( std::abs(ab.getYMax() - bb.getYMax()) );  // North.

          size_t closestSide = ((isCornerPad) ? 0 : 2);
          for ( size_t i=closestSide ; i < distanceToSide.size() ; ++i ) {
            if (distanceToSide[i] < distanceToSide[closestSide])
              closestSide = i;
          }

          switch ( closestSide ) {
            default:
            case 0:  // West.
              capSegment = Horizontal::create( net
                                             , segments[i]->getLayer()
                                             , vspan.getCenter()
                                             , vspan.getSize()
                                             , ab.getXMin()
                                             , hspan.getVMax()
                                             );
              break;
            case 1:  // East.
              capSegment = Horizontal::create( net
                                             , segments[i]->getLayer()
                                             , vspan.getCenter()
                                             , vspan.getSize()
                                             , hspan.getVMin()
                                             , ab.getXMax()
                                             );
              break;
            case 2:  // South.
              capSegment = Vertical::create( net
                                           , segments[i]->getLayer()
                                           , hspan.getCenter()
                                           , hspan.getSize()
                                           , ab.getYMin()
                                           , vspan.getVMax()
                                           );
              break;
            case 3:  // North.
              capSegment = Vertical::create( net
                                           , segments[i]->getLayer()
                                           , hspan.getCenter()
                                           , hspan.getSize()
                                           , vspan.getVMin()
                                           , ab.getYMax()
                                           );
              break;
          }
        }

        if (capSegment) {
          NetExternalComponents::setExternal( capSegment );
          segments[i]->destroy();
          segments[i] = NULL;
        }
      }
    }
  }


  int  LefParser::flushErrors ()
  {
    int code = (hasErrors()) ? 1 : 0;

    for ( size_t ierror=0 ; ierror < _errors.size() ; ++ierror ) {
        string message = "LefImport::load(): " + _errors[ierror];
        cerr << Error(message.c_str(),getString(_library->getName()).c_str()) << endl;
    }
    clearErrors ();

    return code;
  }


  Library* LefParser::parse ( string file )
  {
    cmess1 << "  o  LEF: <" << file << ">" << endl;

    DbU::setStringMode( DbU::Physical, DbU::Micro );

    size_t iext = file.rfind( '.' );
    if (file.compare(iext,4,".lef") != 0)
      throw Error( "LefImport::load(): DEF files must have  \".lef\" extension <%s>.", file.c_str() );

    size_t  islash = file.rfind( '/' );
    islash = (islash == string::npos) ? 0 : islash+1;

    string                libraryName = file.substr( islash, file.size()-4-islash );
    unique_ptr<LefParser> parser      ( new LefParser(file,libraryName) );

    FILE* lefStream = fopen( file.c_str(), "r" );
    if (not lefStream)
      throw Error( "LefImport::load(): Cannot open LEF file \"%s\".", file.c_str() );

  //parser->createLibrary( libraryName );
    lefrRead( lefStream, file.c_str(), (lefiUserData)parser.get() );

    fclose( lefStream );

    if (not parser->getCellGauge()) {
      cerr << Warning( "LefParser::parse(): No default Alliance cell gauge, unable to check the Cell gauge." ) << endl;
    } else if (not parser->getCoreSiteX()) {
      cerr << Warning( "LefParser::parse(): No CORE site found in library, unable to check the Cell gauge." ) << endl;
    } else {
      if (parser->getCoreSiteY() != parser->getCellGauge()->getSliceHeight())
        cerr << Warning( "LefParser::parse(): CRL slice height discrepency %s while LEF is %s."
                       , DbU::getValueString(parser->getCellGauge()->getSliceHeight()).c_str()
                       , DbU::getValueString(parser->getCoreSiteY()).c_str() ) << endl;
        
      if (parser->getCoreSiteX() != parser->getCellGauge()->getSliceStep())
        cerr << Warning( "LefParser::parse(): CRL slice step discrepency %s while LEF is %s."
                       , DbU::getValueString(parser->getCellGauge()->getSliceStep()).c_str()
                       , DbU::getValueString(parser->getCoreSiteX()).c_str() ) << endl;
    }
    
    return parser->getLibrary();
  }

}  // Anonymous namespace.

#endif  // HAVE_LEFDEF


namespace CRL {

  using std::cerr;
  using std::endl;
  using std::string;
  using Hurricane::UpdateSession;


  Library* LefImport::load ( string fileName )
  {
    UpdateSession::open ();

    Library* library = NULL;
#if defined(HAVE_LEFDEF)
    library = LefParser::parse ( fileName );
#else
    cerr << "[ERROR] CRL::LefImport::load(): \n"
         << "  Coriolis2 hasn't been compiled with LEF/DEF support. To enable LEF/DEF\n"
         << "  support, you may obtain parser/driver from Si2 (www.si2.org) then recompile."
         << endl;
#endif

    UpdateSession::close ();

    return library;
  }


  void  LefImport::reset ()
  {
#if defined(HAVE_LEFDEF)
    LefParser::reset();
#endif
  }


  void  LefImport::setMergeLibrary ( Library* library )
  {
#if defined(HAVE_LEFDEF)
    LefParser::setMergeLibrary( library );
#endif
  }


  void  LefImport::setGdsForeignLibrary ( Library* library )
  {
#if defined(HAVE_LEFDEF)
    LefParser::setGdsForeignLibrary( library );
#endif
  }


  void  LefImport::setGdsForeignDirectory ( string path )
  {
#if defined(HAVE_LEFDEF)
    LefParser::setGdsForeignDirectory( path );
#endif
  }


  void  LefImport::setPinFilter ( DbU::Unit xThreshold, DbU::Unit yThreshold, uint32_t flags )
  {
#if defined(HAVE_LEFDEF)
    LefParser::setPinFilter( xThreshold, yThreshold, flags );
#endif
  }

  Layer* LefImport::getLayer ( string name )
  {
#if defined(HAVE_LEFDEF)
    return LefParser::getLayer( name );
#else
    return 0;
#endif
  }

  void  LefImport::addLayer ( string name, Layer* layer )
  {
#if defined(HAVE_LEFDEF)
    LefParser::addLayer( name, layer );
#endif
  }

  void  LefImport::clearLayer ( string name )
  {
#if defined(HAVE_LEFDEF)
    LefParser::clearLayer( name );
#endif
  }
}  // CRL namespace.
