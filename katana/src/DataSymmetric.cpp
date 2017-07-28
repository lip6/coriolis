// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./DataSymmetric.cpp"                          |
// +-----------------------------------------------------------------+

#include "anabatic/AutoSegment.h"
#include "katana/DataSymmetric.h"
#include "katana/Session.h"


namespace {

  using namespace std;


  class Message {
    public:
      inline                Message   ( size_t, string header="" );
      inline size_t         size      () const;
      inline ostringstream& newline   ();
      inline ostringstream& line      ();
      inline void           setHeader ( string );
             void           print     ( ostream& );
    private:
      size_t         _indent;
      string         _header;
      vector<string> _lines;
      ostringstream  _current;
  };


  inline  Message::Message   ( size_t indent, string header )
    : _indent (indent)
    , _header (header)
    , _lines  ()
    , _current()
  { }

  inline size_t         Message::size      () const { return _lines.size(); }
  inline void           Message::setHeader ( string header ) { _header = header; }
  inline ostringstream& Message::line      () { return _current; }

  inline ostringstream& Message::newline ()
  {
    if (_current.str().size())
      _lines.push_back(_current.str());
    _current.str("");
    return _current;
  }

  void Message::print ( ostream& o )
  {
    if (not _header.empty()) _indent = _header.size()+1;

    string head ( _indent, ' ' );
    for ( size_t i=0 ; i<_lines.size() ; ++i ) {
      if ((i == 0) and not _header.empty()) o << _header << " ";
      else                                  o << head;
      o << _lines[i] << endl;
    }
  }


} // Anonymous namespace.


namespace Katana {

  using namespace std;
  using Anabatic::AutoSegment;


  DataSymmetric* DataSymmetric::create ( Net* net )
  {
    NetRoutingState* state = NetRoutingExtension::get( net );

    if (not state or not state->isSymmetric()) return NULL;
    if (state->getSymNet() and not state->isSymMaster()) return NULL;

    return new DataSymmetric ( net );
  }


  DataSymmetric::DataSymmetric ( Net* net )
    : _valid   (true)
    , _net     (net)
    , _symNet  (NULL)
    , _state   (NetRoutingExtension::get(_net))
    , _paireds ()
    , _symIndex(0)
  {
    _symNet = _state->getSymNet();
  }


  void  DataSymmetric::addSymmetrical ( AutoSegment* symmetrical )
  {
    if (_paireds.size() > _symIndex) _paireds[_symIndex++][1] = symmetrical;
    else                             _paireds.push_back( { NULL, symmetrical } );
  }


  AutoSegment* DataSymmetric::getSymmetrical ( AutoSegment* segment ) const
  {
    for ( const array<AutoSegment*,2>& paired : _paireds ) {
      if (segment == paired[0]) return paired[1];
      if (segment == paired[1]) return paired[0];
    }
    return NULL;
  }


