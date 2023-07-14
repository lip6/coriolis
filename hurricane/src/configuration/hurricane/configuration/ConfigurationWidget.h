// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) SU 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header : "./hurricane/configuration/ConfigurationWidget.h" |
// +-----------------------------------------------------------------+


#pragma  once
#include <QFont>
#include <QWidget>
#include <QVector>
class QPushButton;
class QTabWidget;


namespace Cfg {

  class Configuration;
  class Parameter;
  class ParameterWidget;
  class ConfTabWidget;
  class LogWidget;


// -------------------------------------------------------------------
// Class  :  "Cfg::ConfigurationWidget".

  class ConfigurationWidget : public QWidget {
      Q_OBJECT;
    public:
      enum Flags { Embedded       = 0x1
                 , StandAlone     = 0x2
                 , AllTabs        = 0x1
                 , ShownTabs      = 0x2
                 , ExactSet       = 0x4
                 , IncrementalSet = 0x8
                 };
    public:
                              ConfigurationWidget ( unsigned int flags, QWidget* parent=NULL );
    public:            
             QFont&           getBoldFont         ();
      inline QPushButton*     getApplyButton      ();
      inline QPushButton*     getSaveButton       ();
      inline QPushButton*     getCancelButton     ();
             ParameterWidget* find                ( Parameter* ) const;
             ParameterWidget* find                ( const std::string& id ) const;
             ConfTabWidget*   findTab             ( const std::string& name, int mode );
             ConfTabWidget*   findOrCreateTab     ( const std::string& name );
             void             addRuler            ( const std::string& tabName );
             void             addTitle            ( const std::string& tabName
                                                  , const std::string& title );
             void             addSection          ( const std::string& tabName
                                                  , const std::string& section
                                                  , int                column=0 );
             ParameterWidget* addParameter        ( const std::string& tabName
                                                  , Parameter*
                                                  , const std::string& label
                                                  , int                column=0
                                                  , int                span  =1
                                                  , int                flags =0 );
             void             syncSlaves          ();
             void             selectTab           ( const std::string& );
             void             showTabs            ( const std::string&, int mode=IncrementalSet );
             void             hideTabs            ( const std::string&, int mode=IncrementalSet );
    public slots:
             void             applyClicked        ();
    signals:
             void             updateParameters    ();
             void             confOk              ();
             void             needRestart         ();
    private:
      unsigned int             _flags;
      QFont                    _boldFont;
      QTabWidget*              _tabWidget;
      QPushButton*             _apply;
      QPushButton*             _save;
      QPushButton*             _cancel;
      QVector<ConfTabWidget*>  _tabWidgets;
      LogWidget*               _log;
  };


  inline QPushButton* ConfigurationWidget::getApplyButton  () { return _apply; }
  inline QPushButton* ConfigurationWidget::getSaveButton   () { return _save; }
  inline QPushButton* ConfigurationWidget::getCancelButton () { return _cancel; }


// Functions Templates.
  template<typename QTypeWidget>
  QTypeWidget  rparent ( QObject* object )
  {
    QTypeWidget root = NULL;

    while ( object != NULL ) {
      object = object->parent();
      root   = qobject_cast<QTypeWidget>(object);
      if ( root != NULL )
        return root;
    }
    return NULL;
  }


// Misc. Utility.
  std::string  toXml ( const std::string& );


} // Cfg namespace.
