// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         S P I C E  / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./crlcore/SpiceBit.h"                         |
// +-----------------------------------------------------------------+


#pragma  once
#include <cstddef>
#include <string>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
#include "hurricane/Net.h"


namespace Spice {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::PrivateProperty;
  class BitProperty;


// -------------------------------------------------------------------
// Class  :  "::Bit" (declaration).

  class Bit {
    public:
      static const size_t  nindex;
    public:
                           Bit         ( BitProperty*, const Net*, size_t index, std::string name="" );
                           Bit         ( size_t index, std::string name="" );
                          ~Bit         ();
             bool          isExternal  () const;
      inline BitProperty*  getProperty () const;
      inline const Net*    getNet      () const;
      inline std::string   getName     () const;
             size_t        getIndex    () const;
      inline void          destroy     ();
             std::string   _getString  () const;
             Record*       _getRecord  () const;
    private:
      static ptrdiff_t    _offset;
             size_t       _index;
             std::string  _name;
  };


  inline std::string   Bit::getName () const { return _name; }


  struct GreaterBitByIndex {
      inline bool  operator() ( const Bit* lhs, const Bit* rhs ) const
      { return lhs->getIndex() > rhs->getIndex(); }
  };


// -------------------------------------------------------------------
// Class  :  "Spice::BitProperty".

  class BitProperty : public Hurricane::PrivateProperty {
      friend class BitExtension;
    public:
      static  Name                _name;
    public:
      static  BitProperty* create          ( Net* owner, size_t index );
      static  Name         getPropertyName ();
      virtual Name         getName         () const;
      inline  Bit*         getBit          ();
      inline  Net*         getNet          ();
      inline  std::string  getBitName      () const;
      virtual void         onReleasedBy    ( DBo* owner );
      virtual std::string  _getTypeName    () const;
      virtual std::string  _getString      () const;
      virtual Record*      _getRecord      () const;
    protected:
    // Attributes.
      Bit  _bit;
    protected:
    // Constructor.
      inline  BitProperty ( Net* owner, size_t );
  };


  inline BitProperty::BitProperty ( Net* owner, size_t index )
    : PrivateProperty(), _bit(this,owner,index)
  { }

  inline Net*         BitProperty::getNet     () { return const_cast<Net*>( _bit.getNet() ); }
  inline Bit*         BitProperty::getBit     () { return &_bit; }
  inline std::string  BitProperty::getBitName () const { return _bit.getName(); }


// -------------------------------------------------------------------
// Class  :  "Spice::BitExtension".

  class BitExtension {
    public:
      static inline  size_t        getIndex    ( const Net* );
      static inline  std::string   getName     ( const Net* );
      static         Bit*          get         ( const Net* );
      static         void          remove      ( const Net* );
      static         Bit*          create      ( Net*, size_t index=Bit::nindex );
      static inline  void          clearCache  ( Net* );
    private:
      static const Net* _owner;
      static Bit*       _cache;
  };


  inline size_t  BitExtension::getIndex ( const Net* net )
  {
    Bit* bit = get( net );
    return (bit == NULL) ? false : bit->getIndex();
  }

  inline std::string  BitExtension::getName ( const Net* net )
  {
    Bit* bit = get( net );
    return (bit == NULL) ? "?" : bit->getName();
  }

  inline void  BitExtension::clearCache ( Net* net )
  { if (_owner == net) { _owner = NULL; _cache = NULL; } }

  
  inline BitProperty* Bit::getProperty () const { return (BitProperty*)((ptrdiff_t)(this) - _offset); }
  inline const Net*   Bit::getNet      () const { return (const Net*)getProperty()->getOwner(); }
  inline void         Bit::destroy     () { ((Net*)getProperty()->getOwner())->remove( getProperty() ); }


}  // Spice Namespace.


INSPECTOR_P_SUPPORT(Spice::Bit);
