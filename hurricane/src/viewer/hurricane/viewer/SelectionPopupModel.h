// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/SelectionPopupModel.h" |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_SELECTION_POPUP_MODEL_H
#define  HURRICANE_SELECTION_POPUP_MODEL_H

#include <vector>
#include <set>
#include <QFont>
#include <QApplication>
#include <QAbstractTableModel>
#include "hurricane/Commons.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Occurrences.h"
#include "hurricane/viewer/Graphics.h"


namespace Hurricane {

  class Selector;


  class SelectionPopupModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                          SelectionPopupModel  ( QObject* parent=NULL );
                         ~SelectionPopupModel  ();
      void                clearFilter          ();
      void                setFilter            ( OccurrenceFilter );
      OccurrenceFilter    getFilter            ();
      void                loadOccurrences      ( Occurrences, bool showChange=false );
      void                clear                ();
      void                updateLayout         ();
      int                 charWidth            () const;
      int                 rowCount             ( const QModelIndex& parent=QModelIndex() ) const;
      int                 columnCount          ( const QModelIndex& parent=QModelIndex() ) const;
      QVariant            data                 ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
      QVariant            headerData           ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      Occurrence          getOccurrence        ( int row );

    private:
      OccurrenceFilter    _filter;
      vector<Occurrence>* _occurrences;
      int                 _charWidth;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_SELECTION_POPUP_MODEL_H__ 
