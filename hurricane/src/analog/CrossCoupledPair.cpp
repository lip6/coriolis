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
// |  C++ Module  :  "./CrossCoupledPair.cpp"                        |
// +-----------------------------------------------------------------+


#include <string>
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/CrossCoupledPair.h"

namespace Analog {


  const Name CrossCoupledPair::_ccpBulkConnectedName   ("CrossCoupledPairBulkConnected");
  const Name CrossCoupledPair::_ccpBulkUnconnectedName ("CrossCoupledPairBulkUnconnected");
  
  
  CrossCoupledPair::CrossCoupledPair ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
  { }
  
  
  CrossCoupledPair* CrossCoupledPair::create ( Library* library
                                             , const Name& name
                                             , const Type& type
                                             , bool bulkConnected )
  {
    Name deviceName = (bulkConnected) ? _ccpBulkConnectedName : _ccpBulkUnconnectedName;
    preCreate(deviceName);
  
    UpdateSession::open();
    CrossCoupledPair* ccp = new CrossCoupledPair(library,name,type);
    ccp->_postCreate(deviceName);
    ccp->createConnections(bulkConnected);
    UpdateSession::close();
    return ccp;
  }
  
  
  void  CrossCoupledPair::createConnections ( bool bulkConnected )
  {
    Net* s = Net::create(this, Name("S"));
    s->setExternal(true);
  
    Net* d1 = Net::create(this, Name("D1"));
    d1->setExternal(true);
  
    Net* d2 = Net::create(this, Name("D2"));
    d2->setExternal(true);
  
    _m1 = MetaTransistor::create(getSubDevicesLibrary(), Name("M1"));
    Instance* m1Ins = Instance::create(this, Name("M1Instance"), _m1);
    _m2 = MetaTransistor::create(getSubDevicesLibrary(), Name("M2"));
    Instance* m2Ins = Instance::create(this, Name("M2Instance"), _m2);
  
    Plug* mt1SPlug = m1Ins->getPlug(_m1->getSource());
    mt1SPlug->setNet(s);
    Plug* mt2SPlug = m2Ins->getPlug(_m2->getSource());
    mt2SPlug->setNet(s);
    Plug* mt1DPlug = m1Ins->getPlug(_m1->getDrain());
    mt1DPlug->setNet(d1);
    Plug* mt2DPlug = m2Ins->getPlug(_m2->getDrain());
    mt2DPlug->setNet(d2);
    Plug* mt1GPlug = m1Ins->getPlug(_m1->getGate());
    mt1GPlug->setNet(d2);
    Plug* mt2GPlug = m2Ins->getPlug(_m2->getGate());
    mt2GPlug->setNet(d1);
    
    if (not bulkConnected) {
      Net* b = Net::create(this, Name("B"));
      b->setExternal(true);
      Plug* mt1BPlug = m1Ins->getPlug(_m1->getBulk());
      mt1BPlug->setNet(b);
      Plug* mt2BPlug = m2Ins->getPlug(_m2->getBulk());
      mt2BPlug->setNet(b);
    } else {
      Plug* mt1BPlug = m1Ins->getPlug(_m1->getBulk());
      mt1BPlug->setNet(s);
      Plug* mt2BPlug = m2Ins->getPlug(_m2->getBulk());
      mt2BPlug->setNet(s);
    }
  
    setReferenceTransistor(_m1);
  }
  
  
  Name  CrossCoupledPair::getDeviceName () const
  { return (isBulkConnected()) ? _ccpBulkConnectedName : _ccpBulkUnconnectedName; }


  string  CrossCoupledPair::_getTypeName () const
  { return "CrossCoupledPair"; }
  

  Record*  CrossCoupledPair::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_ccpBulkConnectedName"  ,  _ccpBulkConnectedName   ) );
    record->add( getSlot("_ccpBulkUnconnectedName",  _ccpBulkUnconnectedName ) );
    return record;
  }


}  // Analog namespace.
