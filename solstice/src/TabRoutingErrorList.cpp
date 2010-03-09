

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
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TabRoutingErrorList.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QCheckBox>

#include <solstice/TabRoutingErrorList.h>

namespace Solstice {

// -------------------------------------------------------------------
// Class  :  "Hurricane::TabRoutingErrorList".


  TabRoutingErrorList::TabRoutingErrorList ( QWidget* parent )
    : ControllerTab         (parent)
    , _routingErrorListBrowser       (new RoutingErrorListWidget())
    , _syncRoutingErrorList          (new QCheckBox())
    , _syncSelection        (new QCheckBox())
    , _cwCumulativeSelection(false)
  {
    _routingErrorListBrowser->setObjectName ( "controller.tabRoutingErrorList.routingErrorListBrowser" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 0, 10, 0 );
    wLayout->setSpacing ( 0 );

    _syncRoutingErrorList->setText    ( tr("Sync RoutingErrorList") );
    _syncRoutingErrorList->setChecked ( false );
    _syncRoutingErrorList->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect ( _syncRoutingErrorList, SIGNAL(toggled(bool)), this, SLOT(setSyncRoutingErrorList(bool)) );

    _syncSelection->setText    ( tr("Sync Selection") );
    _syncSelection->setChecked ( false );
    _syncSelection->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect ( _syncSelection, SIGNAL(toggled(bool)), this, SLOT(setSyncSelection(bool)) );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins ( 0, 0, 0, 0 );
    commands->addStretch ();
    commands->addWidget  ( _syncRoutingErrorList );
    commands->addStretch ();
    commands->addWidget  ( _syncSelection );
    commands->addStretch ();
    wLayout->addLayout   ( commands );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    wLayout->addWidget ( separator );
    wLayout->addWidget ( _routingErrorListBrowser );

    setLayout ( wLayout );
  }


  void  TabRoutingErrorList::setSyncRoutingErrorList ( bool state )
  {
    if ( state && getCellWidget() ) {
      _routingErrorListBrowser->setCell<SimpleRoutingErrorInformations> ( getCellWidget()->getCell() );
    } else {
      _routingErrorListBrowser->setCell<SimpleRoutingErrorInformations> ( NULL );
    }
  }


  void  TabRoutingErrorList::setSyncSelection ( bool state )
  {
    if ( state && getCellWidget() && _syncRoutingErrorList->isChecked() ) {
      _cwCumulativeSelection = getCellWidget()->cumulativeSelection();
      if ( !_cwCumulativeSelection ) {
        getCellWidget()->openRefreshSession ();
        getCellWidget()->unselectAll ();
        getCellWidget()->closeRefreshSession ();
      }
      getCellWidget()->setShowSelection ( true );
      connect ( _routingErrorListBrowser, SIGNAL(occurrenceSelected  (const Occurrence)), getCellWidget(), SLOT(select  (const Occurrence)) );
      connect ( _routingErrorListBrowser, SIGNAL(occurrenceUnselected(const Occurrence)), getCellWidget(), SLOT(unselect(const Occurrence)) );
      _routingErrorListBrowser->updateSelecteds ();
    } else {
      getCellWidget()->setShowSelection ( false );
      getCellWidget()->setCumulativeSelection ( _cwCumulativeSelection );
      _routingErrorListBrowser->disconnect ( getCellWidget(), SLOT(select  (const Occurrence)) );
      _routingErrorListBrowser->disconnect ( getCellWidget(), SLOT(unselect(const Occurrence)) );
    }
  }


  void  TabRoutingErrorList::setCell ( Cell* cell )
  {
    setSyncRoutingErrorList ( _syncRoutingErrorList->isChecked() );
  }


  void  TabRoutingErrorList::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      _routingErrorListBrowser->setCellWidget<SimpleRoutingErrorInformations> ( cellWidget );
      if ( getCellWidget() ) {
        connect ( getCellWidget(), SIGNAL(cellChanged(Cell*)), this, SLOT(setCell(Cell*)) );
      }
      setSyncRoutingErrorList ( _syncRoutingErrorList->isChecked() );
    }
  }


  void  TabRoutingErrorList::cellPreModificate ()
  {
    _routingErrorListBrowser->setCell<SimpleRoutingErrorInformations> ( NULL );
  }


  void  TabRoutingErrorList::cellPostModificate ()
  {
    setSyncRoutingErrorList ( _syncRoutingErrorList->isChecked() );
  }


} // End of Solstice namespace.
