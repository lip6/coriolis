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
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Contact.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/Library.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/ToolBox.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/CellGauge.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/LefImport.h"


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


  class LefParser {
    public:
      static       DbU::Unit          fromLefUnits             ( int );
      static       Layer*             getLayer                 ( string );
      static       void               addLayer                 ( string, Layer* );
      static       void               reset                    ();
      static       Library*           parse                    ( string file );
                                      LefParser                ( string file, string libraryName );
                                     ~LefParser                ();
      inline       bool               isVH                     () const;
                   Library*           createLibrary            ();
      inline       string             getLibraryName           () const;
      inline       Library*           getLibrary               ( bool create=false );
      inline       Cell*              getCell                  () const;
      inline       void               setCell                  ( Cell* );
      inline       Net*               getNet                   () const;
      inline       void               setNet                   ( Net* );
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
      inline       RoutingGauge*      getRoutingGauge          () const;
      inline       CellGauge*         getCellGauge             () const;
      inline       void               addPinSegment            ( string name, Segment* );
      inline       void               clearPinSegments         ();
    private:                                               
      static       int                _unitsCbk                ( lefrCallbackType_e, lefiUnits*      , lefiUserData );
      static       int                _layerCbk                ( lefrCallbackType_e, lefiLayer*      , lefiUserData );
      static       int                _siteCbk                 ( lefrCallbackType_e, lefiSite*       , lefiUserData );
      static       int                _obstructionCbk          ( lefrCallbackType_e, lefiObstruction*, lefiUserData );
      static       int                _macroCbk                ( lefrCallbackType_e, lefiMacro*      , lefiUserData );
      static       int                _pinCbk                  ( lefrCallbackType_e, lefiPin*        , lefiUserData );
                   void               _pinPostProcess          ();
    private:                                               
                   string              _file;
                   string              _libraryName;
                   Library*            _library;
                   Cell*               _cell;
                   Net*                _net;
                   string              _busBits;
                   double              _unitsMicrons;
                   map< string, vector<Segment*> >  _pinSegments;
      static       map<string,Layer*>  _layerLut;
                   vector<string>      _errors;
                   int                 _nthMetal;
                   int                 _nthCut;
                   RoutingGauge*       _routingGauge;
                   CellGauge*          _cellGauge;
                   DbU::Unit           _minTerminalWidth;
      static       DbU::Unit           _coreSiteX;
      static       DbU::Unit           _coreSiteY;
  };


  inline       bool              LefParser::isVH                     () const { return _routingGauge->isVH(); }
  inline       DbU::Unit         LefParser::getMinTerminalWidth      () const { return _minTerminalWidth; }
  inline       string            LefParser::getLibraryName           () const { return _libraryName; }
  inline       Library*          LefParser::getLibrary               ( bool create ) { if (not _library and create) createLibrary(); return _library; }
  inline       Cell*             LefParser::getCell                  () const { return _cell; }
  inline       void              LefParser::setCell                  ( Cell* cell ) { _cell=cell; }
  inline       Net*              LefParser::getNet                   () const { return _net; }
  inline       void              LefParser::setNet                   ( Net* net ) { _net=net; }
  inline       DbU::Unit         LefParser::fromUnitsMicrons         ( double d ) const { return DbU::fromPhysical(d,DbU::Micro); }
  inline       double            LefParser::getUnitsMicrons          () const { return _unitsMicrons; }
  inline       void              LefParser::setUnitsMicrons          ( double precision ) { _unitsMicrons=precision; }
  inline       int               LefParser::getNthMetal              () const { return _nthMetal; }
  inline       void              LefParser::incNthMetal              () { ++_nthMetal; }
  inline       int               LefParser::getNthCut                () const { return _nthCut; }
  inline       void              LefParser::incNthCut                () { ++_nthCut; }
  inline       RoutingGauge*     LefParser::getRoutingGauge          () const { return _routingGauge; }
  inline       CellGauge*        LefParser::getCellGauge             () const { return _cellGauge; }
  inline       void              LefParser::setCoreSite              ( DbU::Unit x, DbU::Unit y ) { _coreSiteX=x; _coreSiteY=y; }
  inline       DbU::Unit         LefParser::getCoreSiteX             () { return _coreSiteX; }
  inline       DbU::Unit         LefParser::getCoreSiteY             () { return _coreSiteY; }
  inline       bool              LefParser::hasErrors                () const { return not _errors.empty(); }
  inline const vector<string>&   LefParser::getErrors                () const { return _errors; }
  inline       void              LefParser::pushError                ( const string& error ) { _errors.push_back(error); }
  inline       void              LefParser::clearErrors              () { return _errors.clear(); }
  inline       void              LefParser::addPinSegment            ( string name, Segment* s ) { _pinSegments[name].push_back(s); }
  inline       void              LefParser::clearPinSegments         () { _pinSegments.clear(); }


  map<string,Layer*>  LefParser::_layerLut;
  DbU::Unit           LefParser::_coreSiteX = 0;
  DbU::Unit           LefParser::_coreSiteY = 0;


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


  LefParser::LefParser ( string file, string libraryName )
    : _file            (file)
    , _libraryName     (libraryName)
    , _library         (NULL)
    , _cell            (NULL)
    , _net             (NULL)
    , _busBits         ("()")
    , _unitsMicrons    (0.01)
    , _errors          ()
    , _nthMetal        (0)
    , _nthCut          (0)
    , _routingGauge    (NULL)
    , _cellGauge       (NULL)
    , _minTerminalWidth(DbU::fromPhysical(Cfg::getParamDouble("lefImport.minTerminalWidth",0.0)->asDouble(),DbU::UnitPower::Micro))
  {
    _routingGauge = AllianceFramework::get()->getRoutingGauge();
    _cellGauge    = AllianceFramework::get()->getCellGauge();
    
    lefrInit();
    lefrSetUnitsCbk      ( _unitsCbk       );
    lefrSetLayerCbk      ( _layerCbk       );
    lefrSetSiteCbk       ( _siteCbk        );
    lefrSetObstructionCbk( _obstructionCbk );
    lefrSetMacroCbk      ( _macroCbk       );
    lefrSetPinCbk        ( _pinCbk         );
  }


  LefParser::~LefParser ()
  {
    lefrReset();
  }


  Library* LefParser::createLibrary ()
  {
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
      if (layer) {
        parser->addLayer( lefLayer->name(), layer );
        parser->incNthCut();

        cerr << "     - \"" << lefLayer->name() << "\" map to \"" << layer->getName() << "\"" << endl;
      }
    }

    if (lefType == "ROUTING") {
      Layer* layer = techno->getNthMetal( parser->getNthMetal() );
      if (layer) {
        BasicLayer* basicLayer = layer->getBasicLayers().getFirst();
        parser->addLayer( lefLayer->name(), basicLayer );

        cerr << "     - \"" << lefLayer->name() << "\" map to \"" << basicLayer->getName() << "\"" << endl;

        RoutingLayerGauge* gauge = parser->getRoutingGauge()->getLayerGauge( parser->getNthMetal() );

        if (gauge) {
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
    LefParser* parser = (LefParser*)ud;

    if (site->hasClass()) {
      string siteClass = site->siteClass();
      boost::to_upper( siteClass );

      if (siteClass == "CORE") {
        CellGauge* gauge = parser->getCellGauge();

        DbU::Unit lefSliceStep   = DbU::fromPhysical( site->sizeX(), DbU::Micro );
        DbU::Unit lefSliceHeight = DbU::fromPhysical( site->sizeY(), DbU::Micro );
        DbU::Unit crlSliceStep   = gauge->getSliceStep  ();
        DbU::Unit crlSliceHeight = gauge->getSliceHeight();

        if (not parser->getCoreSiteX()
           or ((parser->getCoreSiteX() != crlSliceStep) and (parser->getCoreSiteY() != crlSliceHeight)) ) {
          parser->setCoreSite( lefSliceStep, lefSliceHeight );

          if ( (crlSliceStep == lefSliceStep) and (crlSliceHeight == lefSliceHeight) )
            cerr << "     - Site \"" << site->name() << "\" of class CORE match the Coriolis Cell gauge." << endl;
        }
      }
    }

    return 0;
  }

  
  int  LefParser::_obstructionCbk ( lefrCallbackType_e c, lefiObstruction* obstruction, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

    const Layer* layer       = NULL;
          Cell*  cell        = parser->getCell();
          Net*   blockageNet = cell->getNet( "blockage" );

    if (not blockageNet) {
      blockageNet = Net::create( cell, "blockage" );
      blockageNet->setType( Net::Type::BLOCKAGE );
    }

  //cerr << "       @ _obstructionCbk: " << blockageNet->getName() << endl;
      
    lefiGeometries* geoms = obstruction->geometries();
    for ( int igeom=0 ; igeom < geoms->numItems() ; ++ igeom ) {
      if (geoms->itemType(igeom) == lefiGeomLayerE) {
        layer = parser->getLayer( geoms->getLayer(igeom) )->getBlockageLayer();
      }
      if (not layer) continue;
      
      if (geoms->itemType(igeom) == lefiGeomRectE) {
        lefiGeomRect* r         = geoms->getRect(igeom);
        double        w         = r->xh - r->xl;
        double        h         = r->yh - r->yl;
        Segment*      segment   = NULL;
        if (w >= h) {
          segment = Horizontal::create( blockageNet, layer
                                      , parser->fromUnitsMicrons( (r->yl + r->yh)/2 )
                                      , parser->fromUnitsMicrons( h  )
                                      , parser->fromUnitsMicrons( r->xl )
                                      , parser->fromUnitsMicrons( r->xh )
                                      );
        } else {
          segment = Vertical::create( blockageNet, layer
                                    , parser->fromUnitsMicrons( (r->xl + r->xh)/2 )
                                    , parser->fromUnitsMicrons( w  )
                                    , parser->fromUnitsMicrons( r->yl )
                                    , parser->fromUnitsMicrons( r->yh )
                                    );
        }
      //cerr << "       | " << segment << endl;
      }
    }

    return 0;
  }

  
  int  LefParser::_macroCbk ( lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

    string     cellName = macro->name();
    DbU::Unit  width    = 0;
    DbU::Unit  height   = 0;
    Cell*      cell     = parser->getCell();

    if (cell) {
      cell->setName( cellName );
    } else {
      cell = Cell::create( parser->getLibrary(true), cellName );
      parser->setCell( cell );
    }

    if (macro->hasSize()) {
      width  = parser->fromUnitsMicrons( macro->sizeX() );
      height = parser->fromUnitsMicrons( macro->sizeY() );
      cell->setAbutmentBox( Box( 0, 0, width, height ) );
    }

    parser->_pinPostProcess();
    parser->clearPinSegments();

    cerr << "     - " << cellName
         << " " << DbU::getValueString(width) << " " << DbU::getValueString(height) << "\n" << endl; 
    parser->setCell( NULL );

    return 0;
  }

  
  int  LefParser::_pinCbk ( lefrCallbackType_e c, lefiPin* pin, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

  //cerr << "       @ _pinCbk: " << pin->name() << endl;

    if (not parser->getCell()) parser->setCell( Cell::create( parser->getLibrary(true), "LefImportTmpCell" ) );

    Net* net = Net::create( parser->getCell(), pin->name() );
    net->setExternal( true );

    if (pin->hasDirection()) {
      string lefDir = pin->direction();
      boost::to_upper( lefDir );
      
      if (lefDir == "INPUT"          ) net->setDirection( Net::Direction::IN       );
      if (lefDir == "OUTPUT"         ) net->setDirection( Net::Direction::OUT      );
      if (lefDir == "OUTPUT TRISTATE") net->setDirection( Net::Direction::TRISTATE );
      if (lefDir == "INOUT"          ) net->setDirection( Net::Direction::INOUT    );
    }

    if (pin->hasUse()) {
      string lefUse = pin->use();
      boost::to_upper( lefUse );
      
      if (lefUse == "SIGNAL") net->setType( Net::Type::LOGICAL );
    //if (lefUse == "ANALOG") net->setType( Net::Type::ANALOG  );
      if (lefUse == "CLOCK" ) net->setType( Net::Type::CLOCK   );
      if (lefUse == "POWER" ) net->setType( Net::Type::POWER   );
      if (lefUse == "GROUND") net->setType( Net::Type::GROUND  );
    }

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
          DbU::Unit     w          = parser->fromUnitsMicrons(r->xh - r->xl);
          DbU::Unit     h          = parser->fromUnitsMicrons(r->yh - r->yl);
          Segment*      segment    = NULL;
          float         formFactor = (float)w / (float)h;
          
          if ( (formFactor > 0.5) and not parser->isVH() ) {
            segment = Horizontal::create( net, layer
                                        , parser->fromUnitsMicrons( (r->yl + r->yh)/2 )
                                        , h
                                        , parser->fromUnitsMicrons( r->xl )
                                        , parser->fromUnitsMicrons( r->xh )
                                        );
          } else {
            segment = Vertical::create( net, layer
                                      , parser->fromUnitsMicrons( (r->xl + r->xh)/2 )
                                      , w
                                      , parser->fromUnitsMicrons( r->yl )
                                      , parser->fromUnitsMicrons( r->yh )
                                      );
          }
          if (segment) parser->addPinSegment( pin->name(), segment );
        //cerr << "       | " << segment << endl;
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

    return 0;
  }


  void  LefParser::_pinPostProcess ()
  {
    const Layer*              metal1      = _routingGauge->getLayerGauge( (size_t)0 )->getLayer();
    const RoutingLayerGauge*  gaugeMetal2 = _routingGauge->getLayerGauge( 1 );
          Box                 ab          = _cell->getAbutmentBox();

  //cerr << "       @ _pinPostProcess" << endl;

    for ( auto element : _pinSegments ) {
      string            pinName  = element.first;
      vector<Segment*>& segments = element.second;
      vector<Segment*>  ongrids;

      for ( Segment* segment : segments ) {
        bool isWide = (segment->getWidth() >= getMinTerminalWidth());

      //cerr << "       > " << segment << endl;
        
        if (not segment->getNet()->isSupply()) {
          if (isVH() and (segment->getLayer()->getMask() == metal1->getMask())) {
            Vertical* v = dynamic_cast<Vertical*>( segment );
            if (v) {
              DbU::Unit nearestX = gaugeMetal2->getTrackPosition( ab.getXMin()
                                                                , ab.getXMax()
                                                                , v->getX()
                                                                , Constant::Nearest );

              if (nearestX == v->getX()) {
              } else {
                DbU::Unit neighbor = nearestX
                                   + ((nearestX > v->getX()) ? 1 : -1) * gaugeMetal2->getPitch();

              //cerr << "       | X:" << DbU::getValueString(v->getX())
              //     <<  " nearestX:" << DbU::getValueString(nearestX)
              //     <<  " neighbor:" << DbU::getValueString(neighbor)
              //     << endl;

                if (  (v->getX() - v->getHalfWidth() > neighbor)
                   or (v->getX() + v->getHalfWidth() < neighbor) ) {
                  ongrids.push_back( Vertical::create( v->getNet()
                                                     , v->getLayer()
                                                     , nearestX
                                                     , _routingGauge->getLayerGauge((size_t)0)->getWireWidth()
                                                     , v->getDySource()
                                                     , v->getDyTarget()
                                                     )
                                   );
                //cerr << "       | " << ongrids[ongrids.size()-1] << endl;
                } else {
                // Unpitched and not wide enough to be under a metal2 track, ignore.
                }

                continue;
              }
            }
          }
        }
      
        if (isWide) ongrids.push_back( segment );
      }

      if (ongrids.empty()) {
        cerr << Warning( "LefParser::_pinPostProcess(): Pin \"%s\" has no terminal ongrid."
                       , pinName.c_str() ) << endl;
      } else {
        for ( Segment* segment : ongrids ) {
          NetExternalComponents::setExternal( segment );
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

    if (not parser->getCoreSiteX()) {
      cerr << Warning( "LefParser::parse(): No CORE site found in library, unable to check the Cell gauge." ) << endl;
    } else {
      if (parser->getCoreSiteY() != parser->getCellGauge()->getSliceHeight())
        cerr << Warning( "LefParser::parse(): CRL slice height discrepency %s while LEF is %s."
                       , DbU::getValueString(parser->getCellGauge()->getSliceHeight()).c_str()
                       , DbU::getValueString(parser->getCoreSiteX()).c_str() ) << endl;
        
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
  { LefParser::reset(); }


}  // End of CRL namespace.
