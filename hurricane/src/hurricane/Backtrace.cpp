// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Backtrace.cpp"                               |
// +-----------------------------------------------------------------+
//
// The BFD part of this file is based on the work of Don Hatch (hatch@plunk.org)
//     http://www.plunk.org/~hatch/goodies/backtracefilt.C
//
// Here's the basic idea... consider a sample trace
// generated from backtrace() and backtrace_symbols():
//
//     ./libp2lite.so [0xf5c4b4]
//     ./libp2lite.so(malloc+0x1a6) [0xf5de78]
//     /usr/lib/perl5/libperl.so(Perl_safesysmalloc+0x37) [0x33a2c97]
//     /usr/lib/perl5/libperl.so(Perl_gv_init+0x8b) [0x335ce8a]
//     /usr/lib/perl5/libperl.so(Perl_gv_fetchpv+0x412) [0x335daeb]
//     /usr/lib/perl5/libperl.so(Perl_newXS+0x3e) [0x3381c53]
//     /usr/lib/perl5/libperl.so(Perl_boot_core_PerlIO+0x5d) [0x341edef]
//     /usr/lib/perl5/libperl.so [0x3358d6e]
//     /usr/lib/perl5/libperl.so(perl_parse+0x424) [0x335b862]
//     perl(main+0xc2) [0x804927a]
//     /lib/tls/libc.so.6(__libc_start_main+0xd3) [0x8b4e23]
//     perl [0x8049131]
//
// If we run addr2line on the first address:
//     % addr2line -e ./libp2lite.so 0xf5c4b4
//     ??:0
// It's clueless because 0xf5c4b4 is a dynamically relocated runtime address
// but addr2line expects a static address. :-(
//
// However, look at the next line: it says that at runtime,
// malloc+0x1a6 == 0xf5de78.
// If we compare this to the static address of malloc+0x1a6 in the file,
// this will tell us the relocation offset, which will enable us
// to go back and adjust the first address appropriately.
// We can find the static address of malloc in libp2lite.so
// either by running something like "nm libp2lite.so | grep malloc",
// or:
//     % objdump -d --prefix-addresses libp2lite.so | grep "<malloc+0x1a6>"
//     00010e78 <malloc+0x1a6> add    $0x10,%esp
// So the address malloc+0x1a6 got relocated from 0x00010e78
// in the file to 0xf5de78 at runtime.
//
// Therefore the amount that this text segment got shifted by
// during dynamic loading was 0xf5de78-0x00010e78 = 0xf4d000.
// Knowing that, we can now go back to the first dynamic address 0xf5c4b4
// and figure out its static address: 0xf5c4b4 - 0xf4d000 = 0xf4b4.
// 
// Feeding *that* into addr2line:
//     % addr2line -e ./libp2lite.so 0xf4b4
//     /home/hatch/share/wrk/p2lite/p2lite.C:335
// Yippee!!!
// Similarly, we can find the dynamic relocation offsets for perl
// and libperl.so, allowing us to get info for each line of the stack trace.


#if (defined __linux__ || defined __FreeBSD__ || defined __APPLE__)
#include  <execinfo.h>
#ifdef HAVE_LIBBFD
#include  <bfd.h>
#endif
#endif
#include  <sstream>
#include  <iomanip>
#include  <map>
#include  <boost/regex.hpp>
#include  <boost/program_options.hpp>
namespace boptions = boost::program_options;
#include  "hurricane/Backtrace.h"


namespace {

  using namespace std;
  using Hurricane::demangle;


  std::string  environmentMapper ( std::string environmentName )
  {
    if (environmentName == "HOME") return "home";
    return "";
  }


  string  getHome ()
  {
    boptions::options_description options ( "Environment Variables" );
    options.add_options()( "home", boptions::value<string>(), "User's home directory." );

    boptions::variables_map arguments;
    boptions::store  ( boptions::parse_environment(options,environmentMapper), arguments );
    boptions::notify ( arguments );

    return arguments["home"].as<string>();
  }


#ifdef HAVE_LIBBFD

  class Bfd {
    public:
    // Sub-class Request.
      class Request {
        public:
          inline               Request          ( string dsoPath, string symbol, int64_t symbolOffset, int64_t relocatedAddress );
          inline string        dsoPath          () const;
          inline string        symbol           () const;
          inline int64_t       offsetFromSymbol () const;
          inline int64_t       relocatedAddress () const;
          inline string        fileName         () const;
          inline string        funcName         () const;
          inline unsigned int  lineno           () const;
          inline string&       fileName         ();
          inline string&       funcName         ();
          inline unsigned int& lineno           ();
        private:
          string        _dsoPath;
          string        _symbol;
          int64_t       _offsetFromSymbol;
          int64_t       _relocatedAddress;
          string        _filename;
          string        _funcname;
          unsigned int  _lineno;
      };

