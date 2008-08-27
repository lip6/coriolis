
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
// |  C++ Module  :       "./DynamicLabel.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <QFontMetrics>
# include  <QFont>
# include  <QLabel>
# include  <QHBoxLayout>

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/DynamicLabel.h"


namespace Hurricane {


  DynamicLabel::DynamicLabel ( QWidget* parent )
    : QWidget(parent)
    , _staticLabel(NULL)
    , _dynamicLabel(NULL)
  {
    _staticLabel = new QLabel ();
    _staticLabel->setText ( " V: " );
    _staticLabel->setMinimumSize ( _staticLabel->sizeHint() );
    _staticLabel->setAlignment   ( Qt::AlignHCenter );
  //_staticLabel->setFrameStyle (QFrame::Panel | QFrame::Raised );

    QFont  font   = Graphics::getFixedFont(QFont::Bold);
    int    cwidth = QFontMetrics(font).width('X');

    _dynamicLabel = new QLabel ();
    _dynamicLabel->setText ( "vvv.vu" );
    _dynamicLabel->setIndent      ( cwidth/2 );
    _dynamicLabel->setMinimumSize ( QSize(cwidth*10,QFontMetrics(font).height()) );
    _dynamicLabel->setAlignment   ( Qt::AlignRight );
    _dynamicLabel->setFont        ( font );

    QHBoxLayout* layout = new QHBoxLayout ();
    layout->addWidget ( _staticLabel );
    layout->addWidget ( _dynamicLabel );
    layout->setContentsMargins ( 0, 0, 0, 0 );

    setLayout ( layout );
  }


  QString  DynamicLabel::getStaticText () const
  {
    return _staticLabel->text ();
  }


  QString  DynamicLabel::getDynamicText () const
  {
    return _dynamicLabel->text ();
  }


  void  DynamicLabel::setStaticText ( const QString& text )
  {
    _staticLabel->setText ( text );
  }


  void  DynamicLabel::setDynamicText ( const QString& text )
  {
    _dynamicLabel->setText ( text );
  }


  void  DynamicLabel::setDynamicText ( DbU::Unit u )
  {
    _dynamicLabel->setText ( DbU::getValueString(u,0).c_str() );
  }


} // End of Hurricane namespace.
