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

  using std::cerr;
  using std::endl;
  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "Tramontana::TabEquipotentials".


  TabEquipotentials::TabEquipotentials ( QWidget* parent )
    : ControllerTab         (parent)
    , _browser              (new EquipotentialsWidget())
    , _syncEquipotentials   (new QCheckBox())
    , _showSelection        (new QCheckBox())
    , _showBuried           (new QCheckBox())
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

    _showSelection->setText   ( tr("Show Selection") );
    _showSelection->setChecked( false );
    _showSelection->setFont   ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect( _showSelection, SIGNAL(toggled(bool)), this, SLOT(setShowSelection(bool)) );

    _showBuried->setText   ( tr("Show Buried") );
    _showBuried->setChecked( false );
    _showBuried->setFont   ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect( _showBuried, SIGNAL(toggled(bool)), this, SLOT(setShowBuried(bool)) );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins( 0, 0, 0, 0 );
    commands->addStretch();
    commands->addWidget ( _syncEquipotentials );
    commands->addStretch();
    commands->addWidget ( _showBuried );
    commands->addStretch();
    commands->addWidget ( _showSelection );
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


  void  TabEquipotentials::setShowSelection ( bool state )
  {
    if (not getCellWidget()) return;
    
    _showSelection->blockSignals( true );
    _showSelection->setChecked  ( state );
    if (state) {
      _cwCumulativeSelection = getCellWidget()->cumulativeSelection();
      getCellWidget()->setShowSelection( true );
      connect( _browser, SIGNAL(equipotentialSelect  (Occurrences)), getCellWidget(), SLOT(select  (Occurrences)) );
      connect( _browser, SIGNAL(equipotentialUnselect(Occurrences)), getCellWidget(), SLOT(unselect(Occurrences)) );
      _browser->updateSelecteds();
    } else {
      getCellWidget()->setShowSelection( false );
      getCellWidget()->setCumulativeSelection( _cwCumulativeSelection );
      _browser->disconnect( getCellWidget(), SLOT(select  (Occurrences)) );
      _browser->disconnect( getCellWidget(), SLOT(unselect(Occurrences)) );
    }
    _showSelection->blockSignals( false );
  }


  void  TabEquipotentials::selectionModeChanged ()
  {
    setShowSelection( getCellWidget()->showSelection() );
  }


  void  TabEquipotentials::setShowBuried ( bool state )
  {
    if (not getCellWidget()) return;
    _browser->setShowBuried( state );
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
        connect( getCellWidget(), SIGNAL(cellChanged(Cell*))    , this, SLOT(setCell(Cell*)) );
        connect( getCellWidget(), SIGNAL(selectionModeChanged()), this, SLOT(selectionModeChanged()) );
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
