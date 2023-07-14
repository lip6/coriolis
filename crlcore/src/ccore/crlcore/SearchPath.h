// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/SearchPath.h"                        |
// +-----------------------------------------------------------------+


#ifndef  CRL_SEARCH_PATH_H
#define  CRL_SEARCH_PATH_H

#include <string>
#include <vector>
#include "hurricane/Commons.h"
#include "hurricane/Slot.h"


namespace CRL {

  using Hurricane::Record;
  using Hurricane::_TName;


  class SearchPath {
    public:
      class Element {
        public:
          inline                    Element      ( const std::string& path="", const std::string& name="" );
          inline bool               empty        () const;
          inline const std::string& getPath      () const;
          inline const std::string& getName      () const;
          inline std::string        _getTypeName () const;
                 std::string        _getString   () const;
                 Record*            _getRecord   () const;
        private:
          std::string  _path;
          std::string  _name;
      };
    public:
      static const size_t       npos;
      static std::string        extractLibName ( const std::string& );
                                SearchPath     ();
    public:                                    
      inline void               reset          ();
      inline size_t             append         ( const std::string& path, const std::string& name="" );
             size_t             prepend        ( const std::string& path, const std::string& name="");
             size_t             replace        ( const std::string& path, const std::string&, size_t index );
             void               remove         ( size_t index );
             size_t             locate         ( const std::string&        file
                                               ,       std::ios::openmode  mode =std::ios::in
                                               ,       int                 first=0
                                               ,       int                 last =64 );
             void               select         ( const std::string& );
      inline size_t             getSize        () const;
      inline const std::string& getSelected    () const;
      inline size_t             getIndex       () const;
      inline bool               hasSelected    () const;
             size_t             hasLib         ( const std::string& name ) const;
             size_t             hasPath        ( const std::string& path ) const;
             const Element&     operator[]     ( size_t index ) const;
    private:
      static const std::string         _selectFailed;
             std::vector<Element>      _paths;
             size_t                    _index;
             std::string               _selected;
    private:
                          SearchPath   ( const SearchPath& );
             bool         _canOpen     ( const Element&     directory
                                       , const std::string& file
                                       , std::ios::openmode mode
                                       );
    public:
      inline std::string  _getTypeName () const;
             std::string  _getString   () const;
             Record*      _getRecord   () const;
  };


  // Inline Functions.
  inline void               SearchPath::reset        () { _paths.resize(1); }
  inline size_t             SearchPath::getSize      () const { return _paths.size(); }
  inline const std::string& SearchPath::getSelected  () const { return _selected; }
  inline size_t             SearchPath::getIndex     () const { return _index; }
  inline bool               SearchPath::hasSelected  () const { return _index != npos; }
  inline std::string        SearchPath::_getTypeName () const { return _TName("SearchPath"); }

  inline size_t  SearchPath::append ( const std::string& path, const std::string& name ) {
    _paths.push_back ( Element ( path, name.empty()?extractLibName(path):name ) );
    return _paths.size()-1;
  }

  inline SearchPath::Element::Element ( const std::string& path, const std::string& name )
    : _path(path)
    , _name(name.empty()?SearchPath::extractLibName(path):name)
  { }

  inline bool               SearchPath::Element::empty        () const { return _path.empty() and _name.empty(); }
  inline const std::string& SearchPath::Element::getPath      () const { return _path; }
  inline const std::string& SearchPath::Element::getName      () const { return _name; }
  inline std::string        SearchPath::Element::_getTypeName () const { return "SearchPath::Element"; }


} // CRL namespace.


INSPECTOR_P_SUPPORT(CRL::SearchPath);
INSPECTOR_PR_SUPPORT(CRL::SearchPath::Element);


#endif  // CRL_SEARCH_PATH_H
