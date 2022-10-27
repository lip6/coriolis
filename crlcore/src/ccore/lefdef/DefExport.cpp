// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2010-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        C a d e n c e   D E F   E x p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./crlcore/DefExport.cpp"                  |
// +-----------------------------------------------------------------+


#include  <memory>
#if defined(HAVE_LEFDEF)
#  include  "lefwWriter.hpp"
#  include  "defwWriter.hpp"
#  include  "defwWriterCalls.hpp"
#endif
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Library.h"
#include  "hurricane/UpdateSession.h"
#include  "hurricane/ViaLayer.h"
#include  "hurricane/Rectilinear.h"

#include  "crlcore/Utilities.h"
#include  "crlcore/ToolBox.h"
#include  "crlcore/RoutingGauge.h"
#include  "crlcore/RoutingLayerGauge.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/CellGauge.h"
#include  "crlcore/LefExport.h"
#include  "crlcore/DefExport.h"


#if defined(HAVE_LEFDEF)

namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;


  string toDefName ( string name )
  {
    if (name.empty()) return name;

    if (name[0] == '<')             name.erase ( 0, 1 );
    if (name[name.size()-1] == '>') name.erase ( name.size()-1 );
    for ( size_t i=0 ; i<name.size() ; ++i ) {
      switch ( name[i] ) {
        case ':':
        case '.': name[i] = '_'; break;
      }
    }
    return name;
  }


  string  extractInstanceName ( const RoutingPad* rp )
  {
    ostringstream name;

    Occurrence occurrence = rp->getOccurrence();

    name << getString(occurrence.getOwnerCell()->getName()) << '.';

    if (not rp->getOccurrence().getPath().getHeadPath().isEmpty())
      name << getString(rp->getOccurrence().getPath().getHeadPath().getName()) << ".";

    name << "I." << getString(rp->getOccurrence().getPath().getTailInstance()->getName());

    return toDefName(name.str());
  }


