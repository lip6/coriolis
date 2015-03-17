// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) LIP6 2010-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        C a d e n c e   D E F   E x p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./crlcore/LefExport.cpp"                  |
// +-----------------------------------------------------------------+


#include  <memory>
#ifdef HAVE_LEFDEF
#include  "lefwWriter.hpp"
#include  "lefwWriterCalls.hpp"
#endif
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/RegularLayer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Library.h"
#include  "hurricane/UpdateSession.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/ToolBox.h"
#include  "crlcore/RoutingLayerGauge.h"
#include  "crlcore/RoutingGauge.h"
#include  "crlcore/CellGauge.h"
#include  "crlcore/Catalog.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/LefExport.h"


#ifdef HAVE_LEFDEF

namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;


  string  toLower ( const string& s )
  {
    string lowered;

    for ( size_t i=0 ; i<s.size() ; ++i ) {
      if ( (s[i] < 'A') or (s[i] > 'Z') )
        lowered.push_back( s[i] );
      else
        lowered.push_back( s[i] + (int)'a'-(int)'A' );
    }
    return lowered;
  }


#define  CHECK_STATUS(status)         if ((status) != 0) return checkStatus(status);
#define  RETURN_CHECK_STATUS(status)  return checkStatus(status);


  class LefDriver {
    public:
      static void               drive                 ( const set<Cell*>&
                                                      , const string& libraryName
                                                      , unsigned int  flags
                                                      );
      static int                getUnits              ();
      static double             toLefUnits            ( DbU::Unit );
      static DbU::Unit          getSliceHeight        ();
      static DbU::Unit          getPitchWidth         ();
                               ~LefDriver             ();
             int                write                 ();
    private:                                          
                                LefDriver             ( const set<Cell*>&
                                                      , const string& libraryName
                                                      , unsigned int  flags
                                                      , FILE*
                                                      );
      inline unsigned int       getFlags              () const;
      inline const set<Cell*>   getCells              () const;
      inline const string&      getLibraryName        () const;
      inline AllianceFramework* getFramework          ();
      inline int                getStatus             () const;
             int                checkStatus           ( int status );
    private:                                          
      static int                _versionCbk           ( lefwCallbackType_e, lefiUserData );
      static int                _busBitCharsCbk       ( lefwCallbackType_e, lefiUserData );
      static int                _clearanceMeasureCbk  ( lefwCallbackType_e, lefiUserData );
      static int                _dividerCharCbk       ( lefwCallbackType_e, lefiUserData );
      static int                _unitsCbk             ( lefwCallbackType_e, lefiUserData );
      static int                _extCbk               ( lefwCallbackType_e, lefiUserData );
      static int                _propDefCbk           ( lefwCallbackType_e, lefiUserData );
      static int                _endLibCbk            ( lefwCallbackType_e, lefiUserData );
      static int                _layerCbk             ( lefwCallbackType_e, lefiUserData );
      static int                _macroCbk             ( lefwCallbackType_e, lefiUserData );
      static int                _manufacturingGridCbk ( lefwCallbackType_e, lefiUserData );
      static int                _nonDefaultCbk        ( lefwCallbackType_e, lefiUserData );
      static int                _siteCbk              ( lefwCallbackType_e, lefiUserData );
      static int                _spacingCbk           ( lefwCallbackType_e, lefiUserData );
      static int                _useMinSpacingCbk     ( lefwCallbackType_e, lefiUserData );
      static int                _viaCbk               ( lefwCallbackType_e, lefiUserData );
      static int                _viaRuleCbk           ( lefwCallbackType_e, lefiUserData );
             int                _driveRoutingLayer    ( RoutingLayerGauge* );
             int                _driveCutLayer        ( Layer* );
             int                _driveMacro           ( Cell* );
    private:
      static AllianceFramework* _framework;
      static int                _units;
      static DbU::Unit          _sliceHeight;
      static DbU::Unit          _pitchWidth;
             unsigned int       _flags;
             const set<Cell*>   _cells;
             string             _libraryName;
             FILE*              _lefStream;
             int                _status;
  };


  int                LefDriver::_units       = 100;
  AllianceFramework* LefDriver::_framework   = NULL;
  DbU::Unit          LefDriver::_sliceHeight = 0;
  DbU::Unit          LefDriver::_pitchWidth  = 0;


         int                LefDriver::getUnits       () { return _units; }
         double             LefDriver::toLefUnits     ( DbU::Unit u ) { return DbU::getLambda(u)/**getUnits()*/; }
         DbU::Unit          LefDriver::getSliceHeight () { return _sliceHeight; }
         DbU::Unit          LefDriver::getPitchWidth  () { return _pitchWidth; }; 
  inline AllianceFramework* LefDriver::getFramework   () { return _framework; }
  inline unsigned int       LefDriver::getFlags       () const { return _flags; }
  inline const set<Cell*>   LefDriver::getCells       () const { return _cells; }
  inline int                LefDriver::getStatus      () const { return _status; }
  inline const string&      LefDriver::getLibraryName () const { return _libraryName; }


  LefDriver::LefDriver ( const set<Cell*>& cells, const string& libraryName, unsigned int flags, FILE* lefStream )
    : _flags      (flags)
    , _cells      (cells)
    , _libraryName(libraryName)
    , _lefStream  (lefStream)
    , _status     (0)
  {
    if ( _framework == NULL ) {
      _framework = AllianceFramework::get ();
      CellGauge* cg = _framework->getCellGauge("sxlib");

      _sliceHeight = cg->getSliceHeight ();
      _pitchWidth  = cg->getPitch       ();
    }

    _status = lefwInitCbk ( _lefStream );
    if ( _status != 0 ) return;

    lefwSetVersionCbk           ( _versionCbk           );
    lefwSetBusBitCharsCbk       ( _busBitCharsCbk       );
    lefwSetDividerCharCbk       ( _dividerCharCbk       );
    lefwSetSiteCbk              ( _siteCbk              );
    lefwSetUnitsCbk             ( _unitsCbk             );
    lefwSetManufacturingGridCbk ( _manufacturingGridCbk );
    lefwSetClearanceMeasureCbk  ( _clearanceMeasureCbk  );
    lefwSetExtCbk               ( _extCbk               );
    lefwSetLayerCbk             ( _layerCbk             );
    lefwSetMacroCbk             ( _macroCbk             );
    lefwSetPropDefCbk           ( _propDefCbk           );
    lefwSetSpacingCbk           ( _spacingCbk           );
    lefwSetUseMinSpacingCbk     ( _useMinSpacingCbk     );
    lefwSetNonDefaultCbk        ( _nonDefaultCbk        );
    lefwSetViaCbk               ( _viaCbk               );
    lefwSetViaRuleCbk           ( _viaRuleCbk           );
    lefwSetEndLibCbk            ( _endLibCbk            );
  }


  LefDriver::~LefDriver ()
  { }


  int  LefDriver::write ()
  {
    return checkStatus ( lefwWrite(_lefStream,_libraryName.c_str(),(void*)this) );
  }


  int  LefDriver::checkStatus ( int status )
  {
    if ( (_status=status) != 0 ) {
      lefwPrintError ( _status );
      cerr << Error("LefDriver::drive(): Error occured while driving <%s>.",_libraryName.c_str()) << endl;
    }
    return _status;
  }


  int  LefDriver::_driveRoutingLayer ( RoutingLayerGauge* lg )
  {
    if ( lg == NULL ) return 0;

    string layerName = getString(lg->getLayer()->getName());

    _status = lefwStartLayerRouting ( layerName.c_str() );
    if ( _status != 0 ) return _status;

    _status = lefwLayerRouting ( (lg->getDirection() == Constant::Horizontal) ? "HORIZONTAL" : "VERTICAL"
                               , toLefUnits(lg->getWireWidth())
                               );
    if ( _status != 0 ) return _status;

    _status = lefwLayerRoutingOffset ( toLefUnits(lg->getOffset()) );
    if ( _status != 0 ) return _status;

    _status = lefwLayerRoutingPitch ( toLefUnits(lg->getPitch()) );
    if ( _status != 0 ) return _status;

    _status = lefwLayerRoutingSpacing ( toLefUnits(lg->getPitch()-lg->getWireWidth()-DbU::lambda(1.0)) );
    if ( _status != 0 ) return _status;
 
    return _status = lefwEndLayerRouting ( layerName.c_str() );
  }


  int  LefDriver::_driveCutLayer ( Layer* layer )
  {
    if ( layer == NULL ) return 0;

    _status = lefwStartLayer ( getString(layer->getName()).c_str(), "CUT" );
    if ( _status != 0 ) return _status;

    _status = lefwLayerWidth ( toLefUnits(DbU::lambda(1.0)) );
    if ( _status != 0 ) return _status;
 
    return _status = lefwEndLayer ( getString(layer->getName()).c_str() );
  }


  int  LefDriver::_driveMacro ( Cell* cell )
  {
    _status = lefwStartMacro ( getString(cell->getName()).c_str() );
    CHECK_STATUS(_status);

    Box         abutmentBox   ( cell->getAbutmentBox() );
    double      pitchWidth    = toLefUnits ( LefDriver::getPitchWidth () );
    double      sliceHeight   = toLefUnits ( LefDriver::getSliceHeight() );
    int         slices        = (int)floor( abutmentBox.getHeight() / LefDriver::getSliceHeight() );
    int         pitchs        = (int)floor( abutmentBox.getWidth () / LefDriver::getPitchWidth () );
    const char* macroClass    = NULL;
    const char* macroSubClass = NULL;
   
    if ( slices > 1 ) {
      macroClass    = "BLOCK";
      macroSubClass = "BLACKBOX";
    } else {
      macroClass = "CORE";
      if ( CatalogExtension::isFeed(cell) ) macroSubClass = "SPACER";
    }

    _status = lefwMacroClass ( macroClass, macroSubClass );
    CHECK_STATUS(_status);

    double originX = toLefUnits ( abutmentBox.getXMin() );
    double originY = toLefUnits ( abutmentBox.getYMin() );
    if ( (originX != 0.0) or (originY != 0.0) )
      cerr << Warning("Cell <%s> origin is in (%s,%s), shifting to (0,0)."
                     ,getString(cell->getName()).c_str()
                     ,DbU::getValueString(abutmentBox.getXMin()).c_str()
                     ,DbU::getValueString(abutmentBox.getYMin()).c_str()
                     ) << endl;
    _status = lefwMacroOrigin ( 0.0, 0.0 );
    CHECK_STATUS(_status);

    double sizeX = toLefUnits ( abutmentBox.getWidth () );
    double sizeY = toLefUnits ( abutmentBox.getHeight() );
    _status = lefwMacroSize ( sizeX, sizeY );
    CHECK_STATUS(_status);

    _status = lefwMacroSymmetry ( "X Y" );
    CHECK_STATUS(_status);

    _status = lefwMacroSite ( "core" );
    CHECK_STATUS(_status);

    if ( slices > 1 ) {
      for ( int islice=0 ; islice<slices ; ++islice ) {
        _status = lefwMacroSitePatternStr ( "core"                        // site name.
                                          , originX                       // X origin.
                                          , originY + sliceHeight*islice  // Y origin.
                                          , (islice % 2) ? "FS" : "N"     // orientation.
                                          , pitchs                        // num X.
                                          , 1                             // num Y.
                                          , pitchWidth                    // space X (STEP X).
                                          , sliceHeight                   // space Y (STEP Y).
                                          );
        CHECK_STATUS(_status);
      }
    }

    Net* blockageNet = NULL;

    forEach ( Net*, inet, cell->getNets() ) {
      Net* net = *inet;
      if ( (blockageNet == NULL) and _framework->isBLOCKAGE(net->getName()) )
        blockageNet = net;

      if ( not net->isExternal() ) continue;

      _status = lefwStartMacroPin ( getString(net->getName()).c_str() );
      CHECK_STATUS(_status);

      _status = lefwMacroPinDirection ( "INPUT" );
      CHECK_STATUS(_status);

      if ( net->isSupply() ) {
        _status = lefwMacroPinShape ( "ABUTMENT" );
        CHECK_STATUS(_status);
      }

      const char* pinUse = "SIGNAL";
      if      ( net->isGround() ) pinUse = "GROUND";
      else if ( net->isPower () ) pinUse = "POWER";
      else if ( net->isClock () ) pinUse = "CLOCK";
      _status = lefwMacroPinUse ( pinUse );
      CHECK_STATUS(_status);

      _status = lefwStartMacroPinPort ( NULL );
      CHECK_STATUS(_status);

      const Layer* layer = NULL;
      forEach ( Component*, icomponent, net->getComponents() ) {
        if ( not NetExternalComponents::isExternal(*icomponent) ) continue;

        if ( layer != (*icomponent)->getLayer() ) {
          layer = (*icomponent)->getLayer();
          _status = lefwMacroPinPortLayer ( getString(layer->getName()).c_str(), 0 );
          CHECK_STATUS(_status);
        }

        Box bb ( (*icomponent)->getBoundingBox() );
        _status = lefwMacroPinPortLayerRect ( toLefUnits(bb.getXMin())-originX  // xl
                                            , toLefUnits(bb.getYMin())-originY  // yl
                                            , toLefUnits(bb.getXMax())-originX  // xh
                                            , toLefUnits(bb.getYMax())-originY  // yh
                                            , 0                                 // ITERATE numX
                                            , 0                                 // ITERATE numY
                                            , 0                                 // ITERATE spaceX
                                            , 0                                 // ITERATE spaceY
                                            );
        CHECK_STATUS(_status);
      }

      _status = lefwEndMacroPinPort ();
      CHECK_STATUS(_status);

      _status = lefwEndMacroPin ( getString(net->getName()).c_str() );
      CHECK_STATUS(_status);
    }

#if 0
    _status = lefwStartMacroObs ();
    CHECK_STATUS(_status);

    double supplyHalfWidth   = toLefUnits ( DbU::lambda(6.0) );
    double sliceHeight       = toLefUnits ( LefDriver::getSliceHeight() );
#endif

    double METAL1HalfMinDist = toLefUnits ( DbU::lambda(1.0) );

#if 0
    _status = lefwMacroObsLayer ( "METAL1", METAL1HalfMinDist );
    CHECK_STATUS(_status);

    _status = lefwMacroObsLayerRect ( METAL1HalfMinDist                                  // xl
                                    , METAL1HalfMinDist + supplyHalfWidth                // yl
                                    , sizeX       - METAL1HalfMinDist                    // xh
                                    , sliceHeight - METAL1HalfMinDist - supplyHalfWidth  // yh
                                    , 1                                                  // ITERATE columns
                                    , slices                                             // ITERATE rows
                                    , sizeX                                              // ITERATE spaceX
                                    , toLefUnits(LefDriver::getSliceHeight())            // ITERATE spaceY
                                    );
    CHECK_STATUS(_status);
#endif

    if ( blockageNet != 0 ) {
      _status = lefwStartMacroObs ();
      CHECK_STATUS(_status);

      const Layer* blockageLayer = NULL;
      forEach ( Component*, icomponent, blockageNet->getComponents() ) {
        if ( dynamic_cast<Segment*>(*icomponent) == NULL ) continue;

        if ( blockageLayer != (*icomponent)->getLayer() ) {
          blockageLayer = (*icomponent)->getLayer();

          RegularLayer* routingLayer = NULL;
          forEach ( RegularLayer*, ilayer, DataBase::getDB()->getTechnology()->getRegularLayers() ) {
            if ( (*ilayer)->getBlockageLayer() == NULL ) continue;
            if ( (*ilayer)->getBlockageLayer()->getMask() != blockageLayer->getMask() ) continue;

            routingLayer = *ilayer;
            break;
          }

          if ( routingLayer != NULL ) {
            _status = lefwMacroObsLayer ( getString(routingLayer->getName()).c_str(), METAL1HalfMinDist );
            CHECK_STATUS(_status);
          }
        }

        Box bb ( (*icomponent)->getBoundingBox() );
        _status = lefwMacroObsLayerRect ( toLefUnits(bb.getXMin())-originX  // xl
                                        , toLefUnits(bb.getYMin())-originY  // yl
                                        , toLefUnits(bb.getXMax())-originX  // xh
                                        , toLefUnits(bb.getYMax())-originY  // yh
                                        , 0                                 // ITERATE numX
                                        , 0                                 // ITERATE numY
                                        , 0                                 // ITERATE spaceX
                                        , 0                                 // ITERATE spaceY
                                        );
        CHECK_STATUS(_status);
      }

      _status = lefwEndMacroObs ();
      CHECK_STATUS(_status);
    }

#if 0
    _status = lefwEndMacroObs ();
    CHECK_STATUS(_status);
#endif

    _status = lefwEndMacro ( getString(cell->getName()).c_str() );
    RETURN_CHECK_STATUS(_status);
  }


  int  LefDriver::_versionCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver* driver = (LefDriver*)udata;

    ostringstream comment;
    comment << "For design <" << driver->getLibraryName() << ">.";

    lefwNewLine ();
    lefwAddComment ( "LEF generated by Coriolis2 DEF exporter." );
    lefwAddComment ( comment.str().c_str() );
    lefwNewLine ();

    return driver->checkStatus ( lefwVersion(5,7) );
  }


  int  LefDriver::_busBitCharsCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver* driver = (LefDriver*)udata;
    lefwNewLine ();
    return driver->checkStatus ( lefwBusBitChars("()") );
  }


  int  LefDriver::_dividerCharCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver* driver = (LefDriver*)udata;
    return driver->checkStatus ( lefwDividerChar(".") );
  }


  int  LefDriver::_unitsCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver* driver = (LefDriver*)udata;
    lefwNewLine ();
 
    int status = lefwStartUnits ();
    if ( status != 0 ) return driver->checkStatus(status);
 
    status = lefwUnits ( 0                      // time.
                       , 0                      // capacitance.
                       , 0                      // resistance.
                       , 0                      // power.
                       , 0                      // current.
                       , 0                      // voltage.
                       , LefDriver::getUnits()  // database.
                       );
    if ( status != 0 ) return driver->checkStatus(status);

    return driver->checkStatus ( lefwEndUnits() );
  }


  int  LefDriver::_layerCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver*  driver     = (LefDriver*)udata;
    Technology* technology = DataBase::getDB()->getTechnology();
    const vector<RoutingLayerGauge*>& rg
      = driver->getFramework()->getRoutingGauge("sxlib")->getLayerGauges();

    int status = 0;
    for ( size_t ilayer=0 ; ilayer<rg.size() ; ++ilayer ) {
      if ( ilayer > 0 ) {
        status = driver->_driveCutLayer ( technology->getCutBelow(rg[ilayer]->getLayer()) );
        if ( status != 0 ) return driver->checkStatus(status);
      }

      status = driver->_driveRoutingLayer ( rg[ilayer] );
      if ( status != 0 ) return driver->checkStatus(status);
    }

    return status;
  }


  int  LefDriver::_siteCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver* driver = (LefDriver*)udata;
    int status = lefwSite ( "core"
                          , "CORE"
                          , "Y"
                          , toLefUnits(LefDriver::getPitchWidth ())
                          , toLefUnits(LefDriver::getSliceHeight())
                          );
    if ( status != 0 ) return driver->checkStatus(status);

    return driver->checkStatus(lefwEndSite("core"));
  }


  int  LefDriver::_extCbk ( lefwCallbackType_e, lefiUserData udata )
  {
  //LefDriver* driver = (LefDriver*)udata;
    return 0;
  }


  int  LefDriver::_propDefCbk ( lefwCallbackType_e, lefiUserData udata )
  {
  //LefDriver* driver = (LefDriver*)udata;
    return 0;
  }


  int  LefDriver::_clearanceMeasureCbk ( lefwCallbackType_e, lefiUserData udata )
  {
  //LefDriver* driver = (LefDriver*)udata;
    return 0;
  }


  int  LefDriver::_endLibCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver* driver = (LefDriver*)udata;
    return driver->checkStatus ( lefwEnd() );
  }


  int  LefDriver::_macroCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver* driver = (LefDriver*)udata;

    const set<Cell*>&          cells = driver->getCells ();
    set<Cell*>::const_iterator icell = cells.begin();
    for ( ; (icell != cells.end()) and (driver->getStatus() == 0) ; ++icell ) {
      driver->_driveMacro ( *icell );
    }

    return driver->getStatus();
  }


  int  LefDriver::_manufacturingGridCbk ( lefwCallbackType_e, lefiUserData udata )
  {
#if 1
  // The driver puts it before UNITS, which seems to displease Cadence Encounter.
  // So, as long as it doesn't prevent Encounter to works, disable it. 
    LefDriver* driver = (LefDriver*)udata;
    return driver->checkStatus ( lefwManufacturingGrid ( LefDriver::getUnits()/10.0 ) );
#else
    return 0;
#endif
  }


  int  LefDriver::_nonDefaultCbk ( lefwCallbackType_e, lefiUserData udata )
  {
  //LefDriver* driver = (LefDriver*)udata;
    return 0;
  }


  int  LefDriver::_spacingCbk ( lefwCallbackType_e, lefiUserData udata )
  {
  //LefDriver* driver = (LefDriver*)udata;
    return 0;
  }


  int  LefDriver::_useMinSpacingCbk ( lefwCallbackType_e, lefiUserData udata )
  {
  //LefDriver* driver = (LefDriver*)udata;
    return 0;
  }


  int  LefDriver::_viaCbk ( lefwCallbackType_e, lefiUserData udata )
  {
    LefDriver*  driver     = (LefDriver*)udata;
    Technology* technology = DataBase::getDB()->getTechnology();
    const vector<RoutingLayerGauge*>& rg
      = driver->getFramework()->getRoutingGauge("sxlib")->getLayerGauges();

    int status = 0;
    for ( size_t ilayer=1 ; ilayer<rg.size() ; ++ilayer ) {
      const Layer* topLayer    = rg[ilayer]->getLayer();
      const Layer* bottomLayer = topLayer->getMetalBelow(); 
      const Layer* cutLayer    = topLayer->getCutBelow(); 
      const Layer* viaLayer    = technology->getViaBetween ( topLayer, bottomLayer );

      status = lefwStartVia ( getString(viaLayer->getName()).c_str(), "DEFAULT" );
      if ( status != 0 ) return driver->checkStatus(status);

    // Bottom Layer.
      status = lefwViaLayer ( getString(bottomLayer->getName()).c_str() );
      if ( status != 0 ) return driver->checkStatus(status);

      double side = toLefUnits ( (ilayer == 1) ? DbU::lambda(1.0) : DbU::lambda(1.5) );
      status = lefwViaLayerRect ( -side
                                , -side
                                ,  side
                                ,  side
                                );
      if ( status != 0 ) return driver->checkStatus(status);

    // Cut Layer.
      status = lefwViaLayer ( getString(cutLayer->getName()).c_str() );
      if ( status != 0 ) return driver->checkStatus(status);

      side   = toLefUnits ( DbU::lambda(0.5) );
      status = lefwViaLayerRect ( -side
                                , -side
                                ,  side
                                ,  side
                                );
      if ( status != 0 ) return driver->checkStatus(status);

    // Top Layer.
      status = lefwViaLayer ( getString(topLayer->getName()).c_str() );
      if ( status != 0 ) return driver->checkStatus(status);

      side   = toLefUnits ( DbU::lambda(1.5) );
      status = lefwViaLayerRect ( -side
                                , -side
                                ,  side
                                ,  side
                                );
      if ( status != 0 ) return driver->checkStatus(status);

      status = lefwEndVia ( getString(viaLayer->getName()).c_str() );
      if ( status != 0 ) return driver->checkStatus(status);
    }

    return 0;
    return 0;
  }


  int  LefDriver::_viaRuleCbk ( lefwCallbackType_e, lefiUserData udata )
  {
  //LefDriver* driver = (LefDriver*)udata;
    return 0;
  }


  void  LefDriver::drive ( const set<Cell*>& cells, const string& libraryName, unsigned int flags )
  {
    FILE* lefStream = NULL;
    try {
      string path = "./" + libraryName + ".lef";
      cmess1 << "  o  Export LEF: <" << path << ">" << endl;

      lefStream = fopen ( path.c_str(), "w" );
      if ( lefStream == NULL )
        throw Error("LefDriver::drive(): Cannot open <%s>.",path.c_str());

      auto_ptr<LefDriver> driver ( new LefDriver(cells,libraryName,flags,lefStream) );
      driver->write ();
    }
    catch ( ... ) {
      if ( lefStream != NULL ) fclose ( lefStream );

      throw;
    }
    fclose ( lefStream );
  }


} // End of anonymous namespace.

