
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Header  :       "./RoutingErrorInformations.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __ROUTINGERROR_INFORMATIONS_H__
#define  __ROUTINGERROR_INFORMATIONS_H__

#include  <vector>

#include  <QVariant>

#include  "hurricane/Commons.h"

namespace Hurricane {

  class Cell;
  class Name;

}

namespace Solstice {

  using Hurricane::Name;

  class RoutingError;



// -------------------------------------------------------------------
// Class  :  "RoutingErrorInformations"


  class RoutingErrorInformations {
    public:
                          RoutingErrorInformations ( const RoutingError* routingError );
      virtual            ~RoutingErrorInformations ();
      static  int         getColumnCount  ();
      static  QVariant    getColumnName   ( int column );
      virtual QVariant    getColumn       ( int column );
      inline  const RoutingError*  getRoutingError          () const;
      inline  const Name getName         () const;
      
    protected:
              const RoutingError*  _routingError;
  };


  inline const RoutingError* RoutingErrorInformations::getRoutingError () const
  { return _routingError; }


  inline const Name RoutingErrorInformations::getName () const
  { return _routingError->getErrorName(); }


// -------------------------------------------------------------------
// Class  :  "SimpleRoutingErrorInformations"


  class SimpleRoutingErrorInformations : public RoutingErrorInformations {
    public:
                          SimpleRoutingErrorInformations ( const RoutingError* routingError );
      virtual            ~SimpleRoutingErrorInformations ();
    static  int         getColumnCount        ();
    static  QVariant    getColumnName         ( int column );
    virtual QVariant    getColumn             ( int column );
    inline  size_t      getPlugsCount         () const;
    /**/    string      getDescription        () const;
      
    protected:
              size_t      _plugsCount;
  };


  inline size_t  SimpleRoutingErrorInformations::getPlugsCount () const
  { return _plugsCount; }


// -------------------------------------------------------------------
// Class  :  "AbstractRoutingErrorInformationsVector"


  class AbstractRoutingErrorInformationsVector {
    public:
      virtual                        ~AbstractRoutingErrorInformationsVector  ();
      virtual void                    addRoutingError                         ( const RoutingError* routingError )   = 0;
      virtual RoutingErrorInformations*        getRow                         ( int row )          = 0;
      virtual int                     getColumnCount                 () const             = 0;
      virtual QVariant                getColumnName                  ( int column ) const = 0;
      virtual int                     size                           () const             = 0;
  };


// -------------------------------------------------------------------
// Template Class  :  "RoutingErrorInformationsVector"


  template<typename InformationType>
  class RoutingErrorInformationsVector : public AbstractRoutingErrorInformationsVector {
    public:
      virtual                        ~RoutingErrorInformationsVector ();
      virtual void                    addRoutingError                ( const RoutingError* routingError );
      virtual InformationType*        getRow                ( int row );
      virtual int                     getColumnCount        () const;
      virtual QVariant                getColumnName         ( int column ) const;
      virtual int                     size                  () const;
    protected:
      vector<InformationType>  _routingErrorlist;
  };


  template<typename InformationType>
  RoutingErrorInformationsVector<InformationType>::~RoutingErrorInformationsVector ()
  { }


  template<typename InformationType>
  void  RoutingErrorInformationsVector<InformationType>::addRoutingError ( const RoutingError* routingError )
  {
    _routingErrorlist.push_back ( InformationType(routingError) );
  }


  template<typename InformationType>
  InformationType* RoutingErrorInformationsVector<InformationType>::getRow ( int row )
  {
    if ( row >= (int)_routingErrorlist.size() ) return NULL;

    return &_routingErrorlist[row];
  }


  template<typename InformationType>
  int  RoutingErrorInformationsVector<InformationType>::getColumnCount () const
  {
    return InformationType::getColumnCount();
  }


  template<typename InformationType>
  QVariant  RoutingErrorInformationsVector<InformationType>::getColumnName ( int column ) const
  {
    return InformationType::getColumnName ( column );
  }


  template<typename InformationType>
  int  RoutingErrorInformationsVector<InformationType>::size () const
  {
    return _routingErrorlist.size();
  }


} // End of Hurricane namespace.


#endif // __ROUTINGERROR_INFORMATIONS_H__ 
