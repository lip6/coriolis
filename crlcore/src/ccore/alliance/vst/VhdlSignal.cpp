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
// |  C++ Module  :   "./alliance/vst/VhdlSignal.cpp"                |
// +-----------------------------------------------------------------+


#include <iomanip>
#include "hurricane/Warning.h"
#include "crlcore/VhdlSignal.h"
#include "crlcore/VhdlEntity.h"


namespace Vhdl {

  using namespace std;
  using Hurricane::Warning;


// -------------------------------------------------------------------
// Class  :  "Vhdl::Signal".

  Signal::Signal ( string name )
    : _name(name)
  { }

  Signal::~Signal ()
  { }


  void  Signal::_toVhdlPort ( ostream&       out
                            , int            width
                            , unsigned int   flags
                            , string         name
                            , string         range
                            , Net::Direction direction )
  {
  //out << tab;
    if (flags & Entity::AsInnerSignal) out << "signal ";

    out << setw(width) << left << name << " : ";

    if (flags & Entity::AsPortSignal) {
      switch ( (unsigned int)direction & (Net::Direction::DirIn|Net::Direction::DirOut) ) {
        case Net::Direction::IN:    out << "in";    break;
        case Net::Direction::OUT:   out << "out";   break;
        case Net::Direction::INOUT: out << "inout"; break;
        default:                    out << "linkage";
      }
    }

    if (not range.empty()) {
      switch ( (unsigned int)direction & (Net::Direction::ConnTristate
                                         |Net::Direction::ConnWiredOr) ) {
        case Net::Direction::ConnTristate:  out << " mux_vector" << range << " bus"; break;
        case Net::Direction::ConnWiredOr:   out << " wor_vector" << range << " bus"; break;
        default:                            out << " bit_vector" << range;
      }
    } else {
      switch ( (unsigned int)direction & (Net::Direction::ConnTristate
                                         |Net::Direction::ConnWiredOr) ) {
        case Net::Direction::ConnTristate: out << " mux_bit bus"; break;
        case Net::Direction::ConnWiredOr:  out << " wor_bit bus"; break;
        default:                           out << " bit";
      }
    }
  }


  Record* Signal::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot("_name", &_name) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Vhdl::ScalarSignal".


  ScalarSignal::ScalarSignal ( Net* net )
    : Signal(getString(net->getName()))
    , _bit  (BitExtension::create(net,this))
  { }

  ScalarSignal::~ScalarSignal ()
  { }

  bool            ScalarSignal::isScalar     () const { return true; }
  bool            ScalarSignal::isVector     () const { return false; }
  bool            ScalarSignal::isExternal   () const { return _bit->isExternal(); }
  bool            ScalarSignal::isContiguous () const { return false; }
  const Bit*      ScalarSignal::getBit       () const { return _bit; }
  const Bit*      ScalarSignal::getBit       ( size_t ) const { return _bit; }
  const Net*      ScalarSignal::getNet       () const { return _bit->getNet(); }
  const Net*      ScalarSignal::getNet       ( size_t ) const { return _bit->getNet(); }
  size_t          ScalarSignal::getMin       () const { return Bit::nindex; }
  size_t          ScalarSignal::getMax       () const { return Bit::nindex; }
  Net::Direction  ScalarSignal::getDirection () const { return getNet(0)->getDirection(); }


  void  ScalarSignal::toVhdlPort ( ostream& out, int width, unsigned int flags ) const
  { _toVhdlPort( out, width, flags, getName(), string(), getDirection() ); }


  string  ScalarSignal::_getString () const
  {
    string s = "<ScalarSignal " + getName() + ">";
    return s;
  }


  Record* ScalarSignal::_getRecord () const
  {
    Record* record = Signal::_getRecord();
    if (record) {
      record->add ( getSlot("_bit", _bit) );
    }
    return record;
  }
  

// -------------------------------------------------------------------
// Class  :  "Vhdl::VectorSignal".


  VectorSignal::VectorSignal ( string name )
    : Signal    (name)
    , _bits     ()
    , _direction(Net::Direction::UNDEFINED)
    , _flags    (Contiguous)
  { }

