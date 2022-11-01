// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AboutWindow.cpp"                        |
// +-----------------------------------------------------------------+


#include <unistd.h>
#include <csignal>
#include <memory>
#include <QAction>
#include <QCloseEvent>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QFont>
#include <QFontMetrics>
#include "hurricane/TextTranslator.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/AboutWindow.h"


namespace Hurricane {


  bool  AboutWindow::show ()
  {
    AboutWindow* abw = new AboutWindow();
    return (abw->exec() != QDialog::Rejected);
  }


  AboutWindow::AboutWindow ( QWidget* parent )
    : QDialog( parent )
  {
    setAttribute  ( Qt::WA_DeleteOnClose );
    setModal      ( true );
    setWindowTitle( tr("Coriolis About Window") );
    setToolTip    ( tr("Sorbonne Université Proudly Present") );

    QString  about = "Coriolis EDA System 0.x . . . . . . . . . . . . . .  cgt 1.0\n"
                     "Copyright (c) 2008-2022 . . . . . . . .  Sorbonne Université\n"
                     "Authors . . . . . . . . . . . . . . . . Christophe Alexandre\n"
                     "        . . . . . . . . . . . . . . . . . .  Sophie Belloeil\n"
                     "        . . . . . . . . . . . . . . . . . . Jean-Paul Chaput\n"
                     "        . . . . . . . . . . . . . . . . . . .  Damien Dupuis\n"
                     "        . . . . . . . . . . . . . . . . . . .  Remy Escassut\n"
                     "        . . . . . . . . . . . . . . . . . .  Gabriel Gouvine\n"
                     "        . . . . . . . . . . . . . . . . . . Christian Masson\n"
                     "E-Mail  . . . . . . . . . . . . . . Jean-Paul.Chaput@lip6.fr";

    QFrame* topLine = new QFrame ();
    topLine->setFrameShape( QFrame::HLine );
    topLine->setLineWidth ( 2 );
    QFrame* botLine = new QFrame ();
    botLine->setFrameShape( QFrame::HLine );
    botLine->setLineWidth ( 2 );

    QLabel* logo = new QLabel ();
    logo->setAlignment ( Qt::AlignCenter );
    logo->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    logo->setPixmap    ( QPixmap(":/images/Coriolis-logo-white-4.png").scaledToWidth( Graphics::isHighDpi() ? 200 : 80 ) );
              
    QLabel* title = new QLabel( "CGT" );
    title->setAlignment( Qt::AlignCenter );
    QFont font = title->font();
    font.setPointSize( 72 );
    font.setWeight   ( QFont::Bold );
    title->setFont( font );
              
    QLabel* subTitle = new QLabel ( "Coriolis Graphical Interface" );
    subTitle->setAlignment( Qt::AlignCenter );
    subTitle->setFont( QFont( "Courier", 10, QFont::Bold ));
              
    QLabel* authors = new QLabel( about );
    authors->setAlignment( Qt::AlignCenter );
    authors->setFont( QFont( "Courier", 10, QFont::Bold ));
              
    QLabel* documentation = new QLabel( "<a href=\"http://coriolis.lip6.fr\">http://coriolis.lip6.fr</a>" );
    documentation->setAlignment( Qt::AlignCenter );
    documentation->setFont( QFont( "Courier", 10, QFont::Bold ));

    QPushButton* thanksButton = new QPushButton ();
  //thanksButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    thanksButton->setText      ( tr("Thanks for flying Coriolis") );

    QAction* closeAction = new QAction( tr("Close AboutWindow"), this );
    closeAction->setStatusTip( tr("CloseWindow") );
    closeAction->setShortcut ( QKeySequence(tr("CTRL+W")) );
              
    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->addStretch( 10 );
    vLayout->addWidget( topLine );
    vLayout->addWidget( logo );
    vLayout->addWidget( title );
    vLayout->addStretch( 1 );
    vLayout->addWidget( subTitle );
    vLayout->addWidget( authors );
    vLayout->addStretch( 10 );
    vLayout->addWidget( documentation );
    vLayout->addStretch( 1 );
    vLayout->addWidget( botLine );
    vLayout->addStretch( 10 );
              
    QFrame* frame = new QFrame ();
    frame->setFrameShape ( QFrame::Box );
    frame->setFrameShadow( QFrame::Sunken );
    frame->setLayout     ( vLayout );
    frame->setLineWidth  ( 1 );

    QHBoxLayout* hLayout = new QHBoxLayout ();
  //hLayout->addStretch( 1 );
    hLayout->addWidget ( thanksButton, Qt::AlignCenter );
  //hLayout->addStretch( 1 );
              
    vLayout = new QVBoxLayout ();
    vLayout->addWidget( frame );
    vLayout->addLayout( hLayout );
                
    setLayout( vLayout );

    connect( thanksButton, SIGNAL(clicked  ()), this, SLOT(accept()) );
    connect( closeAction , SIGNAL(triggered()), this, SLOT(accept()) );
  }


}  // End of Hurricane Namespace.
