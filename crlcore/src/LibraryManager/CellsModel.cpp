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
// |  C++ Module  :  "./CellsModel.cpp"                              |
// +-----------------------------------------------------------------+


#include <iostream>
#include <algorithm>
#include <QFont>
#include <QApplication>
#include "vlsisapd/utilities/Path.h"
#include "hurricane/Error.h"
#include "hurricane/Cells.h"
#include "hurricane/Library.h"
#include "hurricane/viewer/Graphics.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/Catalog.h"
#include "crlcore/AcmSigda.h"
#include "crlcore/Ispd05Bookshelf.h"
#include "crlcore/Blif.h"
#include "crlcore/Iccad04Lefdef.h"
#include "crlcore/CellsModel.h"


namespace CRL {

  using namespace std;
  using Hurricane::ForEachIterator;
  using Hurricane::Error;
  using Hurricane::Library;
  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "CellsModel".


  ALibraryMap  CellsModel::_libraries;


  void  CellsModel::deleteStaticDatas ()
  {
    for ( auto ilibrary : _libraries ) {
      for ( auto cellDatas : ilibrary.second ) {
        delete cellDatas;
      }
    }
    _libraries.clear();
  }


  CellsModel::CellsModel ( QObject* parent )
    : QAbstractTableModel(parent)
    , _ilibrary          (_libraries.end())
  {
    if (_libraries.empty()) {
      _ilibrary = _libraries.insert( make_pair((const AllianceLibrary*)NULL,vector<CellDatas*>()) ).first;
    }

    CellLoaders* loaders = CellLoaders::get();
    if (loaders->empty()) {
      loaders->addLoader( new CellLoader("vbe"
                                        , "VHDL Behavioral"
                                        , CellLoader::Native
                                        , Catalog::State::Logical ) );
      loaders->addLoader( new CellLoader("vst"
                                        , "VHDL Structural"
                                        , CellLoader::Native
                                        , Catalog::State::Logical ) );
      loaders->addLoader( new CellLoader("vhd"
                                        , "VHDL Structural (IEEE)"
                                        , CellLoader::Native
                                        , Catalog::State::Logical ) );
      loaders->addLoader( new CellLoader("vhdl"
                                        , "VHDL Structural (IEEE)"
                                        , CellLoader::Native
                                        , Catalog::State::Logical ) );
      loaders->addLoader( new CellLoader("ap"
                                        , "Alliance Physical"
                                        , CellLoader::Native
                                        , Catalog::State::Physical) );
      loaders->addLoader( new CellLoader("bench"
                                        , "ACM/SIGDA (aka MCNC)"
                                        , CellLoader::Importer
                                        , Catalog::State::Logical
                                        , std::bind( &AcmSigda::load, placeholders::_1 ) ) );
      loaders->addLoader( new CellLoader("aux"
                                        , "ISPD'05 (Bookshelf)"
                                        , CellLoader::Importer
                                        , Catalog::State::Logical|Catalog::State::Physical
                                        , std::bind( &Ispd05::load, placeholders::_1 ) ) );
      loaders->addLoader( new CellLoader("blif"
                                        , "BLIF (Yosys/ABC)"
                                        , CellLoader::Importer|CellLoader::MultiCell
                                        , Catalog::State::Logical
                                        , std::bind( &Blif::load, placeholders::_1 ) ) );
    }
  }


  CellsModel::~CellsModel ()
  { }


  QVariant  CellsModel::data ( const QModelIndex& index, int role ) const
  {
    static QFont nameFont   = Graphics::getFixedFont( QFont::Bold );
    static QFont valueFont  = Graphics::getFixedFont( QFont::Normal, false );
    static QFont loadedFont = Graphics::getFixedFont( QFont::Normal, true );

    if (role == Qt::FontRole) {
      if (index.column() == 1) return valueFont;

      const Catalog::State* state = getCellDatas(index.row())->getState();
      if (state and state->isInMemory() )
        return loadedFont;

      return QVariant();
    }

    if ( (not getLibrary()) or (not index.isValid()) ) return QVariant();

    if (role == Qt::DisplayRole) {
      if (index.column() == 0) return getCellDatas(index.row())->getName().c_str();
      if (index.column() == 1) {
        string s;
        const Catalog::State* state = getCellDatas(index.row())->getState();

        if (state) s = getString(state);
        else       s = ">>>>>>";
        s += (getCellDatas(index.row())->isInMemory()) ? "M" : "-";

        return s.c_str();
      }
    }
    return QVariant();
  }


