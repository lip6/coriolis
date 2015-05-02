// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Alliance VHDL / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./alliance/vst/VhdlPortMap.cpp"               |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
#include "crlcore/VhdlPortMap.h"
#include "crlcore/VhdlEntity.h"


namespace Vhdl {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Plug;


// -------------------------------------------------------------------
// Class  :  "Vhdl::PortMap".


  PortMap::PortMap ()
  { }


  PortMap::~PortMap ()
  { }


  const Bit* PortMap::_lookup ( const Bit* masterBit, Instance* instance )
  {
    const Net* masterNet = masterBit->getNet();
    Plug* plug           = instance->getPlug( masterNet );
    if (masterNet->isGlobal() and not plug->isConnected()) {
      Entity*       entity = EntityExtension::get( instance->getCell() );
      const Signal* signal = entity->getGlobal( getString(masterNet->getName()) );
      if (signal) return signal->getBit();

      cerr << Error( "PortMap::_lookup() VHDL extension missing on parent of global <%s>."
                   , getString(masterNet).c_str()
                   ) << endl;
    } else {
      Net* net = plug->getNet();
      if (net) {
        Bit* bit = BitExtension::get( net );
        if (bit) return bit;

        cerr << Error( "PortMap::_lookup() VHDL extension missing on <%s>."
                     , getString(net).c_str()
                     ) << endl;
      } else {
        cerr << Error( "PortMap::_lookup() Unconnected <%s>."
                     , getString(plug).c_str()
                     ) << endl;
      }
    }
    return NULL;
  }


  PortMap* PortMap::create ( const Signal* signal )
  {
    const ScalarSignal* scalarSignal = dynamic_cast<const ScalarSignal*>( signal );
    if (not scalarSignal) {
      const VectorSignal* vectorSignal = dynamic_cast<const VectorSignal*>( signal );
      if (vectorSignal)
        return new VectorPortMap ( vectorSignal );
      else
        throw Error( "PortMap::create() Unable to cast toward <ScalarSignal> or <VectorSignal>." );
    }
    return new ScalarPortMap ( scalarSignal );
  }


// -------------------------------------------------------------------
// Class  :  "Vhdl::ScalarPortMap".
  
  ScalarPortMap::ScalarPortMap ( const ScalarSignal* signal )
    : PortMap ()
    , _signal (signal)
    , _mapping(NULL)
  { }


  ScalarPortMap::~ScalarPortMap ()
  { }


  const ScalarSignal* ScalarPortMap::getSignal () const
  { return _signal; }


  void  ScalarPortMap::doMapping ( Instance* instance )
  { _mapping = _lookup( _signal->getBit(0), instance ); }


  void  ScalarPortMap::toVhdlPortMap ( ostream& out, size_t width ) const
  {
    out << setw(width) << left << _signal->getName() << " => ";
    if (_mapping) out << _mapping->getName();
    else          out << "UNCONNECTED";
  }


// -------------------------------------------------------------------
// Class  :  "Vhdl::VectorPortMap".
  
  VectorPortMap::VectorPortMap ( const VectorSignal* signal )
    : PortMap ()
    , _signal (signal)
    , _mapping()
  {
    for ( size_t index=_signal->getMin() ; index<=_signal->getMax() ; ++index ) {
      const Bit* bit = _signal->getBit( index );
      if (not bit) continue;

      _mapping.insert( pair<size_t,const Bit*>(index,NULL) );
    }
  }


  VectorPortMap::~VectorPortMap ()
  { }


  const VectorSignal* VectorPortMap::getSignal () const
  { return _signal; }


  void  VectorPortMap::doMapping ( Instance* instance )
  {
    for ( size_t index=_signal->getMin() ; index<=_signal->getMax() ; ++index ) {
      if (not _signal->getBit(index)) continue;
      _mapping[ index ] = _lookup( _signal->getBit(index), instance );
    }
  }


  void  VectorPortMap::toVhdlPortMap ( ostream& out, size_t width ) const
  {
    if (getSignal()->isContiguous()) {
      vector<string>  mappedNames;
      int             begin = -1;
      int             end   = -1;
      const Bit*      bit   = NULL;
      const Bit*      bitp  = NULL;
      string          name  = "UNCONNECTED";
      string          namep = "UNCONNECTED";
  
      auto imapping  = _mapping.rbegin();
      auto imappingp = _mapping.rbegin();
  
      for ( ++imapping ; imapping!=_mapping.rend() ; ++imapping, ++imappingp ) {
        bit   = imapping ->second;
        bitp  = imappingp->second;
        name  = (bit ) ? bit ->getSignal()->getName() : "UNCONNECTED";
        namep = (bitp) ? bitp->getSignal()->getName() : "UNCONNECTED";
  
        if (   bit
           and bitp
           and bitp->getSignal()->isVector()
           and bitp->getSignal()->isContiguous()
           and (name == namep)) {
          if (begin < 0) begin = bitp->getIndex();
          if (end   < 0) end   = bit ->getIndex();
  
          int delta = (int)bit->getIndex() - (int)bitp->getIndex();
          if ( not ((delta > 0) xor (begin < end)) ) {
            end = bit->getIndex();
            continue;
          }
        }
  
        if (begin != end) {
          string vdir = (begin < end) ? "to" : "downto";
          mappedNames.push_back( namep + "(" + getString(begin) + " " + vdir + " " + getString(end) + ")" );
        } else {
          if ( (bitp == NULL) or (not bitp->getSignal()->isVector()) ) {
            mappedNames.push_back( namep );
          } else {
            mappedNames.push_back( bitp->getName() );
          }
        }
  
        begin = -1;
        end   = -1;
      }
  
      bitp  = imappingp->second;
      namep = (bitp) ? bitp->getSignal()->getName() : "UNCONNECTED";
  
      if (begin != end) {
        string vdir = (begin < end) ? "to" : "downto";
        mappedNames.push_back( namep + "(" + getString(begin) + " " + vdir + " " + getString(end) + ")" );
      } else {
        if ( (bitp == NULL) or (not bitp->getSignal()->isVector()) ) {
          mappedNames.push_back( namep );
        } else {
          mappedNames.push_back( bitp->getName() );
        }
      }
  
      out << setw(width) << left << _signal->getName() << " => ";
  
      size_t lhsWidth  = 90 - tab.getWidth() - width - 4;
      size_t lhsLength = 0;
  
      bool first = true;
      for ( auto name : mappedNames ) {
        if (not first) { out << " & "; lhsLength += 3; }
  
        if (lhsLength + name.size() > lhsWidth) {
          out << "\n" << tab << setw(width+15) << " ";
          lhsLength = 0;
        }
        lhsLength += name.size();
  
        out << name;
        first = false;
      }
    } else {
      auto imapping  = _mapping.rbegin();
      bool first = true;
      for ( ; imapping!=_mapping.rend() ; ++imapping ) {
        if (not first) out << "\n" << tab << "         , ";
        
        out << setw(width) << left << _signal->getBit(imapping->first)->getName()
            << " => " << imapping->second->getName();
        first = false;
      }
    }
  }


}  // Vhdl namespace.
