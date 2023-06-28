// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/ControllerWidget.h"    |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_CONTROLLER_WIDGET_H
#define  HURRICANE_CONTROLLER_WIDGET_H


#include <QTabWidget>
class QCheckBox;
class QComboBox;

namespace Cfg {
  class ConfigurationWidget;
}

#include "hurricane/Observer.h"
#include "hurricane/Occurrence.h"


namespace Hurricane {

  using Cfg::ConfigurationWidget;

  class Record;
  class Net;
  class Cell;
  class CellWidget;
  class PaletteWidget;
  class GraphicsWidget;
  class DisplayFilterWidget;
  class NetlistWidget;
  class HierarchyWidget;
  class SelectionWidget;
  class InspectorWidget;


// -------------------------------------------------------------------
// Class  :  "Hurricane::ControllerTab".


  class ControllerTab : public QWidget {
      Q_OBJECT;

    public:
                         ControllerTab      ( QWidget* parent=NULL );
      inline CellWidget* getCellWidget      ();
    public slots:           
      virtual void       setCellWidget      ( CellWidget* );
      virtual void       setCell            ( Cell* );
      virtual void       updateTab          ();
      virtual void       cellPreModificate  ();
      virtual void       cellPostModificate ();
      virtual void       graphicsUpdated    ();

    protected:
      CellWidget*    _cellWidget;
  };


