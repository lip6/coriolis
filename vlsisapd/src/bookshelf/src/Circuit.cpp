
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Circuit.cpp"                                 |
// +-----------------------------------------------------------------+


#include  <sstream>
#include  <fstream>
#include  "vlsisapd/utilities/Path.h"
#include  "vlsisapd/bookshelf/Exception.h"
#include  "vlsisapd/bookshelf/Node.h"
#include  "vlsisapd/bookshelf/Net.h"
#include  "vlsisapd/bookshelf/Row.h"
#include  "vlsisapd/bookshelf/Circuit.h"
#include  "vlsisapd/bookshelf/Parser.h"


namespace Bookshelf {


  Circuit::Circuit ()
    : _designName  ()
    , _nodesName   ()
    , _netsName    ()
    , _wtsName     ()
    , _sclName     ()
    , _plName      ()
    , _flags       (0)
    , _numNodes    (0)
    , _numTerminals(0)
    , _numPins     (0)
    , _maxPinId    (0)
    , _numRows     (0)
    , _nodes       ()
    , _nets        ()
    , _rows        ()
  { }


  Circuit::~Circuit ()
  {
    std::map<std::string,Node*>::iterator inode = _nodes.begin();
    for ( ; inode != _nodes.end() ; ++inode )
      delete (*inode).second;
  }


  Node* Circuit::getNode ( const std::string& name ) const
  {
    std::map<std::string,Node*>::const_iterator inode = _nodes.find(name);
    if ( inode != _nodes.end() ) return (*inode).second;

    return NULL;
  }


  Net* Circuit::getNet ( const std::string& name ) const
  {
    std::map<std::string,Net*>::const_iterator inet = _netsByName.find(name);
    if ( inet != _netsByName.end() ) return (*inet).second;

    return NULL;
  }


  void  Circuit::addNode ( Node* node )
  { _nodes.insert ( make_pair(node->getName(),node) ); }


  void  Circuit::addNet ( Net* net )
  { _nets.push_back ( net ); }


  void  Circuit::addRow ( Row* row )
  { _rows.push_back ( row ); }


  Circuit* Circuit::parse ( std::string auxFile, unsigned int flags )
  {
    Parser  parser;
    return parser.parse ( auxFile, flags );
  }


  bool  Circuit::check () const
  {
    bool success = true;
    if ( (size_t)_numNodes != _nodes.size() ) {
      std::cerr << "[ERROR] Nodes number discrepency, "
                << _numNodes << " announceds but "
                << _nodes.size() << " founds." << std::endl;
      success = false;
    }
    if ( (size_t)_numPins != _maxPinId ) {
      std::cerr << "[ERROR] Pins number discrepency, "
                << _numPins << " announceds but "
                << _maxPinId << " founds." << std::endl;
      success = false;
    }
    return success;
  }


  void  Circuit::writeNodesToStream ( std::ostream& o )
  {
    o << "UCLA nodes 1.0" << std::endl;
    o << std::endl;

    o << "NumNodes : "     << std::setw(20) << std::right << _numNodes << std::endl;
    o << "NumTerminals : " << std::setw(16) << std::right << _numTerminals << std::endl;
    
    std::map<std::string,Node*>::iterator inode = _nodes.begin();
    for ( ; inode != _nodes.end() ; ++inode )
      (*inode).second->writeToStream ( o, Nodes );
  }


  void  Circuit::writeNetsToStream ( std::ostream& o )
  {
    o << "UCLA nets 1.0" << std::endl;
    o << std::endl;

    o << "NumNets : " << std::setw(10) << std::right << _nets.size() << std::endl;
    o << "NumPins : " << std::setw(10) << std::right << _numPins     << std::endl;
    
    std::vector<Net*>::iterator inet = _nets.begin();
    for ( ; inet != _nets.end() ; ++inet )
      (*inet)->writeToStream ( o );
  }


