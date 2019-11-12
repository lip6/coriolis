// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Resistor.cpp"                                |
// +-----------------------------------------------------------------+


#include <bitset>
#include <string>
#include "hurricane/Warning.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/MetaResistor.h"
#include "hurricane/analog/Resistor.h"

namespace Analog {


  const Hurricane::Name  Resistor::_resistorName ( "Resistor" );


  Resistor::Resistor ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
    , _metaResistor(NULL)
  { }
  
  
  Resistor* Resistor::create ( Library* library, const Name& name, const Type& type )
  {
    preCreate( _resistorName );
  
    UpdateSession::open();
    Resistor* tr = new Resistor( library, name, type );
    tr->_postCreate( _resistorName );
    tr->createConnections();
    tr->setTerminal( true );
    UpdateSession::close();
    return tr;
  }
  
  
  void  Resistor::_postCreate ( const Name& deviceName )
  {
    Super::_postCreate( deviceName );
  }
  
  
  void  Resistor::createConnections ()
  {
    Net* pin1 = Net::create( this, Name("PIN1") );
    pin1->setExternal(true);
  
    Net* pin2 = Net::create( this, Name("PIN2") );
    pin2->setExternal(true);
  
    _metaResistor = MetaResistor::create( getSubDevicesLibrary(), Name("R1") );
    Instance* metaResistorIns = Instance::create( this, Name("R1Instance"), _metaResistor );
  
    setReferenceResistor( _metaResistor );
  
    Plug* mrPin1Plug = metaResistorIns->getPlug( _metaResistor->getPin1() );
    mrPin1Plug->setNet( pin1 );
    Plug* mrPin2Plug = metaResistorIns->getPlug( _metaResistor->getPin2() );
    mrPin2Plug->setNet( pin2 );
  }
  
  
  Name  Resistor::getDeviceName () const
  {
    return _resistorName;
  }
  
  
  unsigned int  Resistor::getRestriction ( Hurricane::Net* net ) const
  {
    Name namePin1 = getNet("PIN1")->getName();
    Name namePin2 = getNet("PIN2")->getName();
  
  //unsigned int ok    = 0x1;
    unsigned int yes   = 0x2;
    unsigned int west  = 0;
    unsigned int east  = 2;
    unsigned int south = 4;
    unsigned int north = 6;
    unsigned int rule  = 0;

    if (net->getName() == namePin1) {
      rule = (yes << south) | (yes << east) | (yes << west);
    } else {
      if (net->getName() == namePin2) {
        rule = (yes << north) | (yes << east) | (yes << west);
      } else {
        cerr << Error( "Resistor::getRestriction(): Resistor device do not have Net named \"%s\"."
                     , getString(net->getName()).c_str()
                     ) << endl;
      }
    }

    return rule;
  }
  
  
  bool  Resistor::isSame ( Resistor* r )
  {
    StepParameter* w1 = dynamic_cast<StepParameter*>(getParameter("W"));
    StepParameter* l1 = dynamic_cast<StepParameter*>(getParameter("L"));
  
    StepParameter* w2 = dynamic_cast<StepParameter*>(r->getParameter("W"));
    StepParameter* l2 = dynamic_cast<StepParameter*>(r->getParameter("L"));
  
    if (w1->getValue() != w2->getValue()){
      cerr << Warning( "Resistor::isSame(): W are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(r).c_str() );
      return false;
    } if (l1->getValue() != l2->getValue()){
      cerr << Warning( "Resistor::isSame(): L are different \"%s\" vs \"%s\"."
                     , getString(this).c_str(), getString(r).c_str() );
      return false; 
    }
                   
    return true;
  }


}  // Analog namespace.
