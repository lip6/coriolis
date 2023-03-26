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
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );

    if (role == Qt::FontRole) {
      if (index.row() == 0) return QVariant();
      switch (index.column()) {
        case 0:  return nameFont;
        default: return valueFont;
      }
      return QVariant();
    }

    if (not index.isValid()) return QVariant ();

    if (role == Qt::DisplayRole) {
      int row = index.row ();
      return QString::fromStdString( _equipotentials[row]->getName() );
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

    if (section == 0) return QVariant("Equipotential");
    // if (section < _equipotentials->getColumnCount())
    //   return _equipotentials->getColumnName( section );

    return QVariant();
  }


  int  EquipotentialsModel::rowCount ( const QModelIndex& parent ) const
  {
    return _equipotentials.size();
  }


  int  EquipotentialsModel::columnCount ( const QModelIndex& parent ) const
  {
    return 1;
  //return (_equipotentials) ? _equipotentials->getColumnCount() : 1;
  }


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
