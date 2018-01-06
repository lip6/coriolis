// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/HierarchyModel.h"      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_HIERARCHY_MODEL_H
#define  HURRICANE_HIERARCHY_MODEL_H

#include <vector>
#include <QFont>
#include <QApplication>
#include <QAbstractTableModel>
#include "hurricane/Commons.h"
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/HierarchyInformations.h"


namespace Hurricane {

  class Net;
  class Cell;


  class HierarchyModel : public QAbstractItemModel {
      Q_OBJECT;

    public:
                                    HierarchyModel   ( QObject* parent=NULL );
                                   ~HierarchyModel   ();
      inline  void                  setRootCell      ( Cell* cell );
      virtual bool                  hasChildren      ( const QModelIndex& parent=QModelIndex() ) const;
              QModelIndex           index            ( int row, int column, const QModelIndex& parent ) const;
              QModelIndex           parent           ( const QModelIndex& child ) const;
              int                   rowCount         ( const QModelIndex& parent=QModelIndex() ) const;
              int                   columnCount      ( const QModelIndex& parent=QModelIndex() ) const;
              QVariant              data             ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
              QVariant              headerData       ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline  Cell*                 getCell          ();
      inline  HierarchyInfos*       getRoot          ();
      inline  void                  setCell          ( Cell* );
      inline  const HierarchyInfos* infosFromIndex   ( const QModelIndex& ) const;
    private:
      RootHierarchyInfos  _root;
  };


// Inline Functions.

  inline Cell*           HierarchyModel::getCell   () { return  _root.getMasterCell(); }
  inline HierarchyInfos* HierarchyModel::getRoot   () { return &_root; }


  inline void  HierarchyModel::setCell ( Cell* cell )
  {
    beginResetModel();
    _root.setCell( cell );
    endResetModel();
  }


  inline const HierarchyInfos* HierarchyModel::infosFromIndex ( const QModelIndex& index ) const
  {
    if (index.isValid())
      return static_cast<HierarchyInfos*>(index.internalPointer());
    return &_root;
  }


} // End of Hurricane namespace.


#endif // HURRICANE_HIERARCHY_MODEL_H
