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
// |  C++ Module   :  "./TabEquipotentials.cpp"                      |
// +-----------------------------------------------------------------+


#include <QAction>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include "tramontana/TabEquipotentials.h"


namespace Tramontana {

  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "Tramontana::TabEquipotentials".


  TabEquipotentials::TabEquipotentials ( QWidget* parent )
    : ControllerTab         (parent)
    , _browser              (new EquipotentialsWidget())
    , _syncEquipotentials   (new QCheckBox())
    , _syncSelection        (new QCheckBox())
    , _cwCumulativeSelection(false)
  {
    _browser->setObjectName ( "controller.tabEquipotentials.browser" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins( 10, 0, 10, 0 );
    wLayout->setSpacing( 0 );

    _syncEquipotentials->setText   ( tr("Sync Equipotentials") );
    _syncEquipotentials->setChecked( false );
    _syncEquipotentials->setFont   ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect( _syncEquipotentials, SIGNAL(toggled(bool)), this, SLOT(setSyncEquipotentials(bool)) );

    _syncSelection->setText   ( tr("Sync Selection") );
    _syncSelection->setChecked( false );
    _syncSelection->setFont   ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect( _syncSelection, SIGNAL(toggled(bool)), this, SLOT(setSyncSelection(bool)) );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins( 0, 0, 0, 0 );
    commands->addStretch();
    commands->addWidget ( _syncEquipotentials );
    commands->addStretch();
    commands->addWidget ( _syncSelection );
    commands->addStretch();
    wLayout->addLayout  ( commands );

    QFrame* separator = new QFrame ();
    separator->setFrameShape ( QFrame::HLine );
    separator->setFrameShadow( QFrame::Sunken );
    wLayout->addWidget( separator );
    wLayout->addWidget( _browser );

    setLayout( wLayout );
  }


  void  TabEquipotentials::setSyncEquipotentials ( bool state )
  {
    if (state and getCellWidget()) {
      _browser->setCell( getCellWidget()->getCell() );
    } else {
      _browser->setCell( nullptr );
    }
  }


  void  TabEquipotentials::setSyncSelection ( bool state )
  {
    if (state and getCellWidget() and _syncEquipotentials->isChecked()) {
      _cwCumulativeSelection = getCellWidget()->cumulativeSelection();
      if (not _cwCumulativeSelection) {
        getCellWidget()->openRefreshSession ();
        getCellWidget()->unselectAll ();
        getCellWidget()->closeRefreshSession ();
      }
      getCellWidget()->setShowSelection( true );
      connect( _browser, SIGNAL(equipotentialSelect  (const OccurrenceSet&)), getCellWidget(), SLOT(selectSet  (const OccurrenceSet&)) );
      connect( _browser, SIGNAL(equipotentialUnselect(const OccurrenceSet&)), getCellWidget(), SLOT(unselectSet(const OccurrenceSet&)) );
      _browser->updateSelecteds();
    } else {
      getCellWidget()->setShowSelection( false );
      getCellWidget()->setCumulativeSelection( _cwCumulativeSelection );
      _browser->disconnect( getCellWidget(), SLOT(selectSet  (const OccurrenceSet&)) );
      _browser->disconnect( getCellWidget(), SLOT(unselectSet(const OccurrenceSet&)) );
    }
  }


  void  TabEquipotentials::setCell ( Cell* cell )
  {
    setSyncEquipotentials( _syncEquipotentials->isChecked() );
  }


  void  TabEquipotentials::setCellWidget ( CellWidget* cellWidget )
  {
    if (getCellWidget() != cellWidget) {
      ControllerTab::setCellWidget( cellWidget );
      _browser->setCellWidget( cellWidget );
      if (getCellWidget()) {
        connect( getCellWidget(), SIGNAL(cellChanged(Cell*)), this, SLOT(setCell(Cell*)) );
      }
      setSyncEquipotentials( _syncEquipotentials->isChecked() );
    }
  }


  void  TabEquipotentials::cellPreModificate ()
  {
    _browser->setCell( nullptr );
  }


  void  TabEquipotentials::cellPostModificate ()
  {
    setSyncEquipotentials( _syncEquipotentials->isChecked() );
  }




}
