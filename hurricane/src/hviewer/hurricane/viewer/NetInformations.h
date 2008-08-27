
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
// |  C++ Header  :       "./NetInformations.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __NET_INFORMATIONS_H__
#define  __NET_INFORMATIONS_H__

#include  <vector>

#include  <QVariant>

#include  "hurricane/Commons.h"


namespace Hurricane {


  class Net;
  class Cell;


// -------------------------------------------------------------------
// Class  :  "NetInformations"


  class NetInformations {
    public:
                          NetInformations ( const Net* net );
      virtual            ~NetInformations ();
      static  int         getColumnCount  ();
      static  QVariant    getColumnName   ( int column );
      virtual QVariant    getColumn       ( int column );
      inline  const Net*  getNet          () const;
      inline  const Name& getName         () const;
      
    protected:
              const Net*  _net;
  };


  inline const Net* NetInformations::getNet () const
  { return _net; }


  inline const Name& NetInformations::getName () const
  { return _net->getName(); }


// -------------------------------------------------------------------
// Class  :  "SimpleNetInformations"


  class SimpleNetInformations : public NetInformations {
    public:
                          SimpleNetInformations ( const Net* net );
      virtual            ~SimpleNetInformations ();
      static  int         getColumnCount        ();
      static  QVariant    getColumnName         ( int column );
      virtual QVariant    getColumn             ( int column );
      inline  size_t      getPlugsCount         () const;
      
    protected:
              size_t      _plugsCount;
  };


  inline size_t  SimpleNetInformations::getPlugsCount () const
  { return _plugsCount; }


// -------------------------------------------------------------------
// Class  :  "AbstractNetInformationsVector"


  class AbstractNetInformationsVector {
    public:
      virtual                        ~AbstractNetInformationsVector  ();
      virtual void                    addNet                         ( const Net* net )   = 0;
      virtual NetInformations*        getRow                         ( int row )          = 0;
      virtual int                     getColumnCount                 () const             = 0;
      virtual QVariant                getColumnName                  ( int column ) const = 0;
      virtual int                     size                           () const             = 0;
  };


// -------------------------------------------------------------------
// Template Class  :  "NetInformationsVector"


  template<typename InformationType>
  class NetInformationsVector : public AbstractNetInformationsVector {
    public:
      virtual                        ~NetInformationsVector ();
      virtual void                    addNet                ( const Net* net );
      virtual InformationType*        getRow                ( int row );
      virtual int                     getColumnCount        () const;
      virtual QVariant                getColumnName         ( int column ) const;
      virtual int                     size                  () const;
    protected:
      vector<InformationType>  _netlist;
  };


  template<typename InformationType>
  NetInformationsVector<InformationType>::~NetInformationsVector ()
  { }


  template<typename InformationType>
  void  NetInformationsVector<InformationType>::addNet ( const Net* net )
  {
    _netlist.push_back ( InformationType(net) );
  }


  template<typename InformationType>
  InformationType* NetInformationsVector<InformationType>::getRow ( int row )
  {
    if ( row >= (int)_netlist.size() ) return NULL;

    return &_netlist[row];
  }


  template<typename InformationType>
  int  NetInformationsVector<InformationType>::getColumnCount () const
  {
    return InformationType::getColumnCount();
  }


  template<typename InformationType>
  QVariant  NetInformationsVector<InformationType>::getColumnName ( int column ) const
  {
    return InformationType::getColumnName ( column );
  }


  template<typename InformationType>
  int  NetInformationsVector<InformationType>::size () const
  {
    return _netlist.size();
  }


} // End of Hurricane namespace.


#endif // __NET_INFORMATIONS_H__ 