  void  Circuit::writeSclToStream ( std::ostream& o )
  {
    o << "UCLA scl 1.0" << std::endl;
    o << std::endl;

    o << "NumRows : " << std::setw(20) << std::right << _numRows << std::endl;
    
    std::vector<Row*>::iterator irow = _rows.begin();
    for ( ; irow != _rows.end() ; ++irow )
      (*irow)->writeToStream ( o );
  }


  void  Circuit::writePlToStream ( std::ostream& o )
  {
    o << "UCLA pl 1.0" << std::endl;
    o << std::endl;
    
    std::map<std::string,Node*>::iterator inode = _nodes.begin();
    for ( ; inode != _nodes.end() ; ++inode )
      (*inode).second->writeToStream ( o, Pl );
  }


  std::string  Circuit::getAutomaticName ( size_t id )
  {
    std::ostringstream autoName;

    autoName << "NET";
    if ( id == 0 ) autoName << _nets.size();
    else           autoName << id;

    return autoName.str();
  }


  bool  Circuit::hasAutomaticName ( Net* net )
  {
    std::string autoName = getAutomaticName ( net->getId() );
    return net->getName().compare(0,autoName.size(),autoName);
  }


  void  Circuit::resolveNames ()
  {
    for ( size_t i=0 ; i<_nets.size() ; ++i ) {
      if ( _nets[i]->getName().empty() ) continue;
      _netsByName.insert ( make_pair(_nets[i]->getName(),_nets[i]) );
    }

    for ( size_t i=0 ; i<_nets.size() ; ++i ) {
      if ( not _nets[i]->getName().empty() ) continue;

      std::ostringstream format;
      format << "NET" << (_nets[i]->getId()+1);
      
      bool        start    = true;
      std::string autoName = format.str();

      while ( _netsByName.find(autoName) != _netsByName.end() ) {
        if ( start or (*autoName.rbegin() == 'z') ) {
          start = false;
          autoName += 'a';
        } else {
          (*autoName.rbegin())++;
        }
      }
      
      _nets[i]->setName ( autoName );
      _netsByName.insert ( make_pair(autoName,_nets[i]) );
    }
  }


  void  Circuit::drive ( std::string directory, unsigned int flags )
  {
    Utilities::Path  rootDirectory ( directory );
    if ( not rootDirectory.exists() ) {
      rootDirectory.mkdir();
    }

    if ( flags & Nodes ) {
      Utilities::Path nodesPath ( rootDirectory );
      if ( getNodesName().empty() )
        nodesPath /= getDesignName() + ".nodes";
      else
        nodesPath /= getNodesName();

      std::ofstream ofnodes ( nodesPath.c_str() );
      writeNodesToStream ( ofnodes );
      ofnodes.close ();
    }

    if ( flags & Nets ) {
      Utilities::Path netsPath ( rootDirectory );
      if ( getNetsName().empty() )
        netsPath /= getDesignName() + ".nets";
      else
        netsPath /= getNetsName();

      std::ofstream ofnets ( netsPath.c_str() );
      writeNetsToStream ( ofnets );
      ofnets.close ();
    }

    if ( (flags & Scl) and (hasScl()) ) {
      Utilities::Path sclPath ( rootDirectory );
      if ( getSclName().empty() )
        sclPath /= getDesignName() + ".scl";
      else
        sclPath /= getSclName();

      std::ofstream ofscl ( sclPath.c_str() );
      writeSclToStream ( ofscl );
      ofscl.close ();
    }

    if ( (flags & Pl) and (hasPl()) ) {
      Utilities::Path plPath ( rootDirectory );
      if ( getPlName().empty() )
        plPath /= getDesignName() + ".pl";
      else
        plPath /= getPlName();

      std::ofstream ofpl ( plPath.c_str() );
      writePlToStream ( ofpl );
      ofpl.close ();
    }
  }


}  // End of Bookshelf namespace.
