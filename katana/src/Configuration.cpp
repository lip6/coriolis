// -*- mode: C++; explicit-buffer-name: "Configuration.cpp<katana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// +-----------------------------------------------------------------+


#include <string>
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "katana/Configuration.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Technology;


// -------------------------------------------------------------------
// Class  :  "Katana::Configuration".


  Configuration::Configuration ()
    : Anabatic::Configuration()
    , _postEventCb         ()
    , _hTracksReservedLocal(Cfg::getParamInt("katana.hTracksReservedLocal",    3)->asInt())
    , _vTracksReservedLocal(Cfg::getParamInt("katana.vTracksReservedLocal",    3)->asInt())
    , _ripupLimits         ()
    , _ripupCost           (Cfg::getParamInt("katana.ripupCost"           ,      3)->asInt())
    , _eventsLimit         (Cfg::getParamInt("katana.eventsLimit"         ,4000000)->asInt())
    , _flags               (0)
  {
    _ripupLimits[StrapRipupLimit]      = Cfg::getParamInt("katana.strapRipupLimit"      ,16)->asInt();
    _ripupLimits[LocalRipupLimit]      = Cfg::getParamInt("katana.localRipupLimit"      , 7)->asInt();
    _ripupLimits[GlobalRipupLimit]     = Cfg::getParamInt("katana.globalRipupLimit"     , 5)->asInt();
    _ripupLimits[LongGlobalRipupLimit] = Cfg::getParamInt("katana.longGlobalRipupLimit" , 5)->asInt();

    // for ( size_t i=0 ; i<MaxMetalDepth ; ++i ) {
    //   ostringstream paramName;
    //   paramName << "katana.metal" << (i+1) << "MinBreak";

    //   int threshold = 29*50;
    //   switch ( i ) {
    //     case 0:
    //     case 1:
    //       threshold = 2*50;
    //       break;
    //     default:
    //       threshold = 30*50;
    //       break;
    //   }

    //   Cfg::getParamDouble(paramName.str())->setDouble(threshold);
    //   _globalMinBreaks[i] = DbU::lambda (Cfg::getParamDouble(paramName.str())->asDouble());
    // }
  }


  Configuration::Configuration ( const Configuration& other )
    : Anabatic::Configuration(*other.base())
    , _postEventCb         (other._postEventCb)
    , _hTracksReservedLocal(other._hTracksReservedLocal)
    , _vTracksReservedLocal(other._vTracksReservedLocal)
    , _ripupLimits         ()
    , _ripupCost           (other._ripupCost)
    , _eventsLimit         (other._eventsLimit)
  {
    _ripupLimits[StrapRipupLimit]      = other._ripupLimits[StrapRipupLimit];
    _ripupLimits[LocalRipupLimit]      = other._ripupLimits[LocalRipupLimit];
    _ripupLimits[GlobalRipupLimit]     = other._ripupLimits[GlobalRipupLimit];
    _ripupLimits[LongGlobalRipupLimit] = other._ripupLimits[LongGlobalRipupLimit];
  }


  Configuration::~Configuration ()
  { }


  Configuration* Configuration::clone () const
  { return new Configuration(*this); }


  void  Configuration::setRipupLimit ( unsigned int type, unsigned int limit )
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("setRipupLimit(): Bad ripup limit index: %ud (> %ud)."
                   ,type,RipupLimitsTableSize) << endl;
      return;
    }

    _ripupLimits [ type ] = limit;
  }


  void  Configuration::setHTracksReservedLocal ( size_t reserved )
  {
    // if (reserved > getHEdgeCapacity())
    //   throw Error( "Configuration::setHTracksReservedLocal(): tracks reserved for local routing (%d) is greater than edge capacity %d."
    //              , reserved, getHEdgeCapacity() );

    _hTracksReservedLocal = reserved;
  }


  void  Configuration::setVTracksReservedLocal ( size_t reserved )
  {
    // if (reserved > 1.0)
    //   throw Error( "Configuration::setVTracksReservedLocal(): tracks reserved for local routing (%d) is greater than edge capacity %d."
    //              , reserved, getVEdgeCapacity() );

    _vTracksReservedLocal = reserved;
  }


  unsigned int  Configuration::getRipupLimit ( unsigned int type ) const
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("getRipupLimit(): Bad ripup limit index: %u (> %u)."
                   ,type,RipupLimitsTableSize) << endl;
      return 0;
    }
    return _ripupLimits[type];
  }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Katana> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asUInt ("     - Global router H reserved local"     ,_hTracksReservedLocal) << endl;
    cout << Dots::asUInt ("     - Global router V reserved local"     ,_vTracksReservedLocal) << endl;
    cout << Dots::asULong("     - Events limit (iterations)"          ,_eventsLimit) << endl;
    cout << Dots::asUInt ("     - Ripup limit, straps"                ,_ripupLimits[StrapRipupLimit]) << endl;
    cout << Dots::asUInt ("     - Ripup limit, locals"                ,_ripupLimits[LocalRipupLimit]) << endl;
    cout << Dots::asUInt ("     - Ripup limit, globals"               ,_ripupLimits[GlobalRipupLimit]) << endl;
    cout << Dots::asUInt ("     - Ripup limit, long globals"          ,_ripupLimits[LongGlobalRipupLimit]) << endl;

    Super::print ( cell );
  }


  string  Configuration::_getTypeName () const
  { return "Configuration"; }


  string  Configuration::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << " " << getRoutingGauge()->getName() << ">";

    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if ( record ) {
      record->add ( getSlot("_hTracksReservedLocal" ,_hTracksReservedLocal ) );
      record->add ( getSlot("_vTracksReservedLocal" ,_vTracksReservedLocal ) );
      record->add ( getSlot("_ripupCost"            ,_ripupCost            ) );
      record->add ( getSlot("_eventsLimit"          ,_eventsLimit          ) );

      record->add ( getSlot("_ripupLimits[StrapRipupLimit]"     ,_ripupLimits[StrapRipupLimit]     ) );
      record->add ( getSlot("_ripupLimits[LocalRipupLimit]"     ,_ripupLimits[LocalRipupLimit]     ) );
      record->add ( getSlot("_ripupLimits[GlobalRipupLimit]"    ,_ripupLimits[GlobalRipupLimit]    ) );
      record->add ( getSlot("_ripupLimits[LongGlobalRipupLimit]",_ripupLimits[LongGlobalRipupLimit]) );

      // for ( size_t i=0 ; i<MaxMetalDepth ; ++i ) {
      //   ostringstream paramName;
      //   paramName << "metal" << (i+1) << "MinBreak";

      //   record->add ( DbU::getValueSlot(paramName.str(),&_globalMinBreaks[i]) );
      // }
    }
                                     
    return record;
  }



}  // Katana namespace.
