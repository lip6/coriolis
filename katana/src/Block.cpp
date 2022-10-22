// -*- mode: C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Block.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Cell.h"
#include "anabatic/Edge.h"
#include "katana/Block.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using std::cerr;
  using std::endl;
  using std::make_pair;
  using std::ostringstream;
  using std::setw;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::SubTypeCollection;
  using Hurricane::UpdateSession;
  using Hurricane::Transformation;
  using Hurricane::RoutingPad;
  using Hurricane::Instance;
  using Hurricane::Path;
  using Anabatic::Edge;


// -------------------------------------------------------------------
// Class  :  "Katana::Row".


  void  Row::add  ( DbU::Unit x, Occurrence occurrence )
  {
    auto iocc = _occurrences.find( x );
    if (iocc != _occurrences.end()) {
      cerr << Error( "Row::add() In row @%s, two elements at the same X position %s:\n"
                     "        %s\n"
                     "        %s"
                   , DbU::getValueString(_y).c_str()
                   , DbU::getValueString(x).c_str()
                   , getString((*iocc).second).c_str()
                   , getString(occurrence).c_str()
                   ) << endl;
      return;
    }
    _occurrences.insert( make_pair(x,occurrence) );
  }


  void  Row::setY ( DbU::Unit y )
  {
    for ( auto iocc : _occurrences ) {
      Instance* instance = dynamic_cast<Instance*>( iocc.second.getEntity() );
      if (not instance) continue;

      Transformation itransf = instance->getTransformation();
      Transformation dtransf ( itransf.getTx(), itransf.getTy() - _y + y, itransf.getOrientation() );

      instance->setTransformation( dtransf );
    } 

    _y = y;
    DbU::Unit sliceHeight = Session::getSliceHeight() + Session::getPitch( (size_t)0 );

    if (getSouthWest()) {
      GCell* gcellRow     = getSouthWest();
      GCell* gcellChannel = gcellRow->getNorth();

      while ( gcellRow ) {
        gcellRow->setSouthWestCorner( gcellRow->getXMin(), _y );
        gcellRow = gcellRow->getEast();

        if (gcellChannel) {
          gcellChannel->setSouthWestCorner( gcellChannel->getXMin(), _y+sliceHeight );
          gcellChannel = gcellChannel->getEast();
        }
      }
    }
  }


  GCell* Row::createChannel ( GCell* southWest, DbU::Unit channelHeight )
  {
    if (not southWest)
      throw Error( "Row::createChannel(): NULL southWest GCell." );
    if (_occurrences.empty())
      throw Error( "Row::createChannel(): No instances in row." );
    if (southWest->getXMin() != (*_occurrences.begin()).first) {
      UpdateSession::close();
      throw Error( "Row::createChannel(): South-west GCell XMin and Row X min differs.\n"
                   "        * southWest=%s\n"
                   "        * first=%s\n"
                 , getString(southWest).c_str()
                 , DbU::getValueString((*_occurrences.begin()).first).c_str() );
    }
    if (southWest->getYMin() != _y)
      throw Error( "Row::createChannel(): South-west GCell YMIn and Row Y min differs (%s vs. %s)."
                 , DbU::getValueString(southWest->getYMin()).c_str()
                 , DbU::getValueString(_y).c_str()
                 );

    DbU::Unit ypitch      = Session::getPitch( (size_t)0 );
    DbU::Unit sliceHeight = Session::getSliceHeight() + ypitch;
    bool      northRow    = (_y + sliceHeight >= getCell()->getAbutmentBox().getYMax());

    _southWest = southWest;
    GCell* channel = _southWest->hcut( _southWest->getYMin() + sliceHeight );
    GCell* nextSW  = NULL;
    if (not northRow) {
      nextSW = channel->hcut( _southWest->getYMin() + sliceHeight + channelHeight );
      if (not nextSW) {
        throw Error( "Row::createChannel(): Cannot h-cut %s @%s."
                   , getString(channel).c_str()
                   , DbU::getValueString(_southWest->getYMin() + sliceHeight + channelHeight).c_str()
                   );
      }
    }

    southWest->setType( Anabatic::Flags::StdCellRow );
    if (channel) channel->setType( Anabatic::Flags::ChannelRow );

    DbU::Unit xmax = getCell()->getAbutmentBox().getXMax() - sliceHeight;
    DbU::Unit xcut = _southWest->getXMin() + sliceHeight;
    for ( ; xcut < xmax ; xcut += sliceHeight ) {
      southWest = southWest->vcut( xcut );
      southWest->setType( Anabatic::Flags::StdCellRow );
      if (channel) {
        channel = channel->vcut( xcut );
        channel->setType( Anabatic::Flags::ChannelRow );
        channel->getWestEdge()->setFlags( Flags::InfiniteCapacity );
      }
    }

    return nextSW;
  }


  void  Row::routingPadsSubBreak ()
  {
    if (not _southWest)
      throw Error( "Row::routingPadsSubBreak(): NULL southWest GCell." );

    set<Net*>                   nets;
    map<DbU::Unit, RoutingPad*> rps;
    GCell*                      southWest = _southWest;
    while ( southWest ) {
      bool gcellSplitted = false;

      for ( RoutingPad* rp : SubTypeCollection<Component*,RoutingPad*>( getCell()->getComponentsUnder(southWest->getBoundingBox() ) ) ) {
        if (  (rp->getX() >= southWest->getXMax())
           or (rp->getX() <  southWest->getXMin()) ) continue;
        rps.insert( make_pair(rp->getX(),rp) );
      }

      for ( auto irp : rps ) {
        auto inet = nets.find( irp.second->getNet() );
        if (inet == nets.end()) {
          nets.insert( irp.second->getNet() );
        } else {
          southWest = southWest->vcut( irp.second->getX() );
          southWest->setType( Anabatic::Flags::StdCellRow );
          gcellSplitted = true;
          break;
        }
      }

      rps .clear();
      nets.clear();
      if (not gcellSplitted) {
        southWest = southWest->getEast();
      }
    }
  }


  uint32_t  Row::computeChannelHeight ()
  {
    _channelHeight = 2;

    GCell* gcell = getSouthWest()->getNorth();
    while ( gcell ) {
      _channelHeight = std::max( _channelHeight, (uint32_t)gcell->getNetCount() );
      // Edge* east = gcell->getEastEdge();
      // if (east)
      //   _channelHeight = std::max( _channelHeight, east->getRealOccupancy() );
      gcell = gcell->getEast();
    }
    _channelHeight += 2;

    return _channelHeight;
  }


  string  Row::_getString () const
  { return "<Row @" + DbU::getValueString(_y) + " instances:" + getString(_occurrences.size()) + ">"; }


  Record* Row::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( DbU::getValueSlot( "_y", &_y ) );
    record->add( getSlot( "_occurrences", &_occurrences ) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Katana::Block".


  Block::Block ( KatanaEngine* katana, Cell* cell )
    : _katana(katana)
    , _cell  (cell)
    , _rows  ()
  { 
    for ( Occurrence occurrence : _cell->getTerminalNetlistInstanceOccurrences() ) {
      add( occurrence );
    }
    _katana->addBlock( this );
  }


  Block::~Block ()
  {
    for ( Row* row : _rows ) delete row; 
  }


  Row* Block::getAt ( DbU::Unit y ) const
  {
    for ( size_t i=0 ; i<_rows.size() ; ++i ) {
      if (_rows[i]->getY() == y) return _rows[i]; 
    }
    return NULL;
  }


  void  Block::add ( Occurrence occurrence )
  {
    Instance* instance = dynamic_cast<Instance*>(occurrence.getEntity());
    if (not instance) {
      cerr << Warning( "Block::add(): Not an Instance occurrence, %s."
                     ,getString(occurrence).c_str() ) << endl;
      return;
    }

    Transformation transf     = occurrence.getPath().getTransformation( instance->getTransformation() );
    Box            bbInstance = transf.getBox( instance->getMasterCell()->getAbutmentBox() );

    Row* row = getAt( bbInstance.getYMin() );
    if (not row) {
      row = new Row ( this, bbInstance.getYMin() );
      auto irow = _rows.begin();
      for ( ; irow != _rows.end() ; ++irow ) {
        if ((*irow)->getY() >= row->getY()) break; 
      }
      _rows.insert( irow, row );
    }

    row->add( bbInstance.getXMin(), occurrence );
  }


  void  Block::createChannels ()
  {
    bool sessionReUse = Session::isOpen();
    if (not sessionReUse) _katana->openSession();

    DbU::Unit ypitch      = Session::getPitch( (size_t)0 );
    DbU::Unit sliceHeight = Session::getSliceHeight() + ypitch;

    for ( size_t i=0 ; i<_rows.size() ; ++i )
      _rows[i]->setY( i*ypitch + i*sliceHeight );

    Box ab ( _cell->getAbutmentBox() );
    if (not _rows.empty()) {
      _cell->setAbutmentBox( ab.inflate( 0, 0, 0, ypitch*(2*_rows.size()-1) ) );
      _katana->_resizeMatrix();
    }

  //GCell* southWest = _katana->getGCellUnder( ab.getXMin(), ab.getYMin() );
    GCell* southWest = _katana->getSouthWestGCell();
    if (southWest != _katana->getSouthWestGCell())
      throw Error( "Block::createChannels(): Only block are supported for now..." );

    for ( Row* row : _rows )
      southWest = row->createChannel( southWest, ypitch );

    _katana->invalidateRoutingPads();

    Session::close();
    _katana->openSession();

    for ( Row* row : _rows ) row->routingPadsSubBreak();

    if (not sessionReUse) Session::close();
  }


  void  Block::resizeChannels ()
  {
    cmess1 << "  o  Sizing routing channels." << endl;

    bool sessionReUse = Session::isOpen();
    if (not sessionReUse) _katana->openSession();

    DbU::Unit ypitch      = Session::getPitch( (size_t)0 );
    DbU::Unit sliceHeight = Session::getSliceHeight() + ypitch;
    uint32_t  channelSum = 0;
    for ( Row* row : _rows ) channelSum += row->computeChannelHeight(); 

    Box ab = _cell->getAbutmentBox();
    _cell->setAbutmentBox( ab.inflate( 0, 0, 0, ypitch*(channelSum - _rows.size() + 1) ) );
    _katana->_resizeMatrix();

    channelSum = 0;
    for ( size_t irow=0 ; irow < _rows.size() ; ++irow ) {
      _rows[irow]->setY( ab.getYMin() + sliceHeight*irow + ypitch*channelSum );
      channelSum += _rows[irow]->getChannelHeight();

      ostringstream rowName;
      rowName << "     - [" << setw(2) << irow << "]";
      rowName << " @" << DbU::getValueString(_rows[irow]->getY() + sliceHeight);

      ostringstream rowTracks;
      rowTracks << _rows[irow]->getChannelHeight() << " tracks.";

      cmess2 << Dots::asString( rowName.str(), rowTracks.str() ) << endl;
    }
    _katana->_updateGContacts( Flags::Vertical );
    _katana->invalidateRoutingPads();

    if (not sessionReUse) Session::close();
  }


  string  Block::_getString () const
  { return "<Block rows:" + getString(_rows.size()) + ">"; }


  Record* Block::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( getSlot( "_katana",  _katana ) );
    record->add( getSlot( "_rows"  , &_rows   ) );
    return record;
  }


}  // Katana namespace.
