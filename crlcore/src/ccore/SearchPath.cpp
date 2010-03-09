
// -*- C++ -*-


# include  "crlcore/SearchPath.h"


namespace CRL {


  const size_t  SearchPath::npos = (size_t)-1;


  SearchPath::SearchPath ()
    : _paths()
    , _index(npos)
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


  bool  SearchPath::hasPath ( const string& path ) const
  {
    for ( size_t i=0 ; i < _paths.size() ; i++ )
      if ( _paths[i] == path ) return true;
    return false;
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