    public:
      static Bfd*     open               ( string dsoPath );
             void     close              ();
      inline bool     isRelocated        () const;
      inline bfd*     abfd               () const;
      inline string   dsoPath            () const;
      inline int64_t  relocation         () const;
             bool     lookup             ( Request& );
                     ~Bfd                ();
    private:         
                      Bfd                ( string dsoPath );
                      Bfd                ( const Bfd& ) = delete;
             Bfd&     operator=          ( const Bfd& ) = delete;
             void     _clear             ();
             void     _slurpSymtab       ();
             bool     _computeRelocation ( Request& );
    private:
      string        _dsoPath;
      int64_t       _relocationOffset;
      bfd*          _abfd;
      asection*     _textSection;
      asymbol**     _symbolTable;
      int           _nSymbols;
      unsigned int  _symbolSize;
      asymbol*      _scratchSymbol;
      bool          _useMini;
      bool          _useDynamic;
      bool          _isLoaded;
      bool          _isRelocated;
  };


  inline Bfd::Request::Request ( string dsoPath, string symbol, int64_t offsetFromSymbol, int64_t relocatedAddress )
    : _dsoPath         (dsoPath)
    , _symbol          (symbol)
    , _offsetFromSymbol(offsetFromSymbol)
    , _relocatedAddress(relocatedAddress)
    , _filename        ()
    , _funcname        ()
    , _lineno          ()
  { }

  inline string        Bfd::Request::dsoPath          () const { return _dsoPath; };
  inline string        Bfd::Request::symbol           () const { return _symbol; };
  inline int64_t       Bfd::Request::offsetFromSymbol () const { return _offsetFromSymbol; };
  inline int64_t       Bfd::Request::relocatedAddress () const { return _relocatedAddress; };
  inline string        Bfd::Request::fileName         () const { return _filename; };
  inline string        Bfd::Request::funcName         () const { return _funcname; };
  inline unsigned int  Bfd::Request::lineno           () const { return _lineno; };
  inline string&       Bfd::Request::fileName         () { return _filename; };
  inline string&       Bfd::Request::funcName         () { return _funcname; };
  inline unsigned int& Bfd::Request::lineno           () { return _lineno; };


  inline bool  Bfd::isRelocated () const { return _isRelocated; }


  Bfd::Bfd ( string dsoPath )
    : _dsoPath         (dsoPath)
    , _relocationOffset(-1)
    , _abfd            (NULL)
    , _textSection     (NULL)
    , _symbolTable     (NULL)
    , _nSymbols        (0)
    , _symbolSize      (0)
    , _scratchSymbol   (NULL)
    , _useMini         (true)
    , _useDynamic      (false)
    , _isLoaded        (false)
    , _isRelocated     (false)
  { }


  Bfd::~Bfd ()
  {
    _clear();
  }


  void  Bfd::_clear ()
  {
  //if (_scratchSymbol) { free( _scratchSymbol ); _scratchSymbol = NULL; }
    if (_symbolTable)   { free( _symbolTable   ); _symbolTable   = NULL; }
    if (_abfd)          { bfd_close( _abfd );     _abfd          = NULL; }
  }


  void  Bfd::_slurpSymtab ()
  {
    if (_isLoaded or not _abfd) return;
    _isLoaded = true;

    if (not (bfd_get_file_flags(_abfd) & HAS_SYMS)) return;

    if (_useMini) {
      _nSymbols = bfd_read_minisymbols( _abfd, _useDynamic, (void**)&_symbolTable, &_symbolSize );
      if (_nSymbols == 0) {
        _useDynamic = true;
        _nSymbols  = bfd_read_minisymbols( _abfd, _useDynamic, (void**)&_symbolTable, &_symbolSize );
      }
      if (_nSymbols < 0) return;
    } else {
      long symtabSize = bfd_get_symtab_upper_bound( _abfd );
      if (symtabSize < 0) return;
      if (symtabSize > 0) {
        _symbolTable = (asymbol**)malloc( symtabSize );
        _nSymbols    = bfd_canonicalize_symtab( _abfd, _symbolTable );

        if (_nSymbols == 0) {
          free( _symbolTable );
          _symbolTable = NULL;
          return;
        }
      }
    }
  }


