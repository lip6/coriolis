
// -*- C++ -*-


# ifndef  __CRL_SEARCH_PATH__
#   define  __CRL_SEARCH_PATH__


# include  <string>
# include  <vector>

# include  "hurricane/Commons.h"
# include  "hurricane/Slot.h"


namespace CRL {


  using namespace std;

  using Hurricane::Record;
  using Hurricane::_TName;


  class SearchPath {
 
    public:
    // Constants.
      static const size_t    npos;

    // Constructors.
                             SearchPath  ();

    // Methods.
      inline void            reset        ();
      inline void            append       ( const string& path );
             void            replace      ( const string& path, size_t index );
             size_t          locate       ( const string&        file
                                          ,       ios::openmode  mode =ios::in
                                          ,       int            first=0
                                          ,       int            last =64 );
      inline size_t          getSize      () const;
      inline const string&   getSelected  () const;
      inline size_t          getIndex     () const;
             bool            hasSelected  () const;
             bool            hasPath      ( const string& path ) const;
             const string&   operator[]   ( size_t index ) const;

    // Internal - Attributes.
    private:
             vector<string>  _paths;
             size_t          _index;
             string          _selected;

    // Internal - Constructors.
    private:
                             SearchPath   ( const SearchPath& );

    // Hurricane management.
    public:
      inline string          _getTypeName () const;
             string          _getString   () const;
             Record*         _getRecord   () const;
  };


  // Inline Functions.
  inline void          SearchPath::reset        () { _paths.resize(1); }
  inline void          SearchPath::append       ( const string& path ) { _paths.push_back(path); }
  inline size_t        SearchPath::getSize      () const { return _paths.size(); }
  inline const string& SearchPath::getSelected  () const { return _selected; }
  inline size_t        SearchPath::getIndex     () const { return _index; }
  inline bool          SearchPath::hasSelected  () const { return _index != npos; }
  inline string        SearchPath::_getTypeName () const { return _TName("SearchPath"); }



} // End of CRL namespace.


# endif
