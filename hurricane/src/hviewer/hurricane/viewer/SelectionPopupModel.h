
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./SelectionPopupModel.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SELECTION_POPUP_MODEL_H__
#define  __HURRICANE_SELECTION_POPUP_MODEL_H__

#include  <vector>
#include  <set>

#include  <QFont>
#include  <QApplication>
#include  <QAbstractTableModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/Graphics.h"


namespace Hurricane {


  class Selector;


  class SelectionPopupModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                 SelectionPopupModel  ( QObject* parent=NULL );
                                ~SelectionPopupModel  ();
             void                add                  ( Occurrence occurrence, bool showChange=false );
             void                clear                ();
             void                updateLayout         ();
             int                 rowCount             ( const QModelIndex& parent=QModelIndex() ) const;
             int                 columnCount          ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant            data                 ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant            headerData           ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
             Occurrence          getOccurrence        ( int row );

    private:
             vector<Occurrence>* _occurrences;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_SELECTION_POPUP_MODEL_H__ 