#define  CHECK_STATUS_CBK(status,info)         if ((status) != 0) return driver->checkStatus(status,info);
#define  CHECK_STATUS_DRV(status,info)         if ((status) != 0) return checkStatus(status,info);
#define  RETURN_CHECK_STATUS_CBK(status,info)  return driver->checkStatus(status,info);
#define  RETURN_CHECK_STATUS_DRV(status,info)  return checkStatus(status,info);


  class DefDriver {
    public:
      static void          drive            ( Cell* cell, uint32_t flags );
      static int           getUnits         ();
      static int           toDefUnits       ( DbU::Unit );
      static int           toDefOrient      ( Transformation::Orientation );
      static void          toDefCoordinates ( Instance*, Transformation, int& statusX, int& statusY, int& statusOrient );
      static DbU::Unit     getSliceHeight   ();
      static DbU::Unit     getPitchWidth    ();
                          ~DefDriver        ();
             int           write            ();
    private:                                
                           DefDriver        ( Cell*, const string& designName, FILE*, uint32_t flags );
      inline Cell*         getCell          ();
      inline const string& getDesignName    () const;
      inline uint32_t      getFlags         () const;
      inline int           getStatus        () const;
             int           checkStatus      ( int status, string info );
      static int           writeRouting     ( Net*, bool special );
    private:               
      static int           _designCbk       ( defwCallbackType_e, defiUserData );
      static int           _designEndCbk    ( defwCallbackType_e, defiUserData );
      static int           _historyCbk      ( defwCallbackType_e, defiUserData );
      static int           _versionCbk      ( defwCallbackType_e, defiUserData );
      static int           _dividerCbk      ( defwCallbackType_e, defiUserData );
      static int           _busBitCbk       ( defwCallbackType_e, defiUserData );
      static int           _unitsCbk        ( defwCallbackType_e, defiUserData );
      static int           _technologyCbk   ( defwCallbackType_e, defiUserData );
      static int           _dieAreaCbk      ( defwCallbackType_e, defiUserData );
      static int           _gcellGridCbk    ( defwCallbackType_e, defiUserData );
      static int           _rowCbk          ( defwCallbackType_e, defiUserData );
      static int           _trackCbk        ( defwCallbackType_e, defiUserData );
      static int           _viaCbk          ( defwCallbackType_e, defiUserData );
      static int           _pinCbk          ( defwCallbackType_e, defiUserData );
      static int           _pinPropCbk      ( defwCallbackType_e, defiUserData );
      static int           _componentCbk    ( defwCallbackType_e, defiUserData );
      static int           _netCbk          ( defwCallbackType_e, defiUserData );
      static int           _snetCbk         ( defwCallbackType_e, defiUserData );
      static int           _extensionCbk    ( defwCallbackType_e, defiUserData );
      static int           _groupCbk        ( defwCallbackType_e, defiUserData );
      static int           _propDefCbk      ( defwCallbackType_e, defiUserData );
      static int           _regionCbk       ( defwCallbackType_e, defiUserData );
      static int           _scanchainCbk    ( defwCallbackType_e, defiUserData );
    private:
      static int           _units;
      static DbU::Unit     _sliceHeight;
      static DbU::Unit     _pitchWidth;
             Cell*         _cell;
             string        _designName;
             FILE*         _defStream;
             uint32_t      _flags;
             int           _status;
  };


  int        DefDriver::_units       = 1000;
  DbU::Unit  DefDriver::_sliceHeight = 0;
  DbU::Unit  DefDriver::_pitchWidth  = 0;


         int           DefDriver::getUnits       () { return _units; }
         int           DefDriver::toDefUnits     ( DbU::Unit u ) { return (int)(round(DbU::toMicrons(u)*_units)); }
         DbU::Unit     DefDriver::getSliceHeight () { return _sliceHeight; }
         DbU::Unit     DefDriver::getPitchWidth  () { return _pitchWidth; }; 
  inline Cell*         DefDriver::getCell        () { return _cell; }
  inline uint32_t      DefDriver::getFlags       () const { return _flags; }
  inline int           DefDriver::getStatus      () const { return _status; }
  inline const string& DefDriver::getDesignName  () const { return _designName; }


  int  DefDriver::toDefOrient ( Transformation::Orientation orient )
  {
    switch ( orient ) {
      case Transformation::Orientation::ID: return 0; // N.
      case Transformation::Orientation::R1: return 1; // W.
      case Transformation::Orientation::R2: return 2; // S.
      case Transformation::Orientation::R3: return 3; // E.
      case Transformation::Orientation::MX: return 4; // FN.
      case Transformation::Orientation::XR: return 5; // FE.
      case Transformation::Orientation::MY: return 6; // FS.
      case Transformation::Orientation::YR: return 7; // FW.
    }

    return 0; // N
  }


  void  DefDriver::toDefCoordinates ( Instance* instance, Transformation transf, int& statusX, int& statusY, int& statusOrient )
  {
    Transformation inst_transf = instance->getTransformation();
    transf.applyOn( inst_transf );
    statusX      = toDefUnits ( inst_transf.getTx() );
    statusY      = toDefUnits ( inst_transf.getTy() );
    statusOrient = toDefOrient( inst_transf.getOrientation() );

    switch ( inst_transf.getOrientation() ) {
      case Transformation::Orientation::ID: break;
      case Transformation::Orientation::R1: break;
      case Transformation::Orientation::R2:
        statusX -= toDefUnits( instance->getMasterCell()->getAbutmentBox().getWidth() );
        statusY -= toDefUnits( instance->getMasterCell()->getAbutmentBox().getHeight() );
        break;
      case Transformation::Orientation::R3: break;
      case Transformation::Orientation::MX:
        statusX -= toDefUnits( instance->getMasterCell()->getAbutmentBox().getWidth() );
        break;
      case Transformation::Orientation::XR:
        break;
      case Transformation::Orientation::MY:
        statusY -= toDefUnits( instance->getMasterCell()->getAbutmentBox().getHeight() );
        break;
      case Transformation::Orientation::YR:
        break;
    }

  }


  DefDriver::DefDriver ( Cell* cell, const string& designName, FILE* defStream, uint32_t flags )
    : _cell      (cell)
    , _designName(designName)
    , _defStream (defStream)
    , _flags     (flags)
    , _status    (0)
  {
    AllianceFramework* framework = AllianceFramework::get ();
    CellGauge*         cg        = framework->getCellGauge();

    _sliceHeight = cg->getSliceHeight ();
    _pitchWidth  = cg->getPitch       ();
  //_units       = DbU::toGrid( DbU::fromMicrons(1.0) );
    _units       = 1000;

    _status = defwInitCbk ( _defStream );
    if ( _status != 0 ) return;

    defwSetDesignCbk     ( _designCbk     );
    defwSetDesignEndCbk  ( _designEndCbk  );
    defwSetHistoryCbk    ( _historyCbk    );
    defwSetVersionCbk    ( _versionCbk    );
    defwSetDividerCbk    ( _dividerCbk    );
    defwSetBusBitCbk     ( _busBitCbk     );
    defwSetUnitsCbk      ( _unitsCbk      );
    defwSetTechnologyCbk ( _technologyCbk );
    defwSetDieAreaCbk    ( _dieAreaCbk    );
    defwSetGcellGridCbk  ( _gcellGridCbk  );
    defwSetRowCbk        ( _rowCbk        );
    defwSetTrackCbk      ( _trackCbk      );
    defwSetViaCbk        ( _viaCbk        );
    defwSetPinCbk        ( _pinCbk        );
    defwSetPinPropCbk    ( _pinPropCbk    );
    defwSetComponentCbk  ( _componentCbk  );
    defwSetNetCbk        ( _netCbk        );
    defwSetSNetCbk       ( _snetCbk       );
    defwSetExtCbk        ( _extensionCbk  );
    defwSetGroupCbk      ( _groupCbk      );
    defwSetPropDefCbk    ( _propDefCbk    );
    defwSetRegionCbk     ( _regionCbk     );
    defwSetScanchainCbk  ( _scanchainCbk  );
  }


  DefDriver::~DefDriver ()
  { }


  int  DefDriver::write ()
  {
    _cell->flattenNets( Cell::Flags::NoFlags );
    return checkStatus( defwWrite(_defStream,_designName.c_str(), (void*)this )
                      , "write(): Problem while writing DEF." );
  }


  int  DefDriver::checkStatus ( int status, string info )
  {
    if ((_status=status) != 0) {
      defwPrintError( _status );
      ostringstream message;
      message << "DefDriver::checkStatus(): Error occured while driving \"" << _designName << "\".\n";
      message << "        " << info;
      cerr << Error(message.str()) << endl;
    }
    return _status;
  }


  int  DefDriver::_versionCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;

    defwNewLine ();
    defwAddComment ( "DEF generated by Coriolis2 DEF exporter." );
    defwNewLine ();

    return driver->checkStatus( defwVersion(5,7), "_versionCnk(): Failed to write VERSION" );
  }


  int  DefDriver::_designCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    defwNewLine ();
    return driver->checkStatus( defwDesignName(driver->getDesignName().c_str())
                              , "_designCbk(): Failed to write DESIGN" );
  }


  int  DefDriver::_designEndCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    return driver->checkStatus( defwEnd(), "_designEndCbk(): Failed to END design" );
  }


  int  DefDriver::_historyCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  int  DefDriver::_dividerCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    defwNewLine ();
    return driver->checkStatus( defwDividerChar("."), "_dividerCbk(): Failed to drive DIVIDER" );
  }


  int  DefDriver::_busBitCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    return driver->checkStatus( defwBusBitChars("()")
                              , "_busBitCbk(): Failed to drive BUSBITCHAR" );
  }


  int  DefDriver::_unitsCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    defwNewLine ();
    ostringstream info;
    info << "_unitsCnk(): Failed to drive UNITS (" << DefDriver::getUnits() << ")";
    return driver->checkStatus( defwUnits(DefDriver::getUnits()), info.str() );
  }


  int  DefDriver::_technologyCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    return driver->checkStatus( defwTechnology( getString(driver->getCell()->getLibrary()->getName()).c_str() )
                              , "_technologycbk(): Failed to drive TECHNOLOGY" );
  }


  int  DefDriver::_dieAreaCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;

    Box abutmentBox ( driver->getCell()->getAbutmentBox() );
    if ( driver->getFlags() & DefExport::ExpandDieArea ) {
      abutmentBox.inflate ( DefDriver::getPitchWidth() );
    }

    if ( not abutmentBox.isEmpty()) {
      defwNewLine ();
      driver->checkStatus
        ( defwDieArea ( (int)( toDefUnits(abutmentBox.getXMin()) )
                      , (int)( toDefUnits(abutmentBox.getYMin()) )
                      , (int)( toDefUnits(abutmentBox.getXMax()) )
                      , (int)( toDefUnits(abutmentBox.getYMax()) )
                      )
        , "_dieAreaCbk(): Failed to write DIEAERA"
        );
    }
    return driver->getStatus();
  }


  int  DefDriver::_gcellGridCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;

    return 0;
  }


  int  DefDriver::_rowCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver      = (DefDriver*)udata;
    int        status      = 0;
    Cell*      cell        = driver->getCell();
    Box        abutmentBox ( cell->getAbutmentBox() );

    if (abutmentBox.isEmpty()) return 0;

    int   origY     = (int)( toDefUnits(abutmentBox.getYMin()) );
    int   origX     = (int)( toDefUnits(abutmentBox.getXMin()) );
    int   stepY     = (int)( toDefUnits(DefDriver::getSliceHeight()) );
    int   stepX     = (int)( toDefUnits(DefDriver::getPitchWidth ()) );
    int   rowsNb    = abutmentBox.getHeight() / DefDriver::getSliceHeight();
    int   columnsNb = abutmentBox.getWidth () / DefDriver::getPitchWidth ();

    ostringstream comment;
    comment << rowsNb << " rows of " << columnsNb << " pitchs.";
    defwNewLine ();
    defwAddComment ( comment.str().c_str() );

    for ( int row=0 ; row < rowsNb ; ++row ) {
      ostringstream rowId;
      rowId << "row_" << setw(5) << setfill('0') << row;

      status = driver->checkStatus
        ( defwRowStr ( rowId.str().c_str()
                     , "core"
                     , origX
                     , origY
                     , (row%2) ? "FN" : "N"
                     , columnsNb
                     , 1
                     , stepX
                     , stepY
                     )
        , "_rowCbk(): Failed to write ROW"
        );

      if ( status != 0 ) break;

      origY += stepY;
    }

    return 0;
  }


  int  DefDriver::_trackCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver      = (DefDriver*)udata;
    Cell*      cell        = driver->getCell();
    Box        abutmentBox ( cell->getAbutmentBox() );

    if (abutmentBox.isEmpty()) return 0;

    const vector<RoutingLayerGauge*>& rg
      = AllianceFramework::get()->getRoutingGauge()->getLayerGauges();

    int status = 0;

    const char* layerName[1];
    for ( size_t ilayer=0 ; ilayer<rg.size() ; ++ilayer ) {
      string name = getString ( rg[ilayer]->getLayer()->getName() );
      layerName[0] = name.c_str();

      const char* master;  // i.e. direction.
      int         doCount;
      int         doStart;
      int         doStep = toDefUnits ( rg[ilayer]->getPitch() );

      if ( rg[ilayer]->getDirection() == Constant::Horizontal ) {
        master  = "X";
        doStart = toDefUnits ( abutmentBox.getYMin() + rg[ilayer]->getOffset() );
        doCount = toDefUnits ( abutmentBox.getHeight() ) / doStep;
      } else {
        master  = "Y";
        doStart = toDefUnits ( abutmentBox.getXMin() + rg[ilayer]->getOffset() );
        doCount = toDefUnits ( abutmentBox.getWidth() ) / doStep;
      }
      
      status = defwTracks ( master, doStart, doCount, doStep, 1, layerName );
      CHECK_STATUS_CBK(status,"_trackCbk(): Direction neither vertical nor horizontal.");
    }

    RETURN_CHECK_STATUS_CBK(status,"_trackCbk(): Did not complete properly");
  }


  int  DefDriver::_viaCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  int  DefDriver::_pinCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver      = (DefDriver*)udata;
    int        status      = 0;
    Cell*      cell        = driver->getCell();
    int        pinsNb      = 0;

    forEach ( Net*, inet, cell->getNets() ) {
      if ( (*inet)->isExternal() ) ++pinsNb;
    }

    status = defwStartPins ( pinsNb );
    if ( status != 0 )
      return driver->checkStatus( status, "_pinCbk(): Failed to start PINS" );

    forEach ( Net*, inet, cell->getNets() ) {
      if ( not (*inet)->isExternal() ) continue;

      const char* netUse = NULL;
      if ( (*inet)->isGround() ) netUse = "GROUND";
      if ( (*inet)->isPower () ) netUse = "POWER";
      if ( (*inet)->isClock () ) netUse = "CLOCK";

      status = defwPin ( getString((*inet)->getName()).c_str() // pin name.
                       , getString((*inet)->getName()).c_str() // net name (same).
                       , (netUse != NULL) ? 1 : 0              // special.
                       , (netUse != NULL) ? "INPUT" : "INOUT"  // direction.
                       , netUse                                // use.
                       , NULL                                  // placement status.
                       , 0                                     // status X.
                       , 0                                     // status Y.
                       , -1                                    // orient.
                       , NULL                                  // layer.
                       , 0, 0, 0, 0                            // geometry.
                       );
      if ( status != 0 ) return driver->checkStatus(status,"_pinCbk(): Failed to write PIN");
    }

    return driver->checkStatus ( defwEndPins(), "_pinCbk(): Failed to close PINS" );
  }


  int  DefDriver::_pinPropCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  int  DefDriver::_componentCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver      = (DefDriver*)udata;
    int        status      = 0;
    Cell*      cell        = driver->getCell();

    status = defwNewLine ();
    CHECK_STATUS_CBK(status,"_componentCbk(): Did not start properly");

    status = defwStartComponents ( cell->getTerminalNetlistInstanceOccurrences().getSize() );
    CHECK_STATUS_CBK(status,"_componentCbk(): Cannot create instance count");

    for ( Occurrence occurrence : cell->getTerminalNetlistInstanceOccurrences() ) {
      Instance*   instance     = static_cast<Instance*>(occurrence.getEntity());
      string      insname      = toDefName(occurrence.getCompactString());
      const char* source       = NULL;
      const char* statusS      = "UNPLACED";
      int         statusX      = 0;
      int         statusY      = 0;
      int         statusOrient = 0;

      Box            instanceAb     = instance->getMasterCell()->getAbutmentBox();
      Transformation instanceTransf = instance->getTransformation();
      occurrence.getPath().getTransformation().applyOn( instanceTransf );
      instanceTransf.applyOn( instanceAb );

      if (CatalogExtension::isFeed(instance->getMasterCell())) source = "DIST";

      if (instance->getPlacementStatus() == Instance::PlacementStatus::PLACED) statusS = "PLACED";
      if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED ) statusS = "FIXED";
      if (statusS[0] != 'U') {
        toDefCoordinates( instance, occurrence.getPath().getTransformation(), statusX, statusY, statusOrient );
      }

      status = defwComponent ( insname.c_str()
                             , getString((instance)->getMasterCell()->getName()).c_str()
                             , 0             // numNetNames (disabled).
                             , NULL          // netNames (disabled).
                             , NULL          // eeq (electrical equivalence).
                             , NULL          // genName.
                             , NULL          // genParameters.
                             , source        // source (who has created it).
                             , 0             // numForeigns.
                             , NULL          // foreigns.
                             , NULL          // foreignsX[].
                             , NULL          // foreignsY[].
                             , NULL          // foreignsOrient[].
                             , statusS       // status (placement status).
                             , statusX       // status X (disabled).
                             , statusY       // status Y (disabled).
                             , statusOrient  // status orientation (disabled).
                             , 0.0           // weight (disabled).
                             , NULL          // region (disabled).
                             , 0, 0, 0, 0    // region coordinates.
                             );
      if ( status != 0 ) return driver->checkStatus(status,"_componentCbk(): Failed to write COMPONENT");
    }

    return driver->checkStatus ( defwEndComponents(),"_componentCbk(): Failed to close COMPONENTS" );
  }


  int  DefDriver::writeRouting ( Net* net, bool special )
  {
    int status = 0;
    int i = 0;
    for ( Component *component : net->getComponents() ) {
      std::string layer = component->getLayer() ? getString(component->getLayer()->getName()) : "";
      if (layer.size() >= 4 && layer.substr(layer.size() - 4) == ".pin")
        continue;
      if (layer.size() >= 6 && layer.substr(layer.size() - 6) == ".block")
        continue;

      Segment *seg = dynamic_cast<Segment*>(component);
      if (seg) {
        status = (special ? defwSpecialNetPathStart : defwNetPathStart)((i++) ? "NEW" : "ROUTED");
        if (special) {
          status = defwSpecialNetPathLayer(layer.c_str());
          status = defwSpecialNetPathWidth(int(toDefUnits(seg->getWidth())));
        } else {
          status = defwNetPathLayer(layer.c_str(), 0, nullptr);
        }
        double x[2], y[2];
        x[0] = toDefUnits(seg->getSourceX());
        y[0] = toDefUnits(seg->getSourceY());
        x[1] = toDefUnits(seg->getTargetX());
        y[1] = toDefUnits(seg->getTargetY());
        status = (special ? defwSpecialNetPathPoint : defwNetPathPoint)(2, x, y);
      } else {
        Contact *contact = dynamic_cast<Contact*>(component);
        if (contact) {
          const ViaLayer *viaLayer = dynamic_cast<const ViaLayer*>(contact->getLayer());
          if (viaLayer) {
            status = (special ? defwSpecialNetPathStart : defwNetPathStart)((i++) ? "NEW" : "ROUTED");
            if (special)
              status = defwSpecialNetPathLayer(getString(viaLayer->getBottom()->getName()).c_str());
            else
              status = defwNetPathLayer(getString(viaLayer->getBottom()->getName()).c_str(), 0, nullptr);
            double x[1], y[1];
            x[0] = toDefUnits(contact->getX());
            y[0] = toDefUnits(contact->getY());
            status = (special ? defwSpecialNetPathPoint : defwNetPathPoint)(1, x, y);
            status = (special ? defwSpecialNetPathVia : defwNetPathVia)(getString(viaLayer->getName()).c_str());
          }
        } else {
          Rectilinear *rl = dynamic_cast<Rectilinear*>(component);
          if (rl) {
            Box box = rl->getBoundingBox();
            status = (special ? defwSpecialNetPathStart : defwNetPathStart)((i++) ? "NEW" : "ROUTED");
            if (special)
              status = defwSpecialNetPathLayer(layer.c_str());
            else
              status = defwNetPathLayer(layer.c_str(), 0, nullptr);
            double x[1], y[1];
            x[0] = toDefUnits(box.getXMin());
            y[0] = toDefUnits(box.getYMin());
            status = (special ? defwSpecialNetPathPoint : defwNetPathPoint)(1, x, y);
            defwNetPathRect(0, 0, toDefUnits(box.getWidth()), toDefUnits(box.getHeight()));
          }
        }
      }
    }
    if (i > 0)
      status = (special ? defwSpecialNetPathEnd : defwNetPathEnd)();
    return status;
  }


  int  DefDriver::_netCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver      = (DefDriver*)udata;
    int        status      = 0;
    Cell*      cell        = driver->getCell();
    int        netsNb      = 0;

    for ( Net* net : cell->getNets() ) {
      if ( net->isSupply() or net->isClock() ) continue;
      ++netsNb;
    }

    status = defwStartNets ( netsNb );
    if ( status != 0 )
      return driver->checkStatus(status,"_netCbk(): Failed to begin NETS");

    for ( Net* net : cell->getNets() ) {
      if ( net->isSupply() or net->isClock() ) continue;

      string netName = getString( net->getName() );
      if ( driver->getFlags() & DefExport::ProtectNetNames) {
        size_t pos = string::npos;
        if (netName[netName.size()-1] == ')') pos = netName.rfind('(');
        if (pos == string::npos)              pos = netName.size();
        netName.insert( pos, "_net" );
      }
      netName = toDefName( netName );

      status = defwNet ( netName.c_str() );
      if ( status != 0 ) return driver->checkStatus(status,"_netCbk(): Failed to begin NET");

      for ( RoutingPad* rp : net->getRoutingPads() ) {
        Plug *plug = dynamic_cast<Plug*>(rp->getPlugOccurrence().getEntity());
        if (plug) {
          status = defwNetConnection ( extractInstanceName(rp).c_str()
                                     , getString(plug->getMasterNet()->getName()).c_str()
                                     , 0
                                     );
        } else {
          Pin *pin = dynamic_cast<Pin*>(rp->getPlugOccurrence().getEntity());
          if (!pin)
            throw Error("RP PlugOccurrence neither a plug nor a pin!");
          // TODO: do we need to write something ?
        }
        if ( status != 0 ) return driver->checkStatus(status,"_netCbk(): Failed to write RoutingPad");
      }

      status = writeRouting(net, false);
      status = defwNetEndOneNet ();
      if ( status != 0 ) return driver->checkStatus( status, "_neCbk(): Failed to end NET" );
    }

    return driver->checkStatus ( defwEndNets(), "_neCbk(): Failed to end NETS" );
  }


  int  DefDriver::_snetCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver      = (DefDriver*)udata;
    int        status      = 0;
    Cell*      cell        = driver->getCell();
    int        netsNb      = 0;

    forEach ( Net*, inet, cell->getNets() ) {
      if ( (*inet)->isSupply() or (*inet)->isClock() ) ++netsNb;
    }

    status = defwStartSpecialNets ( netsNb );
    if ( status != 0 ) return driver->checkStatus(status,"_snetCbk(): Failed to begin SNETS");

    forEach ( Net*, inet, cell->getNets() ) {
      const char* netUse = NULL;
      if ( (*inet)->isGround() ) netUse = "GROUND";
      if ( (*inet)->isPower () ) netUse = "POWER";
      if ( (*inet)->isClock () ) netUse = "CLOCK";
      if ( netUse == NULL ) continue;

      status = defwSpecialNet ( getString((*inet)->getName()).c_str() );
      if ( status != 0 ) return driver->checkStatus(status,"_snetCbk(): Failed to write SNET");

      status = defwSpecialNetConnection ( "*"
                                        , getString((*inet)->getName()).c_str()
                                        , 0
                                        );
      if ( status != 0 ) return driver->checkStatus(status,"_snetCbk(): Failed to write CONNEXION");

      status = defwSpecialNetUse ( netUse );
      if ( status != 0 ) return driver->checkStatus(status,"_snetCnk(): Failed to write NET USE");

      status = writeRouting(*inet, true);
      if ( status != 0 ) return driver->checkStatus(status,"_snetCnk(): Failed to write special wiring");

      status = defwSpecialNetEndOneNet ();
      if ( status != 0 ) return driver->checkStatus(status,"_snetCnk(): Failed to end SNET");
    }

    return driver->checkStatus( defwEndSpecialNets(), "_snetCnk(): Failed to end SPECIALNETS" );
  }


  int  DefDriver::_extensionCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  int  DefDriver::_groupCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  int  DefDriver::_propDefCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  int  DefDriver::_regionCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  int  DefDriver::_scanchainCbk ( defwCallbackType_e, defiUserData udata )
  {
  //DefDriver* driver = (DefDriver*)udata;
    return 0;
  }


  void  DefDriver::drive ( Cell* cell, uint32_t flags )
  {
    FILE* defStream = NULL;
    try {
      string designName = getString(cell->getName()) + "_export";
      string path       = "./" + designName + ".def";

      cmess1 << "  o  Export DEF: <" << path << ">" << endl;

      defStream = fopen ( path.c_str(), "w" );
      if ( defStream == NULL )
        throw Error("DefDriver::drive(): Cannot open <%s>.",path.c_str());

      unique_ptr<DefDriver> driver ( new DefDriver(cell,designName,defStream,flags) );
      driver->write ();
    }
    catch ( ... ) {
      if ( defStream != NULL ) fclose ( defStream );

      throw;
    }
    fclose ( defStream );
  }


} // End of anonymous namespace.

#endif // HAVE_LEFDEF.


namespace CRL {

  using std::cerr;
  using std::endl;
  using std::string;
  using Hurricane::Library;
  using Hurricane::Transformation;
  using Hurricane::UpdateSession;


  void  DefExport::drive ( Cell* cell, uint32_t flags )
  {
#if defined(HAVE_LEFDEF)
    DefDriver::drive ( cell, flags );

    if ( flags & WithLEF ) LefExport::drive ( cell, LefExport::WithTechnology|LefExport::WithSpacers );
#else
    cerr << "[ERROR] CRL::DefExport::drive(): \n"
         << "  Coriolis2 hasn't been compiled with LEF/DEF support. To enable LEF/DEF\n"
         << "  support, you may obtain parser/driver from Si2 (www.si2.org) then recompile."
         << endl;
#endif
  }


}  // End of CRL namespace.
