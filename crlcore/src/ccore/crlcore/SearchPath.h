
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./SearchPath.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_SEARCH_PATH__
#  define  __CRL_SEARCH_PATH__


#include  <string>
#include  <vector>

#include  "hurricane/Commons.h"
#include  "hurricane/Slot.h"


namespace CRL {


  using Hurricane::Record;
  using Hurricane::_TName;


  class SearchPath {
    public:
      static const size_t       npos;
                                SearchPath  ();
    public:
      inline void               reset        ();
      inline void               append       ( const std::string& path );
      inline void               prepend      ( const std::string& path );
             void               replace      ( const std::string& path, size_t index );
             size_t             locate       ( const std::string&        file
                                             ,       std::ios::openmode  mode =std::ios::in
                                             ,       int                 first=0
                                             ,       int                 last =64 );
      inline size_t             getSize      () const;
      inline const std::string& getSelected  () const;
      inline size_t             getIndex     () const;
             bool               hasSelected  () const;
             size_t             hasPath      ( const std::string& path ) const;
             const std::string& operator[]   ( size_t index ) const;
    private:
             std::vector<std::string>  _paths;
             size_t                    _index;
             std::string               _selected;
    private:
                          SearchPath   ( const SearchPath& );
    public:
      inline std::string  _getTypeName () const;
             std::string  _getString   () const;
             Record*      _getRecord   () const;
  };


  // Inline Functions.
  inline void               SearchPath::reset        () { _paths.resize(1); }
  inline void               SearchPath::prepend      ( const std::string& path ) { _paths.insert ( _paths.begin(), path ); }
  inline void               SearchPath::append       ( const std::string& path ) { _paths.push_back(path); }
  inline size_t             SearchPath::getSize      () const { return _paths.size(); }
  inline const std::string& SearchPath::getSelected  () const { return _selected; }
  inline size_t             SearchPath::getIndex     () const { return _index; }
  inline bool               SearchPath::hasSelected  () const { return _index != npos; }
  inline std::string        SearchPath::_getTypeName () const { return _TName("SearchPath"); }



} // End of CRL namespace.


# endif
