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
// |  C++ Module  :  "./SimpleCurrentMirror.cpp"                     |
// +-----------------------------------------------------------------+


#include <string>
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/SimpleCurrentMirror.h"

namespace Analog {


  const Name SimpleCurrentMirror::_scmBulkConnectedName   ("SimpleCurrentMirrorBulkConnected");
  const Name SimpleCurrentMirror::_scmBulkUnconnectedName ("SimpleCurrentMirrorBulkUnconnected");
  
  
  SimpleCurrentMirror::SimpleCurrentMirror ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
  { }
  
  
  SimpleCurrentMirror* SimpleCurrentMirror::create ( Library* library
                                                   , const Name& name
                                                   , const Type& type
                                                   , bool bulkConnected )
  {
    Name deviceName = (bulkConnected) ? _scmBulkConnectedName :  _scmBulkUnconnectedName;
    preCreate(deviceName);
  
    UpdateSession::open();
    SimpleCurrentMirror* scm = new SimpleCurrentMirror(library,name,type);
    scm->_postCreate(deviceName);
    scm->createConnections(bulkConnected);
    scm->setTerminal( true );
    UpdateSession::close();
  
    return scm;
  }
  
  
  void  SimpleCurrentMirror::createConnections ( bool bulkConnected )
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
      
    setReferenceTransistor(_m1);
      
    Plug* mt1SPlug = m1Ins->getPlug(_m1->getSource());
    mt1SPlug->setNet(s);
    Plug* mt2SPlug = m2Ins->getPlug(_m2->getSource());
    mt2SPlug->setNet(s);
    Plug* mt1DPlug = m1Ins->getPlug(_m1->getDrain());
    mt1DPlug->setNet(d1);
    Plug* mt2DPlug = m2Ins->getPlug(_m2->getDrain());
    mt2DPlug->setNet(d2);
    Plug* mt1GPlug = m1Ins->getPlug(_m1->getGate());
    mt1GPlug->setNet(d1);
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
  }
  
  
  Name  SimpleCurrentMirror::getDeviceName () const
  {
    return (isBulkConnected()) ? _scmBulkConnectedName : _scmBulkUnconnectedName;
  }
  
  unsigned int SimpleCurrentMirror:: getRestriction ( Hurricane::Net* net ) const
  {
    Name netName = net->getName();
    Name nameS   = Name(string("S" ));
    Name nameD1  = Name(string("D1"));
    Name nameD2  = Name(string("D2"));
  
    unsigned int ok    = 0x1;
    unsigned int yes   = 0x2;
  
    unsigned int west  = 0;
    unsigned int east  = 2;
    unsigned int south = 4;
    unsigned int north = 6;
    
    int nfing = this->getNfing();
    unsigned int rule = 0;
      
    if (netName == nameS){
      if        (nfing   == 1){
        rule |= (ok << north) | (ok << south);
      } else if (nfing%2 == 0){
        rule |= (ok << north) | (yes << south) | (yes << east) | (yes << west);
      } else {
        rule |= (ok << north) | (yes << south) | (ok << east) | (ok << west);
      }
    } else if (netName == nameD1){
      if        (nfing   == 1){
        rule |= (yes << north) | (ok << south) | (ok << east) | (yes << west);
      } else if (nfing%2 == 0){
        rule |= (yes << north) | (ok << south) | (ok << east) | (ok << west);
      } else {
        rule |= (yes << north) | (ok << south) | (ok << east) | (yes << west);
      }
    } else if (netName == nameD2){
      if        (nfing   == 1){
        rule |= (ok << north) | (ok << south) | (yes << east);
      } else if (nfing   == 2){
        rule |= (ok << north) | (ok << south);
      } else if (nfing%2 == 1){
        rule |= (ok << north) | (yes << south) | (yes << east) | (ok << west);
      } else {
        rule |= (ok << north) | (yes << south) | (ok << east) | (ok << west);
      }
    } else {
      cerr << "Error(unsigned int getRestrictionsRules( Device* device, Net* net )): Unknown net name." << endl;
    }
    return rule;
  }


}  // Analog namespace.
