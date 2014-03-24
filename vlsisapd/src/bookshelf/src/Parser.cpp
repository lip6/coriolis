// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Parser.cpp"                                  |
// +-----------------------------------------------------------------+


#include  <cctype>
#include  <sstream>
#include  "vlsisapd/bookshelf/Exception.h"
#include  "vlsisapd/bookshelf/Node.h"
#include  "vlsisapd/bookshelf/Pin.h"
#include  "vlsisapd/bookshelf/Net.h"
#include  "vlsisapd/bookshelf/Row.h"
#include  "vlsisapd/bookshelf/Circuit.h"
#include  "vlsisapd/bookshelf/Parser.h"


namespace {

  using namespace std;


  inline long  toLong ( const char* s )
  { long value; istringstream input(s); input >> value; return value; }


  inline size_t  toSizet ( const char* s )
  { size_t value; istringstream input(s); input >> value; return value; }


  inline double  toDouble ( const char* s )
  { double value; istringstream input(s); input >> value; return value; }


}  // End of anonymous namespace.


namespace Bookshelf {


  enum NodesParserState { NodesFormatRevision=1, NodesNumNodes, NodesNumTerminals, NodesNode };
  enum NetsParserState  { NetsFormatRevision=1, NetsNumNets, NetsNumPins, NetsDegree, NetsPin };
  enum PlParserState    { PlFormatRevision=1, PlNodePlace };
  enum SclParserState   { SclFormatRevision=1
                        , SclNumRows
                        , SclCoreRow
                        , SclCoordinate
                        , SclHeight
                        , SclSitewidth
                        , SclSitespacing
                        , SclSiteorient
                        , SclSitesymmetry
                        , SclSubrowOrigin
                        , SclCorerowEnd
                        , SclFinish
                        };


  Parser::Parser ()
    : _lineno (0)
    , _stream ()
    , _buffer ()
    , _tokens ()
    , _flags  (StrictSyntax)
    , _state  (0)
    , _net    (NULL)
    , _row    (NULL)
    , _circuit(NULL)
  { }


  bool  Parser::_openStream ( const Utilities::Path& filePath )
  {
    if ( _stream.is_open() ) _closeStream();
    _stream.open ( filePath.string().c_str() );
    _lineno = 0;

    return _stream.is_open();
  }


  void  Parser::_closeStream ()
  {
    if ( _stream.is_open() ) _stream.close();
  }


  char* Parser::_readLine ()
  {
    bool textBeforeComment = false;

    _flags &= ~Comment;

    if ( _stream.is_open() ) {
      _stream.getline ( _buffer, BufferSize );
      ++_lineno;

      for ( size_t i=0 ; (_buffer[i] != '\0') and (i<BufferSize) ; ++i ) {
        if ( _buffer[i] == '#' ) {
          _flags |= Comment;
          break;
        } else if ( (_buffer[i] == ' ') or (_buffer[i] == '\t') )
          continue;
        textBeforeComment = true;
      }
    } else
      _buffer[0] = '\0';

    if ( (_flags & Comment) and (textBeforeComment) )
      std::cerr << Exception("Text before comment at line %d.",_lineno).what() << std::endl;

    if ( not textBeforeComment ) _flags |= Comment;

    return _buffer;
  }


  void  Parser::_tokenize ()
  {
    static std::string separators     = " \t"; 
           bool        mergeSeparator = true;
           char*       ibuffer        = _buffer;

    _tokens.clear();

    while ( *ibuffer != '\0' ) {
      if ( separators.find(*ibuffer) != std::string::npos ) {
        *ibuffer = '\0';
        mergeSeparator = true;
      } else {
        if ( mergeSeparator ) {
        //std::cerr << _tokens.size() << ":" << ibuffer << std::endl;
          _tokens.push_back ( ibuffer );
        }

        mergeSeparator = false;
      }
      
      ++ibuffer;
    }
  }


