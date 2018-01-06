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
// |  C++ Header  :  "./crlcore/CellsModel.h"                        |
// +-----------------------------------------------------------------+


#ifndef  CRL_CELLS_MODEL_H
#define  CRL_CELLS_MODEL_H

#include <map>
#include <vector>
#include <QAbstractTableModel>
#include "crlcore/AllianceLibrary.h"
#include "crlcore/CellDatas.h"


namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CellsModel".

  typedef std::map<const AllianceLibrary*, std::vector<CellDatas*> >  ALibraryMap;


  class CellsModel : public QAbstractTableModel {
      Q_OBJECT;
    public:
             void                           deleteStaticDatas ();
    public:
                                            CellsModel        ( QObject* parent=NULL );
                                           ~CellsModel        ();
             int                            rowCount          ( const QModelIndex& parent=QModelIndex() ) const;
             int                            columnCount       ( const QModelIndex& parent=QModelIndex() ) const;
             Qt::ItemFlags                  flags             (  const QModelIndex& ) const;
             QVariant                       data              ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant                       headerData        ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
             CellDatas*                     getCellDatas      ( int row ) const;
             void                           emitDataChanged   ( const QModelIndex& );
    public slots:                                             
             void                           updateLibrary     ();
             void                           setLibrary        ( const AllianceLibrary* );
    private:
      inline const std::vector<CellDatas*>& getCellsDatas     () const;
      inline       std::vector<CellDatas*>& getCellsDatas     ();
      inline const AllianceLibrary*         getLibrary        () const;
    private:
      static ALibraryMap                    _libraries;
             ALibraryMap::iterator          _ilibrary;
  };


  inline const AllianceLibrary*         CellsModel::getLibrary    () const
  { return (_ilibrary != _libraries.end()) ? (const AllianceLibrary*)_ilibrary->first : NULL; }

  inline const std::vector<CellDatas*>& CellsModel::getCellsDatas () const { return _ilibrary->second; }
  inline std::vector<CellDatas*>&       CellsModel::getCellsDatas () { return _ilibrary->second; }


}

#endif  // CRL_CELLS_MODEL_H
