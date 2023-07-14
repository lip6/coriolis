// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Uiversité 2013-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/utilities/Path.h"                  |
// +-----------------------------------------------------------------+


#pragma  once
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
                  , Invalidated = 0x00000020
                  };
      enum Mode_T { s_ifmt      = S_IFMT    // 0x00170000
                  , s_ifsock    = S_IFSOCK  // 0x00140000
                  , s_iflnk     = S_IFLNK   // 0x00120000
                  , s_ifreg     = S_IFREG   // 0x00100000
                  , s_ifblk     = S_IFBLK   // 0x00060000
                  , s_ifdir     = S_IFDIR   // 0x00040000
                  , s_ifchr     = S_IFCHR   // 0x00020000
                  , s_ififo     = S_IFIFO   // 0x00010000
                  , s_isuid     = S_ISUID   // 0x00004000
                  , s_isgid     = S_ISGID   // 0x00002000
                  , s_isvtx     = S_ISVTX   // 0x00001000
                  , s_irusr     = S_IWUSR   // 0x00000400
                  , s_iwusr     = S_IWUSR   // 0x00000200
                  , s_ixusr     = S_IXUSR   // 0x00000100
                  , s_irwxu     = s_irusr|s_iwusr|s_ixusr
                  , s_irgrp     = S_IRGRP   // 0x00000040
                  , s_iwgrp     = S_IWGRP   // 0x00000020
                  , s_ixgrp     = S_IXGRP   // 0x00000010
                  , s_irwxg     = s_irgrp|s_iwgrp|s_ixgrp
                  , s_iroth     = S_IROTH   // 0x00000004
                  , s_iwoth     = S_IWOTH   // 0x00000002
                  , s_ixoth     = S_IXOTH   // 0x00000001
                  , s_irwxo     = s_iroth|s_iwoth|s_ixoth
                  };
    public:
      class Stat {
        public:
          inline               Stat   ();
          inline               Stat   ( const Stat& );
          inline  dev_t        dev    () const;
          inline  ino_t        ino    () const;
          inline  mode_t       mode   () const;
          inline  nlink_t      nlink  () const;
          inline  uid_t        uid    () const;
          inline  gid_t        gid    () const;
          inline  dev_t        rdev   () const;
          inline  off_t        size   () const;
          inline  blksize_t    blksize() const;
          inline  blkcnt_t     blocks () const;
          inline  time_t       atime  () const;
          inline  time_t       mtime  () const;
          inline  time_t       ctime  () const;
          inline  struct stat* c_stat ();
        private:
          struct stat  _stat;
      };
    public:
      static Path               cwd         ();
    public:                                 
      inline                    Path        ( const std::string& path="" );
      inline                    Path        ( const char* );
      inline                    Path        ( const Path& );
      inline bool               absolute    () const;
      inline bool               extension   () const;
             bool               exists      () const;
             bool               access      ( unsigned int mode ) const;
             unsigned int       mode        () const;
             Stat               stat        () const;
      inline bool               isfile      () const;
      inline bool               isdir       () const;
      inline bool               islink      () const;
      inline bool               empty       () const;
      inline size_t             size        () const;
      inline Path               head        () const;
      inline Path               tail        () const;
             Path               basename    ( const std::string& ext="" ) const;
             Path               dirname     () const;
             Path               stem        () const;
             Path               subpath     ( size_t begin, size_t end ) const;
             Path               join        ( const Path& tail ) const;
             std::string        ext         () const;
             bool               mkdir       ( unsigned int mode=s_irwxu|s_irwxg|s_irwxo ) const;
             std::vector<Path>  listdir     () const;
             const std::string& toString    () const;
             std::string        toPyModPath () const;
      inline const char*        c_str       () const;
             Path&              operator=   ( const Path& );
             Path&              operator/=  ( const Path& );
      inline Path&              operator/=  ( const char* );
      inline Path&              operator/=  ( const std::string& );
    private:                                
      static int                _toUnistd   ( unsigned int mode );
      static int                _toMode_T   ( unsigned int mode );
      static void               _split      ( const std::string&, std::vector<std::string>&, unsigned int& );
             void               _normalize  ();
             void               _normalize  ( const std::string& );
    private:
      mutable unsigned int              _flags;
              std::vector<std::string>  _elements;
      mutable std::string               _pathcache;
  };


  inline  Path::Stat::Stat ()
  {
    _stat.st_dev     = 0;
    _stat.st_ino     = 0;
    _stat.st_mode    = 0;
    _stat.st_nlink   = 0;
    _stat.st_uid     = 0;
    _stat.st_gid     = 0;
    _stat.st_rdev    = 0;
    _stat.st_size    = 0;
    _stat.st_blksize = 0;
    _stat.st_blocks  = 0;
    _stat.st_atime   = 0;
    _stat.st_mtime   = 0;
    _stat.st_ctime   = 0;
  }

  inline  Path::Stat::Stat ( const Stat& other )
  {
    _stat.st_dev     = other._stat.st_dev;
    _stat.st_ino     = other._stat.st_ino;
    _stat.st_mode    = other._stat.st_mode;
    _stat.st_nlink   = other._stat.st_nlink;
    _stat.st_uid     = other._stat.st_uid;
    _stat.st_gid     = other._stat.st_gid;
    _stat.st_rdev    = other._stat.st_rdev;
    _stat.st_size    = other._stat.st_size;
    _stat.st_blksize = other._stat.st_blksize;
    _stat.st_blocks  = other._stat.st_blocks;
    _stat.st_atime   = other._stat.st_atime;
    _stat.st_mtime   = other._stat.st_mtime;
    _stat.st_ctime   = other._stat.st_ctime;
  }

  inline  dev_t        Path::Stat::dev    () const { return _stat.st_dev; }
  inline  ino_t        Path::Stat::ino    () const { return _stat.st_ino; }
  inline  mode_t       Path::Stat::mode   () const { return _stat.st_mode; }
  inline  nlink_t      Path::Stat::nlink  () const { return _stat.st_nlink; }
  inline  uid_t        Path::Stat::uid    () const { return _stat.st_uid; }
  inline  gid_t        Path::Stat::gid    () const { return _stat.st_gid; }
  inline  dev_t        Path::Stat::rdev   () const { return _stat.st_rdev; }
  inline  off_t        Path::Stat::size   () const { return _stat.st_size; }
  inline  blksize_t    Path::Stat::blksize() const { return _stat.st_blksize; }
  inline  blkcnt_t     Path::Stat::blocks () const { return _stat.st_blocks; }
  inline  time_t       Path::Stat::atime  () const { return _stat.st_atime; }
  inline  time_t       Path::Stat::mtime  () const { return _stat.st_mtime; }
  inline  time_t       Path::Stat::ctime  () const { return _stat.st_ctime; }
  inline  struct stat* Path::Stat::c_stat () { return &_stat; }
  
  
  inline Path::Path ( const char*        path ) : _flags(Invalidated), _elements(), _pathcache() { _normalize(path); }
  inline Path::Path ( const std::string& path ) : _flags(Invalidated), _elements(), _pathcache() { _normalize(path); }
  inline Path::Path ( const Path&        path ) : _flags(path._flags), _elements(path._elements), _pathcache(path._pathcache) {}
  
  inline bool        Path::isfile     () const { return mode() & s_ifmt; }
  inline bool        Path::isdir      () const { return mode() & s_ifdir; }
  inline bool        Path::islink     () const { return mode() & s_iflnk; }
  inline bool        Path::absolute   () const { return _flags&Absolute; }
  inline bool        Path::extension  () const { return _flags&Extension; }
  inline bool        Path::empty      () const { return _elements.empty(); }
  inline size_t      Path::size       () const { return _elements.size(); }
  inline Path        Path::head       () const { return (size() > 1) ? subpath(0,size()-1) : *this; }
  inline Path        Path::tail       () const { return (size() > 1) ? subpath(1,size()) : *this; }
  inline const char* Path::c_str      () const { return Path::toString().c_str(); }
  inline Path&       Path::operator/= ( const std::string& tail ) { (*this) /= Path(tail); return *this; }
  inline Path&       Path::operator/= ( const char* tail ) { (*this) /= Path(tail); return *this; }

  inline Path        operator/ ( const Path&        rop, const Path&        lop ) { return rop.join(lop); }
  inline Path        operator/ ( const std::string& rop, const Path&        lop ) { return Path(rop) / lop; }
  inline Path        operator/ ( const Path&        rop, const char*        lop ) { return rop / Path(lop); }
  inline Path        operator/ ( const Path&        rop, const std::string& lop ) { return rop / Path(lop); }


} // Utilities namespace.