  int  Parser::_keywordCompare ( const string& a, const string& b ) const
  {
    if (_flags & StrictSyntax) return a.compare(b);

    string lowerA = a;
    string lowerB = b;

    for ( size_t i=0 ; i<lowerA.size() ; ++i ) lowerA[i] = tolower( lowerA[i] );
    for ( size_t i=0 ; i<lowerB.size() ; ++i ) lowerB[i] = tolower( lowerB[i] );

    return lowerA.compare(lowerB);
  }


  void  Parser::_parseFormatRevision ( const std::string& slotName )
  {
    std::string formatHeader = "UCLA " + slotName + " 1.0";

    if ( _keywordCompare(formatHeader,_buffer) != 0 )
      throw Exception("Bookshelf::Parse(): Invalid format revision for <.%s> slot.",slotName.c_str());
  }


  size_t  Parser::_parseNum ( const std::string& slotName, const std::string& token )
  {
    _tokenize ();

    if (  (_tokens.size() < 3 )
       or (_keywordCompare(token,_tokens[0]) != 0)
       or (_keywordCompare(":"  ,_tokens[1]) != 0) )
      throw Exception("Bookshelf::Parse(): @%d, Invalid %s in XX <.%s>.",_lineno, token.c_str(),slotName.c_str());

  //std::cerr << _tokens.size() << ":" << _tokens[2] << ":" << _circuit->getNumNodes() << std::endl;

    return toSizet(_tokens[2]);
  }


  void  Parser::_parseNodesNode ()
  {
  //std::cerr << "_buffer: " << _buffer << endl;

    bool         terminal       = false;
    unsigned int symmetry       = 0;
    bool         symmetryTokens = false;
    double       width          = 0.0;
    double       height         = 0.0;

    _tokenize ();

    if ( _tokens.size() < 1 )
      throw Exception("Bookshelf::Parse(): @%d, Invalid Node line in <.nodes>.",_lineno);

    for ( size_t itoken=1 ; itoken<_tokens.size() ; ++itoken ) {
    //std::cerr << "F:" << _tokens[itoken] << " ";
      if ( symmetryTokens ) {
        if ( _keywordCompare("X"  ,_tokens[itoken]) == 0 ) { symmetry |= Symmetry::X; continue; }
        if ( _keywordCompare("Y"  ,_tokens[itoken]) == 0 ) { symmetry |= Symmetry::Y; continue; }
        if ( _keywordCompare("R90",_tokens[itoken]) == 0 ) { symmetry |= Symmetry::R90; continue; }
        symmetryTokens = false;
      }
      if ( _keywordCompare("terminal",_tokens[itoken]) == 0 ) { terminal = true; continue; }
      if ( _keywordCompare(":"       ,_tokens[itoken]) == 0 ) { symmetryTokens = true; continue; }

    //std::cerr << " <X Y>" << std::endl;

      width = toDouble ( _tokens[itoken] );
      if ( ++itoken == _tokens.size() )
        throw Exception("Bookshelf::Parse(): @%d, Invalid Node line in <.nodes>.",_lineno);

      height = toDouble ( _tokens[itoken] );
    }

    _circuit->addNode ( new Node(_tokens[0],width,height,symmetry,terminal) );

    // std::cerr << "name:" << _tokens[0]
    //           << " " << width
    //           << " " << height
    //           << " " << boolalpha << terminal
    //           << " : " << symmetry
    //           << std::endl;
  }


  void  Parser::_parseNodes ( const Utilities::Path& nodesPath )
  {
    _circuit->setNodesName ( nodesPath.string());

    _state = NodesFormatRevision;
    _openStream ( nodesPath );

    _circuit->setFlags ( Circuit::Nodes );

    while ( not _stream.eof() ) {
      _readLine ();
      if ( _state == NodesFormatRevision ) {
        _parseFormatRevision ( "nodes" );
        _state = NodesNumNodes;
        continue;
      } else {
        if ( _isComment() ) continue;
        switch ( _state ) {
          case NodesNumNodes:
            _circuit->setNumNodes(_parseNum("nodes","NumNodes")); _state = NodesNumTerminals; break;
          case NodesNumTerminals:
            _circuit->setNumTerminals(_parseNum("nodes","NumTerminals")); _state = NodesNode; break;
          case NodesNode:
            _parseNodesNode(); break;
        }
      }
    }

    _closeStream ();
    _state = 0;
  }


