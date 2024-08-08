// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023.
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
// |  C++ Module   :  "./ShortCircuitModel.cpp"                      |
// +-----------------------------------------------------------------+



#include <QFont>
#include <QApplication>
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "tramontana/TramontanaEngine.h"


namespace Hurricane {


  ShortCircuitModel::ShortCircuitModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _tramontana(nullptr)
  { }


  ShortCircuitModel::~ShortCircuitModel ()
  { }


  const TramontanaEngine::ShortedSet& ShortCirtuitModel::getShortedNets ()
  {
    static ShortedSet noShortedNets;
    if (not _tramontana) return noShortedNets;
    return _tramontana->getShortedNets();
  }
  

  QVariant  ShortCircuitModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont ( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont ( QFont::Normal, true );

    if ( role == Qt::FontRole ) {
      if ( index.row() == 0 ) return QVariant();
      switch (index.column()) {
        case 0:  return nameFont;
        default: return valueFont;
      }
      return QVariant();
    }

    if ( not index.isValid() ) return QVariant ();

    if ( role == Qt::DisplayRole ) {
      int row = index.row ();
      return _netlist->getRow(row)->getColumn(index.column());
    }
    return QVariant();
  }


  QVariant  ShortCircuitModel::headerData ( int             section
                                          , Qt::Orientation orientation
                                          , int             role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();

    static QFont headerFont = Graphics::getFixedFont( QFont::Bold, false, false, +0 );

    if (role == Qt::FontRole   ) return headerFont;
    if (role != Qt::DisplayRole) return QVariant();

    switch ( section ) {
      case 0: return QVariant( "#shorts" );
      case 1: return QVariant( "Shorted nets" );
    }
    return QVariant();
  }


  int  ShortCircuitModel::rowCount ( const QModelIndex& parent ) const
  { return getShortedNets().size(); }


  int  ShortCircuitModel::columnCount ( const QModelIndex& parent ) const
  { return 2; }


  const ShortCircuit* ShortCircuitModel::getShortCircuit ( int row )
  {
    if (row >= (int)_getShortedNets()->size()) return nullptr;

    return _netlist->getRow(row)->getNet();
  }


} // End of Hurricane namespace.