#endif // HAVE_LEFDEF.


namespace CRL {

  using std::string;
  using std::cerr;
  using std::endl;
  using Hurricane::Library;
  using Hurricane::Transformation;
  using Hurricane::UpdateSession;


  void  LefExport::drive ( Cell* cell, unsigned int flags )
  {
#if HAVE_LEFDEF
    string     libraryName = "symbolic";
    set<Cell*> cells;

    if ( cell != NULL ) {
      libraryName = getString(cell->getName()) + "export";

      forEach ( Instance*, iinstance, cell->getInstances() ) {
        cells.insert ( (*iinstance)->getMasterCell() );
      }
    }

    if ( flags & WithSpacers ) {
    // Ugly. Direct uses of Alliance Framework.
      Cell* spacer = AllianceFramework::get()->getCell("tie_x0",Catalog::State::Views);
      if ( spacer != NULL ) cells.insert ( spacer );

      spacer = AllianceFramework::get()->getCell("rowend_x0",Catalog::State::Views);
      if ( spacer != NULL ) cells.insert ( spacer );
    }

    LefDriver::drive ( cells, libraryName, flags );
#else
    cerr << "[ERROR] CRL::LefExport::drive(Cell*): \n"
         << "  Coriolis2 hasn't been compiled with LEF/DEF support. To enable LEF/DEF"
         << "  support, you may contact Si2 (www.si2.org) then recompile."
         << endl;
#endif
  }


  void  LefExport::drive ( Library* library, unsigned int flags )
  {
#ifdef HAVE_LEFDEF
    string     libraryName = "symbolic";
    set<Cell*> cells;

    if ( library != NULL ) {
      libraryName = getString(library->getName());

      forEach ( Cell*, icell, library->getCells() ) {
        if ( cells.find(*icell) == cells.end())
          cells.insert ( *icell );
      }
    }

    LefDriver::drive ( cells, libraryName, flags );
#else
    cerr << "[ERROR] CRL::LefExport::drive(Library*): \n"
         << "  Coriolis2 hasn't been compiled with LEF/DEF support. To enable LEF/DEF\n"
         << "  support, you may obtain parser/driver from Si2 (www.si2.org) then recompile."
         << endl;
#endif
  }


}  // End of CRL namespace.
