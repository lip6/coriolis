
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./DynamicLabel.cpp"                       |
// +-----------------------------------------------------------------+


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
    _dynamicLabel = new QLabel ();
    _dynamicLabel->setText ( "vvv.vu" );
    _dynamicLabel->setAlignment   ( Qt::AlignRight );
    _dynamicLabel->setFont        ( font );
    setDynamicWidth               ( 10 );

    QHBoxLayout* layout = new QHBoxLayout ();
    layout->addWidget  ( _staticLabel );
    layout->addWidget  ( _dynamicLabel );
    layout->setSpacing ( 0 );
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


  void  DynamicLabel::setDynamicWidth ( int chars, int flags )
  {
    QFont  font   = Graphics::getFixedFont(QFont::Bold);
    int    cwidth = QFontMetrics(font).width('X');

    if ( flags|NoIndent   ) _dynamicLabel->setIndent( 0 );
    if ( flags|HalfIndent ) _dynamicLabel->setIndent( cwidth/2 );
    _dynamicLabel->setMinimumSize( QSize(cwidth*chars,QFontMetrics(font).height()) );
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
