// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./HierarchyInformations.cpp"                   |
// +-----------------------------------------------------------------+


#include <QObject>
#include "hurricane/Name.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/viewer/HierarchyInformations.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "HierarchyInfos"

  HierarchyInfos::HierarchyInfos ( HierarchyInfos* parent, size_t rowInParent )
    : _parent     (parent)
    , _rowInParent(rowInParent)
  { }


  HierarchyInfos::~HierarchyInfos ()
  { }


  int  HierarchyInfos::getColumnCount ()
  { return 2; }


  QVariant  HierarchyInfos::getColumnName ( int column )
  {
    switch ( column ) {
      case 0: return QVariant(QObject::tr("Instance"));
      case 1: return QVariant(QObject::tr("Model/Cell"));
    }
    return QVariant(QObject::tr("Column Out of Bound"));
  }


  QVariant  HierarchyInfos::getColumn ( int column ) const
  {
    if (isRoot()) return QVariant(QObject::tr("RootItem"));

    const Instance* instance = getInstance();
    if (instance) {
      switch ( column ) {
        case 0: return getString(instance->getName()).c_str();
        case 1: return getString(instance->getMasterCell()->getName()).c_str();
      }
    } else {
      switch ( column ) {
        case 0: return "<Top Cell>";
        case 1: return getString(getMasterCell()->getName()).c_str();
      }
    }

    return QVariant(QObject::tr("Column Out of Bound"));
  }


  Path  HierarchyInfos::getPath () const
  {
    if (isRoot() or not getInstance()) return Path();

    return Path( getParent()->getPath(), const_cast<Instance*>(getInstance()) );
  }


// -------------------------------------------------------------------
// Class  :  "LeafHierarchyInfos"


  const vector<HierarchyInfos*>  LeafHierarchyInfos::_instances;


  LeafHierarchyInfos::LeafHierarchyInfos ( const Instance* instance
                                         , HierarchyInfos* parent
                                         , size_t          rowInParent )
    : HierarchyInfos(parent,rowInParent)
    , _instance(instance)
  { }


  LeafHierarchyInfos::~LeafHierarchyInfos ()
  { }


  bool                                LeafHierarchyInfos::isRoot           () const { return false; }
  bool                                LeafHierarchyInfos::isLeaf           () const { return true; }
  bool                                LeafHierarchyInfos::isCollapsed      () const { return true; }
  const HierarchyInfos*               LeafHierarchyInfos::getRow           ( int row ) const { return NULL; }
  int                                 LeafHierarchyInfos::size             () const { return 0; }
  Cell*                               LeafHierarchyInfos::getMasterCell    () const { return _instance->getMasterCell(); }
  const Instance*                     LeafHierarchyInfos::getInstance      () const { return _instance; }
  const std::vector<HierarchyInfos*>& LeafHierarchyInfos::getInstances     () const { return _instances; }
  QString                             LeafHierarchyInfos::getFilterPattern () const { return "<Not avalaible for leaf instances>"; }
  void                                LeafHierarchyInfos::setFilterPattern ( const QString& ) { }
  void                                LeafHierarchyInfos::expand           () { }
  void                                LeafHierarchyInfos::collapse         () { }


