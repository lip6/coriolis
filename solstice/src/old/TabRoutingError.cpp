

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
// |  C++ Module  :       "./TabRoutingError.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#include  <QFrame>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QCheckBox>
#include  <QComboBox>

#include  <hurricane/DataBase.h>
#include  <hurricane/Cell.h>
#include  <hurricane/Occurrence.h>
#include  <hurricane/viewer/Graphics.h>
#include  <hurricane/viewer/CellWidget.h>
#include  <hurricane/viewer/ControllerWidget.h>
#include  "hurricane/viewer/NetInformations.h"
#include  <solstice/RoutingError.h>
#include  <solstice/RoutingErrorListWidget.h>
#include  <solstice/TabRoutingError.h>


namespace Solstice {

// -------------------------------------------------------------------
// Class  :  "Hurricane::TabRoutingError".

  void TabRoutingError::select ( const RoutingError* error)
  {
    cout << " TabRoutingError::SELECT" << flush;
    forEach(Occurrence,occurrence,getCollection(error->getOccurrenceToSel())) {
      cout << " + 1" << flush;
      getCellWidget()->select((*occurrence));
    }
  };
  
  void TabRoutingError::unselect ( const RoutingError* error)
  {
    cout << " TabRoutingError::UNSELECT" << flush;
    forEach(Occurrence,occurrence,getCollection(error->getOccurrenceToSel())) {
    cout << " - 1" << flush;
      getCellWidget()->unselect((*occurrence));
    }
  };

  TabRoutingError::TabRoutingError ( QWidget* parent )
    : ControllerTab              (parent)
    , _routingErrorBrowser       (new RoutingErrorListWidget())
    , _syncRoutingError          (new QCheckBox())
    , _syncSelection             (new QCheckBox())
    , _cwCumulativeSelection     (false)
  {
    _routingErrorBrowser->setObjectName ( "controller.tabRoutingError.routingErrorBrowser" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 0, 10, 0 );
    wLayout->setSpacing ( 0 );

    _syncRoutingError->setText    ( tr("Sync RoutingError") );
    _syncRoutingError->setChecked ( false );
    _syncRoutingError->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect ( _syncRoutingError, SIGNAL(toggled(bool)), this, SLOT(setSyncRoutingError(bool)) );

    _syncSelection->setText    ( tr("Sync Selection") );
    _syncSelection->setChecked ( false );
    _syncSelection->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect ( _syncSelection, SIGNAL(toggled(bool)), this, SLOT(setSyncSelection(bool)) );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins ( 0, 0, 0, 0 );
    commands->addStretch ();
    commands->addWidget  ( _syncRoutingError );
    commands->addStretch ();
    commands->addWidget  ( _syncSelection );
    commands->addStretch ();
    wLayout->addLayout   ( commands );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    wLayout->addWidget ( separator );
    wLayout->addWidget ( _routingErrorBrowser );

    setLayout ( wLayout );
  }


  void  TabRoutingError::setSyncRoutingError ( bool state )
  {

    if ( state && getCellWidget() ) {
    _routingErrorBrowser->setCell<SimpleNetInformations> ( getCellWidget()->getCell() );
  } else {
    _routingErrorBrowser->setCell<SimpleNetInformations> ( NULL );
  }
}


  void  TabRoutingError::setSyncSelection ( bool state )
  {
    if ( state && getCellWidget() && _syncRoutingError->isChecked() ) {
      _cwCumulativeSelection = getCellWidget()->cumulativeSelection();
      if ( !_cwCumulativeSelection ) {
        getCellWidget()->openRefreshSession ();
        getCellWidget()->unselectAll ();
        getCellWidget()->closeRefreshSession ();
      }
      getCellWidget()->setShowSelection ( true );

      connect ( _routingErrorBrowser, 
		SIGNAL(routingErrorSelected  (const RoutingError*)), 
		this,
		SLOT(select  (const RoutingError*)) );

      connect ( _routingErrorBrowser, 
		SIGNAL(routingErrorUnselected(const RoutingError*)), 
		this, 
		SLOT(unselect(const RoutingError*)) );

      _routingErrorBrowser->updateSelecteds ();
    } else {
      getCellWidget()->setShowSelection ( false );
      getCellWidget()->setCumulativeSelection ( _cwCumulativeSelection );
      _routingErrorBrowser->disconnect ( this, SLOT(select  (const RoutingError*)) );
      _routingErrorBrowser->disconnect ( this, SLOT(unselect(const RoutingError*)) );
    }
  }


  void  TabRoutingError::setCell ( Cell* cell )
  {
    setSyncRoutingError ( _syncRoutingError->isChecked() );
  }


  void  TabRoutingError::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      _routingErrorBrowser->setCellWidget<SimpleNetInformations> ( cellWidget );
      if ( getCellWidget() ) {
        connect ( getCellWidget(), SIGNAL(cellChanged(Cell*)), this, SLOT(setCell(Cell*)) );
      }
      setSyncRoutingError ( _syncRoutingError->isChecked() );
    }
  }


  void  TabRoutingError::cellPreModificate ()
  {
    _routingErrorBrowser->setCell<SimpleNetInformations>  ( NULL );
  }


  void  TabRoutingError::cellPostModificate ()
  {
    setSyncRoutingError ( _syncRoutingError->isChecked() );
  }


} // End of Solstice namespace.