  bool  DataSymmetric::checkPairing ()
  {
    const uint64_t mask   = ~(AutoSegment::SegIsReduced);
          Message  errors ( 0, "[ERROR]" );

  // Temporary hardwired: M2 (depth 1) for H pitch, M3 (depth 2) for V pitch.
    DbU::Unit hPitch = Session::getPitch( 1 );
    DbU::Unit vPitch = Session::getPitch( 2 );

    size_t refs = 0;
    size_t syms = 0;
    for ( const array<AutoSegment*,2>& paired : _paireds ) {
      refs += (paired[0]) ? 1 : 0;
      syms += (paired[1]) ? 1 : 0;
    }
    if (refs != syms) {
      errors.newline() << "Segments symmetric sets size mismatch, reference:" << refs
                       << " symmetricals:" << syms << ".";
      _valid = false;
    } else {
      size_t index = 0;
      for ( const array<AutoSegment*,2>& paired : _paireds ) {
        if (paired[0]->isHorizontal() xor paired[1]->isHorizontal()) {
          errors.newline() << "Direction mismatch @ [" << index << "]";
          errors.newline() << "| " << paired[0];
          errors.newline() << "| " << paired[1];
          _valid = false;
        }
        
        if (paired[0]->getLayer() != paired[1]->getLayer()) {
          errors.newline() << "Layer mismatch @ [" << index << "]";
          errors.newline() << "| " << paired[0];
          errors.newline() << "| " << paired[1];
          _valid = false;
        }
        
        if (_state->isSymVertical()) {
          if (paired[0]->isVertical()) {
            if ( (paired[0]->getFlags() ^ paired[1]->getFlags()) & mask ) {
              errors.newline() << "Flags mismatch at index " << index
                               << " "     << paired[0]->getFlags()
                               << " vs. " << paired[1]->getFlags();
              errors.newline() << "| " << paired[0];
              errors.newline() << "| " << paired[1];
              _valid = false;
            }

            if (std::abs( 2*getSymAxis() - paired[0]->getAxis() - paired[1]->getAxis() ) > 5*vPitch ) {
              errors.newline() << "Mirror axis mismatch @ [" << index << "] "
                               << DbU::getValueString(paired[1]->getAxis()) << " (should be: "
                               << DbU::getValueString(2*getSymAxis() - paired[0]->getAxis()) << ")";
              errors.newline() << "| " << paired[0];
              errors.newline() << "| " << paired[1];
              errors.newline() << "| Tolerance (5*vPitch): " << DbU::getValueString(5*vPitch);
              _valid = false;
            }
          } else {
            if ( (paired[0]->getFlags() ^ AutoSegment::swapSourceTargetFlags(paired[1])) & mask ) {
              errors.newline() << "Flags mismatch at index " << index
                               << " "     << paired[0]->getFlags()
                               << " vs. " << paired[1]->getFlags()
                               << " swp " << AutoSegment::swapSourceTargetFlags(paired[1]);
              _valid = false;
            }

            if ( std::abs( paired[0]->getAxis() - paired[1]->getAxis() ) > 5*hPitch ) {
              errors.newline() << "Axis mismatch index " << index << " "
                               << DbU::getValueString(paired[1]->getAxis()) << " (should be:"
                               << DbU::getValueString(paired[0]->getAxis()) << ")";
              errors.newline() << "| " << paired[0];
              errors.newline() << "| " << paired[1];
              errors.newline() << "| Tolerance (5*hPitch): " << DbU::getValueString(5*hPitch);
              _valid = false;
            }
          }
        } else {
          if (paired[0]->isHorizontal()) {
            if ( std::abs( 2*getSymAxis() - paired[0]->getAxis() - paired[1]->getAxis() ) > 5*hPitch ) {
              errors.newline() << "Mirror axis mismatch index " << index << " "
                               << DbU::getValueString(paired[1]->getAxis()) << " (should be:"
                               << DbU::getValueString(2*getSymAxis() - paired[0]->getAxis()) << ")";
              errors.newline() << "| " << paired[0];
              errors.newline() << "| " << paired[1];
              errors.newline() << "| Tolerance (5*hPitch): " << DbU::getValueString(5*hPitch);
              _valid = false;
            }
          } else {
            if ( std::abs( paired[0]->getAxis() != paired[1]->getAxis() ) > 5*vPitch ) {
              errors.newline() << "Axis mismatch index " << index << " "
                               << DbU::getValueString(paired[1]->getAxis()) << " (should be:"
                               << DbU::getValueString(paired[0]->getAxis()) << ")";
              errors.newline() << "| " << paired[0];
              errors.newline() << "| " << paired[1];
              errors.newline() << "| Tolerance (5*vPitch): " << DbU::getValueString(5*vPitch);
              _valid = false;
            }
          }
        }

        ++index;
      }
    }


    errors.newline();
    if (errors.size()) {
    //cmess2 << " pairing failed." << endl;
      errors.print( cmess2 );
    } else {
    //cmess2 << " paired." << endl;
    }

    return _valid;
  }


  void  DataSymmetric::print ( ostream& o ) const
  {
    Message  lines ( 0 );

    lines.newline() << "Paired components of Net \"" << _net->getName() << "\"";
    if (_symNet) lines.line() << " (symmetrical:\"" << _symNet->getName() << "\")";

    size_t index = 0;
    for ( const array<AutoSegment*,2>& paired : _paireds ) {
      lines.newline() << "| " << setw(2) << index << " " << paired[0]; 
      lines.newline() << "+ " << setw(2) << index << " " << paired[1]; 
      ++index;
    }

    lines.newline();
    lines.print( o );
  }


  string  DataSymmetric::_getString () const
  {
    return "<DataSymmetric " + getString(_net->getName()) + ">";
  }


  Record* DataSymmetric::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add( getSlot( "_valid"    ,  _valid    ) );
    record->add( getSlot( "_net"      ,  _net      ) );
    record->add( getSlot( "_symNet"   ,  _symNet   ) );
    record->add( getSlot( "_state"    ,  _state    ) );
    record->add( getSlot( "_paireds"  , &_paireds  ) );
    record->add( getSlot( "_symIndex" , &_symIndex ) );
                                     
    return record;
  }


}  // Katana namespace.
