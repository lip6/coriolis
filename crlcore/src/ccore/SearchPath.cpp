
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


namespace CRL {

  using namespace std;

  const size_t  SearchPath::npos = (size_t)-1;


  SearchPath::SearchPath ()
    : _paths   ()
    , _index   (npos)
    , _selected("")
  {
  }


  void  SearchPath::replace ( const string& path, size_t index )
  {
    if ( index < _paths.size() )
      _paths[index] = path;
  }


  size_t  SearchPath ::locate ( const string& file, ios::openmode mode, int first, int last )
  {
    for ( int i=max(0,first) ; i < min((int)_paths.size(),last) ; i++ ) {
      string path = _paths[i] + "/" + file;

      fstream  filestream ( path.c_str(), mode );
      if ( filestream.is_open() ) {
        filestream.close ();
        _selected = path;
        return _index = i;
      }
    }

    _selected = "<File not found>";
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
