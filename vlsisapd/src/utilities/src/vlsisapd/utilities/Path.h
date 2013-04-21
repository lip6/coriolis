
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2013-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |    M i s c e l l a n e o u s   U t i l i t i e s                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./utilities/Path.h"                            |
// +-----------------------------------------------------------------+


#ifndef VLSISAPD_UTILITIES_PATH_H
#define VLSISAPD_UTILITIES_PATH_H

#include <string>
#include <vector>


namespace Utilities {


  class Path {
    public:
      enum Flag   { NoFlag      = 0x00000000
                  , Absolute    = 0x00000001
                  , Extension   = 0x00000002
                  , PermRead    = 0x00000004
                  , PermWrite   = 0x00000008
                  , PermExecute = 0x00000010
                  , PermAll     = PermRead|PermWrite|PermExecute
                  };
      enum Mode_T { s_ifmt      = 0x00170000
                  , s_ifsock    = 0x00140000
                  , s_iflnk     = 0x00120000
                  , s_ifreg     = 0x00100000
                  , s_ifblk     = 0x00060000
                  , s_ifdir     = 0x00040000
                  , s_ifchr     = 0x00020000
                  , s_ififo     = 0x00010000
                  , s_isuid     = 0x00004000
                  , s_isgid     = 0x00002000
                  , s_isvtx     = 0x00001000
                  , s_irusr     = 0x00000400
                  , s_iwusr     = 0x00000200
                  , s_ixusr     = 0x00000100
                  , s_irwxu     = s_irusr|s_iwusr|s_ixusr
                  , s_irgrp     = 0x00000040
                  , s_iwgrp     = 0x00000020
                  , s_ixgrp     = 0x00000010
                  , s_irwxg     = s_irgrp|s_iwgrp|s_ixgrp
                  , s_iroth     = 0x00000004
                  , s_iwoth     = 0x00000002
                  , s_ixoth     = 0x00000001
                  , s_irwxo     = s_iroth|s_iwoth|s_ixoth
                  };
    public:
      static Path               cwd        ();
    public:
      inline                    Path       ( const std::string& path="" );
      inline                    Path       ( const char* );
      inline                    Path       ( const Path& );
      inline bool               absolute   () const;
      inline bool               extension  () const;
             bool               exists     () const;
             bool               access     ( unsigned int mode ) const;
      inline bool               empty      () const;
      inline size_t             size       () const;
      inline Path               head       () const;
      inline Path               tail       () const;
             Path               basename   ( const std::string& ext="" ) const;
             Path               dirname    () const;
             Path               stem       () const;
             Path               subpath    ( size_t begin, size_t end ) const;
             Path               join       ( const Path& tail ) const;
             std::string        ext        () const;
             bool               mkdir      ( unsigned int mode=s_irwxu|s_irwxg|s_irwxo ) const;
             const std::string& string     () const;
      inline const char*        c_str      () const;
             Path&              operator=  ( const Path& );
             Path&              operator/= ( const Path& );
      inline Path&              operator/= ( const char* );
      inline Path&              operator/= ( const std::string& );
    private:
      static int                _toUnistd  ( unsigned int mode );
      static int                _toMode_T  ( unsigned int mode );
      static void               _split     ( const std::string&, std::vector<std::string>&, unsigned int& );
             void               _normalize ();
             void               _normalize ( const std::string& );
    private:
              unsigned int              _flags;
              std::vector<std::string>  _elements;
      mutable std::string               _pathcache;
  };
  
  
  inline Path::Path ( const char*        path ) : _flags(NoFlag), _elements(), _pathcache() { _normalize(path); }
  inline Path::Path ( const std::string& path ) : _flags(NoFlag), _elements(), _pathcache() { _normalize(path); }
  inline Path::Path ( const Path&        path ) : _flags(path._flags), _elements(path._elements), _pathcache(path._pathcache) {}
  
  inline bool        Path::absolute   () const { return _flags&Absolute; }
  inline bool        Path::extension  () const { return _flags&Extension; }
  inline bool        Path::empty      () const { return _elements.empty(); }
  inline size_t      Path::size       () const { return _elements.size(); }
  inline Path        Path::head       () const { return (size() > 1) ? subpath(0,size()-1) : *this; }
  inline Path        Path::tail       () const { return (size() > 1) ? subpath(1,size()) : *this; }
  inline const char* Path::c_str      () const { return Path::string().c_str(); }
  inline Path&       Path::operator/= ( const std::string& tail ) { (*this) /= Path(tail); return *this; }
  inline Path&       Path::operator/= ( const char* tail ) { (*this) /= Path(tail); return *this; }

  inline Path        operator/ ( const Path&        rop, const Path&        lop ) { return rop.join(lop); }
  inline Path        operator/ ( const std::string& rop, const Path&        lop ) { return Path(rop) / lop; }
  inline Path        operator/ ( const Path&        rop, const char*        lop ) { return rop / Path(lop); }
  inline Path        operator/ ( const Path&        rop, const std::string& lop ) { return rop / Path(lop); }


} // Utilities namespace.

#endif // VLSISAPD_UTILITIES_PATH_H
