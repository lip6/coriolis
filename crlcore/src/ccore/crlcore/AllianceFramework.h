// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/AllianceFramework.h"                 |
// +-----------------------------------------------------------------+

#pragma  once
#include <map>
#include <limits>
#include "hurricane/Cell.h"
#include "crlcore/Environment.h"
#include "crlcore/AllianceLibrary.h"
#include "crlcore/Catalog.h"
#include "crlcore/ParsersDrivers.h"


namespace CRL {

  using Hurricane::Observable;
  using Hurricane::BaseObserver;
  using Hurricane::JsonObject;
  using Hurricane::JsonStack;
  using Hurricane::DbU;
  using Hurricane::Cell;
  using Hurricane::Net;
  class RoutingGauge;
  class CellGauge;


  class AllianceFramework : public DBo {
      typedef DBo Super;
    public:
      enum FunctionsFlags      { NoFlags          = 0
                               , NoPythonInit     = (1<<0)
                               };                 
      enum InstancesCountFlags { Recursive        = (1<<0)
                               , IgnoreFeeds      = (1<<1)
                               , IgnoreDiodes     = (1<<2)
                               , IgnorePowerFeeds = (1<<3)
                               , IgnoreNonLogic   = IgnoreFeeds|IgnoreDiodes|IgnorePowerFeeds
                               , TerminalNetlist  = (1<<4)
                               };                   
      enum LibraryFlags        { CreateLibrary    = (1<<0)
                               , AppendLibrary    = (1<<1)
                               , HasCatalog       = (1<<2)
                               };                 
      enum NotifyFlags         { AddedLibrary     = (1<<0)
                               , RemovedLibrary   = (1<<1)
                               , ConfigChanged    = (1<<2)
                               };
    public:
      static  AllianceFramework*       create                   ( unsigned long flags=NoFlags );
      virtual void                     _postCreate              ();
      virtual void                     _preDestroy              ();
    // Accessors.                     
      static  AllianceFramework*       get                      ();
              string                   getPrint                 () const;
    // Predicates.                    
              Catalog::State*          isInCatalog              ( const Name& );
              Catalog::State*          isInCatalog              ( string );
      inline  bool                     isPOWER                  ( const char*   name );
      inline  bool                     isPOWER                  ( const string& name );
      inline  bool                     isPOWER                  ( const Name&   name );
      inline  bool                     isGROUND                 ( const char*   name );
      inline  bool                     isGROUND                 ( const string& name );
      inline  bool                     isGROUND                 ( const Name&   name );
      inline  bool                     isCLOCK                  ( const char*   name );
      inline  bool                     isCLOCK                  ( const string& name );
      inline  bool                     isCLOCK                  ( const Name&   name );
      inline  bool                     isBLOCKAGE               ( const char*   name );
      inline  bool                     isBLOCKAGE               ( const string& name );
      inline  bool                     isBLOCKAGE               ( const Name&   name );
      inline  bool                     isBLOCKAGE               ( const Net*    net );
      inline  bool                     isPad                    ( const char*   name );
      inline  bool                     isPad                    ( const string& name );
      inline  bool                     isPad                    ( const Name&   name );
      inline  bool                     isPad                    ( const Cell* );
      inline  bool                     isRegister               ( const char*   name );
      inline  bool                     isRegister               ( const string& name );
      inline  bool                     isRegister               ( const Name&   name );
      inline  bool                     isRegister               ( const Cell* );
    // Accessors.
      inline  Environment*             getEnvironment           ();
      inline  Catalog*                 getCatalog               ();
      inline  const Name&              getParentLibraryName     () const;
      inline  Library*                 getParentLibrary         ();
              Library*                 getLibrary               ( unsigned int index );
              Library*                 getLibrary               ( const Name& libName );
              AllianceLibrary*         getAllianceLibrary       ( unsigned int index );
              AllianceLibrary*         getAllianceLibrary       ( const Name& libName, unsigned int flags );
              AllianceLibrary*         getAllianceLibrary       ( Library* );
              AllianceLibrary*         createLibrary            ( const string& path, unsigned int flags, string libName="" );
              AllianceLibrary*         wrapLibrary              ( Library*, unsigned int flags );
      inline  const AllianceLibraries& getAllianceLibraries     () const;
              void                     saveLibrary              ( Library* );
              void                     saveLibrary              ( AllianceLibrary* );
              RoutingGauge*            getRoutingGauge          ( const Name& name="" );
              CellGauge*               getCellGauge             ( const Name& name="" );
              CellGauge*               matchCellGauge           ( DbU::Unit width, DbU::Unit height ) const;
              CellGauge*               matchCellGaugeByHeight   ( DbU::Unit height ) const;
      inline  const Name               getDefaultCGPinLayerName () const;
    // Modifiers.                     
              RoutingGauge*            setRoutingGauge          ( const Name& name="" );
              CellGauge*               setCellGauge             ( const Name& name="" );
              void                     addRoutingGauge          ( RoutingGauge* );
              void                     addCellGauge             ( CellGauge* );
              void                     addObserver              ( BaseObserver* );
              void                     removeObserver           ( BaseObserver* );
              void                     notify                   ( unsigned int flags );
    // Cell Management.               
              Cell*                    cellLoader               ( const string& rpath );
              Cell*                    getCell                  ( const string& name
                                                                , unsigned int  mode
                                                                , unsigned int  depth=(unsigned int)-1 );
              Cell*                    createCell               ( const string& name, AllianceLibrary* library=NULL );
              void                     saveCell                 ( Cell* , unsigned int mode );
              void                     bindLibraries            ();
              unsigned int             loadLibraryCells         ( Library* );
              unsigned int             loadLibraryCells         ( const Name& );
      static  size_t                   getInstancesCount        ( Cell*, unsigned int flags );
    // Hurricane Managment.           
              void                     toJson                   ( JsonWriter* ) const;
      virtual string                   _getTypeName             () const;
      virtual Record*                  _getRecord               () const;

