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
// |  C++ Module  :  "./DifferentialPair.cpp"                        |
// +-----------------------------------------------------------------+


#include <string>
#include "hurricane/Warning.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/DifferentialPair.h"

namespace Analog {
  
  using namespace std;
  using namespace Hurricane;


  const Name DifferentialPair::_dpBulkConnectedName   ("DifferentialPairBulkConnected");
  const Name DifferentialPair::_dpBulkUnconnectedName ("DifferentialPairBulkUnconnected");
  
  
  DifferentialPair::DifferentialPair ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
  { }
  
  
  DifferentialPair* DifferentialPair::create ( Library* library
                                             , const Name& name
                                             , const Type& type
                                             , bool bulkConnected )
  {
    Name deviceName = (bulkConnected) ? _dpBulkConnectedName : _dpBulkUnconnectedName;
    preCreate(deviceName);
  
    UpdateSession::open();
    DifferentialPair* dp = new DifferentialPair( library, name, type );
    dp->_postCreate      ( deviceName );
    dp->createConnections( bulkConnected );
  
    dp->addStepParameter( "B.w" , 1, 1, 1 );
    dp->addStepParameter( "D1.w", 1, 1, 1 );
    dp->addStepParameter( "D2.w", 1, 1, 1 );
    dp->addStepParameter( "G1.w", 1, 1, 1 );
    dp->addStepParameter( "G2.w", 1, 1, 1 );
    dp->addStepParameter( "S.w" , 1, 1, 1 );
    UpdateSession::close();
    return dp;
  }
  
  
  void  DifferentialPair::createConnections ( bool bulkConnected )
  {
    Net* s = Net::create(this, Name("S"));
    s->setExternal(true);
  
    Net* g1 = Net::create(this, Name("G1"));
    g1->setExternal(true);
  
    Net* g2 = Net::create(this, Name("G2"));
    g2->setExternal(true);
  
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
    mt1GPlug->setNet(g1);
    Plug* mt2GPlug = m2Ins->getPlug(_m2->getGate());
    mt2GPlug->setNet(g2);
  
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
  
  
  Name DifferentialPair::getDeviceName() const
  { return (isBulkConnected()) ? _dpBulkConnectedName : _dpBulkUnconnectedName; }
  
  
  unsigned int DifferentialPair::getRestriction ( Hurricane::Net* net ) const
  {
    Name netName = net->getName();
    Name nameS   = Name(string("S" ));
    Name nameD1  = Name(string("D1"));
    Name nameD2  = Name(string("D2"));
    Name nameG1  = Name(string("G1"));
    Name nameG2  = Name(string("G2"));
  
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
        rule |= (yes << north) | (yes << south) | (yes << east) | (yes<< west);
      } else {
        rule |= (yes << north) | (yes << south) | (ok << east) | (ok << west);
      }
    } else if (netName == nameD1){
      if        (nfing   == 1){
        rule |= (ok << north) | (ok << south);
      } else if (nfing   == 2){
        rule |= (ok << north) | (ok << south);
      } else if (nfing%2 == 1){
        rule |= (yes << north) | (ok << south) | (ok << east) | (yes << west);
      } else {
        rule |= (yes << north) | (ok << south) | (ok << east) | (ok << west);
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
    } else if (netName == nameG1){
      if        (nfing   == 1){
        rule |= (ok << north) | (ok << east) | (ok << west);
      } else {
        rule |= (yes << north) | (ok << east) | (ok << west);
      }
    } else if (netName == nameG2){
      if        (nfing   == 1){
        rule |= (ok << south) | (ok << east) | (ok << west);
      } else {
        rule |= (yes << south) | (ok << east) | (ok << west);
      }
    } else {
      cerr << "Error(unsigned int getRestrictionsRules( Device* device, Net* net )): Unknown net name." << endl;
    }
    return rule;
  }
  
  
  bool DifferentialPair::isSame ( DifferentialPair* dp )
  {
    StepParameter*       w1    = dynamic_cast<StepParameter*>(this->getParameter("W"));
    StepParameter*       l1    = dynamic_cast<StepParameter*>(this->getParameter("L"));
    FormFactorParameter* m1    = dynamic_cast<FormFactorParameter*>(this->getParameter("M"));
    ChoiceParameter*     ls1   = dynamic_cast<ChoiceParameter*>(this->getParameter("Layout Styles"));
    StepParameter*       nerc1 = dynamic_cast<StepParameter*>(this->getParameter("NERC"));
    StepParameter*       nirc1 = dynamic_cast<StepParameter*>(this->getParameter("NIRC"));
    StepParameter*       b1    = dynamic_cast<StepParameter*>(this->getParameter("B.w" ));
    StepParameter*       g11   = dynamic_cast<StepParameter*>(this->getParameter("G1.w"));
    StepParameter*       g21   = dynamic_cast<StepParameter*>(this->getParameter("G2.w"));
    StepParameter*       d11   = dynamic_cast<StepParameter*>(this->getParameter("D1.w"));
    StepParameter*       d21   = dynamic_cast<StepParameter*>(this->getParameter("D2.w"));
    StepParameter*       s1    = dynamic_cast<StepParameter*>(this->getParameter("S.w" ));
  
    StepParameter*       w2    = dynamic_cast<StepParameter*>(dp->getParameter("W"));
    StepParameter*       l2    = dynamic_cast<StepParameter*>(dp->getParameter("L"));
    FormFactorParameter* m2    = dynamic_cast<FormFactorParameter*>(dp->getParameter("M"));
    ChoiceParameter*     ls2   = dynamic_cast<ChoiceParameter*>(dp->getParameter("Layout Styles"));
    StepParameter*       nerc2 = dynamic_cast<StepParameter*>(dp->getParameter("NERC"));
    StepParameter*       nirc2 = dynamic_cast<StepParameter*>(dp->getParameter("NIRC"));
    StepParameter*       b2    = dynamic_cast<StepParameter*>(dp->getParameter("B.w" ));
    StepParameter*       g12   = dynamic_cast<StepParameter*>(dp->getParameter("G1.w"));
    StepParameter*       g22   = dynamic_cast<StepParameter*>(dp->getParameter("G2.w"));
    StepParameter*       d12   = dynamic_cast<StepParameter*>(dp->getParameter("D1.w"));
    StepParameter*       d22   = dynamic_cast<StepParameter*>(dp->getParameter("D2.w"));
    StepParameter*       s2    = dynamic_cast<StepParameter*>(dp->getParameter("S.w" ));
  
    if (w1->getValue() != w2->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): W are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false;
    } if (l1->getValue() != l2->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): L are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false; 
    } if (m1->getValue() != m2->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): M are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false;
    } if (ls1->getValue() != ls2->getValue()){ 
      cerr << Warning( "DifferentialPair::isSame(): Layout styles are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false; 
    } if (nerc1->getValue() != nerc2->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): NERC are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false; 
    } if (nirc1->getValue() != nirc2->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): NIRC are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false;
    } if (b1->getValue() != b2->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): B.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false;
    } if (g11->getValue() != g12->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): G1.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false; 
    } if (g21->getValue() != g22->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): G2.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false;
    } if (d11->getValue() != d12->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): D1.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false; 
    } if (d21->getValue() != d22->getValue()){
      cerr << Warning( "DifferentialPair::isSame(): D2.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false;
    } if (s1->getValue() != s2->getValue()) {
      cerr << Warning( "DifferentialPair::isSame(): S.w are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(dp).c_str() );
      return false;
    }
                   
    return true;
  }


  string  DifferentialPair::_getTypeName () const
  { return "DifferentialPair"; }
  

  Record*  DifferentialPair::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_dpBulkConnectedName"  ,  _dpBulkConnectedName   ) );
    record->add( getSlot("_dpBulkUnconnectedName",  _dpBulkUnconnectedName ) );
    return record;
  }


}  // Analog namespace.
