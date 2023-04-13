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
// |  C++ Header   :  "./tramontana/EquipotentialsWidget.h"          |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include "hurricane/Commons.h"
#include "hurricane/Bug.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/CellWidget.h"
#include "tramontana/EquipotentialsModel.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Tramontana {

  using std::string;
  using std::set;
  using Hurricane::Cell;
  using Hurricane::CellWidget;
  using Hurricane::OccurrenceSet;


// -------------------------------------------------------------------
// Class  :  "SelectedEqui".


  class SelectedEqui {
    public:
      inline                      SelectedEqui   ();
      inline                      SelectedEqui   ( const Equipotential*, size_t access=0 );
      inline const Equipotential* getEqui        () const;
      inline size_t               getAccesses    () const;
      inline void                 incAccesses    () const;
      inline void                 setInserted    () const;
      inline void                 resetAccesses  () const;
    private:
              const Equipotential* _equi;
      mutable size_t               _accesses;
  };


  inline                      SelectedEqui::SelectedEqui  () : _equi(nullptr), _accesses(0) { }
  inline                      SelectedEqui::SelectedEqui  ( const Equipotential* equi, size_t accesses ) : _equi(equi), _accesses(accesses) { }
  inline const Equipotential* SelectedEqui::getEqui       () const { return _equi; }
  inline void                 SelectedEqui::setInserted   () const { _accesses = 64; }
  inline size_t               SelectedEqui::getAccesses   () const { return _accesses; }
  inline void                 SelectedEqui::incAccesses   () const { ++_accesses; }
  inline void                 SelectedEqui::resetAccesses () const { _accesses = 0; }


  struct SelectedEquiCompare {
      inline bool operator() ( const SelectedEqui& lhs, const SelectedEqui& rhs ) const;
  };


  inline bool SelectedEquiCompare::operator() ( const SelectedEqui& lhs, const SelectedEqui& rhs ) const
  {
    return lhs.getEqui()->getId() < rhs.getEqui()->getId();
  }


// -------------------------------------------------------------------
// Class  :  "SelectedEquiSet".


  class SelectedEquiSet : public set<SelectedEqui,SelectedEquiCompare>{
    public:
      void  insert         ( const Equipotential* );
      void  forceInserteds ();
      void  resetAccesses  ();
  };


  inline void  SelectedEquiSet::insert ( const Equipotential* equi )
  {
    iterator  iselected = find(SelectedEqui(equi));
    if (iselected != end())
      iselected->incAccesses();
    else
      set<SelectedEqui,SelectedEquiCompare>::insert( SelectedEqui(equi,64) );
  }


  inline void  SelectedEquiSet::forceInserteds ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->setInserted();
  }


  inline void  SelectedEquiSet::resetAccesses ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->resetAccesses();
  }


// -------------------------------------------------------------------
// Class  :  "EquipotentialsWidget".


  class EquipotentialsWidget : public QWidget {
      Q_OBJECT;

    public:
                    EquipotentialsWidget  ( QWidget* parent=nullptr );
      inline  Cell* getCell               ();
              void  setCellWidget         ( CellWidget* );
              void  setCell               ( Cell* );
              void  goTo                  ( int );
              void  updateSelecteds       ();
    signals:                              
              void  equipotentialSelect   ( Occurrences );
              void  equipotentialUnselect ( Occurrences );
              void  reframe               ( const Box& );
    private slots:                        
              void  textFilterChanged     ();
              void  updateSelecteds       ( const QItemSelection& , const QItemSelection& );
              void  fitToEqui             ();

    private:
              CellWidget*            _cellWidget;
              Cell*                  _cell;
              EquipotentialsModel*   _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              int                    _rowHeight;
              SelectedEquiSet        _selecteds;
              bool                   _forceReselect;
  };


  inline  Cell* EquipotentialsWidget::getCell () { return _cell; }


} // Hurricane namespace.