    // Internals - Attributes.
    protected:
      static  const Name               _parentLibraryName;
      static  AllianceFramework*       _singleton;
              Observable               _observers;
              Environment              _environment;
              ParsersMap               _parsers;
              DriversMap               _drivers;
              Catalog                  _catalog;
              AllianceLibraries        _libraries;
              Library*                 _parentLibrary;
              map<Name,RoutingGauge*>  _routingGauges;
              RoutingGauge*            _defaultRoutingGauge;
              map<Name,CellGauge*>     _cellGauges;
              CellGauge*               _defaultCellGauge;

    // Internals - Constructors.
                                 AllianceFramework       ();
                                 AllianceFramework       ( const AllianceFramework& );
              AllianceFramework& operator=               ( const AllianceFramework& );
    // Internals - Destructors.
                                ~AllianceFramework       ();
    // Internals - Methods.
              bool               _readLocate             ( const string& file, unsigned int mode, bool isLib=false );
              bool               _writeLocate            ( const string& file, unsigned int mode, bool isLib=false );
              AllianceLibrary*   _createLibrary          ( const string& path, bool& hasCatalog );
  };

  inline bool         AllianceFramework::isPOWER               ( const char*   name ) { return _environment.isPOWER(name); }
  inline bool         AllianceFramework::isPOWER               ( const string& name ) { return isPOWER(name.c_str()); }
  inline bool         AllianceFramework::isPOWER               ( const Name&   name ) { return isPOWER(getString(name)); }
  inline bool         AllianceFramework::isGROUND              ( const char*   name ) { return _environment.isGROUND(name); }
  inline bool         AllianceFramework::isGROUND              ( const string& name ) { return isGROUND(name.c_str()); }
  inline bool         AllianceFramework::isGROUND              ( const Name&   name ) { return isGROUND(getString(name)); }
  inline bool         AllianceFramework::isCLOCK               ( const char*   name ) { return _environment.isCLOCK(name); }
  inline bool         AllianceFramework::isCLOCK               ( const string& name ) { return isCLOCK(name.c_str()); }
  inline bool         AllianceFramework::isCLOCK               ( const Name&   name ) { return isCLOCK(getString(name)); }
  inline bool         AllianceFramework::isBLOCKAGE            ( const char*   name ) { return _environment.isBLOCKAGE(name); }
  inline bool         AllianceFramework::isBLOCKAGE            ( const string& name ) { return isBLOCKAGE(name.c_str()); }
  inline bool         AllianceFramework::isBLOCKAGE            ( const Name&   name ) { return isBLOCKAGE(getString(name)); }
  inline bool         AllianceFramework::isBLOCKAGE            ( const Net*    net  ) { return isBLOCKAGE(net->getName()); }
  inline bool         AllianceFramework::isPad                 ( const char*   name ) { return _environment.isPad(name); }
  inline bool         AllianceFramework::isPad                 ( const string& name ) { return isPad(name.c_str()); }
  inline bool         AllianceFramework::isPad                 ( const Name&   name ) { return isPad(getString(name)); }
  inline bool         AllianceFramework::isPad                 ( const Cell*   cell ) { return isPad(cell->getName()); }
  inline bool         AllianceFramework::isRegister            ( const char*   name ) { return _environment.isRegister(name); }
  inline bool         AllianceFramework::isRegister            ( const string& name ) { return isRegister(name.c_str()); }
  inline bool         AllianceFramework::isRegister            ( const Name&   name ) { return isRegister(getString(name)); }
  inline bool         AllianceFramework::isRegister            ( const Cell*   cell ) { return isRegister(cell->getName()); }
  inline Environment* AllianceFramework::getEnvironment        () { return &_environment; }
  inline Catalog*     AllianceFramework::getCatalog            () { return &_catalog; }
  inline const Name&  AllianceFramework::getParentLibraryName
                                                               () const { return _parentLibraryName; }
  inline Library*     AllianceFramework::getParentLibrary      () { return  _parentLibrary; }
  inline const AllianceLibraries&
                      AllianceFramework::getAllianceLibraries () const { return _libraries; }
// TEMPORARY.
  inline const Name   AllianceFramework::getDefaultCGPinLayerName
                                                               () const { return "CALU1"; }


  class JsonAllianceFramework : public JsonObject {
    public:
      static void                    initialize           ();
                                     JsonAllianceFramework( unsigned long );
      virtual                       ~JsonAllianceFramework();
      virtual string                 getTypeName          () const;
      virtual JsonAllianceFramework* clone                ( unsigned long ) const;
      virtual void                   toData               ( JsonStack& ); 
    private:
      std::string  _defaultRoutingGauge;
      std::string  _defaultCellGauge;
  };


} // CRL namespace.


INSPECTOR_P_SUPPORT(CRL::AllianceFramework);
