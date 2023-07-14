// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :            C. Alexandre, J.-M. Caba              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CommonGatePair.cpp"                          |
// +-----------------------------------------------------------------+


#include <string>
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/CommonGatePair.h"

namespace Analog {


  const Name CommonGatePair::_cgBulkConnectedName   ("CommonGatePairBulkConnected");
  const Name CommonGatePair::_cgBulkUnconnectedName ("CommonGatePairBulkUnconnected");
  
  CommonGatePair::CommonGatePair ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
  { }
  
  
  CommonGatePair* CommonGatePair::create ( Library*    library
                                         , const Name& name
                                         , const Type& type
                                         , bool        bulkConnected )
  {
    Name deviceName = (bulkConnected) ? _cgBulkConnectedName : _cgBulkUnconnectedName;
    preCreate(deviceName);
  
    UpdateSession::open();
    CommonGatePair* cg = new CommonGatePair(library,name,type);
    cg->_postCreate      (deviceName);
    cg->createConnections(bulkConnected);
    UpdateSession::close();
    return cg;
  }
  
  
  void  CommonGatePair::createConnections ( bool bulkConnected )
  {
    Net* s1 = Net::create(this, Name("S1"));
    s1->setExternal(true);
      
    Net* s2 = Net::create(this, Name("S2"));
    s2->setExternal(true);
      
    Net* d1 = Net::create(this, Name("D1"));
    d1->setExternal(true);
      
    Net* d2 = Net::create(this, Name("D2"));
    d2->setExternal(true);
  
    Net* g12 = Net::create(this, Name("G12"));
    g12->setExternal(true);
      
    _m1 = MetaTransistor::create(getSubDevicesLibrary(), Name("M1"));
    Instance* m1Ins = Instance::create(this, Name("M1Instance"), _m1);
    _m2 = MetaTransistor::create(getSubDevicesLibrary(), Name("M2"));
    Instance* m2Ins = Instance::create(this, Name("M2Instance"), _m2);
      
    setReferenceTransistor(_m1);
      
    Plug* mt1SPlug = m1Ins->getPlug(_m1->getSource());
    mt1SPlug->setNet(s1);
    Plug* mt2SPlug = m2Ins->getPlug(_m2->getSource());
    mt2SPlug->setNet(s2);
    Plug* mt1DPlug = m1Ins->getPlug(_m1->getDrain());
    mt1DPlug->setNet(d1);
    Plug* mt2DPlug = m2Ins->getPlug(_m2->getDrain());
    mt2DPlug->setNet(d2);
    Plug* mt1GPlug = m1Ins->getPlug(_m1->getGate());
    mt1GPlug->setNet(g12);
    Plug* mt2GPlug = m2Ins->getPlug(_m2->getGate());
    mt2GPlug->setNet(g12);
      
    if (not bulkConnected) {
      Net* b = Net::create(this, Name("B"));
      b->setExternal(true);
      Plug* mt1BPlug = m1Ins->getPlug(_m1->getBulk());
      mt1BPlug->setNet(b);
      Plug* mt2BPlug = m2Ins->getPlug(_m2->getBulk());
      mt2BPlug->setNet(b);
    } else {
      Plug* mt1BPlug = m1Ins->getPlug(_m1->getBulk());
      mt1BPlug->setNet(s1);
      Plug* mt2BPlug = m2Ins->getPlug(_m2->getBulk());
      mt2BPlug->setNet(s2);
    }
  }
  
  
  Name  CommonGatePair::getDeviceName () const
  { return (isBulkConnected()) ? _cgBulkConnectedName : _cgBulkUnconnectedName; }


  string  CommonGatePair::_getTypeName () const
  { return "CommonGatePair"; }
  

  Record*  CommonGatePair::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_cgBulkConnectedName"  , _cgBulkConnectedName   ) );
    record->add( getSlot("_cgBulkUnconnectedName", _cgBulkUnconnectedName ) );
    return record;
  }

}  // Analog namespace.
