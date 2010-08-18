
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
// |  C++ Module  :       "./SearchPath.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  "crlcore/SearchPath.h"


namespace {

  using namespace std;


} // End of anonymous namespace.


namespace CRL {

  using namespace std;

  const size_t  SearchPath::npos          = (size_t)-1;
  const string  SearchPath::_selectFailed = "<File or directory not found>";


  SearchPath::SearchPath ()
    : _paths   ()
    , _index   (npos)
    , _selected(_selectFailed)
  { }


  bool SearchPath::_canOpen ( const string& directory, const string& file, ios::openmode mode )
  {
    _selected = directory + "/" + file;
    fstream filestream ( _selected.c_str(), mode );
    if ( filestream.is_open() ) {
      filestream.close ();
      return true;
    }
    _selected = _selectFailed;
    return false;
  }


  void  SearchPath::prepend ( const std::string& path )
  {
    vector<string>::iterator ipath = _paths.begin();

    _index = 0;
    if ( ipath != _paths.end() ) { ++ipath; ++_index; }

    _paths.insert ( ipath, path );
  }


  void  SearchPath::replace ( const string& path, size_t index )
  {
    _index = index;
    if ( index < _paths.size() )
      _paths[index] = path;
  }


  void  SearchPath::select ( const string& path )
  {
    for ( size_t ipath=0 ; ipath < _paths.size() ; ++ipath ) {
      if ( _paths[ipath] == path ) {
        _selected = _paths[ipath];
        _index    = ipath;
        return;
      }
    }

    _selected = _selectFailed;
    _index    = npos;
  }


  size_t  SearchPath::locate ( const string& file, ios::openmode mode, int first, int last )
  {
    if ( hasSelected() and _canOpen(_paths[_index],file,mode) ) return _index;

    for ( int i=max(0,first) ; i < min((int)_paths.size(),last) ; i++ ) {
      if ( _canOpen(_paths[i],file,mode) ) {
        return _index = i;
      }
    }

    return _index = npos;
  }


  size_t  SearchPath::hasPath ( const string& path ) const
  {
    for ( size_t i=0 ; i < _paths.size() ; i++ )
      if ( _paths[i] == path ) return i;
    return npos;
  }


  const string& SearchPath::operator[] ( size_t index ) const
  {
    static const string  OutOfBound = "<index out of bound>";

    if ( index < _paths.size() ) return _paths[index];
    return OutOfBound;
  }


  string  SearchPath::_getString () const
  {
    ostringstream s;

    s << "<SearchPath " << _paths.size()+1 << " directories>";
    return s.str();
  }


  Record *SearchPath::_getRecord () const
  {
    Record* record = new Record ( "<SearchPath>" );
    record->add ( getSlot ( "_paths"   , &_paths    ) );
    record->add ( getSlot ( "_selected", &_selected ) );
    record->add ( getSlot ( "_index"   ,  _index    ) );
    return record;
  }


} // End of CRL namespace.
