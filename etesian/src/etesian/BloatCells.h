// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./etesian/BloatCells.h"                   |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include "hurricane/Box.h"
#include "hurricane/Cell.h"


namespace Etesian {

  using Hurricane::Box;
  using Hurricane::Cell;
  using Hurricane::Occurrence;
  class EtesianEngine;


  class BloatCell {
    public:
      class Compare {
        public:
          inline bool operator() ( const BloatCell* lhs, BloatCell*  rhs ) const;  
          inline bool operator() ( std::string      lhs, BloatCell*  rhs ) const;  
          inline bool operator() ( const BloatCell* lhs, std::string rhs ) const;  
      };
    public:
                           BloatCell ( std::string );
      virtual             ~BloatCell ();
      virtual DbU::Unit    getDx     ( const Cell*, const EtesianEngine* ) const = 0;
      inline  std::string  getName   () const;
    private:
      std::string  _name;
  };


  inline bool BloatCell::Compare::operator() ( const BloatCell* lhs, BloatCell* rhs ) const
  { return (lhs->getName() < rhs->getName()); }

  inline bool BloatCell::Compare::operator() ( std::string      lhs, BloatCell*  rhs ) const
  { return (lhs < rhs->getName()); }
      
  inline bool BloatCell::Compare::operator() ( const BloatCell* lhs, std::string rhs ) const
  { return (lhs->getName() < rhs); }


  inline  std::string  BloatCell::getName () const { return _name; }


  class BloatKey : public BloatCell {
    public:
                         BloatKey ( std::string );
      virtual           ~BloatKey ();
      virtual DbU::Unit  getDx    ( const Cell*, const EtesianEngine* ) const;
  };


  class BloatDisabled : public BloatCell {
    public:
                         BloatDisabled ();
      virtual           ~BloatDisabled ();
      virtual DbU::Unit  getDx         ( const Cell*, const EtesianEngine* ) const;
  };


  class BloatNsxlib : public BloatCell {
    public:
                         BloatNsxlib ();
      virtual           ~BloatNsxlib ();
      virtual DbU::Unit  getDx       ( const Cell*, const EtesianEngine* ) const;
  };


  class BloatFlexlib : public BloatCell {
    public:
                         BloatFlexlib ();
      virtual           ~BloatFlexlib ();
      virtual DbU::Unit  getDx        ( const Cell*, const EtesianEngine* ) const;
  };


  class Bloat3Metals : public BloatCell {
    public:
                         Bloat3Metals ();
      virtual           ~Bloat3Metals ();
      virtual DbU::Unit  getDx        ( const Cell*, const EtesianEngine* ) const;
  };


  class BloatChannel : public BloatCell {
    public:
                         BloatChannel ();
      virtual           ~BloatChannel ();
      virtual DbU::Unit  getDx        ( const Cell*, const EtesianEngine* ) const;
  };


  class Bloat90Percents : public BloatCell {
    public:
                         Bloat90Percents ();
      virtual           ~Bloat90Percents ();
      virtual DbU::Unit  getDx           ( const Cell*, const EtesianEngine* ) const;
  };
  

  class BloatCells {
    public:
      inline                     BloatCells            ( EtesianEngine* );
      inline                    ~BloatCells            ();
                   bool          select                ( std::string );
                   Box           getAb                 ( Occurrence );
      inline const BloatCell*    getSelected           () const;
      inline       DbU::Unit     getDxSpace            () const;
      inline       void          resetDxSpace          ();
    private:
      EtesianEngine*                           _etesian;
      BloatCell*                               _selected;
      std::set<BloatCell*,BloatCell::Compare>  _bloatCells;
      DbU::Unit                                _dxSpace;
  };


  inline  BloatCells::BloatCells ( EtesianEngine* etesian )
    : _etesian   (etesian)
    , _selected  (NULL)
    , _bloatCells()
    , _dxSpace   (0)
  {
    _bloatCells.insert( new BloatDisabled  () );
    _bloatCells.insert( new BloatNsxlib    () );
    _bloatCells.insert( new BloatFlexlib   () );
    _bloatCells.insert( new Bloat3Metals   () );
    _bloatCells.insert( new BloatChannel   () );
    _bloatCells.insert( new Bloat90Percents() );
    select( "disabled" );
  }


  inline  BloatCells::~BloatCells ()
  {
    for ( BloatCell* bloat : _bloatCells ) delete bloat;
  }

  
  inline       DbU::Unit  BloatCells::getDxSpace   () const { return _dxSpace; }
  inline       void       BloatCells::resetDxSpace () { _dxSpace = 0; }
  inline const BloatCell* BloatCells::getSelected  () const { return _selected; }


} // Etesian namespace.
