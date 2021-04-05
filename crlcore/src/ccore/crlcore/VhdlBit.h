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
// |  C++ Header  :   "./crlcore/VhdlBit.h"                          |
// +-----------------------------------------------------------------+


#pragma  once
#include <cstddef>
#include <string>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
#include "hurricane/Net.h"


namespace Vhdl {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::PrivateProperty;

  class Signal;
  class ScalarSignal;
  class VectorSignal;
  class BitProperty;


// -------------------------------------------------------------------
// Class  :  "::Bit" (declaration).

  class Bit {
    public:
      static const size_t  nindex;
    public:
                           Bit         ( BitProperty*
                                       , const Net*
                                       , Signal*
                                       , size_t index );
                           Bit         ( size_t index );
                          ~Bit         ();
             bool          isExternal  () const;
             bool          isElement   () const;
      inline BitProperty*  getProperty () const;
      inline const Net*    getNet      () const;
             const Signal* getSignal   () const;
             std::string   getName     () const;
             size_t        getIndex    () const;
      inline void          destroy     ();
             std::string   _getString  () const;
             Record*       _getRecord  () const;
    private:
      static ptrdiff_t  _offset;
             Signal*    _signal;
             size_t     _index;
  };


  struct GreaterBitByIndex {
      inline bool  operator() ( const Bit* lhs, const Bit* rhs ) const
      { return lhs->getIndex() > rhs->getIndex(); }
  };


// -------------------------------------------------------------------
// Class  :  "Vhdl::BitProperty".

  class BitProperty : public Hurricane::PrivateProperty {
      friend class BitExtension;
    public:
      static  Name                _name;
    public:
      static  BitProperty* create          ( Net*    owner
                                           , Signal* signal
                                           , size_t  index );
      static  Name         getPropertyName ();
      virtual Name         getName         () const;
      inline  Bit*         getBit          ();
      virtual void         onReleasedBy    ( DBo* owner );
      virtual std::string  _getTypeName    () const;
      virtual std::string  _getString      () const;
      virtual Record*      _getRecord      () const;
    protected:
    // Attributes.
      Bit  _bit;
      
    protected:
    // Constructor.
      inline  BitProperty ( Net* owner, Signal*, size_t );
  };


  inline      BitProperty::BitProperty ( Net*    owner
                                       , Signal* signal
                                       , size_t  index
                                       )
    : PrivateProperty(), _bit(this,owner,signal,index)
  { }

  inline Bit* BitProperty::getBit () { return &_bit; }


// -------------------------------------------------------------------
// Class  :  "Vhdl::BitExtension".

  class BitExtension {
    public:
      static inline  bool          isElement ( const Net* );
      static inline  const Signal* getSignal ( const Net* );
      static inline  size_t        getIndex  ( const Net* );
      static         Bit*          get       ( const Net* );
      static         Bit*          create    ( Net*, Signal*, size_t index=Bit::nindex );
    private:
      static const Net* _owner;
      static Bit*       _cache;
  };


  inline bool  BitExtension::isElement ( const Net* net )
  {
    Bit* bit = get( net );
    return (bit == NULL) ? false : bit->isElement();
  }


  inline const Signal* BitExtension::getSignal ( const Net* net )
  {
    Bit* bit = get( net );
    return (bit == NULL) ? NULL : bit->getSignal();
  }


  inline size_t  BitExtension::getIndex ( const Net* net )
  {
    Bit* bit = get( net );
    return (bit == NULL) ? false : bit->getIndex();
  }


  inline BitProperty* Bit::getProperty () const { return (BitProperty*)((ptrdiff_t)(this) - _offset); }
  inline const Net*   Bit::getNet      () const { return (const Net*)getProperty()->getOwner(); }
  inline void         Bit::destroy     () { ((Net*)getProperty()->getOwner())->remove( getProperty() ); }


}  // Vhdl Namespace.


INSPECTOR_P_SUPPORT(Vhdl::Bit);