  inline CellWidget* ControllerTab::getCellWidget () { return _cellWidget; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabGraphics".


  class TabGraphics : public ControllerTab {
      Q_OBJECT;

    public:
                              TabGraphics     ( QWidget* parent=NULL );
      inline  GraphicsWidget* getGraphics     ();
    public slots:
              void            setCellWidget   ( CellWidget* );
      virtual void            graphicsUpdated ();

    protected:
      GraphicsWidget* _graphics;
  };


  inline GraphicsWidget* TabGraphics::getGraphics () { return _graphics; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabDisplayFilter".


  class TabDisplayFilter : public ControllerTab {
      Q_OBJECT;

    public:
                                   TabDisplayFilter ( QWidget* parent=NULL );
      inline  DisplayFilterWidget* getDisplayFilter ();
    public slots:
      virtual void                 setCellWidget    ( CellWidget* );

    protected:
      DisplayFilterWidget* _displayFilter;
  };


  inline DisplayFilterWidget* TabDisplayFilter::getDisplayFilter () { return _displayFilter; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabPalette".


  class TabPalette : public ControllerTab {
      Q_OBJECT;

    public:
                             TabPalette      ( QWidget* parent=NULL );
      inline  PaletteWidget* getPalette      ();
    public slots:
      virtual void           setCellWidget   ( CellWidget* );
      virtual void           graphicsUpdated ();

    protected:
      PaletteWidget* _palette;
  };


  inline PaletteWidget* TabPalette::getPalette () { return _palette; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabNetlist".


  class TabNetlist : public ControllerTab {
      Q_OBJECT;

    public:
                             TabNetlist         ( QWidget* parent=NULL );
      inline  NetlistWidget* getNetlistBrowser  ();
      inline  QCheckBox*     getSyncNetlist     ();
      inline  QCheckBox*     getSyncSelection   ();
      virtual void           cellPreModificate  ();
      virtual void           cellPostModificate ();
    public slots:           
      virtual void           setCell            ( Cell* );
      virtual void           setCellWidget      ( CellWidget* );
      virtual void           setSyncNetlist     ( bool );
      virtual void           setSyncSelection   ( bool );

    protected:
      NetlistWidget* _netlistBrowser;
      QCheckBox*     _syncNetlist;
      QCheckBox*     _syncSelection;
      bool           _cwCumulativeSelection;
  };


  inline NetlistWidget* TabNetlist::getNetlistBrowser () { return _netlistBrowser; }
  inline QCheckBox*     TabNetlist::getSyncNetlist    () { return _syncNetlist; }
  inline QCheckBox*     TabNetlist::getSyncSelection  () { return _syncSelection; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabHierarchy".


  class TabHierarchy : public ControllerTab {
      Q_OBJECT;

    public:
                               TabHierarchy        ( QWidget* parent=NULL );
      inline  HierarchyWidget* getHierarchyBrowser ();
      virtual void             cellPreModificate   ();
      virtual void             cellPostModificate  ();
    public slots:   
      virtual void             setCell             ( Cell* );
      virtual void             setCellWidget       ( CellWidget* );

    protected:
      HierarchyWidget* _hierarchyBrowser;
  };


  inline HierarchyWidget* TabHierarchy::getHierarchyBrowser () { return _hierarchyBrowser; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabSelection".


  class TabSelection : public ControllerTab {
      Q_OBJECT;

    public:
                                TabSelection         ( QWidget* parent=NULL );
      inline  SelectionWidget*  getSelection         ();
      virtual void              updateTab            ();
      virtual void              cellPreModificate    ();
      virtual void              cellPostModificate   ();
    public slots:
      virtual void              setCellWidget        ( CellWidget* );

    protected:
      SelectionWidget* _selection;
      bool             _selectionChanged;
  };


  inline SelectionWidget* TabSelection::getSelection  () { return _selection; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabInspector".


  class TabInspector : public ControllerTab {
      Q_OBJECT;

    public:
                                TabInspector           ( QWidget* parent=NULL );
      inline  InspectorWidget*  getInspectorWidget     ();
      inline  QComboBox*        getBookmarks           ();
      virtual void              updateTab              ();
      virtual void              cellPreModificate      ();
      virtual void              cellPostModificate     ();
    public slots:                                      
      virtual void              setCell                ( Cell* );
      virtual void              setCellWidget          ( CellWidget* );
      virtual void              setSelectionOccurrence ( Occurrence& );
      virtual void              bookmarkChanged        ( int index );

    protected:
      InspectorWidget*  _inspectorWidget;
      QComboBox*        _bookmarks;
      Occurrence        _selectionOccurrence;
      bool              _updateFromSelection;
  };


  inline InspectorWidget*  TabInspector::getInspectorWidget () { return _inspectorWidget; }
  inline QComboBox*        TabInspector::getBookmarks       () { return _bookmarks; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabSettings".


  class TabSettings : public ControllerTab {
      Q_OBJECT;
    public:
                                   TabSettings     ( QWidget* parent=NULL );
      inline  ConfigurationWidget* getSettings     ();
    public slots:
              void                 setCellWidget   ( CellWidget* );
    protected:
      virtual void                 showEvent       ( QShowEvent* );
    private:
      size_t               _timestamp;
      ConfigurationWidget* _configuration;
  };


  // class TabSettings : public ControllerTab {
  //     Q_OBJECT;

  //   public:
  //                        TabSettings   ( QWidget* parent=NULL );
  //     inline QTabWidget* getSettings   ();
  //     inline int         addSetting    ( QWidget* page, const QString& label );
  //   public slots:
  //            void        setCellWidget ( CellWidget* );

  //   protected:
  //     QTabWidget* _settings;
  // };


  // inline QTabWidget* TabSettings::getSettings () { return _settings; }
  // inline int         TabSettings::addSetting  ( QWidget* page, const QString& label ) { return _settings->addTab(page,label); }


// -------------------------------------------------------------------
// Class  :  "Hurricane::ControllerWidget".


  class ControllerWidget : public QTabWidget {
      Q_OBJECT;
      
    public:
      static void                 notify              ( ControllerWidget*, unsigned int flags );
    public:
                                  ControllerWidget    ( QWidget* parent=NULL );
      inline CellWidget*          getCellWidget       ();
      inline GraphicsWidget*      getGraphics         ();
      inline PaletteWidget*       getPalette          ();
      inline DisplayFilterWidget* getDisplayFilter    ();
      inline NetlistWidget*       getNetlistBrowser   ();
      inline HierarchyWidget*     getHierarchyBrowser ();
      inline SelectionWidget*     getSelection        ();
      inline InspectorWidget*     getInspectorWidget  ();
      inline TabSettings*         getSettings         ();
             void                 setCellWidget       ( CellWidget* );
    //inline int                  addSetting          ( QWidget* page, const QString& label );
             void                 insertTabAfter      ( const QString& ref, QWidget*, const QString& label );
    public slots:                                     
             void                 graphicsUpdated     ();
             void                 cellPreModificate   ();
             void                 cellPostModificate  ();
             void                 cellChanged         ( Cell* );
             void                 updateTab           ( int index );
             void                 toggleShow          ();

    protected:
      Observer<ControllerWidget>  _observer;
      CellWidget*                 _cellWidget;
      TabGraphics*                _tabGraphics;
      TabPalette*                 _tabPalette;
      TabDisplayFilter*           _tabDisplayFilter;
      TabNetlist*                 _tabNetlist;
      TabHierarchy*               _tabHierarchy;
      TabSelection*               _tabSelection;
      TabInspector*               _tabInspector;
      TabSettings*                _tabSettings;
  };


  inline CellWidget*          ControllerWidget::getCellWidget       () { return _cellWidget; }
  inline GraphicsWidget*      ControllerWidget::getGraphics         () { return _tabGraphics->getGraphics(); }
  inline PaletteWidget*       ControllerWidget::getPalette          () { return _tabPalette->getPalette(); }
  inline DisplayFilterWidget* ControllerWidget::getDisplayFilter    () { return _tabDisplayFilter->getDisplayFilter(); }
  inline NetlistWidget*       ControllerWidget::getNetlistBrowser   () { return _tabNetlist->getNetlistBrowser(); }
  inline HierarchyWidget*     ControllerWidget::getHierarchyBrowser () { return _tabHierarchy->getHierarchyBrowser(); }
  inline SelectionWidget*     ControllerWidget::getSelection        () { return _tabSelection->getSelection(); }
  inline InspectorWidget*     ControllerWidget::getInspectorWidget  () { return _tabInspector->getInspectorWidget(); }
  inline TabSettings*         ControllerWidget::getSettings         () { return _tabSettings; }
//inline int                  ControllerWidget::addSetting          ( QWidget* page, const QString& label ) { return _tabSettings->addSetting(page,label); }


}  // End of Hurricane namespace.


#endif  // __HURRICANE_CONTROLLER_WIDGET__
