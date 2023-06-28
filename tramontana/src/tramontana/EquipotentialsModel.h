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
// |  C++ Header   :  "./tramontana/EquipotentialsModel.h"           |
// +-----------------------------------------------------------------+


#pragma  once
#include <vector>
#include <QFont>
#include <QApplication>
#include <QAbstractTableModel>
#include "hurricane/Commons.h"
#include "hurricane/Name.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "tramontana/Equipotential.h"
#include "tramontana/TramontanaEngine.h"


namespace Tramontana {

  using Hurricane::Cell;


  class EquipotentialsModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                  EquipotentialsModel ( QObject* parent=NULL );
             void                 setCell      ( Cell* cell );
             int                  rowCount     ( const QModelIndex& parent=QModelIndex() ) const;
             int                  columnCount  ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant             data         ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant             headerData   ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline Cell*                getCell      ();
             const Equipotential* getEqui      ( int row );

    private:
             Cell*                        _cell;
             std::vector<Equipotential*>  _equipotentials;
  };


// Inline Functions.

  inline Cell* EquipotentialsModel::getCell () { return _cell; }


} // Hurricane namespace.
