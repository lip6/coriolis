// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Module   :  "./EquipotentialsModel.cpp"                    |
// +-----------------------------------------------------------------+


#include <QFont>
#include <QBrush>
#include <QApplication>
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "tramontana/EquipotentialsModel.h"


namespace Tramontana {

  using Hurricane::Graphics;


  EquipotentialsModel::EquipotentialsModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _cell              (nullptr)
    , _equipotentials    ()
  { }


  QVariant  EquipotentialsModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont  nameFont          = Graphics::getFixedFont( QFont::Bold );
    static QFont  valueFont         = Graphics::getFixedFont( QFont::Normal, true );
    static QFont  shortedFont       = Graphics::getFixedFont( QFont::Bold, false );
    static QBrush shortedForeground = QBrush( QColor(255,0,0) );

    Equipotential* equi = _equipotentials[ index.row() ];

    if (role == Qt::FontRole) {
      if (equi->hasOpens() or equi->hasShorts()) {
        return QVariant( shortedFont );
      }
      switch (index.column()) {
        case 0:  return nameFont;
        default: return valueFont;
      }
    }

    if (role == Qt::ForegroundRole) {
      if (equi->hasOpens() or equi->hasShorts()) {
        return QVariant( shortedForeground );
      }
      return QVariant();
    }

    if (not index.isValid()) return QVariant ();

    string status;
    if (equi->hasOpens ()) status += "O";
    if (equi->hasShorts()) status += "S";
    if (role == Qt::DisplayRole) {
      switch ( index.column() ) {
        case 0: return QString::fromStdString( status );
        case 1: return QString::fromStdString(            equi->getName() );
        case 2: return QString::fromStdString(            equi->getFlagsAsString() );
        case 3: return QString::fromStdString( getString( equi->getType() ));
        case 4: return QString::fromStdString( getString( equi->getDirection() ));
      }
    }
    return QVariant();
  }


  QVariant  EquipotentialsModel::headerData ( int             section
                                            , Qt::Orientation orientation
                                            , int             role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();

    static QFont headerFont = Graphics::getFixedFont( QFont::Bold, false, false, +0 );

    if (role == Qt::FontRole   ) return headerFont;
    if (role != Qt::DisplayRole) return QVariant();
    switch ( section ) {
      case 0: return QVariant( "Status" );
      case 1: return QVariant( "Name" );
      case 2: return QVariant( "Flags" );
      case 3: return QVariant( "Type" );
      case 4: return QVariant( "Direction" );
    }
    return QVariant();
  }


  int  EquipotentialsModel::rowCount ( const QModelIndex& parent ) const
  { return _equipotentials.size(); }


  int  EquipotentialsModel::columnCount ( const QModelIndex& parent ) const
  { return 5; }


  const Equipotential* EquipotentialsModel::getEqui ( int row )
  {
    if (row >= (int)_equipotentials.size()) return nullptr;
    return _equipotentials[ row ];
  }


  void  EquipotentialsModel::setCell ( Cell* cell )
  {
    if (_cell != cell) {
      emit layoutAboutToBeChanged ();

      if (_cell) _equipotentials.clear();
      _cell = cell;

      if (_cell) {
        TramontanaEngine* tramontana = TramontanaEngine::get( _cell );
        if (tramontana) {
          for ( Equipotential* equi : tramontana->getEquipotentials() )
            _equipotentials.push_back( equi );
        }
      }

      emit layoutChanged ();
    }
  }


} // Tramontana namespace.
