
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
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingErrorInformations.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QObject>

#include  "hurricane/Name.h"
#include  "solstice/RoutingError.h"
#include  "solstice/RoutingErrorInformations.h"


namespace Solstice {


// -------------------------------------------------------------------
// Class  :  "RoutingErrorInformations"


  int  RoutingErrorInformations::getColumnCount ()
  { return 1; }


  QVariant  RoutingErrorInformations::getColumnName ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(QObject::tr("RoutingError"));
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  QVariant  RoutingErrorInformations::getColumn ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(getString(getName()).c_str());
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  RoutingErrorInformations::RoutingErrorInformations ( const RoutingError* routingError )
    : _routingError(routingError)
  { }


  RoutingErrorInformations::~RoutingErrorInformations ()
  { }


// -------------------------------------------------------------------
// Class  :  "SimpleRoutingErrorInformations"

  string SimpleRoutingErrorInformations::getDescription() const {
    return _routingError->getErrorDescription();
  }



  int  SimpleRoutingErrorInformations::getColumnCount ()
  { return 3; }


  QVariant  SimpleRoutingErrorInformations::getColumnName ( int column )
  {
    switch ( column ) {
    case 0: return QVariant(QObject::tr("RoutingError"));
    case 1: return QVariant(QObject::tr("Equipotentials"));
    case 2: return QVariant(QObject::tr("Description"));
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  QVariant  SimpleRoutingErrorInformations::getColumn ( int column )
  {
    switch ( column ) {
    case 0: return QVariant(getString(getName()).c_str());
    case 1: return QVariant((unsigned int)_routingError->getCount());
    case 2: return QVariant(getString( getDescription()).c_str());
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  SimpleRoutingErrorInformations::SimpleRoutingErrorInformations ( const RoutingError* routingError )
    : RoutingErrorInformations(routingError)
    , _plugsCount(_routingError->getAllOccurrences()->size())
  { }


  SimpleRoutingErrorInformations::~SimpleRoutingErrorInformations ()
  { }


// -------------------------------------------------------------------
// Class  :  "AbstractRoutingErrorInformationsVector"


  AbstractRoutingErrorInformationsVector::~AbstractRoutingErrorInformationsVector  ()
  { }


} // End of Hurricane namespace.
