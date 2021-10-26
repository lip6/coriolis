// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2015-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/CellDatas.h"                         |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <vector>
#include "hurricane/utilities/Path.h"
#include "crlcore/Catalog.h"
namespace Hurricane {
  class Cell;
}


namespace CRL {

  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "CellLoader".

  class CellLoader {
    public:
      typedef  std::function<Cell*(std::string)>  Callback;
      enum Flags { NoFlags   = 0x0000
                 , Native    = 0x0001
                 , Importer  = 0x0002
                 , MultiCell = 0x0004
                 , Loaded    = 0x0008
                 , NewViewer = 0x0010
                 };
    public:
      inline               CellLoader   ( std::string  format
                                        , std::string  comment
                                        , unsigned int flags
                                        , unsigned int viewFlags
                                        , Callback     callback=nullptr
                                        );
      inline       std::string   getFormat    () const;
      inline       std::string   getComment   () const;
      inline       unsigned int  getFlags     () const;
      inline       unsigned int  getViewFlags () const;
      inline const Callback&     getCallback  () const;
      inline       void          setFlags     ( unsigned int mask );
      inline       void          resetFlags   ( unsigned int mask );
    private:
      std::string   _format;
      std::string   _comment;
      unsigned int  _flags;
      unsigned int  _viewFlags;
      Callback      _callback;
  };


  inline CellLoader::CellLoader ( std::string  format
                                , std::string  comment
                                , unsigned int flags
                                , unsigned int viewFlags
                                , Callback     callback
                                )
    : _format   (format)
    , _comment  (comment)
    , _flags    (flags)
    , _viewFlags(viewFlags)
    , _callback (callback)
  {
    if (_callback != nullptr) _flags |= Importer;
  }

  inline       std::string           CellLoader::getFormat    () const { return _format; }
  inline       std::string           CellLoader::getComment   () const { return _comment; }
  inline       unsigned int          CellLoader::getFlags     () const { return _flags; }
  inline       unsigned int          CellLoader::getViewFlags () const { return _viewFlags; }
  inline const CellLoader::Callback& CellLoader::getCallback  () const { return _callback; }
  inline       void                  CellLoader::setFlags     ( unsigned int mask ) { _flags |=  mask; }
  inline       void                  CellLoader::resetFlags   ( unsigned int mask ) { _flags &= ~mask; }


  class LessCellLoader {
    public:
      inline bool  operator() ( const CellLoader* lhs, const CellLoader* rhs ) const
      { return lhs->getFormat() < rhs->getFormat(); }
  };


  typedef  std::set<CellLoader*,LessCellLoader>  CellLoaderSet;


// -------------------------------------------------------------------
// Class  :  "CellLoaders".

  class CellLoaders {
    public:
      static        CellLoaders*         get         ();
    public:                     
                                         CellLoaders ();
                                        ~CellLoaders ();
      inline        bool                 empty       () const;
      inline        size_t               size        () const;
      static        unsigned int         lmask       ();
                    const CellLoader*    getLoader   ( std::string format ) const;
                    const CellLoader*    getLoader   ( unsigned int bit ) const;
             inline const CellLoaderSet& getLoaders  () const;
                    void                 addLoader   ( CellLoader* );
    private:
      static unsigned int   _loaderBit;
      static unsigned int   _loaderBitMask;
      static CellLoaders*   _singleton;
             CellLoaderSet  _loaders;
  };


  inline       size_t         CellLoaders::size       () const { return _loaders.size(); }
  inline       bool           CellLoaders::empty      () const { return _loaders.empty(); }
  inline const CellLoaderSet& CellLoaders::getLoaders () const { return _loaders; }


// -------------------------------------------------------------------
// Class  :  "CellDatas".

  class CellDatas {
    public:
      inline static       void             setLibraryPath ( const Utilities::Path& path );
      inline static const Utilities::Path& getLibraryPath ();
    public:
                                   CellDatas        ( std::string name );
                                   CellDatas        ( Cell* );
      inline       bool            isLoaded         () const;
      inline       bool            isInMemory       () const;
      inline       std::string     getName          () const;
      inline       Cell*           getCell          () const;
      inline const Catalog::State* getState         () const;
      inline       unsigned int    getFlags         () const;
      inline       unsigned int    getActiveFlags   () const;
      inline       void            setFlags         ( unsigned int flags );
      inline       void            setActiveFlags   ( unsigned int flags );
      inline       void            resetActiveFlags ( unsigned int flags );
      inline       void            setCell          ( Cell* );
      inline       void            setState         ( const Catalog::State* );
    private:
      static Utilities::Path  _libraryPath;
            std::string       _name;
            unsigned int      _flags;        // Tells what is *available* to be loaded.
            unsigned int      _activeFlags;  // Tells what has *actually* be loaded.
            Cell*             _cell;
      const Catalog::State*   _state;
  };


  inline bool  operator< ( const CellDatas& lhs, const CellDatas& rhs )
  { return lhs.getName() < rhs.getName(); }


  inline bool  LessCellDatas ( const CellDatas* lhs, const CellDatas* rhs )
  { return lhs->getName() < rhs->getName(); }


  inline       void             CellDatas::setLibraryPath   ( const Utilities::Path& path ) { _libraryPath = path; }
  inline const Utilities::Path& CellDatas::getLibraryPath   () { return _libraryPath; }
  inline       bool             CellDatas::isLoaded         () const { return (_activeFlags & CellLoader::Loaded) or (_cell != NULL); }
  inline       bool             CellDatas::isInMemory       () const { return _cell != NULL; }
  inline       std::string      CellDatas::getName          () const { return _name; }
  inline       Cell*            CellDatas::getCell          () const { return _cell; }
  inline const Catalog::State*  CellDatas::getState         () const { return _state; }
  inline       unsigned int     CellDatas::getFlags         () const { return _flags; }
  inline       unsigned int     CellDatas::getActiveFlags   () const { return _activeFlags; }
  inline       void             CellDatas::setFlags         ( unsigned int flags ) { _flags |= flags; }
  inline       void             CellDatas::setActiveFlags   ( unsigned int flags ) { _activeFlags |= flags; }
  inline       void             CellDatas::resetActiveFlags ( unsigned int flags ) { _activeFlags &= ~flags; }
  inline       void             CellDatas::setState         ( const Catalog::State* state ) { _state = state; }
  inline       void             CellDatas::setCell          ( Cell* cell ) { _cell = cell; }


}
