// -*- C++ -*-
//
// This file is part of the VLSI Stand-Alone Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/bookshelf/Parser.cpp"               |
// +-----------------------------------------------------------------+


#ifndef  VLSISAPD_BOOKSHELF_PARSER_H
#define  VLSISAPD_BOOKSHELF_PARSER_H

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  "vlsisapd/utilities/Path.h"


namespace Bookshelf {

  class Row;

  class Parser {
    public:
      enum Flag { NoFlags      = 0x0000
                , Comment      = 0x0001
                , ExtraDatas   = 0x0002
                , StrictSyntax = 0x0004
                };
    public:
                                 Parser                ();
             Circuit*            parse                 ( std::string  designName
                                                       , unsigned int slots
                                                       , unsigned int flags );
      inline void                setFlags              ( unsigned int flags );
      inline void                unsetFlags            ( unsigned int flags );
    private:                                           
             bool                _openStream           ( const Utilities::Path& );
             void                _closeStream          ();
             char*               _readLine             ();
             void                _tokenize             ();
             int                 _keywordCompare       ( const std::string&, const std::string& ) const;
             void                _checkExtraDatas      ( size_t maxtoken, std::vector<char*>& );
             void                _parseFormatRevision  ( const std::string& slotName );
             size_t              _parseNum             ( const std::string& slotName, const std::string& token );
             void                _parseNodes           ( const Utilities::Path& );
             void                _parseNodesNode       ();
             void                _parseNets            ( const Utilities::Path& );
             void                _parseNetsNetDegree   ();
             void                _parseNetsPin         ();
             void                _parseWts             ( const Utilities::Path& );
             void                _parseScl             ( const Utilities::Path& );
             void                _parseSclCoreRow      ();
             void                _parseSclSiteorient   ();
             void                _parseSclSitesymmetry ();
             void                _parseSclSubrowOrigin ();
             void                _parseSclCorerowEnd   ();
             void                _parsePl              ( const Utilities::Path& );
             void                _parsePlNodePlace     ();
      inline bool                _isComment            () const;
      inline bool                _hasExtraDatas        () const;
    private:
      enum Misc { BufferSize=4096 };
    private:
      size_t             _lineno;
      std::ifstream      _stream;
      char               _buffer[BufferSize];
      std::vector<char*> _tokens;
      unsigned int       _flags;
      int                _state;
      Net*               _net;
      Row*               _row;
      Circuit*           _circuit;
  };


  inline void  Parser::setFlags       ( unsigned int flags ) { _flags |=  flags; }
  inline void  Parser::unsetFlags     ( unsigned int flags ) { _flags &= ~flags; }
  inline bool  Parser::_isComment     () const { return _flags&Comment; }
  inline bool  Parser::_hasExtraDatas () const { return _flags&ExtraDatas; }


} // Bookshelf namespace.

#endif  // VLSISAPD_BOOKSHELF_PARSER_H
