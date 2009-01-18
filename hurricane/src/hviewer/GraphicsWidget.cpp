
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
// |  C++ Module  :       "./GraphicsWidget.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QLabel>
#include  <QRadioButton>
#include  <QButtonGroup>
#include  <QGroupBox>
#include  <QVBoxLayout>
#include  <QGridLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/DisplayStyle.h"
#include  "hurricane/viewer/GraphicsWidget.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "GraphicsWidget".


  GraphicsWidget::GraphicsWidget ( QWidget* parent )
    : QWidget(parent)
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

      group->setId       ( button, istyle );
      group->addButton   ( button );
    }

    groupBox->setLayout ( gLayout );
    wLayout->addWidget  ( groupBox );
    wLayout->addStretch ();
    setLayout ( wLayout );

    connect ( group, SIGNAL(buttonClicked(int)), this, SLOT(styleChange(int)) );
  }


  void  GraphicsWidget::styleChange ( int id )
  {
    Graphics::setStyle ( (size_t)id );
  //cerr << "GraphicsWidget::setStyle() - " << getString(Graphics::getStyle()->getName()) << endl;
    emit styleChanged ( (void*)this );
  }



}
