
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |                 Knik - Global Router                            |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>

#include  "hurricane/Warning.h"
#include  "hurricane/Error.h"
#include  "hurricane/Technology.h"
#include  "hurricane/DataBase.h"
#include  "knik/Configuration.h"


namespace Knik {


  using namespace std;

  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::Technology;
  using Hurricane::DataBase;


// -------------------------------------------------------------------
// class  :  "Knik::Configuration"


  Configuration* Configuration::_singleton = NULL;


  Configuration* Configuration::get ()
  {
    if ( !_singleton ) {
      DataBase*   db         = DataBase::getDB ();
      Technology* technology = db->getTechnology ();

      const Layer* pinMetal = technology->getLayer("metal1");
      if ( !pinMetal ) {
        throw Error("Knik::Configuration() - No \"metal1\" in technology.");
      }

      const Layer* gMetalH = technology->getLayer("gmetalh");
      if ( !gMetalH ) {
        cerr << Warning("Knik::Configuration() - No \"gmetalh\" in technology, falling back to \"metal2\".") << endl;
        gMetalH = technology->getLayer("metal2");
        if ( !gMetalH )
          throw Error("Knik::Configuration() - No \"gmetalh\" nor \"metal2\" in technology.");
      }

      const Layer* gMetalV = technology->getLayer("gmetalv");
      if ( !gMetalV ) {
        cerr << Warning("Knik::Configuration() - No \"gmetalv\" in technology, falling back to \"metal3\".") << endl;
        gMetalV = technology->getLayer("metal3");
        if ( !gMetalV )
          throw Error("Knik::Configuration() - No \"gmetalv\" nor \"metal3\" in technology.");
      }

      const Layer* gContact = technology->getLayer("gcontact");
      if ( !gContact ) {
        cerr << Warning("Knik::Configuration() - No \"gcontact\" in technology, falling back to \"VIA23\".") << endl;
        gContact = technology->getLayer("VIA23");
        if ( !gContact )
          throw Error("Knik::Configuration() - No \"gmetalh\" nor \"VIA23\" in technology.");
      }

      _singleton = new Configuration ( pinMetal, gMetalH, gMetalV, gContact );
    }

    return _singleton;
  }

  void Configuration::destroy() {
      delete ( this );
  }

  const Layer* Configuration::getPinMetal() {
    return get()->_getPinMetal();
  }

  const Layer* Configuration::getGMetalH() {
    return get()->_getGMetalH();
  }

  const Layer* Configuration::getGMetalV() {
    return get()->_getGMetalV();
  }

  const Layer* Configuration::getGContact() {
    return get()->_getGContact();
  }

  Configuration::Configuration(const Layer* pinMetal, const Layer* gMetalH, const Layer* gMetalV, const Layer* gContact)
    : _pinMetal(gMetalH)
    , _gMetalH(gMetalH)
    , _gMetalV(gMetalV)
    , _gContact(gContact)
  { }

}  // End of Knik namespace.
