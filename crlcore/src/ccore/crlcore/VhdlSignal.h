// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Alliance VHDL / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./crlcore/VhdlSignal.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include "hurricane/Net.h"
#include "crlcore/VhdlBit.h"


namespace Vhdl {

  using Hurricane::Net;


// -------------------------------------------------------------------
// Class  :  "Vhdl::Signal".

  class Signal {
    public:
      enum Flag { Contiguous = 0x0001 };
    public:
                              Signal       ( std::string name );
      virtual                ~Signal       ();
      virtual bool            isScalar     () const = 0;
      virtual bool            isVector     () const = 0;
      virtual bool            isExternal   () const = 0;
      virtual bool            isContiguous () const = 0;
      virtual size_t          getSize      () const = 0;
      inline  std::string     getName      () const;
      virtual const Bit*      getBit       () const = 0;
      virtual const Bit*      getBit       ( size_t ) const = 0;
      virtual const Net*      getNet       () const = 0;
      virtual const Net*      getNet       ( size_t index ) const = 0;
      virtual Net::Direction  getDirection () const = 0;
      virtual size_t          getMin       () const = 0;
      virtual size_t          getMax       () const = 0;
      virtual void            toVhdlPort   ( std::ostream&, int width, unsigned int flags ) const = 0;
      virtual std::string     _getString   () const = 0;
      virtual Record*         _getRecord   () const;
    protected:
      static  void            _toVhdlPort  ( std::ostream&
                                           , int            width
                                           , unsigned int   flags
                                           , std::string    name
                                           , std::string    range
                                           , Net::Direction );
    protected:
      std::string  _name;
  };


  struct GreaterSignalByName {
      inline bool  operator() ( const Signal* lhs, const Signal* rhs ) const
      { return lhs->getName() < rhs->getName(); }
  };


// -------------------------------------------------------------------
// Class  :  "Vhdl::ScalarSignal".

  class ScalarSignal : public Signal {
    public:
                              ScalarSignal  ( std::string vhdlName, Net* );
      virtual                ~ScalarSignal  ();
      virtual bool            isScalar      () const;
      virtual bool            isVector      () const;
      virtual bool            isExternal    () const;
      virtual bool            isContiguous  () const;
      virtual size_t          getSize       () const;
      virtual const Bit*      getBit        () const;
      virtual const Bit*      getBit        ( size_t ) const;
      virtual const Net*      getNet        () const;
      virtual const Net*      getNet        ( size_t ) const;
      virtual size_t          getMin        () const;
      virtual size_t          getMax        () const;
      virtual Net::Direction  getDirection  () const;
      virtual void            toVhdlPort    ( std::ostream&, int width, unsigned int flags ) const;
      virtual std::string     _getString    () const;
      virtual Record*         _getRecord    () const;
    protected:
      Bit* _bit;
  };
  

// -------------------------------------------------------------------
// Class  :  "Vhdl::VectorSignal".

  class VectorSignal : public Signal {
    public:
                               VectorSignal ( std::string );
      virtual                 ~VectorSignal ();
      virtual bool             isScalar     () const;
      virtual bool             isVector     () const;
      virtual bool             isExternal   () const;
      virtual bool             isContiguous () const;
      virtual size_t           getSize      () const;
      virtual const Bit*       getBit       () const;
      virtual const Bit*       getBit       ( size_t ) const;
      virtual const Net*       getNet       () const;
      virtual const Net*       getNet       ( size_t ) const;
      virtual Net::Direction   getDirection () const;
      virtual size_t           getMin       () const;
      virtual size_t           getMax       () const;
              bool             addNet       ( size_t index, Net* );
      virtual void             toVhdlPort   ( std::ostream&, int width, unsigned int flags ) const;
      virtual std::string     _getString    () const;
      virtual Record*         _getRecord    () const;
    protected:
      std::set<Bit*,GreaterBitByIndex>  _bits;
      Net::Direction                    _direction;
      unsigned int                      _flags;
  };
  

// -------------------------------------------------------------------
// Inline methods.

  inline std::string  Signal::getName () const { return _name; }

}


INSPECTOR_P_SUPPORT(Vhdl::Signal);
INSPECTOR_P_SUPPORT(Vhdl::ScalarSignal);
INSPECTOR_P_SUPPORT(Vhdl::VectorSignal);
