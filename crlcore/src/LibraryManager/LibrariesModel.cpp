// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./LibrariesModel.cpp"                          |
// +-----------------------------------------------------------------+


#include <QFont>
#include <QApplication>
#include "hurricane/Library.h"
#include "hurricane/viewer/Graphics.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/LibrariesModel.h"


namespace CRL {

  using namespace std;
  using Hurricane::Graphics;


  LibrariesModel::LibrariesModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _libraries         (AllianceFramework::get()->getAllianceLibraries())
  { }


  LibrariesModel::~LibrariesModel ()
  { }


  void  LibrariesModel::update ()
  { emit layoutChanged(); }


  QVariant  LibrariesModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont  = Graphics::getFixedFont( QFont::Bold );
    static QFont valueFont = Graphics::getFixedFont( QFont::Normal, true );

    if (role == Qt::FontRole) {
      // if (index.row() == 0) return QVariant();
      // switch (index.column()) {
      //   case 0:  return nameFont;
      //   default: return valueFont;
      // }
      return QVariant();
    }

    if (not index.isValid()) return QVariant();

    if (role == Qt::DisplayRole) {
      int column = index.column ();
      if (column == 0) return index.row();
      if (column == 1) return getString( _libraries[index.row()]->getLibrary()->getName() ).c_str();
    }
    return QVariant();
  }


  QVariant  LibrariesModel::headerData ( int             section
                                     , Qt::Orientation orientation
                                     , int             role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();

    static QFont headerFont = Graphics::getFixedFont( QFont::Bold, false, false, +0 );

    if (role == Qt::FontRole   ) return headerFont;
    if (role != Qt::DisplayRole) return QVariant();

    switch ( section ) {
      case 0: return QVariant("Order");
      case 1: return QVariant("Name");
    }

    return QVariant();
  }


  int  LibrariesModel::rowCount ( const QModelIndex& parent ) const
  { return _libraries.size(); }


  int  LibrariesModel::columnCount ( const QModelIndex& parent ) const
  { return 2; }


  const AllianceLibrary* LibrariesModel::getLibrary ( const QModelIndex& index )
  {
    if ( (not index.isValid()) or (index.row() >= (int)_libraries.size()) ) return NULL;
    return _libraries[index.row()];
  }


  const AllianceLibrary* LibrariesModel::getLibrary ( size_t index )
  {
    if (index > _libraries.size()) return NULL;
    return _libraries[index];
  }


}  // CRL namespace.