  void  Parser::_parseNetsNetDegree ()
  {
  //std::cerr << _buffer << std::endl;

    _tokenize ();

    if (  (_tokens.size() < 3 )
       or (_keywordCompare("NetDegree",_tokens[0]) != 0)
       or (_keywordCompare(":"        ,_tokens[1]) != 0) )
      throw Exception("Bookshelf::Parse(): @%d, Invalid NetDegree in <.nets>.",_lineno);

    std::string name;
    if ( _tokens.size() >= 4 ) name = _tokens[3];

    _net = new Net ( _circuit, (size_t)toLong(_tokens[2]), name );
  }


  void  Parser::_parseNetsPin ()
  {
    unsigned int direction      = Direction::Disabled;
    double       x              = 0;
    double       y              = 0;
    bool         tokenDirection = true;

    _tokenize ();
    if ( _tokens.size() < 1 )
      throw Exception("Bookshelf::Parse(): @%d, Invalid Net line in <.nets>.",_lineno);

    for ( size_t itoken=1 ; itoken<_tokens.size() ; ++itoken ) {
    //std::cerr << "F:" << _tokens[itoken] << " ";
      if ( tokenDirection ) {
        if ( _keywordCompare("I",_tokens[itoken]) == 0 ) { direction |= Direction::Input; }
        if ( _keywordCompare("O",_tokens[itoken]) == 0 ) { direction |= Direction::Output; }
        if ( _keywordCompare("B",_tokens[itoken]) == 0 ) { direction |= Direction::Bidirectional; }
        if ( direction != Direction::Disabled ) {
          tokenDirection = false;
          continue;
        }
      }

      if ( _keywordCompare(":",_tokens[itoken]) == 0 ) {
        tokenDirection = false;

        if ( ++itoken == _tokens.size() )
          throw Exception("Bookshelf::Parse(): @%d, Invalid Net line in <.nets>.",_lineno);

        x = toDouble ( _tokens[itoken] );
        if ( ++itoken == _tokens.size() )
          throw Exception("Bookshelf::Parse(): @%d, Invalid Net line in <.nets>.",_lineno);

        y = toDouble ( _tokens[itoken] );

        break;
      }
    }

    Node* node = _circuit->getNode ( _tokens[0] );
    if ( node == NULL )
      throw Exception("Bookshelf::Parse(): @%d, Invalid Node name line in <.nets>.",_lineno);

    new Pin ( _circuit, node, _net, x, y, direction );

    // std::cerr << "name:" << _tokens[0]
    //           << " " << x
    //           << " " << y
    //           << " " << boolalpha << terminal
    //           << " : " << symmetry
    //           << std::endl;
  }


  void  Parser::_parseNets ( const Utilities::Path& netsPath )
  {
    _circuit->setNetsName ( netsPath.string());

    _state = NetsFormatRevision;
    _openStream ( netsPath );

    _circuit->setFlags ( Circuit::Nets );

    while ( not _stream.eof() ) {
      _readLine ();
      if ( _state == NetsFormatRevision ) {
        _parseFormatRevision ( "nets" );
        _state = NetsNumNets;
        continue;
      } else {
        if ( _isComment() ) continue;
        switch ( _state ) {
          case NetsNumNets:
            _circuit->setNumNets(_parseNum("nets","NumNets")); _state = NetsNumPins; break;
          case NetsNumPins:
            _circuit->setNumPins(_parseNum("nets","NumPins")); _state = NetsDegree;  break;
          case NetsDegree:
            _parseNetsNetDegree (); _state = NetsPin; break;
          case NetsPin:
            _parseNetsPin ();
            if ( _net->getDegree() == _net->getPins().size() ) _state = NetsDegree;
            break;
        }
      }
    }

    if ( _net->getDegree() > _net->getPins().size() )
      throw Exception("Bookshelf::parse(): @EOF, missing pins.");

    _closeStream ();

    _circuit->resolveNames ();

    _state = 0;
  }


