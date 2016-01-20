// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@lip6.fr                   |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/NetInformations.h"     |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_NET_INFORMATIONS_H
#define  HURRICANE_NET_INFORMATIONS_H

#include <vector>
#include <QVariant>
#include "hurricane/Commons.h"


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
      inline  size_t      getRpsCount           () const;
      
    protected:
              QVariant    _netName;
              size_t      _plugsCount;
              size_t      _rpsCount;
  };


  inline size_t  SimpleNetInformations::getPlugsCount () const { return _plugsCount; }
  inline size_t  SimpleNetInformations::getRpsCount   () const { return _rpsCount; }


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


}  // Hurricane namespace.

#endif // HURRICANE_NET_INFORMATIONS_H
