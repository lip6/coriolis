// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SelectCommand.cpp"                      |
// +-----------------------------------------------------------------+


#include <QString>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>
#include "hurricane/Path.h"
#include "hurricane/Entity.h"
#include "hurricane/Net.h"
#include "hurricane/Component.h"
#include "hurricane/Occurrence.h"
#include "hurricane/HyperNet.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/SelectCommand.h"
#include "hurricane/viewer/SelectionPopup.h"


namespace Hurricane {


  typedef Locator<Occurrence>    OccurrenceLoc;
  typedef Collection<Occurrence> OccurrenceCol;


// -------------------------------------------------------------------
// Class  :  "Hurricane::Occurrence_GetNets".


  class Occurrences_GetNets : public OccurrenceCol {

    public:
    // Sub-Class: Locator.
      class Locator : public OccurrenceLoc {
        public:
                                 Locator    ( Occurrences, bool hideAnon );
          inline                 Locator    ( const Locator& );
          inline                ~Locator    ();
          virtual Occurrence     getElement () const;
          virtual OccurrenceLoc* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          OccurrenceLoc*   _primaryLoc;
          Occurrence       _element;
          set<Occurrence>  _netOccurrences;
          bool             _hideAnonymous;
      };

    public:
    // Occurrences_GetNets Methods.
      inline                 Occurrences_GetNets ( Occurrences, bool hideAnon );
      inline                 Occurrences_GetNets ( const Occurrences_GetNets& );
      virtual OccurrenceCol* getClone            () const;
	  virtual OccurrenceLoc* getLocator          () const;
      virtual string         _getString          () const;

    protected:
    // Occurrences_GetNets Attributes.
      Occurrences  _primaryCol;
      bool         _hideAnonymous;
  };

  
  inline Occurrences_GetNets::Locator::Locator ( const Locator &locator )
    : OccurrenceLoc  ()
    , _primaryLoc    (locator._primaryLoc->getClone())
    , _element       ()
    , _netOccurrences()
    , _hideAnonymous (locator._hideAnonymous)
  { }


  inline Occurrences_GetNets::Locator::~Locator ()
  { delete _primaryLoc; }


  inline Occurrences_GetNets::Occurrences_GetNets ( Occurrences primaryCol, bool hideAnon )
    : OccurrenceCol ()
    , _primaryCol   (primaryCol)
    , _hideAnonymous(hideAnon)
  { }


  inline Occurrences_GetNets::Occurrences_GetNets ( const Occurrences_GetNets& occurrences )
    : OccurrenceCol ()
    , _primaryCol   (occurrences._primaryCol)
    , _hideAnonymous(occurrences._hideAnonymous)
  { }


  Occurrences_GetNets::Locator::Locator ( Occurrences primaryCol, bool hideAnon )
    : OccurrenceLoc  ()
    , _primaryLoc    (primaryCol.getLocator())
    , _element       ()
    , _netOccurrences()
    , _hideAnonymous (hideAnon)
  { progress (); }


  OccurrenceLoc* Occurrences_GetNets::Locator::getClone () const
  { return new Locator(*this); }


  Occurrence  Occurrences_GetNets::Locator::getElement () const
  { return _element; }


  bool  Occurrences_GetNets::Locator::isValid () const
  { return _primaryLoc->isValid(); }


  void  Occurrences_GetNets::Locator::progress ()
  {
    cdebug_log(18,0) << "Occurrences_GetNets::Locator::progress()" << endl;

    for ( ; _primaryLoc->isValid() ; _primaryLoc->progress() ) {
      Occurrence element = _primaryLoc->getElement();

      Component* component = dynamic_cast<Component*>( element.getEntity() );
      if (not component) continue;
  
      Net*       net = component->getNet();
      Occurrence netOccurrence ( net, element.getPath() );

      if (_hideAnonymous and QString(getString(net->getName()).c_str()).contains("onymous") )
        continue;

      _element = getHyperNetRootNetOccurrence( netOccurrence );
      if (_netOccurrences.find(_element) != _netOccurrences.end()) continue;
      _netOccurrences.insert( _element );

      break;
    }
  }


