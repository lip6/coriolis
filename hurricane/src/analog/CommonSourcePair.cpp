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
// |  C++ Module  :  "./CommonSourcePair.cpp"                        |
// +-----------------------------------------------------------------+


#include <string>
#include "hurricane/Warning.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/CommonSourcePair.h"

namespace Analog {

  using Hurricane::Warning;


  const Name  CommonSourcePair::_cspBulkConnectedName  ("CommonSourcePairBulkConnected");
  const Name  CommonSourcePair::_cspBulkUnconnectedName("CommonSourcePairBulkUnconnected");
  
  
  CommonSourcePair::CommonSourcePair ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
  { }
  
  
  CommonSourcePair* CommonSourcePair::create ( Library*    library
                                             , const Name& name
                                             , const Type& type
                                             , bool        bulkConnected )
  {
    Name deviceName = (bulkConnected) ? _cspBulkConnectedName : _cspBulkUnconnectedName;
    preCreate(deviceName);
  
    UpdateSession::open();
    CommonSourcePair* csp = new CommonSourcePair(library,name,type);
    csp->_postCreate      (deviceName);
    csp->createConnections(bulkConnected);
  
    csp->addStepParameter("B.w" , 1, 1, 1);
    csp->addStepParameter("D1.w", 1, 1, 1);
    csp->addStepParameter("D2.w", 1, 1, 1);
    csp->addStepParameter("G.w" , 1, 1, 1);
    csp->addStepParameter("S.w" , 1, 1, 1);
    UpdateSession::close();
    return csp;
  }
  
  
  void  CommonSourcePair::createConnections ( bool bulkConnected )
  {
    Net* s = Net::create(this, Name("S"));
    s->setExternal(true);
  
    Net* g = Net::create(this, Name("G"));
    g->setExternal(true);
  
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
    mt1GPlug->setNet(g);
    Plug* mt2GPlug = m2Ins->getPlug(_m2->getGate());
    mt2GPlug->setNet(g);
  
    if (!bulkConnected) {
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
  
  
  Name  CommonSourcePair::getDeviceName () const
  {
    return (isBulkConnected()) ? _cspBulkConnectedName : _cspBulkUnconnectedName; 
  }
  
  
  unsigned int CommonSourcePair::getRestriction ( Hurricane::Net* net ) const
  {
    Name netName = net->getName();
    Name nameS   = Name(string("S" ));
    Name nameD1  = Name(string("D1"));
    Name nameD2  = Name(string("D2"));
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
        rule |= (ok << north) | (ok << south);
      } else if (nfing%2 == 0){
        rule |= (ok << north) | (yes << south) | (yes << east) | (yes << west);
      } else {
        rule |= (ok << north) | (yes << south) | (ok << east) | (ok << west);
      }
    } else if (netName == nameD1){
      if        (nfing   == 1){
        rule |= (ok << north) | (ok << south) | (yes << west);
      } else if (nfing   == 2){
        rule |= (ok << north) | (ok << south);
      } else if (nfing%2 == 1){
        rule |= (ok << north) | (yes << south) | (ok << east) | (yes << west);
      } else {
        rule |= (ok << north) | (yes << south) | (ok << east) | (ok << west);
      }
    } else if (netName == nameD2){
      if        (nfing   == 1){
        rule |= (ok << north) | (ok << south) | (yes << east);
      } else if (nfing   == 2){
        rule |= (ok << north) | (ok << south);
      } else if (nfing%2 == 1){
        rule |= (yes << north) | (ok << south) | (yes << east) | (ok << west);
      } else {
        rule |= (yes << north) | (ok << south) | (ok << east) | (ok << west);
      }
    } else if (netName == nameG){
      rule |= (yes << north) | (ok << east) | (ok << west);
    } else {
      cerr << "Error(unsigned int getRestrictionsRules( Device* device, Net* net )): Unknown net name." << endl;
    }
    return rule;
  }
  

  bool  CommonSourcePair::isSame ( CommonSourcePair* csp )
  {
    StepParameter*       w1    = dynamic_cast<StepParameter*      >(this->getParameter("W"));
    StepParameter*       l1    = dynamic_cast<StepParameter*      >(this->getParameter("L"));
    FormFactorParameter* m1    = dynamic_cast<FormFactorParameter*>(this->getParameter("M"));
    ChoiceParameter*     ls1   = dynamic_cast<ChoiceParameter*    >(this->getParameter("Layout Styles"));
    StepParameter*       nerc1 = dynamic_cast<StepParameter*      >(this->getParameter("NERC"));
    StepParameter*       nirc1 = dynamic_cast<StepParameter*      >(this->getParameter("NIRC"));
    StepParameter*       b1    = dynamic_cast<StepParameter*      >(this->getParameter("B.w" ));
    StepParameter*       g1    = dynamic_cast<StepParameter*      >(this->getParameter("G.w"));
    StepParameter*       d11   = dynamic_cast<StepParameter*      >(this->getParameter("D1.w"));
    StepParameter*       d21   = dynamic_cast<StepParameter*      >(this->getParameter("D2.w"));
    StepParameter*       s1    = dynamic_cast<StepParameter*      >(this->getParameter("S.w" ));
  
    StepParameter*       w2    = dynamic_cast<StepParameter*      >(csp->getParameter("W"));
    StepParameter*       l2    = dynamic_cast<StepParameter*      >(csp->getParameter("L"));
    FormFactorParameter* m2    = dynamic_cast<FormFactorParameter*>(csp->getParameter("M"));
    ChoiceParameter*     ls2   = dynamic_cast<ChoiceParameter*    >(csp->getParameter("Layout Styles"));
    StepParameter*       nerc2 = dynamic_cast<StepParameter*      >(csp->getParameter("NERC"));
    StepParameter*       nirc2 = dynamic_cast<StepParameter*      >(csp->getParameter("NIRC"));
    StepParameter*       b2    = dynamic_cast<StepParameter*      >(csp->getParameter("B.w" ));
    StepParameter*       g2    = dynamic_cast<StepParameter*      >(csp->getParameter("G.w"));
    StepParameter*       d12   = dynamic_cast<StepParameter*      >(csp->getParameter("D1.w"));
    StepParameter*       d22   = dynamic_cast<StepParameter*      >(csp->getParameter("D2.w"));
    StepParameter*       s2    = dynamic_cast<StepParameter*      >(csp->getParameter("S.w" ));
  
    if (w1->getValue() != w2->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): W are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false;
    } if (l1->getValue() != l2->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): L are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false; 
    } if (m1->getValue() != m2->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): M are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false;
    } if (ls1->getValue() != ls2->getValue()){ 
      cerr << Warning( "CommonSourcePair::isSame(): Layout styles are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false; 
    } if (nerc1->getValue() != nerc2->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): NERC are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false; 
    } if (nirc1->getValue() != nirc2->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): NIRC are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false;
    } if (b1->getValue() != b2->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): B.w are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false;
    } if (g1->getValue() != g2->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): G.w are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false; 
    } if (d11->getValue() != d12->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): D1.w are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false; 
    } if (d21->getValue() != d22->getValue()){
      cerr << Warning( "CommonSourcePair::isSame(): D2.w are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false;
    } if (s1->getValue() != s2->getValue()) {
      cerr << Warning( "CommonSourcePair::isSame(): S.w are different \"%s\" vs \"s\"."
                     , getString(this).c_str(), getString(csp).c_str() );
      return false;
    }
                   
    return true;
  }


}  // Analog namespace.
