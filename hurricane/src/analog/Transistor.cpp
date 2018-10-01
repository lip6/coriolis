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
// |  C++ Module  :  "./Transistor.cpp"                              |
// +-----------------------------------------------------------------+


#include <bitset>
#include <string>
#include "hurricane/Warning.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/MetaTransistor.h"
#include "hurricane/analog/Transistor.h"
#include "hurricane/analog/TransistorArguments.h"

namespace Analog {


  const Name  Transistor::_transistorBulkConnectedName  ("TransistorBulkConnected");
  const Name  Transistor::_transistorBulkUnconnectedName("TransistorBulkUnconnected");
  
  
  Transistor::Transistor ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
    , _metaTransistor(NULL)
  { }
  
  
  Transistor* Transistor::create ( Library* library, const Name& name, const Type& type, bool bulkConnected )
  {
    Name  deviceName;
    if (bulkConnected) deviceName = _transistorBulkConnectedName;
    else               deviceName = _transistorBulkUnconnectedName;
    preCreate(deviceName);
  
    UpdateSession::open();
    Transistor* tr = new Transistor(library,name,type);
    tr->_postCreate(deviceName);
    tr->createConnections(bulkConnected);
  
    tr->addStepParameter("B.w" , 1, 1, 1);
    tr->addStepParameter("D.w" , 1, 1, 1);
    tr->addStepParameter("G.w" , 1, 1, 1);
    tr->addStepParameter("S.w" , 1, 1, 1);
    tr->setTerminal( true );
    UpdateSession::close();
    return tr;
  }
  
  
  void  Transistor::_postCreate ( const Name& deviceName )
  {
    Super::_postCreate(deviceName);
  }
  
  
  void  Transistor::createConnections ( bool bulkConnected )
  {
    Net* s = Net::create(this, Name("S"));
    s->setExternal(true);
  
    Net* g = Net::create(this, Name("G"));
    g->setExternal(true);
  
    Net* d = Net::create(this, Name("D"));
    d->setExternal(true);
  
    _metaTransistor = MetaTransistor::create(getSubDevicesLibrary(), Name("M1"));
    Instance* metaTransistorIns = Instance::create(this, Name("M1Instance"), _metaTransistor);
  
    setReferenceTransistor(_metaTransistor);
  
    Plug* mtSPlug = metaTransistorIns->getPlug(_metaTransistor->getSource());
    mtSPlug->setNet(s);
    Plug* mtDPlug = metaTransistorIns->getPlug(_metaTransistor->getDrain());
    mtDPlug->setNet(d);
    Plug* mtGPlug = metaTransistorIns->getPlug(_metaTransistor->getGate());
    mtGPlug->setNet(g);
  
    if (not bulkConnected) {
      Net* b = Net::create(this, Name("B"));
      b->setExternal(true);
      Plug* mtBPlug = metaTransistorIns->getPlug(_metaTransistor->getBulk());
      mtBPlug->setNet(b);
    } else {
      Plug* mtBPlug = metaTransistorIns->getPlug(_metaTransistor->getBulk());
      mtBPlug->setNet(s);
    }
  }
  
  
  Name  Transistor::getDeviceName () const
  {
    if (isBulkConnected()) return _transistorBulkConnectedName;
    else                   return _transistorBulkUnconnectedName;
  }
  
  
  Arguments* Transistor::getArguments ()
  {
    const bitset<4> bs(getBulkType());
  
    return new TransistorArguments ( isNMOS()
                                   , isBulkConnected()
                                   , isSourceFirst()
                                   , bs.to_string()
                                   , getW()
                                   , getL()
                                   , getM()
                                   , getExternalDummy()
                                   );
  }
  
  
  unsigned int Transistor:: getRestriction ( Hurricane::Net* net ) const
  {
    Name netName = net->getName();
    Name nameS   = Name(string("S" ));
    Name nameD   = Name(string("D" ));
    Name nameG   = Name(string("G" ));
  
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
        rule |= (ok << north) | (ok << south) | (yes << west);
      } else if (nfing   == 2){
        rule |= (yes << north) | (ok << south) | (yes << east) | (yes << west);
      } else if (nfing   == 3){
        rule |= (yes << north) | (ok << south) | (ok << east) | (yes << west);
      } else if (nfing%2 == 0){
        rule |= (yes << north) | (yes << south) | (yes << east) | (yes << west);
      } else {
        rule |= (yes << north) | (yes << south) | (ok << east) | (yes << west);
      }
    } else if (netName == nameD){
      if        (nfing   == 1){
        rule |= (ok << north) | (ok << south)| (yes << east);
      } else if (nfing   == 2){
        rule |= (ok << north) | (ok << south);
      } else if (nfing   == 3){
        rule |= (ok << north) | (yes << south) | (yes << east) | (ok << west);
      } else if (nfing%2 == 0){
        rule |= (ok << north) | (yes << south) | (ok << east) | (ok << west);
      } else {
        rule |= (ok << north) | (yes << south) | (yes << east) | (ok << west);
      }
    } else if (netName == nameG){
      if (nfing == 1){
        rule |= (ok << north) | (ok << east) | (ok << west);
      } else {
        rule |= (yes << north) | (ok << east) | (ok << west);
      }
    } else {
      cerr << "Error(unsigned int getRestrictionsRules( Device* device, Net* net )): Unknown net name." << endl;
    }
    return rule;
  }
  
  
  bool Transistor::isSame(Transistor* ts)
  {
    StepParameter*       w1    = dynamic_cast<StepParameter*      >(this->getParameter("W"));
    StepParameter*       l1    = dynamic_cast<StepParameter*      >(this->getParameter("L"));
    FormFactorParameter* m1    = dynamic_cast<FormFactorParameter*>(this->getParameter("M"));
    ChoiceParameter*     ls1   = dynamic_cast<ChoiceParameter*    >(this->getParameter("Layout Styles"));
    StepParameter*       nerc1 = dynamic_cast<StepParameter*      >(this->getParameter("NERC"));
    StepParameter*       nirc1 = dynamic_cast<StepParameter*      >(this->getParameter("NIRC"));
    StepParameter*       b1    = dynamic_cast<StepParameter*      >(this->getParameter("B.w" ));
    StepParameter*       g1    = dynamic_cast<StepParameter*      >(this->getParameter("G.w"));
    StepParameter*       d1    = dynamic_cast<StepParameter*      >(this->getParameter("D.w"));
    StepParameter*       s1    = dynamic_cast<StepParameter*      >(this->getParameter("S.w" ));
  
    StepParameter*       w2    = dynamic_cast<StepParameter*      >(ts->getParameter("W"));
    StepParameter*       l2    = dynamic_cast<StepParameter*      >(ts->getParameter("L"));
    FormFactorParameter* m2    = dynamic_cast<FormFactorParameter*>(ts->getParameter("M"));
    ChoiceParameter*     ls2   = dynamic_cast<ChoiceParameter*    >(ts->getParameter("Layout Styles"));
    StepParameter*       nerc2 = dynamic_cast<StepParameter*      >(ts->getParameter("NERC"));
    StepParameter*       nirc2 = dynamic_cast<StepParameter*      >(ts->getParameter("NIRC"));
    StepParameter*       b2    = dynamic_cast<StepParameter*      >(ts->getParameter("B.w" ));
    StepParameter*       g2    = dynamic_cast<StepParameter*      >(ts->getParameter("G.w"));
    StepParameter*       d2    = dynamic_cast<StepParameter*      >(ts->getParameter("D.w"));
    StepParameter*       s2    = dynamic_cast<StepParameter*      >(ts->getParameter("S.w" ));
  
    if (w1->getValue() != w2->getValue()){
      cerr << Warning( "Transistor::isSame(): W are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false;
    } if (l1->getValue() != l2->getValue()){
      cerr << Warning( "Transistor::isSame(): L are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false; 
    } if (m1->getValue() != m2->getValue()){
      cerr << Warning( "Transistor::isSame(): M are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false;
    } if (ls1->getValue() != ls2->getValue()){ 
      cerr << Warning( "Transistor::isSame(): Layout styles are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false; 
    } if (nerc1->getValue() != nerc2->getValue()){
      cerr << Warning( "Transistor::isSame(): NERC are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false; 
    } if (nirc1->getValue() != nirc2->getValue()){
      cerr << Warning( "Transistor::isSame(): NIRC are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false;
    } if (b1->getValue() != b2->getValue()){
      cerr << Warning( "Transistor::isSame(): B.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false;
    } if (g1->getValue() != g2->getValue()){
      cerr << Warning( "Transistor::isSame(): G.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false; 
    } if (d1->getValue() != d2->getValue()){
      cerr << Warning( "Transistor::isSame(): D.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false; 
    } if (s1->getValue() != s2->getValue()) {
      cerr << Warning( "Transistor::isSame(): S.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(ts).c_str() );
      return false;
    }
                   
    return true;
  }


}  // Analog namespace.
