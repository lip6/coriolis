// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./GraphicsWidget.cpp"                     |
// +-----------------------------------------------------------------+


#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/DisplayStyle.h"
#include "hurricane/viewer/GraphicsWidget.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "GraphicsWidget".


  GraphicsWidget::GraphicsWidget ( QWidget* parent )
    : QWidget     (parent)
    , _cellWidget (NULL)
    , _updateState(ExternalEmit)
  {
    setAttribute   ( Qt::WA_QuitOnClose, false );
    setWindowTitle ( tr("Display Styles") );
    setFont        ( Graphics::getNormalFont(true) );

    QButtonGroup* group    = new QButtonGroup ();
    QGroupBox*    groupBox = new QGroupBox    ( tr("Display Styles") );
    QGridLayout*  gLayout  = new QGridLayout  ();
    QVBoxLayout*  wLayout  = new QVBoxLayout  ();

    const vector<DisplayStyle*>& styles = Graphics::getStyles ();
    DisplayStyle* activeStyle = Graphics::getStyle ();

    size_t hideFallback = (styles.size() > 1) ? 1 : 0;
    for ( size_t istyle=hideFallback ; istyle < styles.size() ; istyle++ ) {
      QRadioButton* button  = new QRadioButton ();
      button->setText ( getString(styles[istyle]->getName()).c_str() );
      if ( activeStyle == styles[istyle] )
        button->setChecked ( true );
      QLabel* label = new QLabel ();
      label->setText ( styles[istyle]->getDescription().c_str() );
      label->setFont ( Graphics::getNormalFont() );

      gLayout->addWidget ( button, istyle-hideFallback, 0 );
      gLayout->addWidget ( label , istyle-hideFallback, 1 );
      group->addButton   ( button, istyle );
    }
    if (Graphics::isHighDpi()) gLayout->setContentsMargins( 30, 30, 30, 30 );

    groupBox->setLayout ( gLayout );
    wLayout->addWidget  ( groupBox );
    wLayout->addStretch ();
    setLayout ( wLayout );

    connect ( group, SIGNAL(buttonClicked(int)), this, SLOT(setStyle(int)) );
  }


  void  GraphicsWidget::setCellWidget ( CellWidget* cw )
  {
    if ( _cellWidget ) {
      disconnect ( _cellWidget, 0, this       , 0 );
      disconnect ( this       , 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if ( !_cellWidget ) return;

    connect ( _cellWidget, SIGNAL(styleChanged()), this, SLOT(changeStyle()) );

    _updateState = ExternalEmit;
    changeStyle ();
  }


  void  GraphicsWidget::changeStyle ()
  {
    if ( _updateState != InternalEmit ) {
    // Should read style here and sets the widget accordingly.
    }
    _updateState = ExternalEmit;
  }


  void  GraphicsWidget::setStyle ( int id )
  {
    if ( _cellWidget ) {
      _updateState = InternalEmit;
      _cellWidget->setStyle ( (size_t)id );
    }
  }



}
