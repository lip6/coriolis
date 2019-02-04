// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/SlicingDataModel.h"                     |
// +-----------------------------------------------------------------+


#ifndef  BORA_SLICING_DATA_MODEL_H
#define  BORA_SLICING_DATA_MODEL_H

#include <vector>
#include <QAbstractTableModel>
namespace Hurricane {
  class Cell;
}
#include "bora/SlicingNode.h"


namespace Bora {

  using Hurricane::Cell;


  class SlicingDataModel : public QAbstractTableModel {
      Q_OBJECT;
    public:
               SlicingDataModel ( QObject* parent=NULL );
              ~SlicingDataModel ();
      void     setCell          ( Cell* cell );
      int      rowCount         ( const QModelIndex& parent=QModelIndex() ) const;
      int      columnCount      ( const QModelIndex& parent=QModelIndex() ) const;
      QVariant data             ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
      QVariant headerData       ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      BoxSet*  getBoxSet        ( int index );
      void     updateContents   ();
    private:
      Cell* _cell; 
  };
  

}  // Bora namespace.

#endif  // BORA_SLICING_DATA_MODEL_H
