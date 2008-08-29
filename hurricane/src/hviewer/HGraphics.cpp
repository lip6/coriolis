
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
// |  C++ Module  :       "./HGraphics.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QLabel>
#include  <QRadioButton>
#include  <QButtonGroup>
#include  <QGroupBox>
#include  <QGridLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/DisplayStyle.h"
#include  "hurricane/viewer/HGraphics.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "HGraphics".


  HGraphics::HGraphics ( QWidget* parent )
    : QWidget(parent)
  {
    setAttribute   ( Qt::WA_QuitOnClose, false );
    setWindowTitle ( tr("Display Styles") );
    setFont        ( Graphics::getNormalFont(true) );

    QButtonGroup* group    = new QButtonGroup ();
    QGroupBox*    groupBox = new QGroupBox    ( tr("Display Styles") );
    QGridLayout*  gLayout  = new QGridLayout  ();
    QGridLayout*  wLayout  = new QGridLayout  ();

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
    wLayout->addWidget ( groupBox, 0, 0 );
    setLayout ( wLayout );

    connect ( group, SIGNAL(buttonClicked(int)), this, SLOT(styleChanged(int)) );
  }


  void  HGraphics::styleChanged ( int id )
  {
    Graphics::setStyle ( (size_t)id );
    cerr << "HGraphics::setStyle() - " << getString(Graphics::getStyle()->getName()) << endl;
    emit styleChanged ();
  }



}
