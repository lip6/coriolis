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
// |  C++ Header  :  "./crlcore/ViewsModel.h"                        |
// +-----------------------------------------------------------------+


#ifndef  CRL_VIEWS_MODEL_H
#define  CRL_VIEWS_MODEL_H

#include <vector>
#include <QAbstractTableModel>
#include "crlcore/AllianceLibrary.h"

namespace Hurricane {
  class Cell;
}


namespace CRL {

  using Hurricane::Cell;
  class CellDatas;
  class CellLoader;


// -------------------------------------------------------------------
// Class  :  "ViewsModel".


  class ViewsModel : public QAbstractTableModel {
      Q_OBJECT;
    public:
                     ViewsModel     ( QObject* parent=NULL );
                    ~ViewsModel     ();
      int            rowCount       ( const QModelIndex& parent=QModelIndex() ) const;
      int            columnCount    ( const QModelIndex& parent=QModelIndex() ) const;
      QVariant       data           ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
      QVariant       headerData     ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      Qt::ItemFlags  flags          ( const QModelIndex& index ) const;
      bool           setData        ( const QModelIndex& index, const QVariant& value, int role );
    signals:
      void           updatedLibrary ();
      void           selectedCell   ( Cell*, unsigned int );
      void           loadedCell     ();
    public slots:                   
      void           setCellDatas   ( CellDatas* );
      void           loadCell       ();
      void           setNewViewer   ( int state );
    private:
      CellDatas*                      _cellDatas;
      std::vector<const CellLoader*>  _viewLoaders;
      std::vector<size_t>             _viewSizes;
  };


}

#endif  // CRL_VIEWS_MODEL_H
