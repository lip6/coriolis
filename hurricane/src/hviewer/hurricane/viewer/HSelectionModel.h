
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Header  :       "./HSelectionModel.h"                      |
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


namespace Hurricane {


  class Selector;


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


  class HSelectionModel : public QAbstractTableModel {
      Q_OBJECT;

    public:
                                     HSelectionModel ( QObject* parent=NULL );
                                    ~HSelectionModel ();
             void                    setSelection    ( const set<Selector*>& selection );
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
