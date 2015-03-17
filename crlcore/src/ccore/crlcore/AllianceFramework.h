
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/AllianceFramework.h"                 |
// +-----------------------------------------------------------------+


#ifndef  CRL_ALLIANCE_FRAMEWORK_H
#define  CRL_ALLIANCE_FRAMEWORK_H

#include  <map>
#include  <limits>
#include  "hurricane/Cell.h"
#include  "crlcore/Environment.h"
#include  "crlcore/AllianceLibrary.h"
#include  "crlcore/Catalog.h"
#include  "crlcore/ParsersDrivers.h"


namespace CRL {

  using Hurricane::Cell;
  using Hurricane::Net;
  class RoutingGauge;
  class CellGauge;


  class AllianceFramework {
    public:
      enum InstancesCountFlags { Recursive=0x1, IgnoreFeeds=0x2 };
      enum LibraryFlags        { CreateLibrary=0x1, InSearchPath=0x2, HasCatalog=0x4 };
    public:
    // Constructors.
      static AllianceFramework* create                   ();
    // Destructors.
             void               destroy                  ();
    // Accessors.
      static AllianceFramework* get                      ();
             string             getPrint                 () const;
    // Predicates.
      inline bool               isPOWER                  ( const char*   name );
      inline bool               isPOWER                  ( const string& name );
      inline bool               isPOWER                  ( const Name&   name );
      inline bool               isGROUND                 ( const char*   name );
      inline bool               isGROUND                 ( const string& name );
      inline bool               isGROUND                 ( const Name&   name );
      inline bool               isCLOCK                  ( const char*   name );
      inline bool               isCLOCK                  ( const string& name );
      inline bool               isCLOCK                  ( const Name&   name );
      inline bool               isBLOCKAGE               ( const char*   name );
      inline bool               isBLOCKAGE               ( const string& name );
      inline bool               isBLOCKAGE               ( const Name&   name );
      inline bool               isBLOCKAGE               ( const Net*    net );
      inline bool               isPad                    ( const char*   name );
      inline bool               isPad                    ( const string& name );
      inline bool               isPad                    ( const Name&   name );
      inline bool               isPad                    ( const Cell* );
    // Accessors.
      inline Environment*       getEnvironment           ();
      inline Catalog*           getCatalog               ();
      inline const Name&        getParentLibraryName     () const;
      inline Library*           getParentLibrary         ();
             Library*           getLibrary               ( unsigned int index );
             AllianceLibrary*   getAllianceLibrary       ( unsigned int index );
             AllianceLibrary*   getAllianceLibrary       ( const Name& libName, unsigned int& flags );
             AllianceLibrary*   getAllianceLibrary       ( Library* );
             AllianceLibrary*   createLibrary            ( const string& path, unsigned int& flags, string libName="" );
             void               saveLibrary              ( Library* );
             void               saveLibrary              ( AllianceLibrary* );
             RoutingGauge*      getRoutingGauge          ( const Name& name="" );
             CellGauge*         getCellGauge             ( const Name& name="" );
      inline const Name         getDefaultCGPinLayerName () const;
    // Modifiers.
             void               addRoutingGauge          ( RoutingGauge* );
             void               addCellGauge             ( CellGauge* );
    // Cell Management.
             Cell*              getCell                  ( const string& name
                                                         , unsigned int  mode
                                                         , unsigned int  depth=(unsigned int)-1 );
             Cell*              createCell               ( const string& name, AllianceLibrary* library=NULL );
             void               saveCell                 ( Cell* , unsigned int mode );
             unsigned int       loadLibraryCells         ( Library* );
             unsigned int       loadLibraryCells         ( const Name& );
      static size_t             getInstancesCount        ( Cell*, unsigned int flags );
    // Hurricane Managment.
      inline string             _getTypeName             () const;
             string             _getString               () const;
             Record*            _getRecord               () const;

    // Internals - Attributes.
    protected:
      static const Name          _parentLibraryName;
      static AllianceFramework*  _singleton;
             Environment         _environment;
             ParsersMap          _parsers;
             DriversMap          _drivers;
             Catalog             _catalog;
             AllianceLibraries   _libraries;
             Library*            _parentLibrary;
             RoutingGauge*       _defaultRoutingGauge;
             map<const Name,RoutingGauge*>
                                 _routingGauges;
             map<const Name,CellGauge*>
                                 _cellGauges;
             CellGauge*          _defaultCellGauge;

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
              void               _bindLibraries          ();
  };

  inline bool         AllianceFramework::isPOWER          ( const char*   name ) { return _environment.isPOWER(name); }
  inline bool         AllianceFramework::isPOWER          ( const string& name ) { return isPOWER(name.c_str()); }
  inline bool         AllianceFramework::isPOWER          ( const Name&   name ) { return isPOWER(getString(name)); }
  inline bool         AllianceFramework::isGROUND         ( const char*   name ) { return _environment.isGROUND(name); }
  inline bool         AllianceFramework::isGROUND         ( const string& name ) { return isGROUND(name.c_str()); }
  inline bool         AllianceFramework::isGROUND         ( const Name&   name ) { return isGROUND(getString(name)); }
  inline bool         AllianceFramework::isCLOCK          ( const char*   name ) { return _environment.isCLOCK(name); }
  inline bool         AllianceFramework::isCLOCK          ( const string& name ) { return isCLOCK(name.c_str()); }
  inline bool         AllianceFramework::isCLOCK          ( const Name&   name ) { return isCLOCK(getString(name)); }
  inline bool         AllianceFramework::isBLOCKAGE       ( const char*   name ) { return _environment.isBLOCKAGE(name); }
  inline bool         AllianceFramework::isBLOCKAGE       ( const string& name ) { return isBLOCKAGE(name.c_str()); }
  inline bool         AllianceFramework::isBLOCKAGE       ( const Name&   name ) { return isBLOCKAGE(getString(name)); }
  inline bool         AllianceFramework::isBLOCKAGE       ( const Net*    net  ) { return isBLOCKAGE(net->getName()); }
  inline bool         AllianceFramework::isPad            ( const char*   name ) { return _environment.isPad(name); }
  inline bool         AllianceFramework::isPad            ( const string& name ) { return isPad(name.c_str()); }
  inline bool         AllianceFramework::isPad            ( const Name&   name ) { return isPad(getString(name)); }
  inline bool         AllianceFramework::isPad            ( const Cell*   cell ) { return isPad(cell->getName()); }
  inline Environment* AllianceFramework::getEnvironment   () { return &_environment; }
  inline Catalog*     AllianceFramework::getCatalog       () { return &_catalog; }
  inline const Name&  AllianceFramework::getParentLibraryName
                                                          () const { return _parentLibraryName; }
  inline Library*     AllianceFramework::getParentLibrary () { return  _parentLibrary; }
// TEMPORARY.
  inline const Name   AllianceFramework::getDefaultCGPinLayerName
                                                          () const { return "CALU1"; }
  inline string       AllianceFramework::_getTypeName     () const { return "AllianceFramework"; }


} // CRL namespace.


INSPECTOR_P_SUPPORT(CRL::AllianceFramework);

#endif // CRL_ALLIANCE_FRAMEWORK_H
