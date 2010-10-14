
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ConfigurationWidget.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <string>
#include <vector>
#include <map>
#include <map>
#include <QApplication>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "vlsisapd/configuration/Configuration.h"
#include "vlsisapd/configuration/ParameterWidget.h"
#include "vlsisapd/configuration/ConfTabWidget.h"
#include "vlsisapd/configuration/LogWidget.h"
#include "vlsisapd/configuration/ConfigurationWidget.h"


namespace Cfg {

  using std::max;
  using std::cerr;
  using std::endl;
  using std::string;
  using std::set;
  using std::map;
  using std::pair;
  using std::make_pair;
  using std::vector;


// -------------------------------------------------------------------
// Class  :  "Cfg::ConfigurationWidget".


  ConfigurationWidget::ConfigurationWidget ( unsigned int flags, QWidget* parent )
    : QWidget   (parent)
    , _flags    (flags)
    , _boldFont (QApplication::font())
    , _tabWidget(new QTabWidget())
    , _apply    (new QPushButton())
    , _save     (NULL)
    , _cancel   (NULL)
    , _log      (NULL)
  {
    _boldFont.setBold ( true );

    setAttribute ( Qt::WA_QuitOnClose, false );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->addWidget ( _tabWidget );

    QHBoxLayout* hLayout = new QHBoxLayout ();
    hLayout->addStretch ();

    _apply->setText ( tr( (_flags&Embedded)?"Apply":"OK") );
    _apply->setStatusTip(tr("apply changes in memory"));
    hLayout->addWidget  ( _apply );
    hLayout->addStretch ();

    if ( _flags & StandAlone ) {
      _save = new QPushButton ();
      _save->setText ( tr("Save") );
      _save->setStatusTip(tr("save changes to file"));
      hLayout->addWidget  ( _save );
      hLayout->addStretch ();

      _cancel = new QPushButton ();
      _cancel->setText ( tr("Cancel") );
      _cancel->setStatusTip(tr("cancel changes"));
      hLayout->addWidget  ( _cancel );
      hLayout->addStretch ();
    }

    vLayout->addLayout ( hLayout );
    vLayout->addStretch ();

    setLayout ( vLayout );

    connect ( _apply, SIGNAL(clicked()), this, SLOT(applyClicked()) );
  }


  QFont& ConfigurationWidget::getBoldFont ()
  { return _boldFont; }


  void  ConfigurationWidget::addRuler ( const string& tabName )
  {
    ConfTabWidget* tab = findOrCreate ( tabName );
    tab->addRuler ();
  }


  void  ConfigurationWidget::addTitle (  const string& tabName, const string& title )
  {
    ConfTabWidget* tab = findOrCreate ( tabName );
    tab->addTitle ( title );
  }


  void  ConfigurationWidget::addSection (  const string& tabName, const string& section, int column )
  {
    ConfTabWidget* tab = findOrCreate ( tabName );
    tab->addSection ( section, column );
  }


  ParameterWidget* ConfigurationWidget::addParameter ( const string&      tabName
                                                     , Parameter*         parameter
                                                     , const std::string& label
                                                     , int                column
                                                     , int                span
                                                     , int                flags )
  {
    ParameterWidget* pw = find(parameter);
    if ( pw != NULL ) {
      cerr << "[ERROR] Parameter <" << parameter->getId() << "> already added." << endl;
      return pw;
    }

    ConfTabWidget* tab = findOrCreate ( tabName );
    return tab->addParameter ( parameter, label, column, span, flags );
  }


  ParameterWidget* ConfigurationWidget::find ( Parameter* parameter ) const
  {
    if ( parameter == NULL ) return NULL;
    return find ( parameter->getId() );
  }


  ParameterWidget* ConfigurationWidget::find ( const string& id ) const
  { return findChild<ParameterWidget*>(id.c_str()); }


  ConfTabWidget* ConfigurationWidget::findOrCreate ( const string& tabName )
  {
    ConfTabWidget* tab = findChild<ConfTabWidget*>(tabName.c_str());
    if ( tab != NULL ) return tab;

    tab = new ConfTabWidget ( tabName );
    _tabWidget->addTab ( tab, tabName.c_str() );

    connect ( this, SIGNAL(updateParameters()), tab, SIGNAL(updateParameters()) );
    if (_save)
      connect ( _save , SIGNAL(clicked()), tab, SIGNAL(updateParameters()) );

    return tab;
  }


  void  ConfigurationWidget::syncSlaves ()
  {
    const map<const string,Parameter*>& parameters = Configuration::get()->getParameters ();
    map<const string,Parameter*>::const_iterator iparam = parameters.begin();

    for ( ; iparam != parameters.end() ; ++iparam ) {
      if ( (*iparam).second->getSlaves().empty() ) continue;

      ParameterWidget* pw = find ( (*iparam).first );
      if ( pw == NULL ) continue;

      pw->enableSlaves ( pw->getParameter()->asBool() );
    }
  }


  void   ConfigurationWidget::applyClicked ()
  {
    emit updateParameters();

    Configuration* configuration = Configuration::get();
    if ( _log == NULL ) _log = new LogWidget(this);

    if ( configuration->hasLogs(Configuration::LogNeedExist) ) {
      _log->updateLogs(Configuration::LogNeedExist);
      _log->exec();
      return;
    }

    if ( configuration->hasLogs(Configuration::LogRestart) ) {
      _log->updateLogs(Configuration::LogRestart);
      if ( _log->exec() == QDialog::Accepted ) {
        emit needRestart();
      } else {
        configuration->restoreFromLogs(Configuration::LogRestart);
        configuration->clearLogs      (Configuration::LogRestart);
        return;
      }
    }

    emit confOk();
  }


  void  ConfigurationWidget::selectTab ( const std::string& tabName )
  {
    QString qtabName ( tabName.c_str() );
    for ( int itab=0 ; itab<_tabWidget->count() ; ++itab ) {
      if ( _tabWidget->tabText(itab) == qtabName ) {
        _tabWidget->setCurrentIndex ( itab );
        return;
      }
    }
  }
  

  string  toXml ( const string& source )
  {
    static vector< pair<string,string> > translations;

    if ( translations.empty() ) {
      translations.push_back ( make_pair("&","&amp;") );
      translations.push_back ( make_pair("<","&lt;") );
      translations.push_back ( make_pair(">","&gt;") );
    }

    string translated = source;

    for ( size_t pos=0 ; pos<translated.size() ; ) {
      bool match = false;

      for ( size_t i=0 ; i<translations.size() ; ++i ) {
        const string& original    = translations[i].first;
        const string& translation = translations[i].second;

        if ( translated.compare(pos,original.size(),original) == 0 ) {
          translated.replace(pos,original.size(),translation);
          pos += translation.size();
          match = true;
          break;
        }
      }

      pos += (match) ? 0 : 1;
    }

    return translated;
  }


}  // End of Cfg namespace.
