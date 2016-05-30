// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./AnabaticEngine.cpp"                          |
// +-----------------------------------------------------------------+


#include <sstream>
#include <iostream>
#include "hurricane/Error.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Cell.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::cerr;
  using std::cout;
  using std::endl;
  using std::ostringstream;
  using Hurricane::Error;
  using Hurricane::RegularLayer;
  using Hurricane::Cell;
  using Hurricane::UpdateSession;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AnabaticEngine".

  Name  AnabaticEngine::_toolName = "Anabatic";


  AnabaticEngine* AnabaticEngine::get ( const Cell* cell )
  { return static_cast<AnabaticEngine*>(ToolEngine::get(cell,staticGetName())); }


  const Name& AnabaticEngine::staticGetName ()
  { return _toolName; }


  const Name& AnabaticEngine::getName () const
  { return _toolName; }


  AnabaticEngine::AnabaticEngine ( Cell* cell )
    : Super(cell)
    , _configuration (new ConfigurationConcrete())
    , _matrix        ()
    , _gcells        ()
    , _viewer        (NULL)
    , _flags         (Flags::NoFlags)
    , _stamp         (-1)
  {
    _matrix.setCell( cell, _configuration->getSliceHeight() );
    Edge::unity = _configuration->getSliceHeight();
  }


  void  AnabaticEngine::_postCreate ()
  {
    Super::_postCreate();

    UpdateSession::open();
    GCell::create( this );
    UpdateSession::close();
  }


  AnabaticEngine* AnabaticEngine::create ( Cell* cell )
  {
    if (not cell) throw Error( "AnabaticEngine::create(): NULL cell argument." );
    if (cell->getAbutmentBox().isEmpty())
      throw Error( "AnabaticEngine::create(): %s has no abutment box." , getString(cell).c_str() );
    
    AnabaticEngine* engine = new AnabaticEngine ( cell );
    engine->_postCreate();
    return engine;
  }


  AnabaticEngine::~AnabaticEngine ()
  {
    delete _configuration;
  }


  void  AnabaticEngine::_preDestroy ()
  {
    _clear();
    Super::_preDestroy();
  }


  void  AnabaticEngine::_clear ()
  {
    _flags |= Flags::Destroy;

    for ( GCell* gcell : _gcells ) gcell->_destroyEdges();
    for ( GCell* gcell : _gcells ) gcell->destroy();
    _gcells.clear();
  }


  Configuration* AnabaticEngine::getConfiguration ()
  { return _configuration; }


  int  AnabaticEngine::getCapacity ( Interval span, Flags flags ) const
  {
    int           capacity = 0;
    Box           ab       = getCell()->getAbutmentBox();
    RoutingGauge* rg       = _configuration->getRoutingGauge();

    span.inflate( 0, -1 );
    if (span.isEmpty()) return 0;

    const vector<RoutingLayerGauge*>& layerGauges = rg->getLayerGauges();
    for ( size_t depth=0 ; depth <= _configuration->getAllowedDepth() ; ++depth ) {
      if (layerGauges[depth]->getType() != Constant::Default) continue;

      if (flags & Flags::Horizontal) {
        if (layerGauges[depth]->getDirection() != Constant::Horizontal) continue;
        capacity += layerGauges[depth]->getTrackNumber( span.getVMin() - ab.getYMin()
                                                      , span.getVMax() - ab.getYMin() );
      //cdebug.log(110) << "Horizontal edge capacity:" << capacity << endl;
      }

      if (flags & Flags::Vertical) {
        if (layerGauges[depth]->getDirection() != Constant::Vertical) continue;
        capacity += layerGauges[depth]->getTrackNumber( span.getVMin() - ab.getXMin()
                                                      , span.getVMax() - ab.getXMin() );
      //cdebug.log(110) << "Vertical edge capacity:" << capacity << endl;
      }
    }

    return capacity;
  }


  void  AnabaticEngine::reset ()
  {
    _clear();
    _flags.reset( Flags::Destroy );

    UpdateSession::open();
    GCell::create( this );
    UpdateSession::close();
  }


  string  AnabaticEngine::_getTypeName () const
  { return getString(_toolName); }


  string  AnabaticEngine::_getString () const
  {
    ostringstream os;
    os << "<" << _toolName << " " << _cell->getName() << ">";
    return os.str();
  }


  Record* AnabaticEngine::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_configuration",  _configuration) );
    record->add( getSlot("_gcells"       , &_gcells       ) );
    record->add( getSlot("_matrix"       , &_matrix       ) );
    record->add( getSlot("_flags"        , &_flags        ) );
    return record;
  }


}  // Anabatic namespace.