  void  Parser::_parseWts ( const Utilities::Path& nodesPath )
  {
  //_circuit->setWtsName ( wtsPath.string());

  //std::cout << "Parser::_parseWts()" << std::endl;
  }


  void  Parser::_parseSclCoreRow ()
  {
    _tokenize ();

    if (  (_tokens.size() < 2)
       or (_keywordCompare("CoreRow"   ,_tokens[0]) != 0)
       or (_keywordCompare("Horizontal",_tokens[1]) != 0) )
      throw Exception("Bookshelf::Parse(): @%d, Invalid CoreRow line in <.scl>.",_lineno);

    _row = new Row ();
    _circuit->addRow ( _row );
  }


  void  Parser::_parseSclSiteorient ()
  {
    _tokenize ();

    if (  (_tokens.size() < 3)
       or (_keywordCompare("Siteorient",_tokens[0]) != 0)
       or (_keywordCompare(":"         ,_tokens[1]) != 0) )
      throw Exception("Bookshelf::Parse(): @%d, Invalid Row XX siteorient line in <.scl>.",_lineno);

    if      ( _keywordCompare("N" ,_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::N); }
    else if ( _keywordCompare("E" ,_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::E); }
    else if ( _keywordCompare("S" ,_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::S); }
    else if ( _keywordCompare("W" ,_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::W); }
    else if ( _keywordCompare("FN",_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::FN); }
    else if ( _keywordCompare("FE",_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::FE); }
    else if ( _keywordCompare("FS",_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::FS); }
    else if ( _keywordCompare("FW",_tokens[2]) == 0 ) { _row->setSiteorient(Orientation::FW); }
    else {
    //throw Exception("Bookshelf::Parse(): @%d, Invalid Row siteorient line in <.scl>.",_lineno);
      _row->setSiteorient(Orientation::N);
    }
  }


  void  Parser::_parseSclSitesymmetry ()
  {
    _tokenize ();

    if (  (_tokens.size() < 3)
       or (_keywordCompare("Sitesymmetry",_tokens[0]) != 0)
       or (_keywordCompare(":"           ,_tokens[1]) != 0) )
      throw Exception("Bookshelf::Parse(): @%d, Invalid Row sitesymmetry line in <.scl>.",_lineno);

    if      ( _keywordCompare("X"  ,_tokens[2]) == 0 ) { _row->setSitesymmetry(Symmetry::X); }
    else if ( _keywordCompare("Y"  ,_tokens[2]) == 0 ) { _row->setSitesymmetry(Symmetry::Y); }
    else if ( _keywordCompare("R90",_tokens[2]) == 0 ) { _row->setSitesymmetry(Symmetry::R90); }
    else {
      _row->setSitesymmetry(Symmetry::X);
    //throw Exception("Bookshelf::Parse(): @%d, Invalid Row sitesymmetry line in <.scl>.",_lineno);
    }
  }


  void  Parser::_parseSclSubrowOrigin ()
  {
    _tokenize ();

    if (  (_tokens.size() < 6)
       or (_keywordCompare("SubrowOrigin",_tokens[0]) != 0)
       or (_keywordCompare(":"           ,_tokens[1]) != 0)
       or (_keywordCompare("Numsites"    ,_tokens[3]) != 0)
       or (_keywordCompare(":"           ,_tokens[4]) != 0) )
      throw Exception("Bookshelf::Parse(): @%d, Invalid Row SubrowOrigin line in <.scl>.",_lineno);

    _row->setSubrowOrigin ( toDouble(_tokens[2]) );
    _row->setNumsites     ( toDouble(_tokens[5]) );
  }


  void  Parser::_parseSclCorerowEnd ()
  {
    _tokenize ();

    if (  (_tokens.size() < 1)
       or (_keywordCompare("End",_tokens[0]) != 0) )
      throw Exception("Bookshelf::Parse(): @%d, Invalid CoreRow end line in <.scl>.",_lineno);

    _row = NULL;
  }