// -------------------------------------------------------------------
// Class  :  "InstHierarchyInfos"

  InstHierarchyInfos::InstHierarchyInfos ( const Instance* instance
                                         , HierarchyInfos* parent
                                         , size_t          rowInParent )
    : HierarchyInfos(parent,rowInParent)
    , _flags    (HierarchyInfos::Collapsed)
    , _instance (instance)
    , _instances()
  { }


  InstHierarchyInfos::~InstHierarchyInfos ()
  {
    collapse();
  }


  bool                                InstHierarchyInfos::isRoot           () const { return false; }
  bool                                InstHierarchyInfos::isLeaf           () const { return false; }
  bool                                InstHierarchyInfos::isCollapsed      () const { return _flags & HierarchyInfos::Collapsed; }
  Cell*                               InstHierarchyInfos::getMasterCell    () const { return _instance->getMasterCell(); }
  const Instance*                     InstHierarchyInfos::getInstance      () const { return _instance; }
  const std::vector<HierarchyInfos*>& InstHierarchyInfos::getInstances     () const { return _instances; }
  QString                             InstHierarchyInfos::getFilterPattern () const { return _filter.pattern(); }


  int  InstHierarchyInfos::size () const
  {
    if (_flags & HierarchyInfos::Collapsed) {
      const_cast<InstHierarchyInfos*>(this)->expand();
    }
    return _instances.size();
  }


  void  InstHierarchyInfos::setFilterPattern ( const QString& pattern )
  {
    _filter.setPattern( pattern );

    if (_flags & HierarchyInfos::Collapsed) return;

    size_t i = 0;
    for ( Instance* instance : getMasterCell()->getInstances() ) {
      if (instance->getMasterCell()->isFeed()) continue;

      if ( (i < _instances.size()) and (instance == _instances[i]->getInstance()) ) {
      // The instance *is already* in the list.
        if ( not _filter.isEmpty() and (_filter.indexIn(getString(instance->getName()).c_str()) < 0) ) {
          delete _instances[i];
          _instances.erase( _instances.begin()+i );
        } else {
          ++i;
        }
      } else {
      // The instance *is not* in the list.
        if (_filter.isEmpty() or (_filter.indexIn(getString(instance->getName()).c_str()) >= 0) ) {
          HierarchyInfos* infos = NULL;
          if (instance->isLeaf()) infos = new LeafHierarchyInfos( instance, this, 0 );
          else                    infos = new InstHierarchyInfos( instance, this, 0 );

          _instances.insert( _instances.begin()+i, infos );
          ++i;
        }
      }
    }

    for ( size_t i=0 ; i<_instances.size() ; ++i ) _instances[i]->setRowInParent( i );
  }


  void  InstHierarchyInfos::expand ()
  {
    if (not (_flags & HierarchyInfos::Collapsed)) return;
    _flags &= ~HierarchyInfos::Collapsed;

    if (not getMasterCell()) return;

    for ( Instance* instance : getMasterCell()->getInstances() ) {
      if (instance->getMasterCell()->isFeed()) continue;
      if (not _filter.isEmpty()) {
        if (_filter.indexIn(getString(instance->getName()).c_str()) < 0) continue;
      }

      HierarchyInfos* infos = NULL;
      if (instance->isLeaf()) infos = new LeafHierarchyInfos( instance, this, _instances.size() );
      else                    infos = new InstHierarchyInfos( instance, this, _instances.size() );

      _instances.push_back( infos );
    }
  }


  void  InstHierarchyInfos::collapse ()
  {
    if (_flags & HierarchyInfos::Collapsed) return;
    _flags |= HierarchyInfos::Collapsed;

    for ( HierarchyInfos* info : _instances ) delete info;

    vector<HierarchyInfos*>().swap( _instances );
  }


  const HierarchyInfos* InstHierarchyInfos::getRow ( int row ) const
  {
    if (_flags & HierarchyInfos::Collapsed) {
      const_cast<InstHierarchyInfos*>(this)->expand();
    }

    if (row >= (int)_instances.size()) return NULL;
    return _instances[row];
  }


// -------------------------------------------------------------------
// Class  :  "TopCellHierarchyInfos"

  TopCellHierarchyInfos::TopCellHierarchyInfos ( Cell* cell, HierarchyInfos* parent )
    : InstHierarchyInfos(NULL,parent,0)
    , _rootCell(cell)
  { expand(); }


  TopCellHierarchyInfos::~TopCellHierarchyInfos ()
  { collapse(); }


  bool            TopCellHierarchyInfos::isRoot         () const { return false; }
  Cell*           TopCellHierarchyInfos::getMasterCell  () const { return _rootCell; }
  const Instance* TopCellHierarchyInfos::getInstance    () const { return NULL; }


  void  TopCellHierarchyInfos::setCell ( Cell* rootCell )
  {
    collapse();
    _rootCell = rootCell;
    expand();
  }


// -------------------------------------------------------------------
// Class  :  "RootHierarchyInfos"


  RootHierarchyInfos::RootHierarchyInfos ( Cell* cell )
    : HierarchyInfos(NULL,0)
    , _instances()
  {
    _instances.push_back( new TopCellHierarchyInfos(cell,this) );
  }


  RootHierarchyInfos::~RootHierarchyInfos ()
  { delete _instances[0]; }


  bool                                RootHierarchyInfos::isRoot           () const { return true; }
  bool                                RootHierarchyInfos::isLeaf           () const { return false; }
  bool                                RootHierarchyInfos::isCollapsed      () const { return false; }
  int                                 RootHierarchyInfos::size             () const { return _instances.size(); }
  const HierarchyInfos*               RootHierarchyInfos::getRow           ( int row ) const { return (row==0) ? _instances[0] : NULL; }
  Cell*                               RootHierarchyInfos::getMasterCell    () const { return _instances[0]->getMasterCell(); }
  const Instance*                     RootHierarchyInfos::getInstance      () const { return NULL; }
  const std::vector<HierarchyInfos*>& RootHierarchyInfos::getInstances     () const { return _instances; }
  QString                             RootHierarchyInfos::getFilterPattern () const { return "<Not avalaible for Root>"; }
  void                                RootHierarchyInfos::setFilterPattern ( const QString& ) { }
  void                                RootHierarchyInfos::expand           () { }
  void                                RootHierarchyInfos::collapse         () { }
  void                                RootHierarchyInfos::setCell          ( Cell* topCell ) { dynamic_cast<TopCellHierarchyInfos*>(_instances[0])->setCell(topCell); }

} // End of Hurricane namespace.
