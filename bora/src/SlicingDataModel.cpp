// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :          Jean-Paul Chaput, Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./SlicingDataModel.h"                          |
// +-----------------------------------------------------------------+


#include <iostream>
#include <QFont>
#include "hurricane/DbU.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/analog/AnalogCellExtension.h"
#include "bora/SlicingDataModel.h"


namespace Bora {


  using namespace std;
  using Hurricane::Graphics;
  using Hurricane::DbU;
  using Analog::AnalogCellExtension;


  SlicingDataModel::SlicingDataModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _cell(NULL)
  { }


  SlicingDataModel::~SlicingDataModel ()
  { }

  
  void SlicingDataModel::setCell( Hurricane::Cell* cell )
  {
    _cell = cell;
  }


  QVariant SlicingDataModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _cell );

    if ( (not slicingtree) or (not index.isValid()) ) return QVariant();

    if ( role == Qt::SizeHintRole ) {
      switch (index.column()) {
        case 0: return 200;
        default:
        case 1: return -1;
      }
    }

    if ( role == Qt::FontRole ) {
      switch (index.column()) {
        case 0: return nameFont;
        default:
        case 1: return valueFont;
      }
    }

    if ( role == Qt::DisplayRole ) {
      int row = index.row();
      if ( row < (int)rowCount() ) {
        NodeSets nodeSets = slicingtree->getNodeSets();
        switch ( index.column() ) {
          case 0: return QVariant( row );
          case 1: return QVariant( DbU::toPhysical(nodeSets[row]->getHeight()        , DbU::Micro) );
          case 2: return QVariant( DbU::toPhysical(nodeSets[row]->getWidth()         , DbU::Micro) );
          case 3: return QVariant( nodeSets[row]->getOccupationArea()                              );
          case 4: return QVariant( nodeSets[row]->getCpt()                                         );
          case 5: return QVariant( (double) ( nodeSets[row]->getWidth()
                                   / nodeSets[row]->getHeight()) );
          case 6: return QVariant( DbU::getPhysical( nodeSets[row]->getWidth() ,DbU::Micro) 
                                 * DbU::getPhysical( nodeSets[row]->getHeight(),DbU::Micro) );
          default:
            if (index.column() >= columnCount()) return QVariant();
        }
      }
    }
    if ( role == Qt::UserRole ){
      return QVariant( index.row() );
    }

    return QVariant();
  }


  QVariant SlicingDataModel::headerData ( int section, Qt::Orientation orientation, int role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();

    static QFont headerFont = Graphics::getFixedFont ( QFont::Bold, false, false, +0 );

    if ( role == Qt::FontRole    ) return headerFont;
    if ( role != Qt::DisplayRole ) return QVariant();

    switch ( section ) {
      case 0: return QVariant ( tr("Order"              ) );
      case 1: return QVariant ( QString::fromUtf8("Height (µm)") );
      case 2: return QVariant ( QString::fromUtf8("Width (µm)") );
      case 3: return QVariant ( tr("Occupation Area (%)") );
      case 4: return QVariant ( tr("Counter"            ) );
      case 5: return QVariant ( tr("Ratio (W/H)"        ) );
      case 6: return QVariant ( QString::fromUtf8("Area (µm²)") );
      default:
        if (section >= columnCount()) return QVariant();
    }

    return QVariant ( tr("?") );
  }


  int SlicingDataModel::rowCount ( const QModelIndex& parent ) const
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _cell );
    if (slicingtree != NULL){
      NodeSets nodeSets = slicingtree->getNodeSets();
      return nodeSets.size();
    } else {
      return  0;
    }
  }


  int SlicingDataModel::columnCount ( const QModelIndex& parent ) const
  {
    return 7;
  }


  BoxSet* SlicingDataModel::getBoxSet( int index )
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _cell );
    if (slicingtree){
      NodeSets nodeSets = slicingtree->getNodeSets();
      return nodeSets[index];
    } 
    return NULL;
  }


  void SlicingDataModel::updateContents()
  {
    beginResetModel();
    endResetModel();
  }


}  // Bora namespace.