  Bfd* Bfd::open ( string dsoPath )
  {
    Bfd* obfd = new Bfd ( dsoPath );

    obfd->_abfd = bfd_openr( dsoPath.c_str(), NULL );

    if (not obfd->_abfd) return obfd;
    if (not bfd_check_format(obfd->_abfd,bfd_object)) { obfd->_clear(); return obfd; }

    obfd->_textSection = bfd_get_section_by_name( obfd->_abfd, ".text" );
    if (not obfd->_textSection) { obfd->_clear(); return obfd; }

    obfd->_slurpSymtab();

    if (not (bfd_get_section_flags(obfd->_abfd, obfd->_textSection) & SEC_ALLOC)) { obfd->_clear(); return obfd; }

    obfd->_scratchSymbol = bfd_make_empty_symbol( obfd->_abfd );
    if (not obfd->_scratchSymbol) { obfd->_clear(); return obfd; }

    return obfd;
  }


  bool  Bfd::_computeRelocation ( Bfd::Request& request )
  {
    if (not _abfd) return false;

    int       index = 0;
    bfd_byte* entry = (bfd_byte*)_symbolTable;
    bfd_byte* end   = entry + _nSymbols*_symbolSize;

    for ( ; entry < end ; entry += _symbolSize, ++index ) {
      asymbol* dsoSymbol = bfd_minisymbol_to_symbol( _abfd, _useDynamic, entry, _scratchSymbol );
      if (not dsoSymbol) continue;

      symbol_info info;
      bfd_get_symbol_info( _abfd, dsoSymbol, &info );
      if ((info.type == 'T') or (info.type == 'W')) {
        if (string(info.name) == request.symbol()) {
          _isRelocated      = true;
          _relocationOffset = request.relocatedAddress() - request.offsetFromSymbol() - (int64_t)info.value;
          return true;
        }
      }
    }

    return false;
  }


  bool  Bfd::lookup ( Bfd::Request& request )
  {
    if (not _isRelocated and not _computeRelocation(request)) return false;

    int64_t       dsoAddress      = request.relocatedAddress() - _relocationOffset;
    bfd_vma       textSectionVma  = bfd_get_section_vma( _abfd, _textSection );
    bfd_size_type textSectionSize = bfd_section_size   ( _abfd, _textSection );
    const char*   fileName        = NULL;
    const char*   funcName        = NULL;

    if (  (dsoAddress < (int64_t) textSectionVma)
       or (dsoAddress > (int64_t)(textSectionVma+textSectionSize) ) ) {
      return false;
    }

    bool found = bfd_find_nearest_line( _abfd
                                      , _textSection
                                      , _symbolTable
                                      , dsoAddress - textSectionVma
                                      , &fileName
                                      , &funcName
                                      , &request.lineno() );

    if (found) {
      if (fileName) request.fileName() = fileName;
      if (funcName) request.funcName() = funcName;
    }
    return found;
  }


  class Bfds {
    public:
            Bfds      ();
           ~Bfds      ();
      Bfd*  find      ( string dsoPath );
      bool  lookup    ( Bfd::Request& );
    private:
            Bfds      ( const Bfds& ) = delete;
      Bfds& operator= ( const Bfds& ) = delete;
    private:
      map<const string,Bfd*>  _bfds;
  };


  Bfds::Bfds ()
    : _bfds()
  { }


  Bfds::~Bfds ()
  {
    for ( auto ielement : _bfds ) delete ielement.second;
  }


  Bfd* Bfds::find ( string dsoPath )
  {
    auto ibfd = _bfds.find( dsoPath );
    if (ibfd != _bfds.end()) return ibfd->second;

    Bfd* obfd = Bfd::open( dsoPath );
    if (not obfd) return NULL;

    _bfds.insert( make_pair(dsoPath,obfd) );

    return obfd;
  }


  bool  Bfds::lookup ( Bfd::Request& request )
  {
    Bfd* obfd = find( request.dsoPath() );
    if (not obfd) return false;

    return obfd->lookup( request );
  }

#endif


}  // Anonymous namespace.


namespace Hurricane {

  using std::string;
  using std::vector;
  using std::setw;
  using std::setfill;
  using std::ostringstream;


// -------------------------------------------------------------------
// Class  :  "Hurricane::Backtrace".


