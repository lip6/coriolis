
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./bookshelf/Parser.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __VLSISAPD_BOOKSHELF_PARSER__
#define  __VLSISAPD_BOOKSHELF_PARSER__

#include  <boost/filesystem/operations.hpp>
#include  <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

#include  <string>
#include  <iostream>
#include  <vector>


namespace Bookshelf {


  class Parser {
    public:
      enum Flag { Comment=0x1, ExtraDatas=0x2 };
    public:
                                 Parser                ();
             Circuit*            parse                 ( std::string designName, unsigned int flags );
    private:                                           
             bool                _openStream           ( const bfs::path& );
             void                _closeStream          ();
             char*               _readLine             ();
             void                _tokenize             ();
             void                _checkExtraDatas      ( size_t maxtoken, std::vector<char*>& );
             void                _parseFormatRevision  ( const std::string& slotName );
             size_t              _parseNum             ( const std::string& slotName, const std::string& token );
             void                _parseNodes           ( const bfs::path& );
             void                _parseNodesNode       ();
             void                _parseNets            ( const bfs::path& );
             void                _parseNetsNetDegree   ();
             void                _parseNetsPin         ();
             void                _parseWts             ( const bfs::path& );
             void                _parseScl             ( const bfs::path& );
             void                _parseSclCoreRow      ();
             void                _parseSclSiteorient   ();
             void                _parseSclSitesymmetry ();
             void                _parseSclSubrowOrigin ();
             void                _parseSclCorerowEnd   ();
             void                _parsePl              ( const bfs::path& );
             void                _parsePlNodePlace     ();
      inline bool                _isComment            () const;
      inline bool                _hasExtraDatas        () const;
    private:
      enum Misc { BufferSize=4096 };
    private:
      size_t             _lineno;
      bfs::ifstream      _stream;
      char               _buffer[BufferSize];
      std::vector<char*> _tokens;
      unsigned int       _flags;
      int                _state;
      Net*               _net;
      Row*               _row;
      Circuit*           _circuit;
  };


  inline bool  Parser::_isComment     () const { return _flags&Comment; }
  inline bool  Parser::_hasExtraDatas () const { return _flags&ExtraDatas; }


} // End of Bookshelf namespace.


#endif  // __VLSISAPD_BOOKSHELF_PARSER__
