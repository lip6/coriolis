#include <iostream>
#include <iomanip>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Contact.h"
#include "hurricane/Net.h"
#include "hurricane/Segment.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "Seabreeze/Configuration.h"

namespace Seabreeze {

  using std::string;
  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::Technology;
  using Hurricane::RoutingPad;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using Hurricane::Cell;
  using Hurricane::Net;
  using Hurricane::DataBase;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::DbU;

//------------------------------------------------------------------------
// Class : "Seabreeze::Configuration"

  Configuration::Configuration () 
    : _Rct (1)
    , _Rsm (1)
    , _Csm (1)
  {}

  Configuration::~Configuration () 
  {}

  Configuration::Configuration ( const Configuration& other )
    : _Rct (other._Rct)
    , _Rsm (other._Rsm)
    , _Csm (other._Csm)
  {}

  Configuration* Configuration::clone () const
  { return new Configuration(*this); }

  double Configuration::getRct () 
  {
    return _Rct;
  }

  double Configuration::getRsm ()
  {
    return _Rsm;
  }

  double Configuration::getCsm ()
  {
    return _Csm;
  }
/*
  Record* Configuration::_getRecord () const 
  {

  }

  string Configuration::_getString () const
  {
    
  }
*/
  string Configuration::_getTypeName () const
  {
    return "Configuration";
  }
}