  QVariant  CellsModel::headerData ( int             section
                                   , Qt::Orientation orientation
                                   , int             role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();

    static QFont headerFont = Graphics::getFixedFont( QFont::Bold, false, false, +0 );

    if (role == Qt::FontRole   ) return headerFont;
    if (role != Qt::DisplayRole) return QVariant();

    switch ( section ) {
      case 0: return QVariant("Name");
      case 1: return QVariant("State");
    }

    return QVariant();
  }


  Qt::ItemFlags  CellsModel::flags (  const QModelIndex& index ) const
  {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

  //if (index.isValid() and (getCellDatas(index.row())->getState()->isInMemory() ) )
  //  return Qt::NoItemFlags;

    return flags;
  }


  int  CellsModel::rowCount ( const QModelIndex& parent ) const
  { return getCellsDatas().size(); }


  int  CellsModel::columnCount ( const QModelIndex& parent ) const
  { return 2; }


  CellDatas* CellsModel::getCellDatas ( int row ) const
  {
    if (row >= (int)getCellsDatas().size()) return NULL;
    return const_cast<CellDatas*>( getCellsDatas()[row] );
  }


  void  CellsModel::emitDataChanged ( const QModelIndex& index )
  {
    emit layoutAboutToBeChanged();
    emit dataChanged( index, index );
    emit layoutChanged();
  }


  void  CellsModel::updateLibrary ()
  {
    const AllianceLibrary* library = getLibrary();
    _ilibrary = _libraries.end();

    setLibrary( library );
  }


  void  CellsModel::setLibrary ( const AllianceLibrary* library )
  {
    if (getLibrary() == library) return;

    _ilibrary = _libraries.find( (const AllianceLibrary*)library );
    if (_ilibrary == _libraries.end()) {
      _ilibrary = _libraries.insert( make_pair(library,vector<CellDatas*>()) ).first;
    }

    emit layoutAboutToBeChanged();

    if (not getLibrary()) return;

    Catalog*     catalog = AllianceFramework::get()->getCatalog();
    CellLoaders* loaders = CellLoaders::get();

    getCellsDatas().clear();
    forEach ( Cell*, icell, library->getLibrary()->getCells() ) {
      CellDatas* datas = new CellDatas(*icell);
      getCellsDatas().push_back( datas );
    }
    std::sort( getCellsDatas().begin(), getCellsDatas().end(), LessCellDatas );

    Utilities::Path libraryPath ( getString(getLibrary()->getPath()) );
    CellDatas::setLibraryPath( libraryPath );

    vector<Utilities::Path>  cellFiles = libraryPath.listdir();
    for ( Utilities::Path cellFile : cellFiles ) {
      const CellLoader* loader = loaders->getLoader( cellFile.ext() );
      if (not loader) continue;

      bool       added    = false;
      string     cellName = cellFile.basename("."+cellFile.ext()).toString();
      CellDatas* key      = new CellDatas( cellName );

      auto idatas = lower_bound( getCellsDatas().begin(), getCellsDatas().end(), key, LessCellDatas );

      if (  (idatas               == getCellsDatas().end())
         or ((*idatas)->getName() != cellName             ) ) {
        getCellsDatas().push_back( key );
        key->setState( catalog->getState(cellName) );

        added  = true;
      } else {
        delete key;
        key = *idatas;
      }

      key->setFlags( loader->getFlags() & CellLoaders::lmask() );

      if (added) std::sort( getCellsDatas().begin(), getCellsDatas().end(), LessCellDatas );
    }

    emit layoutChanged();
  }


}  // CRL namespace.
