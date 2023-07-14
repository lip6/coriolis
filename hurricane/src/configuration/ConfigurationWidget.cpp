// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ConfigurationWidget.cpp"                |
// +-----------------------------------------------------------------+


#include <string>
#include <vector>
#include <map>
#include <map>
#include <QApplication>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "hurricane/configuration/Configuration.h"
#include "hurricane/configuration/ParameterWidget.h"
#include "hurricane/configuration/ConfTabWidget.h"
#include "hurricane/configuration/LogWidget.h"
#include "hurricane/configuration/ConfigurationWidget.h"


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
    : QWidget    (parent)
    , _flags     (flags)
    , _boldFont  (QApplication::font())
    , _tabWidget (new QTabWidget())
    , _apply     (new QPushButton())
    , _save      (NULL)
    , _cancel    (NULL)
    , _tabWidgets()
    , _log       (NULL)
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
    ConfTabWidget* tab = findOrCreateTab ( tabName );
    tab->addRuler ();
  }


  void  ConfigurationWidget::addTitle (  const string& tabName, const string& title )
  {
    ConfTabWidget* tab = findOrCreateTab ( tabName );
    tab->addTitle ( title );
  }


  void  ConfigurationWidget::addSection (  const string& tabName, const string& section, int column )
  {
    ConfTabWidget* tab = findOrCreateTab ( tabName );
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
      cerr << "[ERROR] ParameterWidget for <" << parameter->getId() << "> already added." << endl;
      return pw;
    }

    ConfTabWidget* tab = findOrCreateTab ( tabName );
    return tab->addParameter ( parameter, label, column, span, flags );
  }


  ParameterWidget* ConfigurationWidget::find ( Parameter* parameter ) const
  {
    if ( parameter == NULL ) return NULL;
    return find ( parameter->getId() );
  }


  ParameterWidget* ConfigurationWidget::find ( const string& id ) const
  { return findChild<ParameterWidget*>(id.c_str()); }


  ConfTabWidget* ConfigurationWidget::findOrCreateTab ( const string& tabName )
  {
  //ConfTabWidget* tab = findChild<ConfTabWidget*>(tabName.c_str());
    ConfTabWidget* tab = findTab(tabName,AllTabs);

    if ( tab != NULL ) return tab;

    tab = new ConfTabWidget ( this, tabName );
    _tabWidget->addTab ( tab, tabName.c_str() );
    _tabWidgets.push_back ( tab );

    connect ( this, SIGNAL(updateParameters()), tab, SIGNAL(updateParameters()) );
    if (_save)
      connect ( _save , SIGNAL(clicked()), tab, SIGNAL(updateParameters()) );

    return tab;
  }


  void  ConfigurationWidget::syncSlaves ()
  {
    const map<string,Parameter*>& parameters = Configuration::get()->getParameters ();
    map<string,Parameter*>::const_iterator iparam = parameters.begin();

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


  ConfTabWidget* ConfigurationWidget::findTab ( const std::string& tabName, int mode )
  {
    QString qtabName ( tabName.c_str() );

    if ( mode & ShownTabs ) {
      for ( int itab=0 ; itab<_tabWidgets.size() ; ++itab ) {
        if ( _tabWidgets[itab]->objectName() == qtabName ) return _tabWidgets[itab];
      }
    } else {
    // AllTabs.
      for ( int itab=0 ; itab<_tabWidget->count() ; ++itab ) {
        if ( _tabWidget->tabText(itab) == qtabName ) {
          return qobject_cast<ConfTabWidget*>(_tabWidget->widget(itab));
        }
    }
    }

    return NULL;
  }


  void  ConfigurationWidget::selectTab ( const std::string& tabName )
  {
    ConfTabWidget* tab = findTab ( tabName, ShownTabs );
    if ( tab ) _tabWidget->setCurrentWidget ( tab );
  }


  void  ConfigurationWidget::showTabs ( const std::string& tabNames, int mode )
  {
    _tabWidget->setUpdatesEnabled ( false );

    QString     qtabNames ( tabNames.c_str() );
    QStringList qtabList  = qtabNames.split ( ";" );

    if ( mode & ExactSet ) _tabWidget->clear ();

    int insertIndex = 0;
    for ( int itab=0 ; itab<_tabWidgets.size() ; ++itab ) {
      ConfTabWidget* tab      = _tabWidgets[itab];
      int            tabIndex = _tabWidget->indexOf ( tab );

      if ( (tabIndex < 0) and qtabList.contains(tab->objectName()) ) {
        tabIndex = _tabWidget->insertTab ( insertIndex, tab, tab->objectName() );
      }
      if ( tabIndex >= 0 ) insertIndex = tabIndex+1;
    }

    _tabWidget->setUpdatesEnabled ( true );
  }


  void  ConfigurationWidget::hideTabs ( const std::string& tabNames, int mode )
  {
    _tabWidget->setUpdatesEnabled ( false );

    QString     qtabNames ( tabNames.c_str() );
    QStringList qtabList  = qtabNames.split ( ";" );

    if ( mode & ExactSet ) _tabWidget->clear ();

    for ( int itab=0 ; itab<_tabWidgets.size() ; ++itab ) {
      ConfTabWidget* tab      = _tabWidgets[itab];
      int            tabIndex = _tabWidget->indexOf ( tab );

      if ( (tabIndex >= 0) and qtabList.contains(tab->objectName()) ) 
        _tabWidget->removeTab ( tabIndex );

      if ( (mode & ExactSet) and not qtabList.contains(tab->objectName()) ) 
        _tabWidget->addTab ( tab, tab->objectName() );
    }

    _tabWidget->setUpdatesEnabled ( true );
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


}  // Cfg namespace.
