
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
// |  C++ Header  :       "./SelectionModel.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SELECTION_MODEL_H__
#define  __HURRICANE_SELECTION_MODEL_H__

#include  <vector>
#include  <set>

#include  <QFont>
#include  <QApplication>
#include  <QAbstractTableModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/Selector.h"


namespace Hurricane {


  class OccurrenceItem {
    public:
      enum Flags { Selected=1 };
    public:
      inline                OccurrenceItem ( Occurrence occurrence, unsigned int flags=Selected );
      inline  void          toggle         ();
      inline  bool          operator==     ( const OccurrenceItem& other ) const;
    public:
              unsigned int  _flags;
              Occurrence    _occurrence;
  };


  inline OccurrenceItem::OccurrenceItem ( Occurrence occurrence, unsigned int flags )
    : _flags(flags)
    , _occurrence(occurrence)
  { }


  inline void  OccurrenceItem::toggle ()
  {
    if ( _flags & Selected ) _flags &= ~Selected;
    else                     _flags |=  Selected;
  }


  inline  bool  OccurrenceItem::operator== ( const OccurrenceItem& other ) const
  {
    return _occurrence == other._occurrence;
  }


  class SelectionModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                     SelectionModel  ( QObject* parent=NULL );
                                    ~SelectionModel  ();
             void                    setSelection    ( const SelectorSet& selection );
             void                    toggleSelection ( Occurrence occurrence );
             Occurrence              toggleSelection ( const QModelIndex& index );
             int                     rowCount        ( const QModelIndex& parent=QModelIndex() ) const;
             int                     columnCount     ( const QModelIndex& parent=QModelIndex() ) const;
             QVariant                data            ( const QModelIndex& index, int role=Qt::DisplayRole ) const;
             QVariant                headerData      ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
             const Occurrence        getOccurrence   ( int row );
             bool                    isCumulative    () const;
    public slots:
             void                    clear           ();

    private:
             vector<OccurrenceItem>  _selection;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_SELECTION_MODEL_H__ 
