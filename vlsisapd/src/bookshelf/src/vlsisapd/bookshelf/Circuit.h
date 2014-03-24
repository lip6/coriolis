// -*- C++ -*-
//
// This file is part of the VLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./bookshelf/Circuit.h"                    |
// +-----------------------------------------------------------------+


#ifndef  VLSISAPD_BOOKSHELF_CIRCUIT_H
#define  VLSISAPD_BOOKSHELF_CIRCUIT_H

#include  <iostream>
#include  <string>
#include  <map>
#include  <vector>


namespace Bookshelf {

  class Node;
  class Net;
  class Row;


  class Circuit {
    public:
      enum Slot { Nets=0x1, Nodes=0x2, Wts=0x4, Scl=0x8, Pl=0x10, AllSlots=Nodes|Nets|Wts|Scl|Pl };
    public:
      static Circuit*                     parse              ( std::string  design
                                                             , unsigned int slots =AllSlots
                                                             , unsigned int flags =0
                                                             );
             void                         drive              ( std::string  directory
                                                             , unsigned int slots    =AllSlots
                                                             );
    public:                               
                                          Circuit            ();
                                         ~Circuit            ();
      inline bool                         hasNets            () const;
      inline bool                         hasNodes           () const;
      inline bool                         hasWts             () const;
      inline bool                         hasScl             () const;
      inline bool                         hasPl              () const;
             bool                         hasAutomaticName   ( Net* );
      inline const std::string&           getDesignName      () const;
      inline const std::string&           getNodesName       () const;
      inline const std::string&           getNetsName        () const;
      inline const std::string&           getWtsName         () const;
      inline const std::string&           getSclName         () const;
      inline const std::string&           getPlName          () const;
      inline size_t                       getNumNodes        () const;
      inline size_t                       getNumTerminals    () const;
      inline size_t                       getNumNets         () const;
      inline size_t                       getNumPins         () const;
      inline size_t                       getNumRows         () const;
             Node*                        getNode            ( const std::string& ) const;
             Net*                         getNet             ( const std::string& ) const;
      inline std::map<std::string,Node*>& getNodes           ();       
      inline std::vector<Net*>&           getNets            ();       
      inline std::vector<Row*>&           getRows            ();       
             std::string                  getAutomaticName   ( size_t id=0 );
      inline size_t                       getPinId           ();
             bool                         check              () const;
      inline void                         setFlags           ( long );
      inline void                         setDesignName      ( const std::string& );
      inline void                         setNodesName       ( const std::string& );
      inline void                         setNetsName        ( const std::string& );
      inline void                         setWtsName         ( const std::string& );
      inline void                         setSclName         ( const std::string& );
      inline void                         setPlName          ( const std::string& );
      inline void                         setNumNodes        ( size_t );
      inline void                         setNumTerminals    ( size_t );
      inline void                         setNumNets         ( size_t );
      inline void                         setNumPins         ( size_t );
      inline void                         setNumRows         ( size_t );
             void                         addNode            ( Node* );
             void                         addNet             ( Net* );
             void                         addRow             ( Row* );
             void                         resolveNames       ();
             void                         writeNodesToStream ( std::ostream& );
             void                         writeNetsToStream  ( std::ostream& );
             void                         writeSclToStream   ( std::ostream& );
             void                         writePlToStream    ( std::ostream& );
    private:
      std::string                  _designName;
      std::string                  _nodesName;
      std::string                  _netsName;
      std::string                  _wtsName;
      std::string                  _sclName;
      std::string                  _plName;
      long                         _flags;
      size_t                       _numNodes;
      size_t                       _numTerminals;
      size_t                       _numNets;
      size_t                       _numPins;
      size_t                       _maxPinId;
      size_t                       _numRows;
      std::map<std::string,Node*>  _nodes;
      std::map<std::string,Net*>   _netsByName;
      std::vector<Net*>            _nets;
      std::vector<Row*>            _rows;
  };


  inline bool                         Circuit::hasNets         () const { return _flags&Nets; }
  inline bool                         Circuit::hasNodes        () const { return _flags&Nodes; }
  inline bool                         Circuit::hasWts          () const { return _flags&Wts; }
  inline bool                         Circuit::hasScl          () const { return _flags&Scl; }
  inline bool                         Circuit::hasPl           () const { return _flags&Pl; }
  inline const std::string&           Circuit::getDesignName   () const { return _designName; };
  inline const std::string&           Circuit::getNodesName    () const { return _nodesName; };
  inline const std::string&           Circuit::getNetsName     () const { return _netsName; };
  inline const std::string&           Circuit::getWtsName      () const { return _wtsName; };
  inline const std::string&           Circuit::getSclName      () const { return _sclName; };
  inline const std::string&           Circuit::getPlName       () const { return _plName; };
  inline size_t                       Circuit::getNumNodes     () const { return _numNodes; }
  inline size_t                       Circuit::getNumTerminals () const { return _numTerminals; }
  inline size_t                       Circuit::getNumNets      () const { return _numNets; }
  inline size_t                       Circuit::getNumPins      () const { return _numPins; }
  inline size_t                       Circuit::getNumRows      () const { return _numRows; }
  inline std::map<std::string,Node*>& Circuit::getNodes        () { return _nodes; }
  inline std::vector<Net*>&           Circuit::getNets         () { return _nets; }
  inline std::vector<Row*>&           Circuit::getRows         () { return _rows; }
  inline size_t                       Circuit::getPinId        () { return _maxPinId++; }
  inline void                         Circuit::setFlags        ( long flags ) { _flags |= flags; }
  inline void                         Circuit::setDesignName   ( const std::string& name ) { _designName=name; };
  inline void                         Circuit::setNodesName    ( const std::string& name ) { _nodesName=name; };
  inline void                         Circuit::setNetsName     ( const std::string& name ) { _netsName=name; };
  inline void                         Circuit::setWtsName      ( const std::string& name ) { _wtsName=name; };
  inline void                         Circuit::setSclName      ( const std::string& name ) { _sclName=name; };
  inline void                         Circuit::setPlName       ( const std::string& name ) { _plName=name; };
  inline void                         Circuit::setNumNodes     ( size_t value ) { _numNodes=value; }
  inline void                         Circuit::setNumTerminals ( size_t value ) { _numTerminals=value; }
  inline void                         Circuit::setNumNets      ( size_t value ) { _numNets=value; }
  inline void                         Circuit::setNumPins      ( size_t value ) { _numPins=value; }
  inline void                         Circuit::setNumRows      ( size_t value ) { _numRows=value; }


} // End of Bookshelf namespace.

#endif  // VLSISAPD_BOOKSHELF_CIRCUIT_H