  void  Parser::_parseScl ( const Utilities::Path& sclPath )
  {
    _circuit->setSclName ( sclPath.string());

    _state = SclFormatRevision;
    _openStream ( sclPath );

    _circuit->setFlags ( Circuit::Scl );

    while ( not _stream.eof() and (_state != SclFinish) ) {
      _readLine ();
      if ( _state == SclFormatRevision ) {
        _parseFormatRevision ( "scl" );
        _state = SclNumRows;
        continue;
      } else {
        if ( _isComment() ) continue;
        switch ( _state ) {
          case SclNumRows:
            _circuit->setNumRows(_parseNum("scl","NumRows")); _state = SclCoreRow; break;
          case SclCoreRow:
            _parseSclCoreRow(); _state = SclCoordinate;  break;
          case SclCoordinate:
            _row->setCoordinate(_parseNum ("scl","Coordinate")); _state = SclHeight; break;
          case SclHeight:
            _row->setHeight(_parseNum ("scl","Height")); _state = SclSitewidth; break;
          case SclSitewidth:
            _row->setSitewidth(_parseNum ("scl","Sitewidth")); _state = SclSitespacing; break;
          case SclSitespacing:
            _row->setSitespacing(_parseNum ("scl","Sitespacing")); _state = SclSiteorient; break;
          case SclSiteorient:
            _parseSclSiteorient(); _state = SclSitesymmetry;  break;
          case SclSitesymmetry:
            _parseSclSitesymmetry(); _state = SclSubrowOrigin;  break;
          case SclSubrowOrigin:
            _parseSclSubrowOrigin(); _state = SclCorerowEnd;  break;
          case SclCorerowEnd:
            _parseSclCorerowEnd ();
            if ( _circuit->getRows().size() >= _circuit->getNumRows() ) {
              _state = SclFinish;
            } else {
              _state = SclCoreRow;
            }
            break;
        }
      }
    }

    _closeStream ();
    _state = 0;
  }


  void  Parser::_parsePlNodePlace ()
  {
    unsigned int orientation      = Orientation::N;
    bool         orientationToken = false;
    double       x                = 0;
    double       y                = 0;

    _tokenize ();

    if ( _tokens.size() < 3 )
      throw Exception("Bookshelf::Parse(): @%d, Invalid Node Placement line in <.pl>.",_lineno);

    for ( size_t itoken=1 ; itoken<_tokens.size() ; ++itoken ) {
    //std::cerr << "F:" << _tokens[itoken] << " ";
      if ( orientationToken ) {
        if ( _keywordCompare("N" ,_tokens[itoken]) == 0 ) { orientation |= Orientation::N; continue; }
        if ( _keywordCompare("E" ,_tokens[itoken]) == 0 ) { orientation |= Orientation::E; continue; }
        if ( _keywordCompare("S" ,_tokens[itoken]) == 0 ) { orientation |= Orientation::S; continue; }
        if ( _keywordCompare("W" ,_tokens[itoken]) == 0 ) { orientation |= Orientation::W; continue; }
        if ( _keywordCompare("FN",_tokens[itoken]) == 0 ) { orientation |= Orientation::FN; continue; }
        if ( _keywordCompare("FE",_tokens[itoken]) == 0 ) { orientation |= Orientation::FE; continue; }
        if ( _keywordCompare("FS",_tokens[itoken]) == 0 ) { orientation |= Orientation::FS; continue; }
        if ( _keywordCompare("FW",_tokens[itoken]) == 0 ) { orientation |= Orientation::FW; continue; }
        break;
      }
      if ( _keywordCompare(":",_tokens[itoken]) == 0 ) { orientationToken = true; continue; }

    //std::cerr << " <X Y>" << std::endl;

      x = toDouble ( _tokens[itoken] );
      if ( ++itoken == _tokens.size() )
        throw Exception("Bookshelf::Parse(): @%d, Invalid Node line in <.pl>.",_lineno);

      y = toDouble ( _tokens[itoken] );
    }

    Node* node = _circuit->getNode ( _tokens[0] );
    if ( node == NULL )
      throw Exception("Bookshelf::Parse(): @%d, Unknown Node <%s> line in <.pl>.",_lineno,_tokens[0]);

    node->setX           ( x );
    node->setY           ( y );
    node->setOrientation ( orientation );
  }


