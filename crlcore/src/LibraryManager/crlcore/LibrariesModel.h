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
// |  C++ Header  :  "./crlcore/LibrariesModel.h"                    |
// +-----------------------------------------------------------------+


#ifndef  CRL_LIBRARIES_MODEL_H
#define  CRL_LIBRARIES_MODEL_H

#include <vector>
#include <QAbstractTableModel>
#include "crlcore/AllianceLibrary.h"


namespace CRL {

// -------------------------------------------------------------------
// Class  :  "LibrariesModel".


  class LibrariesModel : public QAbstractTableModel {
      Q_OBJECT;
    public:
                                    LibrariesModel ( QObject* parent=NULL );
                                   ~LibrariesModel ();
             int                    rowCount       ( const QModelIndex& parent=QModelIndex() ) const;
             int                    columnCount    ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant               data           ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant               headerData     ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
             const AllianceLibrary* getLibrary     ( const QModelIndex& );
             const AllianceLibrary* getLibrary     ( size_t );
             void                   update         ();
    private:
      const AllianceLibraries& _libraries;
  };


}

#endif  // CRL_LIBRARIES_MODEL_H
