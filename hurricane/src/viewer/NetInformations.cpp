// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@lip6.fr                   |
// | =============================================================== |
// |  C++ Module  :       "./NetInformations.cpp"                    |
// +-----------------------------------------------------------------+


#include <QObject>
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/viewer/NetInformations.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "NetInformations"


  int  NetInformations::getColumnCount ()
  { return 1; }


  QVariant  NetInformations::getColumnName ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(QObject::tr("Net"));
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  QVariant  NetInformations::getColumn ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(getString(getName()).c_str());
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  NetInformations::NetInformations ( const Net* net )
    : _net(net)
  { }


  NetInformations::~NetInformations ()
  { }


// -------------------------------------------------------------------
// Class  :  "SimpleNetInformations"


  int  SimpleNetInformations::getColumnCount ()
  { return 6; }


  QVariant  SimpleNetInformations::getColumnName ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(QObject::tr("Net"));
      case 1: return QVariant(QObject::tr("Plugs"));
      case 2: return QVariant(QObject::tr("RPs"));
      case 3: return QVariant(QObject::tr("Flags"));
      case 4: return QVariant(QObject::tr("Type"));
      case 5: return QVariant(QObject::tr("Direction"));
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  QVariant  SimpleNetInformations::getColumn ( int column )
  {
    switch ( column ) {
      case 0: return _netName;
      case 1: return (unsigned int)_plugsCount;
      case 2:
        if (_net->isGlobal()) {
          if (not _rpsCount) return "N/A";
          string s = getString(_rpsCount);
          return s.c_str();
        }
        return (unsigned int)_rpsCount;
      case 3: return QString::fromStdString( getString( _net->_getFlagsAsString() ));
      case 4: return QString::fromStdString( getString( _net->getType() ));
      case 5: return QString::fromStdString( getString( _net->getDirection() ));
        
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  SimpleNetInformations::SimpleNetInformations ( const Net* net )
    : NetInformations(net)
    , _netName       (getString(getName()).c_str())
    , _plugsCount    (_net->getPlugs().getSize())
    , _rpsCount      (_net->getRoutingPads().getSize())
  { }


  SimpleNetInformations::~SimpleNetInformations ()
  { }


// -------------------------------------------------------------------
// Class  :  "AbstractNetInformationsVector"


  AbstractNetInformationsVector::~AbstractNetInformationsVector  ()
  { }


} // End of Hurricane namespace.