  string  Occurrences_GetNets::Locator::_getString () const
  {
    string s = "<" + _TName("Occurrences_GetNets::Locator") + ">";
    return s;
  }


  OccurrenceCol* Occurrences_GetNets::getClone () const
  { return new Occurrences_GetNets(*this); }


  OccurrenceLoc* Occurrences_GetNets::getLocator () const
  { return new Locator(_primaryCol,_hideAnonymous); }


  string  Occurrences_GetNets::_getString () const
  {
    string s = "<" + _TName("Occurrences_GetNets") + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "SelectCommand".


  string SelectCommand::_name = "SelectCommand";


  const string& SelectCommand::getStaticName ()
  { return _name; }


  SelectCommand::SelectCommand ()
    : AreaCommand    ()
    , _selectionPopup(NULL)
    , _selectMode    (0)
  {
    _selectionPopup = new SelectionPopup ();

    connect ( _selectionPopup, SIGNAL(selectionToggled(Occurrence))
            , this           , SIGNAL(selectionToggled(Occurrence)) );
  }


  SelectCommand::~SelectCommand ()
  { delete _selectionPopup; }


  const string& SelectCommand::getName () const
  { return _name; }


  void  SelectCommand::keyPressEvent ( QKeyEvent* event )
  {
    if ( event->key() == Qt::Key_N ) {
      event->accept();
      _selectMode++;
      _selectMode = _selectMode % 3;
    }
  }


  void  SelectCommand::mousePressEvent (  QMouseEvent* event )
  {
    if (isActive()) return;

    if (event->button() == Qt::RightButton) {
      if (not event->modifiers() ) {
        event->accept();
        setActive        ( true );
        setStartPoint    ( event->pos() );
        setDrawingEnabled( true );
      } else if (event->modifiers() == Qt::ControlModifier) {
        event->accept();

        QRect selectArea ( event->pos() - QPoint(2,2), QSize(4,4) );
        Occurrences  selection;
        switch ( _selectMode ) {
          case AllMode: // 0
            selection = _cellWidget->getOccurrencesUnder( selectArea );
            break;
          case NetMode: // 1
            selection = Occurrences_GetNets( _cellWidget->getOccurrencesUnder(selectArea), false );
            break;
          case NoAnonNetMode: // 2
            selection = Occurrences_GetNets( _cellWidget->getOccurrencesUnder(selectArea), true );
            break;
        }
        _selectionPopup->loadOccurrences( selection );
        _selectionPopup->updateLayout();
        _selectionPopup->move( event->globalPos() );
        _selectionPopup->showPopup();
      }
    }
  }


  void  SelectCommand::mouseReleaseEvent ( QMouseEvent* event )
  {
    if ( !isActive() ) return;
  //_startPoint = _stopPoint = event->pos();

    event->accept ();

    setActive( false );
    setDrawingEnabled( false );

    QRect selectArea;
    if ( _startPoint == _stopPoint )
      selectArea = QRect( _startPoint - QPoint(2,2), QSize(4,4) );
    else
      selectArea = QRect( _startPoint, _stopPoint );
      
  //_cellWidget->unselectAll ();
    _cellWidget->selectOccurrencesUnder( _cellWidget->screenToDbuBox(selectArea) );
    setShowSelection( not _cellWidget->getSelectorSet().empty() );
  }


  void  SelectCommand::setCellWidget ( CellWidget* cellWidget )
  {
    Super::setCellWidget( cellWidget );
    _selectionPopup->setCellWidget( cellWidget );
  }

  
  void  SelectCommand::setShowSelection ( bool state )
  {
  //cerr << "SelectCommand::setShowSelection(): "
  //     << state << " vs. " << _cellWidget->showSelection() << endl;
    if (_cellWidget->showSelection() != state)
      _cellWidget->setShowSelection( state );
    else
      _cellWidget->refresh();
  }


} // End of Hurricane namespace.
