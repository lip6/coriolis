// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        S e a b r e e z e  -  Timing Analysis                    |
// |                                                                 |
// |  Author      :                   Vu Hoang Anh PHAM              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./seabreeze/Configuration.h"                   |
// +-----------------------------------------------------------------+

#pragma once
#include <string>
#include <vector>
#include <hurricane/DbU.h>

namespace Seabreeze {

  using std::string;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::DbU;
  using Hurricane::Contact;
  using Hurricane::Segment;


//------------------------------------------------------------------------
// Class : "Seabreeze::Configuration"

  class Configuration {
    public :
                             Configuration   ();
                             Configuration   ( const Configuration& );
                            ~Configuration   ();
      virtual Configuration* clone           () const;
      inline  double         getRct          () const;
      inline  double         getRsm          () const;
      inline  double         getCsm          () const;
    //virtual Record*        _getRecord      () const;
    //virtual string         _getString      () const;
      virtual string         _getTypeName    () const;
    protected :
    // Attributes
      double _Rct;
      double _Rsm;
      double _Csm;
    private :
      Configuration& operator= ( const Configuration& ) = delete;
  };


  inline double  Configuration::getRct () const { return _Rct; }
  inline double  Configuration::getRsm () const { return _Rsm; }
  inline double  Configuration::getCsm () const { return _Csm; }


} // Seabreeze namespace.
