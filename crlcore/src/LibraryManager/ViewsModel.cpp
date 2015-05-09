// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ViewsModel.cpp"                              |
// +-----------------------------------------------------------------+


#include <iostream>
#include <sstream>
#include <QFont>
#include <QApplication>
#include "hurricane/Library.h"
#include "hurricane/viewer/Graphics.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/ViewsModel.h"
#include "crlcore/CellDatas.h"
#include "crlcore/LibraryManager.h"


namespace CRL {

  using namespace std;
  using Hurricane::Graphics;


  ViewsModel::ViewsModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _cellDatas         (NULL)
    , _viewLoaders       ()
    , _viewSizes         ()
  { }


  ViewsModel::~ViewsModel ()
  { }


  Qt::ItemFlags ViewsModel::flags ( const QModelIndex& index ) const
  {
    if (index.isValid() and _cellDatas->isLoaded() )
      return Qt::NoItemFlags;

    Qt::ItemFlags result = QAbstractTableModel::flags(index);
    if (index.column() == 0) result |= Qt::ItemIsUserCheckable;
    return result;
  }


  QVariant  ViewsModel::data ( const QModelIndex& index, int role ) const
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

    if (role == Qt::TextAlignmentRole) {
      int column = index.column();
      if (column == 1) return Qt::AlignRight;
    }

    if (role == Qt::DisplayRole) {
      int column = index.column();
      if (column == 0) {
        const CellLoader* loader = _viewLoaders[ index.row() ];
        string label = loader->getFormat() + " [";
        size_t count = 0;
        if (loader->getViewFlags() & Catalog::State::Logical ) { label += "Lo"; ++count; }
        if (loader->getViewFlags() & Catalog::State::Physical) { label += ((count++)?"+":""); label += "Ph"; }
        label += "]";
        return label.c_str();
      }
      if (column == 1) {
        double value = (double)_viewSizes[index.row()] / 1024.0;
        char   unit  = 'K';

        if (value >= 1024.0) { value /= 1024.0; unit = 'M'; }
        if (value >= 1024.0) { value /= 1024.0; unit = 'G'; }

        ostringstream os;
        os << setprecision(2) << value << " " << unit << "iB";

        return os.str().c_str();
      }
    }

    if (role == Qt::CheckStateRole) {
      if (index.column() == 0) {
        return (_cellDatas->getActiveFlags() & _viewLoaders[index.row()]->getFlags())
          ? Qt::Checked : Qt::Unchecked;
      }
    }

    return QVariant();
  }


  bool  ViewsModel::setData ( const QModelIndex& index, const QVariant& value, int role )
  {
    if (index.isValid()) {
      if ( (index.column() == 0) and (role == Qt::CheckStateRole) ) {
        if (_cellDatas->getActiveFlags() & _viewLoaders[index.row()]->getFlags()) {
          _cellDatas->resetActiveFlags( _viewLoaders[index.row()]->getFlags() & CellLoaders::lmask() );
        } else {
          _cellDatas->setActiveFlags( _viewLoaders[index.row()]->getFlags() & CellLoaders::lmask() );
        }
        emit dataChanged( index, index );
      }
    }
    return false;
  }


  QVariant  ViewsModel::headerData ( int             section
                                   , Qt::Orientation orientation
                                   , int             role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();

    static QFont headerFont = Graphics::getFixedFont( QFont::Bold, false, false, +0 );

    if (role == Qt::FontRole   ) return headerFont;
    if (role != Qt::DisplayRole) return QVariant();

    switch ( section ) {
      case 0: return QVariant("View [type]");
      case 1: return QVariant("Size");
    }

    return QVariant();
  }


  int  ViewsModel::rowCount ( const QModelIndex& parent ) const
  { return _viewLoaders.size(); }


  int  ViewsModel::columnCount ( const QModelIndex& parent ) const
  { return 2; }


  void ViewsModel::setCellDatas ( CellDatas* cellDatas )
  {
    _cellDatas = cellDatas;

    emit layoutAboutToBeChanged();

    _viewLoaders.clear();
    _viewSizes  .clear();
    if (_cellDatas != NULL) {
      unsigned int flags = _cellDatas->getFlags() & CellLoaders::lmask();

      CellLoaders* loaders = CellLoaders::get();
      for ( size_t bit=16 ; bit<32 ; ++bit ) {
        unsigned int bitmask = 1 << bit;

        if (not (flags & bitmask)) continue;
        const CellLoader* loader = loaders->getLoader( bitmask );
        if (loader) {
          _viewLoaders.push_back( loader );

          string                 file = _cellDatas->getName() + "." + loader->getFormat();
          Utilities::Path        path = CellDatas::getLibraryPath() / file;
          Utilities::Path::Stat  stat = path.stat();
          _viewSizes.push_back( stat.size() );
        }
      }
    }

    emit layoutChanged();
  }


  void  ViewsModel::setNewViewer ( int state )
  {
    if (state == Qt::Checked  ) _cellDatas->setActiveFlags  ( CellLoader::NewViewer );
    if (state == Qt::Unchecked) _cellDatas->resetActiveFlags( CellLoader::NewViewer );
  }


  void  ViewsModel::loadCell ()
  {
    if (not _cellDatas) return;

    emit layoutAboutToBeChanged();

    Cell*             cell         = NULL;
    const CellLoader* activeLoader = NULL;

    cell = _cellDatas->getCell();
    if ( cell or _cellDatas->isLoaded() ) {
      emit selectedCell( cell, _cellDatas->getActiveFlags() );
    } else {
      unsigned int  loadFlags = 0;
      unsigned int  viewFlags = 0;
      CellLoaders*  loaders   = CellLoaders::get();
      for ( size_t bit=16 ; bit<32 ; ++bit ) {
        unsigned int bitmask = 1 << bit;
        
        if (not (_cellDatas->getActiveFlags() & bitmask)) continue;
        const CellLoader* loader = loaders->getLoader( bitmask );
        if (loader) {
          if (not activeLoader) activeLoader = loader;
          loadFlags |= loader->getFlags();
          viewFlags |= loader->getViewFlags();
        }
      
        if (not loadFlags or not viewFlags) return;
      
        if ( (loadFlags & CellLoader::Native) and  (loadFlags & CellLoader::Importer) )
          return;
      }
      
      if (loadFlags & CellLoader::Native) {
        cell = AllianceFramework::get()->getCell( _cellDatas->getName(), viewFlags );
      }
      
      if (loadFlags & CellLoader::Importer) {
        cell = activeLoader->getCallback()( _cellDatas->getName() );
      }
      
      if (cell) {
        _cellDatas->setActiveFlags( CellLoader::Loaded );
        _cellDatas->setCell ( cell );

        emit selectedCell( cell, _cellDatas->getActiveFlags() );
        emit loadedCell();

        if (loadFlags & CellLoader::MultiCell)
          emit updatedLibrary();
      }
    }

    emit layoutChanged();
  }


}  // CRL namespace.
