// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2024.
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
// |  C++ Header   :  "./tramontana/OpenCircuitModel.h"              |
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
#include "tramontana/TramontanaEngine.h"


namespace Tramontana {

  class OpenCircuit;


// -------------------------------------------------------------------
// Class  :  "Tramontana::OpenCircuitModel".

  class OpenCircuitModel : public QAbstractTableModel {
      Q_OBJECT;
    public:
                                 OpenCircuitModel ( QObject* parent=NULL );
                                ~OpenCircuitModel ();
                   void          setTramontana    ( TramontanaEngine* );
             const TramontanaEngine::OpenSet&
                                 getOpenNets      () const;
                   int           rowCount         ( const QModelIndex& parent=QModelIndex() ) const;
                   int           columnCount      ( const QModelIndex& parent=QModelIndex() ) const;
                   QVariant      data             ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
                   QVariant      headerData       ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      inline       Tramontana*   getTramontana    ();
                   const Tramontana::OpenCircuit
                                 getOpenCircuit   ( int row );

    private:
             TramontanaEngine* _tramontana;
  };


  inline Tramontana* OpenCircuitModel::getTramontana () { return _tramontana; }

  inline void  OpenCircuitModel::setTramontana ( Tramontana* tramontana )
  {
    if (_tramontana != tramontana) {
      emit layoutAboutToBeChanged ();
      _tramontana = tramontana;
      emit layoutChanged ();
    }
  }


} // Tramontana namespace.
