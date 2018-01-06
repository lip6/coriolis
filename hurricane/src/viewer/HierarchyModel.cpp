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
// |  C++ Module  :       "./HierarchyModel.cpp"                     |
// +-----------------------------------------------------------------+


#include <QFont>
#include <QApplication>
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/HierarchyInformations.h"
#include "hurricane/viewer/HierarchyModel.h"


namespace Hurricane {


  HierarchyModel::HierarchyModel ( QObject* parent )
    : QAbstractItemModel(parent)
    , _root             (NULL)
  { }


  HierarchyModel::~HierarchyModel ()
  { }


  bool  HierarchyModel::hasChildren ( const QModelIndex& parent ) const
  {
    if (not parent.isValid()) return true;

    const HierarchyInfos* infos = infosFromIndex(parent);
    if (not infos) return true;

    return not infos->isLeaf();
  }


  QVariant  HierarchyModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont( QFont::Normal, true );

    if (role == Qt::FontRole) {
      switch (index.column()) {
        case 0:  return nameFont;
        default: return valueFont;
      }
      return QVariant();
    }

    if (not index.isValid()) return QVariant();

    if (role == Qt::DisplayRole) {
      const HierarchyInfos* infos = infosFromIndex( index );
      if (infos)
        return infos->getColumn( index.column() );
    }
    return QVariant();
  }


  QVariant  HierarchyModel::headerData ( int             section
                                       , Qt::Orientation orientation
                                       , int             role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();

    static QFont headerFont = Graphics::getFixedFont( QFont::Bold, false, false, +0 );

    if (role == Qt::FontRole    ) return headerFont;
    if (role != Qt::DisplayRole ) return QVariant();
    if (role == Qt::SizeHintRole) return (Graphics::isHighDpi()) ? 600 : 300;

    return HierarchyInfos::getColumnName( section );
  }


  int  HierarchyModel::rowCount ( const QModelIndex& parent ) const
  {
    const HierarchyInfos* infos = infosFromIndex( parent );
    if (not infos) return 0;

    if (infos->isCollapsed()) {
      cerr << "HierarchyModel::rowCount(): collapsed " << infos->size() << " " << infos->getInstance() << endl; 
    }

    return infos->size();
  }


  int  HierarchyModel::columnCount ( const QModelIndex& parent ) const
  { return _root.getColumnCount(); }


  QModelIndex  HierarchyModel::index ( int row, int column, const QModelIndex& parent ) const
  {
    if (not _root.getMasterCell() or (row < 0) or (column < 0)) return QModelIndex();

    const HierarchyInfos* parentInfos = infosFromIndex( parent );
    const HierarchyInfos* childInfos  = parentInfos->getRow( row );

    if (not childInfos) return QModelIndex();

    QModelIndex index = createIndex( row, column, (void*)childInfos );
    // cerr << "HierarchyModel::index(): " << index.row() << "+" << index.column()
    //      << ":" << index.internalPointer()
    //      << " " << childInfos->getMasterCell() << endl;

    return index;
  }


  QModelIndex  HierarchyModel::parent ( const QModelIndex& child ) const
  {
    const HierarchyInfos* childInfos = infosFromIndex( child );
    if (not childInfos) return QModelIndex();

    const HierarchyInfos* parentInfos = childInfos->getParent();
    if (not parentInfos) return QModelIndex();

    const HierarchyInfos* grandParentInfos = parentInfos->getParent();
    if (not grandParentInfos) return QModelIndex();

    QModelIndex index = createIndex( parentInfos->getRowInParent(), 0, (void*)parentInfos );
    // cerr << "HierarchyModel::index(): " << index.row() << "+" << index.column()
    //      << ":" << index.internalPointer()
    //      << " " << childInfos->getMasterCell() << endl;

    return index;
  }


} // Hurricane namespace.
