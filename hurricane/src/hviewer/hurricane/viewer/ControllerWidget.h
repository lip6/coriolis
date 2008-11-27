
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
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ControllerWidget.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_CONTROLLER_WIDGET__
#define  __HURRICANE_CONTROLLER_WIDGET__


#include  <QTabWidget>
class QCheckBox;
class QComboBox;


namespace Hurricane {


  class Record;
  class Net;
  class Cell;
  class CellWidget;
  class PaletteWidget;
  class GraphicsWidget;
  class DisplayFilterWidget;
  class NetlistWidget;
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

    protected:
      CellWidget*    _cellWidget;
  };


  inline CellWidget* ControllerTab::getCellWidget () { return _cellWidget; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabNetlist".


  class TabNetlist : public ControllerTab {
      Q_OBJECT;

    public:
                             TabNetlist         ( QWidget* parent=NULL );
      inline  NetlistWidget* getNetlistBrowser  ();
      inline  QCheckBox*     getSyncButton      ();
      virtual void           cellPreModificate  ();
      virtual void           cellPostModificate ();
    public slots:           
      virtual void           setCell            ( Cell* );
      virtual void           setCellWidget      ( CellWidget* );
      virtual void           setSynchronize     ( bool );

    protected:
      NetlistWidget* _netlistBrowser;
      QCheckBox*     _synchronize;
  };


  inline NetlistWidget* TabNetlist::getNetlistBrowser () { return _netlistBrowser; }
  inline QCheckBox*     TabNetlist::getSyncButton     () { return _synchronize; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabSelection".


  class TabSelection : public QWidget {
      Q_OBJECT;

    public:
                               TabSelection         ( QWidget* parent=NULL );
      inline CellWidget*       getCellWidget        ();
      inline SelectionWidget*  getSelection         ();
             void              updateTab            ();
             void              cellPreModificate    ();
             void              cellPostModificate   ();
    public slots:
             void              setCell              ( Cell* );
             void              setCellWidget        ( CellWidget* );

    protected:
      CellWidget*      _cellWidget;
      SelectionWidget* _selection;
      bool             _selectionChanged;
  };


  inline CellWidget*      TabSelection::getCellWidget () { return _cellWidget; }
  inline SelectionWidget* TabSelection::getSelection  () { return _selection; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabInspector".


  class TabInspector : public QWidget {
      Q_OBJECT;

    public:
                               TabInspector       ( QWidget* parent=NULL );
      inline CellWidget*       getCellWidget      ();
      inline InspectorWidget*  getInspectorWidget ();
      inline QComboBox*        getBookmarks       ();
             void              updateTab          ();
             void              cellPreModificate  ();
             void              cellPostModificate ();
    public slots:
             void              setCell            ( Cell* );
             void              setCellWidget      ( CellWidget* );
             void              setSelectionRecord ( Record* );
             void              bookmarkChanged    ( int index );

    protected:
      CellWidget*       _cellWidget;
      InspectorWidget*  _inspectorWidget;
      QComboBox*        _bookmarks;
      Record*           _selectionRecord;
      bool              _updateFromSelection;
  };


  inline CellWidget*       TabInspector::getCellWidget      () { return _cellWidget; }
  inline InspectorWidget*  TabInspector::getInspectorWidget () { return _inspectorWidget; }
  inline QComboBox*        TabInspector::getBookmarks       () { return _bookmarks; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::ControllerWidget".


  class ControllerWidget : public QTabWidget {
      Q_OBJECT;
      
    public:
                                  ControllerWidget   ( QWidget* parent=NULL );
      inline CellWidget*          getCellWidget      ();
      inline GraphicsWidget*      getGraphics        ();
      inline PaletteWidget*       getPalette         ();
      inline DisplayFilterWidget* getDisplayFilter   ();
      inline NetlistWidget*       getNetlistBrowser  ();
      inline SelectionWidget*     getSelection       ();
      inline InspectorWidget*     getInspectorWidget ();
             void                 setCellWidget      ( CellWidget* );
    public slots:                 
             void                 cellPreModificate  ();
             void                 cellPostModificate ();
             void                 cellChanged        ( Cell* );
             void                 updateTab          ( int index );

    protected:
      CellWidget*          _cellWidget;
      GraphicsWidget*      _graphics;
      PaletteWidget*       _palette;
      DisplayFilterWidget* _displayFilter;
      TabNetlist*          _tabNetlist;
      TabSelection*        _tabSelection;
      TabInspector*        _tabInspector;
  };


  inline CellWidget*          ControllerWidget::getCellWidget      () { return _cellWidget; }
  inline GraphicsWidget*      ControllerWidget::getGraphics        () { return _graphics; }
  inline PaletteWidget*       ControllerWidget::getPalette         () { return _palette; }
  inline DisplayFilterWidget* ControllerWidget::getDisplayFilter   () { return _displayFilter; }
  inline NetlistWidget*       ControllerWidget::getNetlistBrowser  () { return _tabNetlist->getNetlistBrowser(); }
  inline SelectionWidget*     ControllerWidget::getSelection       () { return _tabSelection->getSelection(); }
  inline InspectorWidget*     ControllerWidget::getInspectorWidget () { return _tabInspector->getInspectorWidget(); }


}  // End of Hurricane namespace.


#endif  // __HURRICANE_CONTROLLER_WIDGET__
