
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicSolsticeEngine.cpp"              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


	#include  <QObject>

	#include  "hurricane/Name.h"
	#include  "hurricane/Net.h"
	#include  "hurricane/Cell.h"
	#include  "solstice/RoutingErrorInformations.h"


	namespace Solstice {


	// -------------------------------------------------------------------
	// Class  :  "RoutingErrorInformations"



	  RoutingErrorInformations::RoutingErrorInformations ( const RoutingError* error )
	    : _error(error)
	  { }


	  RoutingErrorInformations::~RoutingErrorInformations ()
	  { }


	// -------------------------------------------------------------------
	// Class  :  "SimpleRoutingErrorInformations"


	  int  RoutingErrorInformations::getColumnCount ()
	  { return 3; }


	  QVariant  RoutingErrorInformations::getColumnName ( int column )
	  {
	    switch ( column ) {
	      case 0: return QVariant(QObject::tr("Cell"));
	      case 1: return QVariant(QObject::tr("Type"));
	      case 2: return QVariant(QObject::tr("Description"));
	    }
	    return QVariant(QObject::tr("Column Out of Bound"));
	  }


	  QVariant  RoutingErrorInformations::getColumn ( int column )
	  {
	    switch ( column ) {
	    case 0: return QVariant(getString(getCell()->getName()).c_str());
	      case 1: return QVariant(getString(getName()).c_str());
	      case 2: return QVariant(getString(getDescription()).c_str());
	    }
	    return QVariant(QObject::tr("Column Out of Bound"));
	  }




  RoutingErrorInformationsVector::~RoutingErrorInformationsVector ()
  { }



  void  RoutingErrorInformationsVector::addRoutingError  ( const RoutingError* error )
  {
    _errorlist.push_back ( (error) );
  }



  RoutingErrorInformations* RoutingErrorInformationsVector::getRow ( int row )
  {
    if ( row >= (int)_errorlist.size() ) return NULL;

    return &_errorlist[row];
  }



  int  RoutingErrorInformationsVector::getColumnCount () const
  {
    return RoutingErrorInformations::getColumnCount();
  }



  QVariant  RoutingErrorInformationsVector::getColumnName ( int column ) const
  {
    return RoutingErrorInformations::getColumnName ( column );
  }



  int  RoutingErrorInformationsVector::size () const
  {
    return _errorlist.size();
  }









	} // End of Solstice namespace.