  bool            Backtrace::_inConstructor  = false;
  TextTranslator  Backtrace::_textTranslator = TextTranslator::toTextTranslator();
  const size_t    Backtrace::_stackSize      = 50;


// Examples of stack symbol string:
// * Linux:
//     nwidget(_ZN18SchematicException4initEb+0x47) [0x4515e1]
// * OSX:
//     3 libstdc++.6.dylib 0x9142514b _ZSt9terminatev + 29


  Backtrace::Backtrace ()
    : _stack()
  {
    if (_inConstructor) {
      _stack.push_back( "[BUG] Backtrace::Backtrace(): An error occurred in the backtace *istself*." );
      _stack.push_back( "" );
      _stack.push_back( "      Under RHEL 6, this may be due to a link with a wrong version of <libbfd>," );
      _stack.push_back( "      please check that you have the <devtoolset-2-binutils-devel> package" );
      _stack.push_back( "      installed." );
      _stack.push_back( "" );
      _stack.push_back( "      For other OSs, check for any problems related to BFD." );
      return;
    }
    _inConstructor = true;

#ifndef HAVE_LIBBFD
    _stack.push_back( "Build without BFD (<b>libbfd</b>) support, no filename and line number." );
#endif

#if (defined __linux__ || defined __FreeBSD__ || defined __APPLE__)
    void*  rawStack [ _stackSize ];
    size_t depth    = backtrace ( rawStack, _stackSize );
    char** symbols  = backtrace_symbols ( rawStack, depth );
#ifdef HAVE_LIBBFD
    Bfds   bfds;
#endif
#endif

#if (defined __linux__ || defined __FreeBSD__)
    boost::regex  re ( "(?<dsoPath>[^(]+)\\((?<symbol>[^+]+)\\+(?<symoff>[^)]+)\\) \\[(?<rlocAddr>.+)]" ); 
    boost::cmatch match;
    string        homeDir = getHome();

    for ( size_t i=0 ; i<depth ; ++i ) {
      if (boost::regex_search(symbols[i],match,re)) {
        int64_t       symbolOffset = std::stol( match["symoff"  ], 0, 16 );
        int64_t       rlocAddress  = std::stol( match["rlocAddr"], 0, 16 );
        ostringstream debugline;

#ifdef HAVE_LIBBFD
        Bfd::Request  request ( match["dsoPath"], match["symbol"], symbolOffset, rlocAddress );

        if (bfds.lookup(request) and not request.fileName().empty()) {
          string fileName = request.fileName();
          if (not request.fileName().compare( 0, homeDir.size(), homeDir ))
            fileName = "~" + request.fileName().substr( homeDir.size() );

          debugline << "<b>" << demangle(request.funcName()) << "</b>&nbsp;&nbsp;"
                    << fileName << ":" << request.lineno();
        } else {
#endif
          string demangled = demangle( match["symbol"] );
          if (demangled.empty()) demangled = match["symbol"];

          debugline << "<b>" << demangled << "</b>";
#ifdef HAVE_LIBBFD
        }
#endif
        _stack.push_back( debugline.str() );
      } else {
        _stack.push_back( symbols[i] );
      }
    }
#else
#  ifdef  __APPLE__
    boost::regex re ( "(\\d+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+\\+\\s+(\\d+)" ); 
    boost::cmatch match;

    for ( size_t i=0 ; i<depth ; ++i ) {
      if ( boost::regex_search(symbols[i],match,re) ) {
        string function  ( match[4].first, match[4].second );
        string demangled ( demangle(function.c_str()) );
        if (demangled.empty())
          _stack.push_back( (demangled.empty()) ? function : demangled );
        else {
          string reformated ( match[1].first, match[1].second );
          reformated += " <b>" + demangled + "</b>";
          _stack.push_back( reformated );
        }
      } else {
        _stack.push_back( symbols[i] );
      }
    }
#  else
    _stack.push_back( "Backtrace only supported under FreeBSD, Linux and OSX." );
#  endif
#endif
    _inConstructor = false;
  }


  Backtrace::~Backtrace ()
  { }


  string  Backtrace::htmlWhere () const
  {
    ostringstream where;

    for ( size_t depth=0 ; depth<_stack.size() ; ++depth )
      where << "<tt>[" << setw(2) << setfill('0') << depth << "] " << _stack[depth] << "</tt><br>";

    return where.str();
  }


}  // End of Hurricane namespace.
