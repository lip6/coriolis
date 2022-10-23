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
// |  C++ Module  :       "./BloatCells.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "etesian/BloatProperty.h"
#include "etesian/EtesianEngine.h"


namespace Etesian {

  using std::cerr;
  using std::endl;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DbU;


  BloatCell::BloatCell ( std::string name )
    : _name(name)
  { }


  BloatCell::~BloatCell ()
  { }


  BloatKey::BloatKey ( string key )
    : BloatCell(key)
  { }


  BloatKey::~BloatKey ()
  { }


  DbU::Unit  BloatKey::getDx ( const Cell*, const EtesianEngine* ) const
  {
    cerr << Error( "BloatKey::getAb() must never be called (\"%s\").", getName().c_str() ) << endl;
    return 0;
  }  


  BloatDisabled::BloatDisabled ()
    : BloatCell("disabled")
  { }


  BloatDisabled::~BloatDisabled ()
  { }


  DbU::Unit  BloatDisabled::getDx ( const Cell* cell, const EtesianEngine* ) const
  { return 0; }  


  BloatNsxlib::BloatNsxlib ()
    : BloatCell("nsxlib")
  { }


  BloatNsxlib::~BloatNsxlib ()
  { }


  DbU::Unit  BloatNsxlib::getDx ( const Cell* cell, const EtesianEngine* etesian ) const
  {
    Box ab ( cell->getAbutmentBox() );
    DbU::Unit vpitch = etesian->getSliceStep();;
    int       xsize  = (ab.getWidth() + vpitch - 1) / vpitch;

    if (xsize < 6) return vpitch*4;
    if (xsize < 8) return vpitch*2;
    
    return 0;
  }  


  BloatFlexlib::BloatFlexlib ()
    : BloatCell("Flexlib")
  { }


  BloatFlexlib::~BloatFlexlib ()
  { }


  DbU::Unit  BloatFlexlib::getDx ( const Cell* cell, const EtesianEngine* etesian ) const
  {
    Box ab ( cell->getAbutmentBox() );
    DbU::Unit vpitch = etesian->getSliceStep();;
    int       xsize  = (ab.getWidth() + vpitch - 1) / vpitch;

    if (cell->getName() == "mx2_x2"  ) return 2*vpitch;
    if (cell->getName() == "mx3_x2"  ) return 2*vpitch;
    if (cell->getName() == "sff1r_x4") return   vpitch;
    if (xsize < 6) return vpitch*2;
    
    return 0;
  }  


  Bloat3Metals::Bloat3Metals ()
    : BloatCell("3metals")
  { }


  Bloat3Metals::~Bloat3Metals ()
  { }


  DbU::Unit  Bloat3Metals::getDx ( const Cell* cell, const EtesianEngine* etesian ) const
  {
    int terminals = 0;
    for ( Net* net : cell->getNets() ) {
      if (net->isExternal() and not net->isPower()) ++terminals;
    }

    Box ab ( cell->getAbutmentBox() );
    DbU::Unit vpitch = etesian->getSliceStep();;
    int       xsize  = (ab.getWidth() + vpitch - 1) / vpitch;

    // float termRatio = (float)terminals / (float)(ab.getWidth() / vpitch);
    // if (termRatio > 0.5) {
    //   return vpitch*6;
    // }

    if (xsize < 4) return vpitch*4;
    if (xsize < 6) return vpitch*2;
    if (xsize < 8) return vpitch*1;
    
    return 0;
  }  


  BloatChannel::BloatChannel ()
    : BloatCell("channel")
  { }


  BloatChannel::~BloatChannel ()
  { }


  DbU::Unit  BloatChannel::getDx ( const Cell* cell, const EtesianEngine* etesian ) const
  {
    int terminals = 0;
    for ( Net* net : cell->getNets() ) {
      if (net->isExternal() and not net->isPower()) ++terminals;
    }

    Box ab ( cell->getAbutmentBox() );
    DbU::Unit vpitch = etesian->getSliceStep();;
    int       xsize  = (ab.getWidth() + vpitch - 1) / vpitch;

    // float termRatio = (float)terminals / (float)(ab.getWidth() / vpitch);
    // if (termRatio > 0.5) {
    //   return vpitch*6;
    // }

    // if (xsize < 4) return vpitch*4;
    // if (xsize < 6) return vpitch*2;
    // if (xsize < 8) return vpitch*1;
    
    return vpitch*3;
  }  


  Bloat90Percents::Bloat90Percents ()
    : BloatCell("90%")
  { }


  Bloat90Percents::~Bloat90Percents ()
  { }


  DbU::Unit  Bloat90Percents::getDx ( const Cell* cell, const EtesianEngine* etesian ) const
  {
    int terminals = 0;
    for ( Net* net : cell->getNets() ) {
      if (net->isExternal() and not net->isPower()) ++terminals;
    }

    Box ab ( cell->getAbutmentBox() );
    DbU::Unit vpitch = etesian->getSliceStep();;
    int       xsize  = (ab.getWidth() + vpitch - 1) / vpitch;

    // float termRatio = (float)terminals / (float)(ab.getWidth() / vpitch);
    // if (termRatio > 0.5) {
    //   return vpitch*6;
    // }

    if (xsize < 4) return vpitch*11;
    if (xsize < 6) return vpitch*8;
    if (xsize < 8) return vpitch*6;
    
    return vpitch*3;
  }  


  bool  BloatCells::select ( std::string profile )
  {
    BloatKey key ( profile );
    
    auto ibloat = _bloatCells.find( &key );
    if (ibloat != _bloatCells.end()) {
      _selected = *ibloat;
      return true;
    }

    cerr << Warning( "BloatCells::select(): No profile named \"%s\", using \"disabled\"."
                   , profile.c_str()
                   ) << endl;

    return select( "disabled" );
  }

  
  Box  BloatCells::getAb ( Occurrence instanceOcc )
  {
    Instance* instance = dynamic_cast<Instance*>( instanceOcc.getEntity() );
    if (not instance) {
      cerr << Error( "BloatCells::getAb(): Occurrence argument do not refer an Instance (skipped).\n"
                     "(%s)"
                   , getString(instanceOcc).c_str() ) << endl;
      return Box();
    }
    if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED)
      return instance->getMasterCell()->getAbutmentBox();

    DbU::Unit   dx    = 0;
    Cell*       cell  = instance->getMasterCell();
    BloatState* state = BloatExtension::get( instanceOcc );

    if (state) dx = state->getTracksCount() * _etesian->getSliceStep();
    else       dx = _selected->getDx( cell, _etesian );

    _dxSpace += dx;
    Box ab = cell->getAbutmentBox();
    return ab.inflate( 0, 0, dx, 0 );
  }


}  // Etesian namespace.