  void  Parser::_parsePl ( const Utilities::Path& plPath )
  {
    _circuit->setPlName ( plPath.string());

    _state = PlFormatRevision;
    _openStream ( plPath );

    _circuit->setFlags ( Circuit::Pl );

    while ( not _stream.eof() ) {
      _readLine ();
      if ( _state == PlFormatRevision ) {
        _parseFormatRevision ( "pl" );
        _state = PlNodePlace;
        continue;
      } else {
        if ( _isComment() ) continue;
        _parsePlNodePlace ();
      }
    }
  }


  Circuit* Parser::parse ( std::string designName, unsigned int slots, unsigned int flags )
  {
    _flags = flags;

    Utilities::Path auxPath ( designName+".aux" );
    if ( not auxPath.exists() ) {
      throw Exception ( "%s .aux file not found.", auxPath.string().c_str() );
    }

    _circuit = new Circuit ();
    _circuit->setDesignName ( designName );

    std::cout << "  o  Reading Bookshelf: <" << auxPath.string() << ">." << std::endl;

    _openStream  ( auxPath );
    _readLine    ();
    _closeStream ();

    _tokenize ();
    if ( _keywordCompare(":",_tokens[1]) == 0 ) {
    // Re-ordering files: .nodes, .nets, .wts, .scl, .pl.
      std::string ordereds [5];

      for ( size_t extension=0 ; extension<5 ; ++extension ) {
        for ( size_t i=2 ; i<_tokens.size() ; ++i ) {
          std::string file ( _tokens[i] );
          size_t      iext = file.rfind ( '.' );

          switch ( extension ) {
            case 0:
              if ( (file.compare(iext,6,".nodes") == 0) and (slots & Circuit::Nodes) )
                ordereds[0] = _tokens[i];
              break;
            case 1:
              if ( (file.compare(iext,5,".nets") == 0) and (slots & Circuit::Nets) )
                ordereds[1] = _tokens[i];
              break;
            case 2:
              if ( (file.compare(iext,4,".wts") == 0) and (slots & Circuit::Wts) )
                ordereds[2] = _tokens[i];
              break;
            case 3:
              if ( (file.compare(iext,4,".scl") == 0) and (slots & Circuit::Scl) )
                ordereds[3] = _tokens[i];
              break;
            case 4:
              if ( (file.compare(iext,3,".pl") == 0) and (slots & Circuit::Pl) )
                ordereds[4] = _tokens[i];
              break;
          }
          if ( not ordereds[extension].empty() ) break;
        }
      }

      if ( ordereds[0].empty() )
        throw Exception("Bookshelf::Parser(): .aux file do not contains <.nodes>.");

      if ( ordereds[1].empty() )
        throw Exception("Bookshelf::Parser(): .aux file do not contains <.nets>.");

      for ( size_t iext=0 ; iext<5 ; ++iext ) {
        if ( ordereds[iext].empty() ) continue;

        Utilities::Path slotPath ( ordereds[iext] );
        if ( slotPath.exists() ) {
          std::cout << "     - Reading <" << slotPath.string() << ">" << std::endl;

          switch ( iext ) {
            case 0: _parseNodes ( slotPath ); break;
            case 1: _parseNets  ( slotPath ); break;
            case 2: _parseWts   ( slotPath ); break;
            case 3: _parseScl   ( slotPath ); break;
            case 4: _parsePl    ( slotPath ); break;
          }
        } else {
          Exception e ( "Bookshelf::parser(): Slot file <%s> not found", slotPath.string().c_str() );
          if ( iext < 2 ) throw e;
          else            std::cerr << e.what() << std::endl;
        }
      }
    } else
      throw Exception ( "Syntax error in .aux file." );

    return _circuit;
  }


} // End of Bookself namespace.
