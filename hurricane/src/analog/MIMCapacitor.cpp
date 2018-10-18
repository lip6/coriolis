// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./MIMCapacitor.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/UpdateSession.h"
#include "hurricane/analog/MetaCapacitor.h"
#include "hurricane/analog/MIMCapacitor.h"

namespace Analog {
  
  using namespace Hurricane;


  const Name MIMCapacitor::_capacitorName ("MIMCapacitor");
  

  MIMCapacitor::MIMCapacitor ( Library* library, const Name& name, const CapacitorFamily::Type& type )
    : Super(library, name, type)
    , _metaCapacitor(NULL)
    , _cDraw        (0.0)
    , _cBorder      (0.0)
    , _cParasite    (0.0)
    , _c            (NULL)
  { }

  
  MIMCapacitor* MIMCapacitor::create ( Library* library, const Name& name, const CapacitorFamily::Type& type ) 
  {
    preCreate( _capacitorName );
    UpdateSession::open();
    MIMCapacitor* cp = new MIMCapacitor( library, name, type );
    cp->_postCreate( _capacitorName );
    cp->createConnections();
    cp->setTerminal( true );
    UpdateSession::close();
    return cp;
  }
  

  void MIMCapacitor::_postCreate ( const Name& deviceName ) 
  {
    Super::_postCreate(deviceName);
  
  // get minimum and maximum value from technology
  //AnalogEnv* aenv = AnalogEnv::get();
  //AnalogTechnology* atechno = aenv->getAnalogTechnology();
  //DbU::Unit capacitorMinL = atechno->getPhysicalRule("capacitorMinL").getValue();
  //DbU::Unit capacitorMaxL = atechno->getPhysicalRule("capacitorMaxL").getValue();
  //DbU::Unit capacitorMinW = atechno->getPhysicalRule("capacitorMinW").getValue();
  //DbU::Unit capacitorMaxW = atechno->getPhysicalRule("capacitorMaxW").getValue();
  // For now define arbitrary values:
  //DbU::Unit capacitorMinC = DbU::physicalToDbu(0.25, DbU::Micro);
  //DbU::Unit capacitorMaxC = DbU::physicalToDbu(25  , DbU::Micro);
    _c = addCapacitorParameter( "C", 0.0 );
  }
  

  void MIMCapacitor::createConnections () 
  {
    Net* p1 = Net::create( this, Name("P1") );
    p1->setExternal( true );
  
    Net* p2 = Net::create( this, Name("P2") );
    p2->setExternal( true );
  
    _metaCapacitor = MetaCapacitor::create( getSubDevicesLibrary(), Name("C1") );
    Instance* metaCapacitorIns = Instance::create( this, Name("C1Instance"), _metaCapacitor );
    setReferenceCapacitor( _metaCapacitor );
  
    Plug* mcC1Plug = metaCapacitorIns->getPlug( _metaCapacitor->getPlate1() );
    mcC1Plug->setNet( p1 );
    Plug* mcC2Plug = metaCapacitorIns->getPlug( _metaCapacitor->getPlate2() );
    mcC2Plug->setNet( p2 );
  }


  Name MIMCapacitor::getDeviceName () const
  { return _capacitorName; }


}  // Analog namespace.
