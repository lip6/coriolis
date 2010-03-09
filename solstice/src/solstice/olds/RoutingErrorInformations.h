
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
// |  C++ Header  :       "./RoutingErrorInformations.cpp"           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef  __SOLSTICE_ROUTINGERROR_INFORMATIONS__
#define  __SOLSTICE_ROUTINGERROR_INFORMATIONS__

#include  <vector>

#include  <QVariant>

#include  "hurricane/Commons.h"

#include "solstice/RoutingError.h"

namespace Hurricane {
  
}

namespace Solstice {

  using Hurricane::Name;
  using std::vector;
  class Cell;


// -------------------------------------------------------------------
// Class  :  "RoutingErrorInformations"

  
  class RoutingErrorInformations {
  public:
    RoutingErrorInformations ( const RoutingError* error );
    virtual            ~RoutingErrorInformations ();
    static  int         getColumnCount  ();
    static  QVariant    getColumnName   ( int column );
    virtual QVariant    getColumn       ( int column );
    inline  const RoutingError*  getRoutingError          () const;
    inline  const Name   getName            () const;
    inline  const string getDescription     () const;  
    inline  const Cell* getCell     () const;  
  protected:
    const RoutingError*  _error;
  };
  

  inline const RoutingError* RoutingErrorInformations::getRoutingError () const
  { return _error; }


  inline const Name RoutingErrorInformations::getName () const
  { 
    return _error->getName(); 
  }

  inline const string RoutingErrorInformations::getDescription () const
  { 
    return _error->getDescription(); 
  }

  inline const Cell* RoutingErrorInformations::getCell () const
  { 
    return _error->getCell(); 
  }


// -------------------------------------------------------------------
// Class  :  "RoutingErrorInformationsVector"


  class RoutingErrorInformationsVector {
    public:
      virtual                        ~RoutingErrorInformationsVector  ();
      virtual void                    addRoutingError                         ( const RoutingError* error );
      virtual RoutingErrorInformations*        getRow                         ( int row );
      virtual int                     getColumnCount                 () const             ;
      virtual QVariant                getColumnName                  ( int column ) const ;
      virtual int                     size                           () const             ;


    vector<RoutingErrorInformations>  _errorlist;
  };




} // End of Solstice namespace.


#endif // __ROUTING_ERROR_INFORMATIONS_H__ 