  VectorSignal::~VectorSignal ()
  { for ( auto bit : _bits ) bit->getProperty()->destroy(); }


  bool            VectorSignal::isScalar     () const { return false; }
  bool            VectorSignal::isVector     () const { return true; }
  bool            VectorSignal::isExternal   () const { return (*_bits. begin())->isExternal(); }
  size_t          VectorSignal::getMin       () const { return (*_bits.rbegin())->getIndex(); }
  size_t          VectorSignal::getMax       () const { return (*_bits. begin())->getIndex(); }
  Net::Direction  VectorSignal::getDirection () const { return _direction; }


  const Bit* VectorSignal::getBit () const
  { return *_bits.rend(); }


  const Bit* VectorSignal::getBit ( size_t index ) const
  {
    Bit  key ( index );
    auto inet = _bits.find( &key );
    if (inet != _bits.end()) return (*inet);

    return NULL;
  }


  const Net* VectorSignal::getNet () const
  {
    const Bit* bit = getBit( getMin() );
    return (bit) ? bit->getNet() : NULL;
  }


  const Net* VectorSignal::getNet ( size_t index ) const
  {
    const Bit* bit = getBit( index );
    return (bit) ? bit->getNet() : NULL;
  }


  bool  VectorSignal::isContiguous () const
  {
    auto inet1 = _bits.rbegin();
    auto inet2 = inet1;

    for ( ++inet1; inet1!=_bits.rend() ; ++inet1, ++inet2 ) {
      if ((*inet1)->getIndex() != (*inet2)->getIndex()+1) return false;
    }

    return true;
  }


  void  VectorSignal::toVhdlPort ( ostream& out, int width, unsigned int flags ) const
  {
    if (isContiguous()) {
      string range = "(" + getString(getMax()) + " downto " + getString(getMin()) + ")";
      _toVhdlPort( out, width, flags, getName(), range, getDirection() );
    } else {

      bool first = true;
      for ( auto bit : _bits ) {
        string name = getName() + "_" + getString(bit->getIndex());
        if (not first) {
          if (flags & Entity::AsPortSignal) out << "\n" << tab << "     ; ";
          else                              out << ";\n" << tab;
        }
        first = false;

        _toVhdlPort( out, width, flags, name, string(), getDirection() );
      }

      if (flags & Entity::AsPortSignal)
        out << "\n" << tab << "     -- Vector <" << getName() << "> is holed, unvectorized.";
      else
        out << ";\n" << tab << "-- Vector <" << getName() << "> is holed, unvectorized.";
    }
  }


  bool  VectorSignal::addNet ( size_t index, Net* net )
  {
    const Net* check = getNet( index );
    if (check) {
      if (check == net) return true;

      cerr << Warning( "VectorSignal::addNet() Attempt to create twice index %u for nets:\n"
                       "          %s (previous)\n"
                       "          %s (new)"
                     , index
                     , getString(check->getName()).c_str()
                     , getString(net  ->getName()).c_str()
                     ) << endl;
      return false;
    }

    if (_direction == Net::Direction::UNDEFINED)
      _direction = net->getDirection();
    else {
      _direction |= net->getDirection();
    }

    _bits.insert( BitExtension::create(net,this,index) );

    auto inet1 = _bits.rbegin();
    auto inet2 = inet1;

    _flags |= Contiguous;
    for ( ++inet1; inet1!=_bits.rend() ; ++inet1, ++inet2 ) {
      if ((*inet1)->getIndex() != (*inet2)->getIndex()+1) {
        _flags &= ~Contiguous;
        break;
      }
    }

    return true;
  }


  string  VectorSignal::_getString () const
  {
    string s = "<VectorSignal " + getName()
             + "(" + getString(getMax()) + " downto " + getString(getMin()) + ") ";
    s += (_flags & Contiguous) ? 'C' : '-';
    s += ">";
    return s;
  }


  Record* VectorSignal::_getRecord () const
  {
    Record* record = Signal::_getRecord();
    if (record) {
      record->add ( getSlot("_bits"     , &_bits     ) );
      record->add ( getSlot("_direction", &_direction) );
      record->add ( getSlot("_flags"    , &_flags    ) );
    }
    return record;
  }


}  // Vhdl namespace.
