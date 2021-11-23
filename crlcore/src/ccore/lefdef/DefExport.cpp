// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        C a d e n c e   D E F   E x p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
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


#define  CHECK_STATUS_CBK(status)         if ((status) != 0) return driver->checkStatus(status);
#define  CHECK_STATUS_DRV(status)         if ((status) != 0) return checkStatus(status);
#define  RETURN_CHECK_STATUS_CBK(status)  return driver->checkStatus(status);
#define  RETURN_CHECK_STATUS_DRV(status)  return checkStatus(status);


  class DefDriver {
    public:
      static void          drive            ( Cell* cell, unsigned int flags );
      static int           getUnits         ();
      static int           toDefUnits       ( DbU::Unit );
      static int           toDefOrient      ( Transformation::Orientation );
      static void          toDefCoordinates ( Instance*, Transformation, int& statusX, int& statusY, int& statusOrient );
      static DbU::Unit     getSliceHeight   ();
      static DbU::Unit     getPitchWidth    ();
                          ~DefDriver        ();
             int           write            ();
    private:                                
                           DefDriver        ( Cell*, const string& designName, FILE*, unsigned int flags );
      inline Cell*         getCell          ();
      inline const string& getDesignName    () const;
      inline unsigned int  getFlags         () const;
      inline int           getStatus        () const;
             int           checkStatus      ( int status );
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
             unsigned int  _flags;
             int           _status;
  };


  int        DefDriver::_units       = 100;
  DbU::Unit  DefDriver::_sliceHeight = 0;
  DbU::Unit  DefDriver::_pitchWidth  = 0;


         int           DefDriver::getUnits       () { return _units; }
         int           DefDriver::toDefUnits     ( DbU::Unit u ) { return (int)round(DbU::getLambda(u)*getUnits()); }
         DbU::Unit     DefDriver::getSliceHeight () { return _sliceHeight; }
         DbU::Unit     DefDriver::getPitchWidth  () { return _pitchWidth; }; 
  inline Cell*         DefDriver::getCell        () { return _cell; }
  inline unsigned int  DefDriver::getFlags       () const { return _flags; }
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
    instance->getTransformation().applyOn( transf );
    statusX      = toDefUnits ( transf.getTx() );
    statusY      = toDefUnits ( transf.getTy() );
    statusOrient = toDefOrient( transf.getOrientation() );

    switch ( transf.getOrientation() ) {
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


  DefDriver::DefDriver ( Cell* cell, const string& designName, FILE* defStream, unsigned int flags )
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
    _units = DbU::toGrid(DbU::fromMicrons(1.0));

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
    return checkStatus ( defwWrite(_defStream,_designName.c_str(),(void*)this) );
  }


  int  DefDriver::checkStatus ( int status )
  {
    if ( (_status=status) != 0 ) {
      defwPrintError ( _status );
      ostringstream message;
      message << "DefDriver::drive(): Error occured while driving <" << _designName << ">.";
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

    return driver->checkStatus ( defwVersion(5,7) );
  }


  int  DefDriver::_designCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    defwNewLine ();
    return driver->checkStatus ( defwDesignName(driver->getDesignName().c_str()) );
  }


  int  DefDriver::_designEndCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    return driver->checkStatus ( defwEnd() );
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
    return driver->checkStatus ( defwDividerChar(".") );
  }


  int  DefDriver::_busBitCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    return driver->checkStatus ( defwBusBitChars("()") );
  }


  int  DefDriver::_unitsCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    defwNewLine ();
    return driver->checkStatus ( defwUnits (DefDriver::getUnits()) );
  }


  int  DefDriver::_technologyCbk ( defwCallbackType_e, defiUserData udata )
  {
    DefDriver* driver = (DefDriver*)udata;
    return driver->checkStatus ( defwTechnology( getString(driver->getCell()->getLibrary()->getName()).c_str() ) );
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
      CHECK_STATUS_CBK(status);
    }

    RETURN_CHECK_STATUS_CBK(status);
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
    if ( status != 0 ) return driver->checkStatus(status);

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
      if ( status != 0 ) return driver->checkStatus(status);
    }

    return driver->checkStatus ( defwEndPins() );
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
    CHECK_STATUS_CBK(status);

    status = defwStartComponents ( cell->getTerminalNetlistInstanceOccurrences().getSize() );
    CHECK_STATUS_CBK(status);

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
      if ( status != 0 ) return driver->checkStatus(status);
    }

    return driver->checkStatus ( defwEndComponents() );
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
    if ( status != 0 ) return driver->checkStatus(status);

    for ( Net* net : cell->getNets() ) {
      if ( net->isSupply() or net->isClock() ) continue;

      size_t pos     = string::npos;
      string netName = getString( net->getName() );
      if (netName[netName.size()-1] == ')') pos = netName.rfind('(');
      if (pos == string::npos)              pos = netName.size();
      netName.insert( pos, "_net" );
      netName = toDefName( netName );

      status = defwNet ( netName.c_str() );
      if ( status != 0 ) return driver->checkStatus(status);

      for ( RoutingPad* rp : net->getRoutingPads() ) {
        status = defwNetConnection ( extractInstanceName(rp).c_str()
                                   , getString(static_cast<Plug*>(rp->getPlugOccurrence().getEntity())->getMasterNet()->getName()).c_str()
                                   , 0
                                   );
        if ( status != 0 ) return driver->checkStatus(status);
      }

      status = defwNetEndOneNet ();
      if ( status != 0 ) return driver->checkStatus(status);
    }

    return driver->checkStatus ( defwEndNets() );
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
    if ( status != 0 ) return driver->checkStatus(status);

    forEach ( Net*, inet, cell->getNets() ) {
      const char* netUse = NULL;
      if ( (*inet)->isGround() ) netUse = "GROUND";
      if ( (*inet)->isPower () ) netUse = "POWER";
      if ( (*inet)->isClock () ) netUse = "CLOCK";
      if ( netUse == NULL ) continue;

      status = defwSpecialNet ( getString((*inet)->getName()).c_str() );
      if ( status != 0 ) return driver->checkStatus(status);

      status = defwSpecialNetConnection ( "*"
                                        , getString((*inet)->getName()).c_str()
                                        , 0
                                        );
      if ( status != 0 ) return driver->checkStatus(status);

      status = defwSpecialNetUse ( netUse );
      if ( status != 0 ) return driver->checkStatus(status);

      status = defwSpecialNetEndOneNet ();
      if ( status != 0 ) return driver->checkStatus(status);
    }

    return driver->checkStatus ( defwEndSpecialNets() );
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


  void  DefDriver::drive ( Cell* cell, unsigned int flags )
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


  void  DefExport::drive ( Cell* cell, unsigned int flags )
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
