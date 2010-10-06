
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LogWidget.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <set>
#include  <string>
#include  <QCloseEvent>
#include  <QLabel>
#include  <QPushButton>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QFrame>

#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/LogWidget.h"


namespace Cfg {

  using std::set;
  using std::string;
  using std::cerr;
  using std::endl;


  LogWidget::LogWidget ( QWidget* parent )
    : QDialog (parent)
    , _restartMessage  (new QLabel())
    , _needExistMessage(new QLabel())
  {
    setModal      ( true );
    setWindowTitle( tr("<Configuration Message>") );
    setToolTip    ( tr("You should better follow these instructions...") );

    _restartMessage->setTextFormat ( Qt::RichText );
    _restartMessage->setText       ( "<b>Oups! I did it again!</b>" );

    QLabel* ok = new QLabel ();
    ok->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    ok->setPixmap     ( QPixmap(":/images/warning.png") );
    ok->setStyleSheet ( "QLabel { background-color: #FF9999;"
                        "         padding:          5px }" );

    QPushButton* okButton = new QPushButton ();
    okButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    okButton->setText       ( tr("Continue") );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addStretch ( 1 );
    hLayout2->addWidget  ( okButton, Qt::AlignCenter );
    hLayout2->addStretch ( 1 );

    QVBoxLayout* vLayout1 = new QVBoxLayout ();
    vLayout1->setContentsMargins ( 10, 10, 10, 10 );
    vLayout1->setSpacing ( 0 );
    vLayout1->addWidget  ( _restartMessage  , Qt::AlignCenter );
    vLayout1->addWidget  ( _needExistMessage, Qt::AlignCenter );
    vLayout1->addSpacing ( 10 );
    vLayout1->addLayout  ( hLayout2, Qt::AlignCenter );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->setSizeConstraint  ( QLayout::SetFixedSize );
    hLayout1->setContentsMargins ( 0, 0, 0, 0 );
    hLayout1->addWidget          ( ok );
    hLayout1->addLayout          ( vLayout1 );

    setLayout ( hLayout1 );

    connect ( okButton, SIGNAL(clicked()), this, SLOT(accept()) );
  }


  void  LogWidget::closeEvent ( QCloseEvent* event )
  {
    event->ignore ();
  }


  void  LogWidget::updateLogs ()
  {
    Configuration* configuration = Configuration::get();

    for ( size_t ilog=0 ; ilog<Configuration::LogTypeSize ; ++ilog ) {
      const set<string>& logs          = configuration->getLogs(ilog);
      QLabel*            messageLabel  = NULL;
      QString            contents;
      const char*        header        = NULL;

      switch ( ilog ) {
        case Configuration::LogRestart:
          messageLabel = _restartMessage;
          header       = "<b>Program needs restart for these parameters to take effect:</b><br>";
          break;
        case Configuration::LogNeedExist:
          messageLabel = _needExistMessage;
          header       = "<b>Those parameters needs file/path to exists:</b><br>";
          break;
      }
      if ( messageLabel == NULL ) continue;

      if ( not logs.empty() ) {
        contents += header;

        set<string>::const_iterator iid = logs.begin();
        for ( ; iid != logs.end() ; ++iid ) {
          Parameter* p = configuration->getParameter((*iid));
          if ( p != NULL ) {
            WidgetDescription* desc = configuration->getLayout().getWidget ( p->getId() );

            contents += "&nbsp;&nbsp;&nbsp;";
            if ( desc != NULL ) {
              contents += desc->getLabel().c_str();
              contents += "&nbsp;&nbsp;";
            }
            contents += "<tt>&lt;";
            contents += p->getId().c_str();
            contents += "&gt;</tt>: ";
            if ( p->asString().size() > 30 ) contents += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
            contents += p->asString().c_str();
            contents += "<br>";
          }
        }
      }

      messageLabel->setText ( contents );
    }
  }


}  // End of Hurricane Namespace.
