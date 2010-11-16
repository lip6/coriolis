
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoContact.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdlib>
#include  <climits>
#include  <sstream>

#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Layer.h"
#include  "hurricane/ViaLayer.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/Contact.h"
#include  "hurricane/Plug.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/UpdateSession.h"
#include  "hurricane/DebugSession.h"

#include  "crlcore/RoutingGauge.h"

#include  "katabatic/GCell.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/AutoVertical.h"
#include  "katabatic/AutoHorizontal.h"
#include  "katabatic/Session.h"


namespace {

  /*! \class        SegmentEnd
   *  \brief        Segment manipulator (\b internal)
   *
   *                SegmentPosition compute detailed informations about how an
   *                Segment is connected to the current AutoContact. This
   *                object act as a cache, avoiding to recalculate the position
   *                information many times.
   *              
   *                It also provide uniform way of resizing the Segment extention
   *                from the AutoContact.
   */ 
 
  /*! \var          bool  SegmentEnd::_isSourceHook;
   *                set to \b true if the Segment is attached to the AutoContact
   *                through it's source hook (\b false for target \c hook).
   */
 
  /*! \function     SegmentEnd::SegmentEnd ( AutoSegment* segment, bool isSourceHook );
   *  \param        segment      the supporting segment.
   *  \param        isSourceHook initialize _isSourceHook.
   */
 
  /*! \function     static SegmentEnd* SegmentEnd::create ( Hook* hook, bool checking );
   *  \param        contact The AutoContact we are currently processing.
   *  \return       The appropriate SegmentPosition.
   *
   *                This create function allocate the relevant HorizontalPosition or
   *                VerticalPosition derived object, providing an uniform allocator
   *                function.
   */
 
  /*! \function     virtual bool SegmentEnd::isVertical () const;
   *  \return       \b true if the associated Segment is vertical.
   */
 
  /*! \function     virtual bool SegmentEnd::isHorizontal () const;
   *  \return       \b true if the associated Segment is horizontal.
   */
 
  /*! \function     bool SegmentEnd::isSourceHook () const;
   *  \return       The _isSourceHook value (accessor).
   */
 
  /*! \function     bool SegmentEnd::isGlobal () const;
   *  \return       The _isGlobal value (accessor).
   */
 
  /*! \function     virtual DbU::Unit  SegmentEnd::getAxis () const;
   *  \return       For horizontal segment, the Y coordinate and the X coordinate for
   *                vertical ones.
   */
 
  /*! \function     Layer* SegmentEnd::getLayer () const;
   *  \return       The layer of the segment.
   */
 
  /*! \function     Point  SegmentEnd::getEnd () const;
   *  \return       The position of the segment's extremity attached to the AutoContact.
   */
 
 
  /*! \function     void  SegmentEnd::setDelta ( DbU::Unit delta );
   *  \param        delta the new value of the extention.
   *
   *                Adjust the segment's extention. Coordinate must be expressed
   *                as an offset to the absolute coordinate of the relevant hook.
   */
 
  /*! \function     void  SegmentEnd::orient ();
   *                restore correct orientation of the segment (source \e lower than
   *                target), usually needed after a setDelta().
   */
 
 
 
 
  /*! \class        HorizontalEnd
   *  \brief        Horizontal Segment manipulator (\b internal)
   *
   *                This class must only be accessed through it's base class
   *                SegmentPosition, it's constructed through SegmentPosition::create().
   */
 
 
  /*! \class        VerticalEnd
   *  \brief        Vertical Segment manipulator (\b internal)
   *
   *                This class must only be accessed through it's base class
   *                SegmentPosition, it's constructed through SegmentPosition::create().
   */


  using namespace std;
  using namespace Hurricane;
  using namespace Katabatic;


// -------------------------------------------------------------------
// Local Variables.

// const char* badAutoContactAnchor =
//   "AutoContact::AutoContact() :\n\n"
//   "    Only <RoutinPad> are supported to anchor an AutoContact :\n"
//   "    %s\n";

// const char* nonAdjacentLayers =
//   "%s :\n\n"
//   "    %s and %s are not adjacent, cannot build a VIA.\n";

// const char* missingAutoLayer =
//   "AutoContact::create() :\n\n"
//   "    DataBase is lacking \"AutoLayer\" layer, please check technology file.\n";

// const char* emptyJunctionBox =
//   " Empty JunctionBox in %s (internal error).";

  const char* badHookType =
    "Hook of %s is neither a SourceHook nor a TargetHook (internal error).";

  const char* missingAutoSegment =
    "No AutoSegment associated to %s (internal error).";


// Forward Declarations.
  class StackedContact;


// -------------------------------------------------------------------
// Class  :  "::UPoint".


  class UPoint {
    // Constructors.
    public:
      inline            UPoint ( bool isHorizontal, DbU::Unit ux, DbU::Unit uy );
    // Accessors.
      inline DbU::Unit  getUX  () const;
      inline DbU::Unit  getUY  () const;
      inline DbU::Unit  getX   () const;
      inline DbU::Unit  getY   () const;
    // Modifiers.
      inline void       setUX  ( DbU::Unit ux );
      inline void       setUY  ( DbU::Unit uy );
    // Attributes.
    protected:
             bool       _isHorizontal;
             DbU::Unit  _ux;
             DbU::Unit  _uy;
  };


// Inline Functions.
  inline            UPoint::UPoint ( bool isHorizontal, DbU::Unit ux, DbU::Unit uy )
                                   : _isHorizontal(isHorizontal), _ux(ux), _uy(uy) {}
  inline DbU::Unit  UPoint::getUX  () const { return _ux; }
  inline DbU::Unit  UPoint::getUY  () const { return _uy; }
  inline DbU::Unit  UPoint::getX   () const { return (_isHorizontal)?_ux:_uy; }
  inline DbU::Unit  UPoint::getY   () const { return (_isHorizontal)?_uy:_ux; }
  inline void       UPoint::setUX  ( DbU::Unit ux ) { _ux = ux; }
  inline void       UPoint::setUY  ( DbU::Unit uy ) { _uy = uy; }


// -------------------------------------------------------------------
// Class  :  "::SegmentEnd".


  class SegmentEnd {
    public:
      struct Compare : public binary_function<SegmentEnd*,SegmentEnd*,bool> {
          inline bool  operator() ( const SegmentEnd* lhs, const SegmentEnd* rhs ) const;
      };
    public:
    // AutoSegment & Segment wrapped functions.
      inline  bool                 isGlobal       () const;
      inline  bool                 isLocal        () const;
      inline  bool                 isHorizontal   () const;
      inline  bool                 isVertical     () const;
      inline  bool                 isSlackened    () const;
      inline  Net*                 getNet         () const;
      inline  const Layer*         getLayer       () const;
      inline  Hook*                getSourceHook  ();
      inline  Hook*                getTargetHook  ();
      inline  DbU::Unit            getSourceX     () const;
      inline  DbU::Unit            getSourceY     () const;
      inline  DbU::Unit            getTargetX     () const;
      inline  DbU::Unit            getTargetY     () const;
      inline  DbU::Unit            getSourceU     () const;
      inline  DbU::Unit            getTargetU     () const;
      inline  DbU::Unit            getDuSource    () const;
      inline  DbU::Unit            getDuTarget    () const;
      inline  DbU::Unit            getAxis        () const;
      inline  DbU::Unit            getWidth       () const;
      inline  void                 setDuSource    ( DbU::Unit );
      inline  void                 setDuTarget    ( DbU::Unit );
      inline  void                 invert         ();
      inline  void                 invalidate     ();
    // Constructors & Destructors.
      inline                       SegmentEnd     ( AutoSegment* , bool isSourceHook );
      virtual                     ~SegmentEnd     ();
      static  SegmentEnd*          create         ( Hook*, bool checking );
    // Accessors.                                 
      static  size_t               getAllocateds  ();
      inline  bool                 isSourceHook   () const;
      inline  AutoSegment*         getSegment     () const;
      virtual Hook*                getHook        () const;
      inline  Point                getEnd         () const;
      inline  DbU::Unit            getEndX        () const;
      inline  DbU::Unit            getEndY        () const;
      virtual vector<SegmentEnd*>* getForks       ();
      virtual vector<SegmentEnd*>* getAligneds    ();
    // Modifiers.                                 
              void                 setDelta       ( DbU::Unit );
      virtual void                 orient         ();
      virtual void                 addFork        ( SegmentEnd* );
      virtual void                 addAligned     ( SegmentEnd* );
      virtual void                 split          ( vector<StackedContact*>& );
    // Inspector Managment.
      inline  Record*              _getRecord     () const;
      inline  string               _getString     () const;
      inline  string               _getTypeName   () const;
    protected:
    // Attributes.
      static  size_t               _allocateds;
              AutoSegment*         _autoSegment; 
              bool                 _isSourceHook;
  };


}

INSPECTOR_P_SUPPORT(SegmentEnd);

namespace {

         size_t               SegmentEnd::_allocateds    = 0;

                              SegmentEnd::~SegmentEnd    () { _allocateds--; }
         size_t               SegmentEnd::getAllocateds  () { return _allocateds; }
  inline bool                 SegmentEnd::isGlobal       () const { return _autoSegment->isGlobal(); }
  inline bool                 SegmentEnd::isLocal        () const { return _autoSegment->isLocal(); }
  inline bool                 SegmentEnd::isHorizontal   () const { return _autoSegment->isHorizontal(); }
  inline bool                 SegmentEnd::isVertical     () const { return _autoSegment->isVertical(); }
  inline bool                 SegmentEnd::isSlackened    () const { return _autoSegment->isSlackened(); }
  inline AutoSegment*         SegmentEnd::getSegment     () const { return _autoSegment; }
  inline Net*                 SegmentEnd::getNet         () const { return _autoSegment->getNet(); }
  inline const Layer*         SegmentEnd::getLayer       () const { return _autoSegment->getLayer(); }
  inline Hook*                SegmentEnd::getSourceHook  () { return _autoSegment->getSourceHook(); }
  inline Hook*                SegmentEnd::getTargetHook  () { return _autoSegment->getTargetHook(); }
  inline DbU::Unit            SegmentEnd::getSourceX     () const { return _autoSegment->getSourceX(); }
  inline DbU::Unit            SegmentEnd::getSourceY     () const { return _autoSegment->getSourceY(); }
  inline DbU::Unit            SegmentEnd::getTargetX     () const { return _autoSegment->getTargetX(); }
  inline DbU::Unit            SegmentEnd::getTargetY     () const { return _autoSegment->getTargetY(); }
  inline DbU::Unit            SegmentEnd::getSourceU     () const { return _autoSegment->getSourceU(); }
  inline DbU::Unit            SegmentEnd::getTargetU     () const { return _autoSegment->getTargetU(); }
  inline DbU::Unit            SegmentEnd::getDuSource    () const { return _autoSegment->getDuSource(); }
  inline DbU::Unit            SegmentEnd::getDuTarget    () const { return _autoSegment->getDuTarget(); }
  inline DbU::Unit            SegmentEnd::getAxis        () const { return _autoSegment->getAxis(); }
  inline DbU::Unit            SegmentEnd::getWidth       () const { return _autoSegment->getWidth(); }
  inline void                 SegmentEnd::setDuSource    ( DbU::Unit du ) { _autoSegment->setDuSource(du); }
  inline void                 SegmentEnd::setDuTarget    ( DbU::Unit du ) { _autoSegment->setDuTarget(du); }
  inline void                 SegmentEnd::invert         () { _autoSegment->invert(); }
  inline void                 SegmentEnd::invalidate     () { _autoSegment->invalidate(); }
  inline bool                 SegmentEnd::isSourceHook   () const { return _isSourceHook; }
  inline Point                SegmentEnd::getEnd         () const { return isSourceHook() ? Point(getSourceX(),getSourceY()) : Point(getTargetX(),getTargetY()); }
  inline DbU::Unit            SegmentEnd::getEndX        () const { return isSourceHook() ? getSourceX() : getTargetX(); }
  inline DbU::Unit            SegmentEnd::getEndY        () const { return isSourceHook() ? getSourceY() : getTargetY(); }
         vector<SegmentEnd*>* SegmentEnd::getForks       () { return NULL; }
         vector<SegmentEnd*>* SegmentEnd::getAligneds    () { return NULL; }
         void                 SegmentEnd::addFork        ( SegmentEnd* ) {}
         void                 SegmentEnd::addAligned     ( SegmentEnd* ) {}
         void                 SegmentEnd::split          ( vector<StackedContact*>& ) {}
  inline Record*              SegmentEnd::_getRecord     () const { return _autoSegment->_getRecord(); }
  inline string               SegmentEnd::_getString     () const { return _autoSegment->_getString(); }
  inline string               SegmentEnd::_getTypeName   () const { return "Katabatic::SegmentEnd"; }

  inline bool  SegmentEnd::Compare::operator() ( const SegmentEnd* lhs, const SegmentEnd* rhs ) const
  { return AutoSegment::CompareCanonical() ( lhs->getSegment(), rhs->getSegment() ); }

  inline  SegmentEnd::SegmentEnd ( AutoSegment* segment, bool isSourceHook )
    : _autoSegment(segment)
    , _isSourceHook(isSourceHook)
  {
    _allocateds++;
  }


  Hook* SegmentEnd::getHook () const
  {
    if ( isSourceHook() ) return _autoSegment->getSourceHook();
    return _autoSegment->getTargetHook();
  }


  void  SegmentEnd::orient ()
  {
    if ( ( !isGlobal() ) && ( getSourceU() > getTargetU() ) ) {
      ltrace(99) << "Orient() before - " << this << endl;

      invert ();
      DbU::Unit duSource = getDuSource();
      DbU::Unit duTarget = getDuTarget();
      setDuSource ( duTarget );
      setDuTarget ( duSource );
      _isSourceHook = !_isSourceHook;

      ltrace(99) << "Orient() after  - " << this << endl;
    }
  }


  void  SegmentEnd::setDelta ( DbU::Unit delta )
  {
    ltrace(99) << "setDelta(" << DbU::getLambda(delta) << ") - " << this << endl;

    if ( isSourceHook() ) {
      if ( getDuSource() != delta ) {
        setDuSource ( delta );
        ltrace(99) << "DuSource actualized: " << this << endl;
      }
    } else {
      if ( getDuTarget() != delta ) {
        setDuTarget ( delta );
        ltrace(99) << "DuTarget actualized: " << this << endl;
      }
    }

    orient ();
  }


}


namespace {


// -------------------------------------------------------------------
// Class  :  "::ForkCompare".


  class ForkCompare {
    public:
      inline       ForkCompare ( bool increasing ) : _increasing(increasing) {};
      inline bool  operator()  ( SegmentEnd* lhs, SegmentEnd* rhs ) const;
    protected:
      bool  _increasing;
  };


  bool  ForkCompare::operator() ( SegmentEnd* lhs, SegmentEnd* rhs ) const
  {
    bool superior = rhs->isGlobal();

    if ( lhs->getAxis() == rhs->getAxis() ) {
      if ( lhs->isLocal() && rhs->isLocal() )
        return false;
    } else
      superior = lhs->getAxis() > rhs->getAxis();

    return (_increasing) ? !superior : superior;
  }


// -------------------------------------------------------------------
// Class  :  "::StackedContact".


  class StackedContact : public Contact {

    public:
      static  StackedContact* create         ( Net* , const Layer* , DbU::Unit x, DbU::Unit y );
                              StackedContact ( Net* , const Layer* , DbU::Unit x, DbU::Unit y );
      virtual                ~StackedContact ();
    public:
              void            setAnchor      ( Component* );
              void            addLayer       ( const Layer* );
              void            attachSlave    ( SegmentEnd* );
              void            breakUp        ();

    protected:
              vector<bool>    _useds;
              Component*      _anchor;
    private:
                              StackedContact ( const StackedContact& );
              StackedContact& operator=      ( const StackedContact& );
  };


// -------------------------------------------------------------------
// Class  :  "::GlobalEnd".


  class GlobalEnd : public SegmentEnd {
    public:
                                   GlobalEnd        ( AutoSegment* , bool isSourceHook );
      virtual                     ~GlobalEnd        (); 
      virtual vector<SegmentEnd*>* getForks         ();
      virtual vector<SegmentEnd*>* getAligneds      ();
      virtual void                 split            ( vector<StackedContact*>& );
      virtual void                 addFork          ( SegmentEnd* );
      virtual void                 addAligned       ( SegmentEnd* );
      virtual Segment*             _create          ( Contact* source, Contact* target ) = 0;
    protected:
              vector<SegmentEnd*>  _forks;
              vector<SegmentEnd*>  _aligneds;
  };


  GlobalEnd::GlobalEnd ( AutoSegment* segment, bool isSourceHook )
    : SegmentEnd(segment,isSourceHook)
    , _forks()
    , _aligneds()
  {}


  GlobalEnd::~GlobalEnd ()
  {} 


  vector<SegmentEnd*>* GlobalEnd::getForks ()
  {
    return &_forks;
  }


  vector<SegmentEnd*>* GlobalEnd::getAligneds ()
  {
    return &_aligneds;
  }


  void  GlobalEnd::addFork ( SegmentEnd* segmentEnd )
  {
    for ( size_t i = 0 ; i < _forks.size() ; i++ ) {
      if ( _forks[i] == segmentEnd ) return;
    }
    _forks.push_back ( segmentEnd );
  }


  void  GlobalEnd::addAligned ( SegmentEnd* segmentEnd )
  {
    for ( size_t i = 0 ; i < _aligneds.size() ; i++ ) {
      if ( _aligneds[i] == segmentEnd ) return;
    }
    _aligneds.push_back ( segmentEnd );
  }


  void  GlobalEnd::split ( vector<StackedContact*>& stackedContacts )
  {
    ltrace(99) << "GlobalEnd::split() - " << this << endl;
    ltracein(99);

    sort ( _forks.begin(), _forks.end(), ForkCompare(!isSourceHook()) );

    StackedContact*               contact1        = NULL;
    StackedContact*               contact2        = NULL;
    const Layer*                  contactLayer    = NULL;
    Hook*                         hook            = getHook();
    vector<SegmentEnd*>::iterator it              = _forks.begin();

    if ( it == _forks.end() ) {
      ltrace(99) << "No forks!" << endl;
      stackedContacts.push_back ( StackedContact::create ( getNet()
                                                         , getLayer()
                                                         , getEndX()
                                                         , getEndY()
                                                         ) );
      stackedContacts.back()->attachSlave ( this );

      ltraceout(99);
      return;
    }

    ltrace(99) << "splitted Axis " << DbU::getLambda(getAxis()) << endl;
    ltrace(99) << "iterator Axis " << DbU::getLambda((*it)->getAxis()) << " " << (*it) << endl;

    UPoint center ( isHorizontal(), (*it)->getAxis(), getAxis() );
    contactLayer = Session::getTechnology()->getViaBetween ( getLayer(), (*it)->getLayer() );
    stackedContacts.push_back
      ( StackedContact::create ( getNet(), contactLayer, center.getX(), center.getY() ) );
    contact1 = stackedContacts.back ();
    contact1->attachSlave ( *it );

    hook->detach ();
    hook->attach ( contact1->getBodyHook() );
    setDelta ( 0 );
      
    for ( it++ ; it != _forks.end() ; it++ ) {
      ltrace(99) << "splitted Axis " << DbU::getLambda(getAxis()) << endl;
      ltrace(99) << "iterator Axis " << DbU::getLambda((*it)->getAxis()) << " " << (*it) << endl;

      if ( (*(it-1))->getAxis() != (*it)->getAxis() ) {
        UPoint  center ( isHorizontal(), (*it)->getAxis(), getAxis() );
        contactLayer = Session::getTechnology()->getViaBetween ( getLayer(), (*it)->getLayer() );
        stackedContacts.push_back
          ( StackedContact::create ( getNet(), contactLayer, center.getX(), center.getY() ) );
        contact2 = stackedContacts.back ();
      } else {
        contact2 = contact1;
      }

      if ( contact1 != contact2 ) {
        if ( !isSourceHook() ) _create ( contact1, contact2 );
        else                   _create ( contact2, contact1 );
      }

      contact2->attachSlave ( *it );
      contact1 = contact2;
    }

    for ( it = _aligneds.begin() ; it != _aligneds.end() ; it++ ) {
      if ( isSourceHook() == (*it)->isSourceHook() ) {
        stackedContacts.front()->attachSlave ( *it );
      } else {
        stackedContacts.back()->attachSlave ( *it );
      }
    }

    ltraceout(99);
  }


// -------------------------------------------------------------------
// Class  :  "::HorizontalEnd".


  class HorizontalEnd : public GlobalEnd {
    public:
                        HorizontalEnd ( AutoSegment* , bool isSourceHook );
      virtual          ~HorizontalEnd ();
      virtual Segment*  _create       ( Contact* source , Contact* target );
  };


  HorizontalEnd::HorizontalEnd  ( AutoSegment* horizontal, bool isSourceHook )
    : GlobalEnd(horizontal,isSourceHook)
  { }


  HorizontalEnd::~HorizontalEnd ()
  { }


  Segment* HorizontalEnd::_create ( Contact* source , Contact* target )
  {
    return Horizontal::create ( source, target, getLayer(), getAxis(), getWidth() );
  }


// -------------------------------------------------------------------
// Class  :  "::VerticalEnd".


  class VerticalEnd : public GlobalEnd {
    public:
                        VerticalEnd ( AutoSegment* , bool isSourceHook );
      virtual          ~VerticalEnd (); 
      virtual Segment*  _create     ( Contact* source , Contact* target );
  };


  VerticalEnd::VerticalEnd ( AutoSegment* vertical, bool isSourceHook )
    : GlobalEnd(vertical,isSourceHook)
  { } 


  VerticalEnd::~VerticalEnd ()
  { }


  Segment* VerticalEnd::_create ( Contact* source , Contact* target )
  {
    return Vertical::create ( source, target, getLayer(), getAxis(), getWidth() );
  }


// -------------------------------------------------------------------
// Static Generic Constructor for all SegmentEnd.


  SegmentEnd* SegmentEnd::create ( Hook* hook, bool checking )
  {
    bool isSourceHook = (dynamic_cast<Segment::SourceHook*>(hook));
    if ( !isSourceHook && ( dynamic_cast<Segment::TargetHook*>(hook) == NULL ) ) {
      cerr << Error ( badHookType, getString(hook->getComponent()).c_str() ) << endl;
      return NULL;
    }

    AutoSegment* autoSegment = Session::lookup ( dynamic_cast<Segment*>(hook->getComponent()) );

    if ( !autoSegment )
      throw Error ( missingAutoSegment, getString(hook->getComponent()).c_str() );

    if ( !checking )
      autoSegment->invalidate ();

    if ( autoSegment->isGlobal() ) {
      if ( autoSegment->isHorizontal() )
        return new HorizontalEnd ( autoSegment, isSourceHook );
      else
        return new VerticalEnd ( autoSegment, isSourceHook );
    } else {
      if ( autoSegment->isHorizontal() )
        return new SegmentEnd ( autoSegment, isSourceHook );
    }
    return new SegmentEnd ( autoSegment, isSourceHook );
  }


// -------------------------------------------------------------------
// Class  :  "::JunctionBox".


  class JunctionBox {

    public:
    // Constructor & Destructor.
                    JunctionBox        ( AutoContact*, bool checking );
                   ~JunctionBox        ();
    // Predicates.                     
      inline bool   isVFlat            ();
      inline bool   isHFlat            ();
      inline bool   isFlat             ();
      inline bool   isPunctual         ();
      inline bool   isVEmpty           ();
      inline bool   isHEmpty           ();
      inline bool   isEmpty            ();
             bool   isHExtended        ();
             bool   isVExtended        ();
             bool   canGoOutside       ( const AutoSegment* ) const;
             bool   canHDesalignate    () const;
             bool   canVDesalignate    () const;
             void   getZSpan           ( size_t& zMin, size_t& zMax ) const;
    // Modifiers.                    
             void   checkTopology      ();
             void   revalidateTopology ();
             void   computeGlobalStem  ();
             void   computeAlignate    ();
             void   mergeX             ( DbU::Unit );
             void   mergeY             ( DbU::Unit );
             void   mergeAnchor        ();
             void   merge              ( Hook* );
             void   postMerge          ( SegmentEnd* );
             void   resizePunctual     ();
             void   resize             ();
             void   breakUpPunctual    ();
             void   breakUp            ();
             void   split              ();
             void   splitTerminal      ();
             void   updateContacts     ( VirtualContacts& );
             void   restoreHConnexity  ( DbU::Unit x, bool split );
             void   restoreVConnexity  ( DbU::Unit y, bool split );

    protected:
    // Attributes.
      RoutingGauge*        _routingGauge;
      AutoContact*         _contact;
      Component*           _anchor;
      DbU::Unit            _xMin;
      DbU::Unit            _yMin;
      DbU::Unit            _xMax;
      DbU::Unit            _yMax;
      SegmentEnd*          _globalStem;
      vector<SegmentEnd*>  _globalEnds;
      vector<SegmentEnd*>  _localEnds;
      vector<size_t>       _layerStack;
      size_t               _wireCount;
      bool                 _failsafe;
      bool                 _checking;
  };


  inline bool  JunctionBox::isVFlat     () { return _yMin == _yMax; }
  inline bool  JunctionBox::isHFlat     () { return _xMin == _xMax; }
  inline bool  JunctionBox::isFlat      () { return isHFlat() || isVFlat(); }
  inline bool  JunctionBox::isPunctual  () { return isHFlat() && isVFlat(); }
  inline bool  JunctionBox::isVEmpty    () { return _yMin > _yMax; }
  inline bool  JunctionBox::isHEmpty    () { return _xMin > _xMax; }
  inline bool  JunctionBox::isEmpty     () { return isHEmpty() || isVEmpty(); }


  JunctionBox::JunctionBox ( AutoContact* contact, bool checking )
    : _routingGauge(Session::getRoutingGauge())
    , _contact     (contact)
    , _anchor      (NULL)
    , _xMin        (+1)
    , _yMin        (+1)
    , _xMax        (-1)
    , _yMax        (-1)
    , _globalStem  (NULL)
    , _globalEnds  ()
    , _localEnds   ()
    , _layerStack  (Session::getRoutingGauge()->getDepth())
    , _wireCount   (0)
    , _failsafe    (false)
    , _checking    (checking)
  {
    ltrace(110) << "JunctionBox() " << contact << endl;
    ltracein(109);

    forEach ( Hook*, hook, _contact->getBodyHook()->getSlaveHooks() ) merge ( *hook );
    mergeAnchor ();

    sort ( _globalEnds.begin(), _globalEnds.end(), SegmentEnd::Compare() );
    sort ( _localEnds.begin() , _localEnds.end() , SegmentEnd::Compare() );

    if ( ( _wireCount < 1 ) && !_contact->getAnchor() )
      cerr << Warning("%s has less than 2 wires (%d)"
                     ,getString(_contact).c_str(),_wireCount) << endl;

    computeGlobalStem ();
    if ( _globalStem ) {
      ltrace(109) << "_globalStem: " << _globalStem << endl;
      for ( size_t i=0 ; i < _localEnds.size() ; i++ )
        postMerge ( _localEnds[i] );
      for ( size_t i=0 ; i < _globalEnds.size() ; i++ )
        postMerge ( _globalEnds[i] );
      
      ltraceout(109);
      return;
    }

    if ( isPunctual() || (isFlat() && !_contact->getAnchor()) ) {
      ltraceout(109);
      return;
    }

    // JunctionBox is empty, non-flat or flat with anchor.
    _failsafe = true;
    ltraceout(109);
  }
  

  JunctionBox::~JunctionBox ()
  {
    for ( size_t i = 0 ; i < _globalEnds.size() ; i++ ) delete _globalEnds[i];
    for ( size_t i = 0 ; i < _localEnds.size()  ; i++ ) delete _localEnds[i];
  }


  bool  JunctionBox::isHExtended ()
  {
    if ( _globalStem ) {
      ltrace(109) << "JunctionBox::isHExtended(): " << _globalStem->isHorizontal() << endl;
      return _globalStem->isHorizontal();
    }
    return _contact->isHAlignate();
  }


  bool  JunctionBox::isVExtended ()
  {
    if ( _globalStem ) {
      ltrace(109) << "JunctionBox::isVExtended(): " << _globalStem->isVertical() << endl;
      return _globalStem->isVertical();
    }
    return _contact->isVAlignate();
  }


  void  JunctionBox::getZSpan ( size_t& zMin, size_t& zMax ) const
  {
    bool minFound = false;

    zMin = 0;
    zMax = 0;
    for ( size_t i=0 ; i<_layerStack.size() ; i++ ) {
      if ( !minFound ) {
        if ( _layerStack[i] > 0 ) {
          zMin = zMax = i;
          minFound = true;
        }
      } else {
        if ( _layerStack[i] > 0 ) {
          zMax = i;
        }
      }
    }
  }


  void  JunctionBox::revalidateTopology ()
  {
    _contact->setInvalidatedTopology ( false );
    _contact->setCorner ( false );
    if ( !_contact->getAnchor() ) {
      size_t  horizontals = 0;
      size_t  verticals   = 0;

      for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
        if ( _globalEnds[i]->isHorizontal() ) horizontals++;
        else                                  verticals++;
      }
      for ( size_t i=0 ; i < _localEnds.size() ; i++ ) {
        if ( _localEnds[i]->isHorizontal() ) horizontals++;
        else                                 verticals++;
      }
      _contact->setCorner (   (horizontals == 1)
                          and (verticals   == 1)
                          and (not _contact->isHAlignate())
                          and (not _contact->isVAlignate()) );
    }
   
    checkTopology ();
  }


  void  JunctionBox::checkTopology ()
  {
    vector<string> errors;

    bool    anchored           = (_contact->getAnchor());
    bool    alignateHorizontal = _contact->isHAlignate() || anchored;
    bool    alignateVertical   = _contact->isVAlignate() || anchored;
    bool    globalHorizontal   = false;
    bool    globalVertical     = false;
    size_t  localHorizontals   = 0;
    size_t  localVerticals     = 0;

    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isHorizontal() ) {
        globalHorizontal = true;
        localHorizontals++;
      } else {
        globalVertical = true;
        localVerticals++;
      }
    }

    for ( size_t i=0 ; i < _localEnds.size() ; i++ ) {
      if ( _localEnds[i]->isHorizontal() ) localHorizontals++;
      else                                 localVerticals++;
    }

    if ( !(localHorizontals || globalHorizontal || anchored) )
      errors.push_back ( "No horizontal components" );

    if ( !(localVerticals || globalVertical || anchored) )
      errors.push_back ( "No vertical components" );

    if ( !alignateHorizontal && !globalVertical && (localHorizontals > 1) )
      errors.push_back ( "Disconnecteds horizontals components" );

    if ( !alignateVertical && !globalHorizontal && (localVerticals > 1) )
      errors.push_back ( "Disconnecteds verticals components" );

    if ( _globalEnds.size() > 3 )
      errors.push_back ( "More than three globals" );

    if ( ( _globalEnds.size() == 3 ) && ( _localEnds.size() ) )
      errors.push_back ( "Three globals AND locals" );

    if ( _globalEnds.size() + _localEnds.size() + ((anchored)?1:0) < 2 )
      errors.push_back ( "Less than two connections" );

    if ( errors.size() ) {
      cerr << Error("AutoContact topology of %s",getString(_contact).c_str()) << endl;
      if ( anchored )
        cerr << "        A: " << _contact->getAnchor() << endl;

      for ( size_t i=0 ; i < _globalEnds.size() ; i++ )
        cerr << "        G: " << _globalEnds[i] << endl;

      for ( size_t i=0 ; i < _localEnds.size() ; i++ )
        cerr << "        L: " << _localEnds[i] << endl;

      for ( size_t i=0 ; i < errors.size() ; i++ )
        cerr << "        " << errors[i] << endl;
    }
  }


  bool  JunctionBox::canGoOutside ( const AutoSegment* segment ) const
  {
    if ( _contact->isCorner () ) return true;
    if ( _contact->getAnchor() ) return false;

    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isHorizontal() xor segment->isHorizontal() ) {
        ltrace(200) << "canGoOutside(): true (has global) " << _contact << endl;
        return true;
      }
    }

    return false;
  }


  bool  JunctionBox::canHDesalignate () const
  {
    if (  _contact->getAnchor  () ) return false;
    if ( !_contact->isHAlignate() ) return false;

    SegmentEnd* globalHorizontal = NULL;
    SegmentEnd* globalVertical   = NULL;

    size_t verticals = 0;
    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      ltrace(109) << _globalEnds[i]->getSegment() << endl;
      if ( _globalEnds[i]->isHorizontal() ) {
        globalHorizontal = _globalEnds[i];
      } else {
        globalVertical = _globalEnds[i];
        verticals++;
      }
    }

    for ( size_t i=0 ; i < _localEnds.size() ; i++ ) {
      ltrace(109) << _localEnds[i]->getSegment() << endl;
      if ( _localEnds[i]->isVertical() ) verticals++;
    }

    ltrace(200) << "canHDesalignate(Contact*) - " << _contact << endl;
    ltrace(200) << "  Vertical stem: " << globalVertical
                << " isVAlignate():"   << _contact->isVAlignate()
                << " verticals:"       << verticals
                << endl;
    return (globalVertical != NULL) && (_contact->isVAlignate() || (verticals == 1));
  }


  bool  JunctionBox::canVDesalignate () const
  {
    if (  _contact->getAnchor  () ) return false;
    if ( !_contact->isVAlignate() ) return false;

    SegmentEnd* globalHorizontal = NULL;
    SegmentEnd* globalVertical   = NULL;

    size_t horizontals = 0;
    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isHorizontal() ) {
        globalHorizontal = _globalEnds[i];
        horizontals++;
      } else {
        globalVertical = _globalEnds[i];
      }
    }

    for ( size_t i=0 ; i < _localEnds.size() ; i++ ) {
      ltrace(109) << _localEnds[i]->getSegment() << endl;
      if ( _localEnds[i]->isHorizontal() ) horizontals++;
    }

    ltrace(200) << "canVDesalignate(Contact*) - " << _contact << endl;
    ltrace(200) << "  Vertical stem: " << globalHorizontal
                << " isHAlignate():"   << _contact->isHAlignate()
                << " horizontals:"     << horizontals
                << endl;
    return (globalHorizontal != NULL) && (_contact->isHAlignate() || (horizontals == 1));
  }


  void  JunctionBox::restoreHConnexity ( DbU::Unit x, bool split )
  {
    ltrace(200) << "restoreHConnexity() - @" << DbU::getValueString(x) << " " << _contact << endl;

    _contact->invalidate ();

    if ( _contact->isHAlignate() ) return;
    if ( _contact->getAnchor  () ) {
      _contact->setHAlignate ( true );
      return;
    }

    bool                slackened    = false;
    SegmentEnd*         verticalStem = NULL;
    vector<SegmentEnd*> horizontals;
    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isHorizontal() ) {
        horizontals.push_back ( _globalEnds[i] ) ;
      } else {
        verticalStem = _globalEnds[i];
      }
    }

    if ( verticalStem ) {
      ltrace(200) << "Done nothing has vertical stem: " << verticalStem << endl;
      return;
    }

    for ( size_t i=0 ; i<_localEnds.size() ; i++ ) {
      if ( _localEnds[i]->isHorizontal() )
        horizontals.push_back ( _localEnds[i] ) ;
      else
        slackened = slackened or _localEnds[i]->isSlackened();
    }

    if ( (horizontals.size() == 1) and (horizontals[0]->isGlobal()) ) {
      ltrace(200) << "Done nothing, has only a horizontal stem: " << horizontals[0] << endl;
      return;
    }

    if ( !split ) {
      _contact->setHAlignate ( true );
      return;
    }

    GCell*       gcell = _contact->getGCell ();
    Net*         net   = _contact->getNet   ();
    const Layer* layer = dynamic_cast<const ViaLayer*>(_contact->getLayer())->getTop();
    if ( _routingGauge->getLayerDirection(layer) != Constant::Vertical )
      layer = dynamic_cast<const ViaLayer*>(_contact->getLayer())->getBottom();
    
    _contact->setVAlignate ( true );
    AutoContact* splitContact = NULL;
    for ( size_t i=1 ; i<horizontals.size() ; i++ ) {
      if ( splitContact ) splitContact->setVAlignate ( true );

      ltrace(200) << "| Separate " << horizontals[i]->getSegment()->base()
                  << ":" << horizontals[i]->getSegment() << endl;

      horizontals[i]->getHook()->detach();

      splitContact = AutoContact::create ( gcell, net, _contact->getLayer() );
      AutoSegment* segment = AutoVertical::create ( _contact 
                                                  , splitContact
                                                  , layer
                                                  , x //globalHorizontals[i]->getAxis()
                                                  , DbU::lambda(2.0)
                                                  , AutoSegment::Local
                                                  , false
                                                  , false
                                                  );
      horizontals[i]->getHook()->attach ( splitContact->getContact()->getBodyHook() );
      segment->setSlackened ( slackened );
      ltrace(200) << "restoreHConnexity() strap:  " << segment << endl;
    }
  }


  void  JunctionBox::restoreVConnexity ( DbU::Unit y, bool split )
  {
    ltrace(200) << "restoreVConnexity() - @" << DbU::getValueString(y) << " " << _contact << endl;

    _contact->invalidate ();

    if ( _contact->isVAlignate() ) return;
    if ( _contact->getAnchor  () ) {
      _contact->setVAlignate ( true );
      return;
    }

    bool                slackened      = false;
    SegmentEnd*         horizontalStem = NULL;
    vector<SegmentEnd*> verticals;
    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isVertical() ) {
        verticals.push_back ( _globalEnds[i] ) ;
      } else {
        horizontalStem = _globalEnds[i];
      }
    }

    if ( horizontalStem ) {
      ltrace(200) << "Done nothing, has horizontal stem: " << horizontalStem << endl;
      return;
    }

    for ( size_t i=0 ; i<_localEnds.size() ; i++ ) {
      if ( _localEnds[i]->isVertical() )
        verticals.push_back ( _localEnds[i] ) ;
      else
        slackened = slackened or _localEnds[i]->isSlackened();
    }

    if ( (verticals.size() == 1) and (verticals[0]->isGlobal()) ) {
      ltrace(200) << "Done nothing, has only a vertical stem: " << verticals[0] << endl;
      return;
    }

    if ( !split ) {
      _contact->setVAlignate ( true );
      return;
    }

    GCell*       gcell = _contact->getGCell ();
    Net*         net   = _contact->getNet   ();
    const Layer* layer = dynamic_cast<const ViaLayer*>(_contact->getLayer())->getBottom();
    if ( _routingGauge->getLayerDirection(layer) != Constant::Horizontal )
      layer = dynamic_cast<const ViaLayer*>(_contact->getLayer())->getTop();
    
    _contact->setHAlignate ( true );
    AutoContact* splitContact = NULL;
    for ( size_t i=1 ; i<verticals.size() ; i++ ) {
      if ( splitContact ) splitContact->setHAlignate ( true );

      ltrace(200) << "| Separate " << verticals[i]->getSegment()->base()
                  << ":" << verticals[i]->getSegment() << endl;

      verticals[i]->getHook()->detach();

      splitContact = AutoContact::create ( gcell, net, _contact->getLayer() );
      AutoSegment* segment = AutoHorizontal::create ( _contact 
                                                    , splitContact
                                                    , layer
                                                    , y //globalVerticals[i]->getAxis()
                                                    , DbU::lambda(2.0)
                                                    , AutoSegment::Local
                                                    , false
                                                    , false
                                                    );
      verticals[i]->getHook()->attach ( splitContact->getContact()->getBodyHook() );
      segment->setSlackened ( slackened );
      ltrace(200) << "restoreVConnexity() strap:  " << segment << endl;
    }
  }


  void  JunctionBox::computeAlignate ()
  {

    bool    globalHorizontal   = false;
    bool    globalVertical     = false;
    size_t  horizontals        = 0;
    size_t  verticals          = 0;

    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isHorizontal() ) {
        globalHorizontal = true;
        horizontals++;
      } else {
        globalVertical = true;
        verticals++;
      }
    }

    for ( size_t i=0 ; i < _localEnds.size() ; i++ ) {
      if ( _localEnds[i]->isHorizontal() ) horizontals++;
      else                                 verticals++;
    }

    if ( !globalVertical && (horizontals > 1) )
      _contact->setHAlignate ( true );

    if ( !globalHorizontal && (verticals > 1) )
      _contact->setVAlignate ( true );

    ltrace(109) << "computeAlignate(): [AFTER] " << _contact << endl;
  }


  void  JunctionBox::computeGlobalStem ()
  {
    SegmentEnd* globalHorizontal = NULL;
    SegmentEnd* globalVertical   = NULL;
    size_t      horizontals      = 0;
    size_t      verticals        = 0;

    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isHorizontal() ) {
        globalHorizontal = _globalEnds[i];
        horizontals++;
      } else {
        globalVertical = _globalEnds[i];
        verticals++;
      }
    }

    for ( size_t i=0 ; i < _localEnds.size() ; i++ ) {
      if ( _localEnds[i]->isHorizontal() ) horizontals++;
      else                                 verticals++;
    }

    ltrace(109) << "computeGlobalStem(): " << _contact << endl;
    ltrace(109) << "| h:" << horizontals << " v:" << verticals << endl;

    if ( globalVertical && (horizontals > 1) && !_contact->isHAlignate() ) {
      ltrace(109) << "| Vertical GlobalStem h:" << horizontals << " v:" << verticals << endl;
      _globalStem = globalVertical;
    }

    if ( globalHorizontal && (verticals > 1) && !_contact->isVAlignate() ) {
      ltrace(109) << "| Horizontal GlobalStem h:" << horizontals << " v:" << verticals << endl;
      _globalStem = globalHorizontal;
    }


    if ( (horizontals > 1) && (verticals > 1) && !_contact->isVAlignate() && !_contact->isHAlignate() ) {
      ltrace(109) << "* Bad AutoContact: " << _contact << endl;

      for ( size_t i=0 ; i < _globalEnds.size() ; i++ )
        ltrace(109) << "| G: " << _globalEnds[i] << endl;

      for ( size_t i=0 ; i < _localEnds.size() ; i++ )
        ltrace(109) << "| L: " << _localEnds[i] << endl;
    }
  }


  void  JunctionBox::mergeX ( DbU::Unit x )
  {
    if ( _xMin > _xMax ) { _xMin = _xMax = x; return; }
    if ( x < _xMin     ) { _xMin = x; return; }
    if ( x > _xMax     ) { _xMax = x; return; }
  }


  void  JunctionBox::mergeY ( DbU::Unit y )
  {
    if ( _yMin > _yMax ) { _yMin = _yMax = y; return; }
    if ( y < _yMin     ) { _yMin = y; return; }
    if ( y > _yMax     ) { _yMax = y; return; }
  }


  void  JunctionBox::mergeAnchor ()
  {
    _anchor = _contact->getAnchor ();
    if ( !_anchor ) return;

    RoutingPad* rp = dynamic_cast<RoutingPad*>(_anchor);
    if ( !rp ) {
      _failsafe = true;
      return;
    }

    _layerStack [ _routingGauge->getLayerDepth(_anchor->getLayer()) ] ++;
    _wireCount++;

    Point source = rp->getSourcePosition ();
    Point target = rp->getTargetPosition ();
    if ( ( source.getX() == target.getX() ) || isHEmpty() ) {
      ltrace(109) << "merge() Axis "
                  << DbU::getLambda(source.getX()) << " [RoutingPad] "
                  << rp->getLayer() << endl;
      mergeX ( source.getX() );
    }
    if ( ( source.getY() == target.getY() ) || isVEmpty() ) {
      ltrace(109) << "merge() Axis "
                  << DbU::getLambda(source.getY()) << " [RoutingPad] "
                  << rp->getLayer() << endl;
      mergeY ( source.getY() ); 
    }
  }


  void  JunctionBox::merge ( Hook* hook )
  {
    SegmentEnd* segmentEnd = SegmentEnd::create ( hook, _checking );
    if ( !segmentEnd ) return;

    _layerStack [ _routingGauge->getLayerDepth(segmentEnd->getLayer()) ] ++;
    _wireCount++;

    if ( segmentEnd->isGlobal() ) {
      ltrace(109) << "merge() Axis "
                  << DbU::getLambda(segmentEnd->getAxis())
                  << " [global] " << segmentEnd << endl;
      _globalEnds.push_back ( segmentEnd );
    } else {
      ltrace(109) << "merge() Axis " << DbU::getLambda(segmentEnd->getAxis())
                  << " [local] " << segmentEnd << endl;
      _localEnds.push_back ( segmentEnd );
    }

    if ( segmentEnd->isHorizontal() ) mergeY ( segmentEnd->getAxis() );
    else                              mergeX ( segmentEnd->getAxis() );
  }


  void  JunctionBox::postMerge ( SegmentEnd* segmentEnd )
  {
    if ( segmentEnd == _globalStem ) return;

    if ( _globalStem->isHorizontal() xor segmentEnd->isHorizontal() )
      _globalStem->addFork ( segmentEnd );
    else
      _globalStem->addAligned ( segmentEnd );
  }


  void  JunctionBox::resizePunctual ()
  {
    Point     center;
    DbU::Unit delta;

    if ( isEmpty() ) center = _contact->getCenter();
    else if ( isPunctual() ) {
      center.setX ( _xMin );
      center.setY ( _yMin );
    } else {
      center.setX ( (_xMin+_xMax)/2 );
      center.setY ( (_yMin+_yMax)/2 );
    }

    for ( size_t i=0 ; i < _globalEnds.size() ; i++ ) {
      if ( _globalEnds[i]->isHorizontal() )
        delta = center.getX() - _contact->getX();
      else
        delta = center.getY() - _contact->getY();
      _globalEnds[i]->setDelta ( delta );
    }

    for ( size_t i=0 ; i < _localEnds.size() ; i++ ) {
      if ( _localEnds[i]->isHorizontal() )
        delta = center.getX() - _contact->getX();
      else
        delta = center.getY() - _contact->getY();

      _localEnds[i]->setDelta ( delta );
    }

    Box constraint = _contact->getConstraintBox()/*.inflate(DbU::lambda(0.5))*/;
    if ( !constraint.contains(center) )
      cerr << Bug("%s [%s %s] outside constraint %s."
                 ,getString(_contact).c_str()
                 ,DbU::getValueString(center.getX()).c_str()
                 ,DbU::getValueString(center.getY()).c_str()
                 ,getString(constraint).c_str()) << endl;
  }


  void  JunctionBox::resize ()
  {
    ltrace(99) << "JunctionBox::resize() - <Box "
               << DbU::getLambda(_xMin) << " " << DbU::getLambda(_yMin) << " "
               << DbU::getLambda(_xMax) << " " << DbU::getLambda(_yMax)
               << ">" << endl;

    if ( !_wireCount ) {
      cerr << Warning("Standalone %s skipped.",getString(_contact).c_str()) << endl;
      return;
    }

    if ( _failsafe || isPunctual() || !_globalStem )
      resizePunctual ();
    else {
      // Flat geometry.
      DbU::Unit deltaMin;
      DbU::Unit deltaMax;
      DbU::Unit deltaFork;

      if ( _globalStem->isHorizontal() ) {
        deltaMin  = _xMin - _contact->getX();
        deltaMax  = _xMax - _contact->getX();
        deltaFork = _yMin - _contact->getY();
      } else {
        deltaMin  = _yMin - _contact->getY();
        deltaMax  = _yMax - _contact->getY();
        deltaFork = _xMin - _contact->getX();
      }

      _globalStem->setDelta ( _globalStem->isSourceHook() ? deltaMin : deltaMax );

      vector<SegmentEnd*>* forks    = _globalStem->getForks();
      vector<SegmentEnd*>* aligneds = _globalStem->getAligneds();

      for ( size_t i=0 ; i < aligneds->size() ; i++ )
        (*aligneds)[i]->setDelta ( (*aligneds)[i]->isSourceHook() ? deltaMax : deltaMin );

      for ( size_t i=0 ; i < forks->size() ; i++ )
        (*forks)[i]->setDelta ( deltaFork );
    }
  }


  void  JunctionBox::split ()
  {
    ltrace(200) << "JunctionBox::split() - " << _contact << endl;

    size_t zMin, zMax;
    size_t anchorDepth = 0;
    getZSpan ( zMin, zMax );

    if ( _anchor ) {
      ltrace(200) << "Anchor: " << _anchor << endl;

      anchorDepth = _routingGauge->getLayerDepth(_anchor->getLayer());
      if ( anchorDepth == 0 ) {
        if ( zMax-zMin < 2 ) return;
        splitTerminal ();
        _contact->split ();
        return;
      }
    }

    ltracein(200);
    ltrace(200) << "Z span: [" << zMin << ":" << zMax << "]" << endl;
    ltrace(200) << "Global Stem: " << _globalStem << endl;

    if ( zMax-zMin > 3 ) {
      cerr << Error("AutoContact::split(): Spans on more than 4 layers, ignoring."
                    "\n                     %s",getString(_contact).c_str()) << endl;
      ltraceout(200);
      return;
    }

    _contact->invalidate ();

    if ( zMax == zMin ) {
      ltrace(200) << "NULL Z span, no layer change." << endl;
      ltraceout(200);
      return;
    }

    if ( zMax-zMin < 2 ) {
      const Layer* contactLayer = _routingGauge->getContactLayer(zMin);
      _contact->setLayer ( contactLayer );
      ltrace(200) << "Needs only to change Layer." << endl;
      ltraceout(200);
      return;
    }

  // The complete case.
    bool                hExtended = isHExtended();
    bool                vExtended = isVExtended();
    bool                xFound    = false;
    bool                yFound    = false;
    vector<SegmentEnd*> hBottom;
    vector<SegmentEnd*> vBottom;
    vector<SegmentEnd*> hTop;
    vector<SegmentEnd*> vTop;
    Point               position;

    for ( size_t i=0; i < _globalEnds.size() ; i++ ) {
      ltrace(200) << "| G: " << _globalEnds[i] << endl;
      if ( _globalEnds[i]->isHorizontal() ) {
        if ( not yFound ) {
          yFound = true;
          position.setY ( _globalEnds[i]->getAxis() );
        }
        if ( _routingGauge->getLayerDepth(_globalEnds[i]->getLayer()) < zMin+2 )
          hBottom.push_back ( _globalEnds[i] );
        else
          hTop.push_back ( _globalEnds[i] );
      } else {
        if ( not xFound ) {
          xFound = true;
          position.setX ( _globalEnds[i]->getAxis() );
        }
        if ( _routingGauge->getLayerDepth(_globalEnds[i]->getLayer()) < zMin+2 )
          vBottom.push_back ( _globalEnds[i] );
        else
          vTop.push_back ( _globalEnds[i] );
      }
    }

    for ( size_t i=0; i < _localEnds.size() ; i++ ) {
      ltrace(200) << "| L: " << _localEnds[i] << endl;
      if ( _localEnds[i]->isHorizontal() ) {
        if ( not yFound ) {
          yFound = true;
          position.setY ( _localEnds[i]->getAxis() );
        }
        if ( _routingGauge->getLayerDepth(_localEnds[i]->getLayer()) < zMin+2 )
          hBottom.push_back ( _localEnds[i] );
        else
          hTop.push_back ( _localEnds[i] );
      } else {
        if ( not xFound ) {
          xFound = true;
          position.setX ( _localEnds[i]->getAxis() );
        }
        if ( _routingGauge->getLayerDepth(_localEnds[i]->getLayer()) < zMin+2 )
          vBottom.push_back ( _localEnds[i] );
        else
          vTop.push_back ( _localEnds[i] );
      }
    }

    if ( not xFound ) position.setX ( _xMin );
    if ( not yFound ) position.setY ( _yMin );

    for ( size_t i=0 ; i<hTop.size() ; i++ ) hTop[i]->getHook()->detach ();
    for ( size_t i=0 ; i<vTop.size() ; i++ ) vTop[i]->getHook()->detach ();

    AutoContact* corner = AutoContact::create ( _contact->getGCell()
                                              , _contact->getNet()
                                              , _routingGauge->getContactLayer(zMin+1)
                                              );
    ltrace(200) << "Corner " << corner << endl;

    AutoSegment* segment = AutoSegment::create ( _contact
                                               , corner
                                               , _routingGauge->getLayerDirection(zMin+1)
                                               , AutoSegment::Local
                                               );
    DbU::Unit axis = (segment->isHorizontal()) ? position.getY() : position.getX();
    segment->setLayer       ( _routingGauge->getRoutingLayer(zMin+1) );
    segment->setAxis        ( axis );
    segment->setSlackened   ( true );
    segment->setLayerChange ( true );
    ltrace(200) << "Corner @" << DbU::getValueString(axis) << " " << segment << endl;

    if ( vExtended ) _contact->setVAlignate ( true );
    if ( hExtended ) _contact->setHAlignate ( true );

    AutoContact* secondary = NULL;
    if ( zMax-zMin == 3 ) {
      secondary = AutoContact::create ( _contact->getGCell()
                                      , _contact->getNet()
                                      , _routingGauge->getContactLayer(zMin+2)
                                      );
      ltrace(200) << "Secondary " << secondary << endl;

      segment = AutoSegment::create ( corner
                                    , secondary
                                    , _routingGauge->getLayerDirection(zMin+2)
                                    , AutoSegment::Local
                                    );
      axis = (segment->isHorizontal()) ? position.getY() : position.getX();
      segment->setLayer       ( _routingGauge->getRoutingLayer(zMin+2) );
      segment->setAxis        ( axis );
      segment->setCanonical   ( true );
      segment->setSlackened   ( true );
      segment->setLayerChange ( true );
      ltrace(200) << "Secondary @" << DbU::getValueString(axis) << " " << segment << endl;
    } else
      secondary = corner;

    for ( size_t i=0 ; i<hTop.size() ; i++ ) hTop[i]->getHook()->attach ( secondary->getBodyHook() );
    for ( size_t i=0 ; i<vTop.size() ; i++ ) vTop[i]->getHook()->attach ( secondary->getBodyHook() );

    if ( _contact->isVAlignate() ) secondary->setVAlignate(true);
    if ( _contact->isHAlignate() ) secondary->setHAlignate(true);

    if ( hExtended ) {
      ltrace(200) << "Original was H extended: restore V connexity." << endl;
      _contact ->restoreVConnexity ( position.getY(), true );
      secondary->restoreVConnexity ( position.getY(), true );
    }

    if ( vExtended ) {
      ltrace(200) << "Original was V extended: restore H connexity." << endl;
      _contact ->restoreHConnexity ( position.getX(), true );
      secondary->restoreHConnexity ( position.getX(), true );
    }

    ltraceout(200);
  }


  void  JunctionBox::splitTerminal ()
  {
    ltrace(200) << "JunctionBox::splitTerminal(): AutoSegment connected to RoutingPad." << endl;
    ltracein(200);
    Point position = _contact->getPosition ();

    _contact->getAnchorHook()->detach ();

    RoutingPad* routingPad = dynamic_cast<RoutingPad*> ( _anchor );
    if ( !routingPad ) {
      cerr << Bug("JunctionBox::splitTerminal(): %s is not anchored on a <RoutingPad>."
                 ,getString(_contact).c_str()) << endl;
      ltraceout(200);
      return;
    }

    AutoContact* rpContact = AutoContact::fromRp ( _contact->getGCell()
                                                 , routingPad
                                                 , _routingGauge->getContactLayer(0)
                                                 , position
                                                 , DbU::lambda(1.0), DbU::lambda(1.0)
                                                 );
    AutoContact* via23 = AutoContact::create ( _contact->getGCell()
                                             , _contact->getNet()
                                             , _routingGauge->getContactLayer(1)
                                             );

    AutoSegment* segment = AutoSegment::create ( rpContact
                                               , via23
                                               , Constant::Horizontal
                                               , AutoSegment::Local
                                               , true  // terminal.
                                               , false // Temporary: *not* collapsed.
                                               );
    segment->setLayer ( _routingGauge->getRoutingLayer(1) );
    segment->setAxis  ( position.getY() );
    ltrace(200) << "@" << DbU::getValueString(position.getY()) << segment << endl;

    segment = AutoSegment::create ( via23
                                  , _contact
                                  , Constant::Vertical
                                  , AutoSegment::Local
                                  , false // terminal.
                                  , false //true  // collapsed.
                                  );
    segment->setAxis   ( position.getX() );
    segment->setLayer  ( _routingGauge->getRoutingLayer(2) );
    _contact->setLayer ( _routingGauge->getContactLayer(1) );
    ltrace(200) << "@" << DbU::getValueString(position.getX()) << segment << endl;

    ltraceout(200);
  }


  void  JunctionBox::breakUpPunctual ()
  {
    Component*      anchor = _contact->getAnchor();
    Point           center;
    StackedContact* stackedContact;

    if ( isEmpty() ) center = _contact->getCenter();
    else if ( isPunctual() ) {
      center.setX ( _xMin );
      center.setY ( _yMin );
    } else {
      center.setX ( (_xMin+_xMax)/2 );
      center.setY ( (_yMin+_yMax)/2 );
    }

    const Layer* layer = NULL;
    if      ( !_localEnds.empty () ) layer = _localEnds [0]->getLayer();
    else if ( !_globalEnds.empty() ) layer = _globalEnds[0]->getLayer();
    else if ( anchor )               layer = anchor->getLayer ();

    stackedContact = StackedContact::create ( _contact->getNet()
                                            , layer
                                            , center.getX()
                                            , center.getY()
                                            );

    if ( anchor ) stackedContact->setAnchor ( anchor );

    for ( size_t i=0 ; i < _globalEnds.size() ; i++ )
      stackedContact->attachSlave ( _globalEnds[i] );

    for ( size_t i=0 ; i < _localEnds.size() ; i++ )
      stackedContact->attachSlave ( _localEnds[i] );

    stackedContact->breakUp ();
    stackedContact->destroy ();
    ltrace(99) << "JunctionBox::breakUpPunctual() succeded" << endl;
  }


  void  JunctionBox::breakUp ()
  {
    ltrace(99) << "JunctionBox::breakUp() - <Box "
               << DbU::getLambda(_xMin) << " " << DbU::getLambda(_yMin) << " "
               << DbU::getLambda(_xMax) << " " << DbU::getLambda(_yMax)
               << ">" << endl;

    if ( !_wireCount ) {
      cerr << Warning("Standalone %s skipped.",getString(_contact).c_str()) << endl;
      return;
    }

    if ( _failsafe or isPunctual() )
      breakUpPunctual ();
    else {
      if ( not _globalStem ) {
        cerr << Bug ( "JunctionBox::breakUp(): Flatten geometry whitout global stem,\n"
                      "      on %p:%s."
                    , _contact->base()
                    , getString(_contact).c_str()
                    ) << endl;
        breakUpPunctual ();
      } else {
      // Flat geometry.
        vector<StackedContact*>  stackedContacts;

        _globalStem->split ( stackedContacts );
        for ( size_t i=0 ; i < stackedContacts.size() ; i++ ) {
          stackedContacts[i]->breakUp ();
          stackedContacts[i]->destroy ();
        }
      }
    }
    ltrace(99) << "JunctionBox::breakUp() succeded" << endl;
  }


  void  JunctionBox::updateContacts ( VirtualContacts& vcs )
  {
    ltrace(109) << "JunctionBox::updateContacts()" << endl;
    ltracein(109);

    vcs.clear ();

    Component*  anchor = _contact->getAnchor ();
    RoutingPad* rp     = dynamic_cast<RoutingPad*>(anchor);
    if ( rp )
      vcs.merge ( Point(_xMin,_yMin), rp->getLayer() );

    vector<SegmentEnd*>::iterator it  = _localEnds.begin();
    for ( ; it != _localEnds.end() ; it++ )
      vcs.merge ( (*it)->getEnd(), (*it)->getLayer() ); 
     
    for ( it = _globalEnds.begin() ; it != _globalEnds.end() ; it++ )
      vcs.merge ( (*it)->getEnd(), (*it)->getLayer() );

    ltraceout(109);
  }


  StackedContact::~StackedContact ()
  {
    ltrace(99) << "StackedContact::~StackedContact() - " << (void*)this << endl;
  }


  StackedContact::StackedContact ( Net* net, const Layer* layer, DbU::Unit x, DbU::Unit y )
    : Contact(net,layer,x,y)
    , _useds(Session::getRoutingGauge()->getDepth())
    , _anchor(NULL)
  {
    for ( unsigned index = 0 ; index < Session::getRoutingGauge()->getDepth() ; index++ )
      _useds [ index ] = false;

    forEach ( BasicLayer*, basicLayer, getLayer()->getBasicLayers() )
      addLayer ( *basicLayer );
  }


  StackedContact* StackedContact::create ( Net* net, const Layer* layer, DbU::Unit x, DbU::Unit y )
  {
    StackedContact* contact = new StackedContact ( net, layer, x, y );
    contact->_postCreate ();

    ltrace(99) << "create: " << contact << endl;

    return contact;
  }


  void  StackedContact::addLayer ( const Layer* layer )
  {
    if ( !Session::getTechnology()->isMetal(layer) ) return;

    unsigned int index = Session::getRoutingGauge()->getLayerDepth ( layer );
    if ( index == UINT_MAX ) return;

    ltrace(99) << "StackedContact::addLayer() - " << layer << " [" << index << "]" << endl;

    _useds [ index ]  = true;
  }


  void  StackedContact::setAnchor ( Component* anchor )
  {
    if ( (_anchor=anchor) ) {
      forEach ( BasicLayer*, layer, _anchor->getLayer()->getBasicLayers() )
        addLayer ( *layer );
    }
  }


  void  StackedContact::attachSlave ( SegmentEnd* segmentEnd )
  {
    ltrace(88) << "add at " << getCenter() << " " << segmentEnd->getLayer() << endl;

    addLayer ( segmentEnd->getLayer() );

    segmentEnd->setDelta ( 0 );

    Hook* hook = segmentEnd->getHook();
    hook->detach ();
    hook->attach ( getBodyHook() );
  }


  void  StackedContact::breakUp ()
  {
    ltrace(99) << "StakedContact::breakUp() - " << this << endl;

    unsigned int  zMin     = 0;
    unsigned int  zMax     = Session::getRoutingGauge()->getDepth() - 1;
    Contact*      contacts [ zMax+1 ];

    for ( unsigned int i=0 ; i <= zMax ; i++ ) contacts[i] = NULL;

    for ( ; (zMin <= zMax) && !_useds[zMin] ; zMin++ );
    for ( ; (zMax > 0    ) && !_useds[zMax] ; zMax-- );

    ltrace(99) << "Contact depth span [" << zMin << ":" << zMax << "]" << endl;

    if ( zMin >  zMax ) return;
    if ( zMin == zMax ) {
      if ( _anchor )
        contacts[zMin] = Contact::create ( _anchor
                                         , Session::getRoutingGauge()->getRoutingLayer(zMin)
                                         , getX() - _anchor->getX()
                                         , getY() - _anchor->getY()
                                         , DbU::lambda(1.0), DbU::lambda(1.0)
                                         );
      else
        contacts[zMin] = Contact::create ( getNet() 
                                         , Session::getRoutingGauge()->getRoutingLayer(zMin)
                                         , getX()
                                         , getY()
                                         , DbU::lambda(1.0), DbU::lambda(1.0)
                                         );
      ltrace(88) << "Creating [" << zMin << "] " << contacts[zMin] << endl;
    } else {
      if ( _anchor )
        contacts[zMin] = contacts[zMin+1]
                       = Contact::create ( _anchor
                                         , Session::getRoutingGauge()->getContactLayer(zMin)
                                         , getX() - _anchor->getX()
                                         , getY() - _anchor->getY()
                                         , DbU::lambda(1.0), DbU::lambda(1.0)
                                         );
      else
        contacts[zMin] = contacts[zMin+1]
                       = Contact::create ( getNet() 
                                         , Session::getRoutingGauge()->getContactLayer(zMin)
                                         , getX()
                                         , getY()
                                         , DbU::lambda(1.0), DbU::lambda(1.0)
                                         );
      ltrace(88) << "Creating [" << zMin << "] " << contacts[zMin] << endl;

      for ( unsigned int j = zMin+1 ; j < zMax ; j++ ) {
        contacts[j] = contacts[j+1]
                    = Contact::create ( contacts[j-1]
                                      , Session::getRoutingGauge()->getContactLayer(j)
                                      , 0
                                      , 0
                                      , DbU::lambda(1.0), DbU::lambda(1.0)
                                      );
        ltrace(88) << "Creating [" << j << "] " << contacts[j] << endl;
      }
    }

    Hook* bodyHook = getBodyHook ();
    Hook* currHook = bodyHook->getNextHook ();

    while ( currHook != bodyHook ) {
      Hook* nextHook = currHook->getNextHook ();

      currHook->_setNextHook ( currHook );
      Segment* segment = dynamic_cast<Segment*>(currHook->getComponent());
      ltrace(88) << "Reattach [" << Session::getRoutingGauge()->getLayerDepth(segment->getLayer())
                 << "] " << segment << endl;
      currHook->attach ( contacts[Session::getRoutingGauge()->getLayerDepth(segment->getLayer())]->getBodyHook() );

      currHook = nextHook;
    }

    bodyHook->_setNextHook ( bodyHook );
    ltrace(99) << "StakedContact::breakUp() succeeded" << endl;
  }


// -------------------------------------------------------------------
// Class  :  "::FixedJunctionBox".


  class FixedJunctionBox {

    public:
    // Constructor & Destructor.
                    FixedJunctionBox     ( AutoContact*, bool checking );
                   ~FixedJunctionBox     ();
    // Modifiers.
             void   checkTopology   ();
             void   merge           ( Hook* );
             void   resize          ();
             void   breakUp         ();

    protected:
    // Attributes.
      AutoContact*         _contact;
      vector<SegmentEnd*>  _segmentEnds;
      bool                 _checking;
  };


  FixedJunctionBox::FixedJunctionBox ( AutoContact* contact, bool checking )
    : _contact(contact)
    , _segmentEnds()
    , _checking(checking)
  {
    ltrace(110) << "FixedJunctionBox() " << contact << endl;
    ltracein(109);

    forEach ( Hook*, hook, _contact->getBodyHook()->getSlaveHooks() ) merge ( *hook );

    ltraceout(109);
  }
  

  FixedJunctionBox::~FixedJunctionBox ()
  {
    for ( size_t i=0 ; i<_segmentEnds.size() ; i++ ) delete _segmentEnds[i];
  }


  void  FixedJunctionBox::checkTopology ()
  { }


  void  FixedJunctionBox::merge ( Hook* hook )
  {
    SegmentEnd* segmentEnd = SegmentEnd::create ( hook, _checking );
    if ( !segmentEnd ) return;

    ltrace(109) << "merge() Axis "
                << DbU::getLambda(segmentEnd->getAxis())
                << " [global] " << segmentEnd << endl;
    _segmentEnds.push_back ( segmentEnd );
  }


  void  FixedJunctionBox::resize ()
  {
    for ( size_t i=0 ; i<_segmentEnds.size() ; i++ ) _segmentEnds[i]->setDelta(0);
  }


  void  FixedJunctionBox::breakUp ()
  {
    Component*      anchor = _contact->getAnchor();
    StackedContact* stackedContact;

    const Layer* layer = NULL;
    if      ( !_segmentEnds.empty () ) layer = _segmentEnds[0]->getLayer();
    else if ( anchor )                 layer = anchor->getLayer ();

    stackedContact = StackedContact::create ( _contact->getNet()
                                            , layer
                                            , _contact->getX()
                                            , _contact->getY()
                                            );

    if ( anchor ) stackedContact->setAnchor ( anchor );

    for ( size_t i=0 ; i < _segmentEnds.size() ; i++ )
      stackedContact->attachSlave ( _segmentEnds[i] );

    stackedContact->breakUp ();
    stackedContact->destroy ();
    ltrace(99) << "FixedJunctionBox::breakUp() succeded" << endl;
  }


} // End of local namespace.




namespace Katabatic {


// -------------------------------------------------------------------
// Class  :  "Katabatic::VirtualContacts::VC".


  bool operator== ( const VirtualContacts::VC& lhs
                  , const VirtualContacts::VC& rhs )
  {
    return lhs._point == rhs._point;
  }


  void  VirtualContacts::VC::merge ( const Layer* layer )
  {
    if ( _layer->contains(layer) ) return;
    Layer *newLayer = Session::getTechnology()->getLayer ( _layer->getMask() & layer->getMask() );

    if ( !newLayer ) return;
    _layer = newLayer;
  }


  void  VirtualContacts::merge ( const Point& point, const Layer* layer )
  {
    VC  vc ( point, layer );

    ltrace(109) << "VirtualContacts::merge() " << point << " " << layer << endl;

    vector<VC>::iterator  it  = _vcs.begin();
    vector<VC>::iterator  end = _vcs.end();

    for ( ; it != end ; it++ ) {
      if ( *it == vc ) { (*it).merge ( layer ); return; }
    }

    _vcs.push_back ( vc );
    _boundingBox.merge ( Box(point).inflate(DbU::lambda(1.0)) );
  }


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContact".


  size_t      AutoContact::_maxId       = 0;
  size_t      AutoContact::_allocateds  = 0;
  const Name  AutoContact::_goName      = "Katabatic::AutoContact";


  AutoContact* AutoContact::fromRp ( GCell*       gcell
                                   , RoutingPad*  routingPad
                                   , const Layer* layer
                                   , Point        point
                                   , DbU::Unit    width
                                   , DbU::Unit    height
                                   , bool         fixed
                                   )
  {
    routingPad->getBodyHook()->detach ();

    DbU::Unit  x      = 0;
    DbU::Unit  y      = 0;
    Entity*    entity = routingPad->getOccurrence().getEntity();

    // Assumes there is no rotation in the Transformation.
    if      ( dynamic_cast<Horizontal*>(entity) ) { x = point.getX(); }
    else if ( dynamic_cast<Vertical*  >(entity) ) { y = point.getY(); }

    return AutoContact::create ( gcell
                               , routingPad
                               , layer
                               , x, y
                               , width, height
                               , false, false
                               , fixed
                               );
  }


  AutoContact::AutoContact ( GCell*   gcell
                           , Contact* contact
                           , bool     hAlignate
                           , bool     vAlignate
                           )
    : ExtensionGo     (contact->getCell())
    , _id             (_maxId++)
    , _contact        (contact)
    , _gcell          (gcell)
    , _invalid        (false)
    , _invalidTopology(true)
    , _isTerminal     (false)
    , _fixed          (false)
    , _hAlignate      (hAlignate)
    , _vAlignate      (vAlignate)
    , _isCorner       (false)
    , _dxMin          (0)
    , _dxMax          (_gcell->getXMax()-_gcell->getX())
    , _dyMin          (0)
    , _dyMax          (_gcell->getYMax()-_gcell->getY())
    , _subContacts    ()
  {
    _allocateds++;
    _gcell->addContact ( this );
  }


  void  AutoContact::_postCreate ()
  {
    ExtensionGo::_postCreate ();

    restoreNativeConstraintBox ();

    ltrace(90) << "Native CBox: " << this
               << " <" << DbU::getLambda(getCBXMin())
               << " "  << DbU::getLambda(getCBYMin())
               << " "  << DbU::getLambda(getCBXMax())
               << " "  << DbU::getLambda(getCBYMax()) << ">" << endl;
    
    Session::link ( this );
    invalidate ();

    ltrace(90) << "AutoContact::_postCreate() - " << this << " in " << _gcell << endl;
  }


  void  AutoContact::_preDestroy ()
  {
    DebugSession::open ( _contact->getNet() );

    ltrace(90) << "AutoContact::_preDestroy() - <AutoContact id:" << _id << ">" << endl;

    if ( not _contact->getSlaveComponents().isEmpty() ) {
      cerr << Error("Base contact still have slaves components, cancelled.\n"
                   "        (%s)"
                   ,_getString().c_str()) << endl;
      DebugSession::close ();
      return;
    }

    if ( not Session::doDestroyTool() ) {
      _gcell->removeContact ( this );
      Session::unlink ( this );
    }
        
    ExtensionGo::_preDestroy ();
    if ( Session::doDestroyBaseContact() )
      _contact->destroy ();

    DebugSession::close ();
  }


  AutoContact::~AutoContact ()
  {
    _allocateds--;
  }


  AutoContact* AutoContact::create (       GCell* gcell
                                   ,       Net*   net
                                   , const Layer* layer
                                   ,       bool   hAlignate
                                   ,       bool   vAlignate
                                   )
  {
    Contact* contact = Contact::create ( net
                                       , layer
                                       , gcell->getCenter().getX()
                                       , gcell->getCenter().getY()
                                       , DbU::lambda(2.0)
                                       , DbU::lambda(2.0)
                                       );
    AutoContact* autoContact = new AutoContact ( gcell, contact, hAlignate, vAlignate );

    autoContact->_postCreate ();

    ltrace(90) << "create(net*) " << autoContact << endl;
    return autoContact;
  }


  AutoContact* AutoContact::create (       GCell*      gcell
                                   ,       RoutingPad* rp
                                   , const Layer*      layer
                                   , const DbU::Unit   dx
                                   , const DbU::Unit   dy
                                   , const DbU::Unit   width
                                   , const DbU::Unit   height
                                   ,       bool        hAlignate
                                   ,       bool        vAlignate
                                   ,       bool        fixed
                                   )
  {
    Contact* contact = Contact::create ( rp
                                       , layer
                                       , dx /*- rp->getX()*/
                                       , dy /*- rp->getY()*/
                                       , width
                                       , height
                                       );
    AutoContact* autoContact = new AutoContact ( gcell, contact, hAlignate, vAlignate );
    
    autoContact->setFixed    ( fixed );
    autoContact->setTerminal ( true );
    autoContact->_postCreate ();

    ltrace(90) << "create(RoutingPad*) " << autoContact << endl;
    return autoContact;
  }


  bool  AutoContact::canDestroy ( bool error ) const
  {
    if ( not _contact->getSlaveComponents().isEmpty() ) {
      if ( error ) {
        cerr << Error("Base contact still have slaves components, cancelled.\n"
                     "        (%s)"
                     ,_getString().c_str()) << endl;
      }
      return false;
    }
    return true;
  }


  size_t  AutoContact::getSegmentEndAllocateds ()
  {
    return SegmentEnd::getAllocateds();
  }


  size_t  AutoContact::getAllocateds ()
  {
    return _allocateds;
  }


  const Name& AutoContact::getStaticName ()
  {
    return _goName;
  }


  const Name& AutoContact::getName () const
  {
    return _goName;
  }


  unsigned int  AutoContact::getMinDepth () const
  {
    size_t minDepth = (size_t)-1;
    Component* anchor = getAnchor ();
    if ( anchor ) {
      minDepth = min ( minDepth, Session::getRoutingGauge()->getLayerDepth(anchor->getLayer()) );
    //ltrace(200) << "Anchor:" << anchor << endl;
    }

    forEach ( Component*, icomponent, getSlaveComponents() ) {
      minDepth = min ( minDepth, Session::getRoutingGauge()->getLayerDepth(icomponent->getLayer()) );
    //ltrace(200) << "Slave:" << *icomponent << endl;
    }

    return (unsigned int)minDepth;
  }


  void  AutoContact::getLengths ( DbU::Unit* lengths, set<AutoSegment*>& processeds )
  {
    forEach ( Hook*, ihook, getBodyHook()->getSlaveHooks() ) {
      bool isSourceHook = (dynamic_cast<Segment::SourceHook*>(*ihook));
      if ( !isSourceHook && ( dynamic_cast<Segment::TargetHook*>(*ihook) == NULL ) ) {
        cerr << Error ( badHookType, getString(ihook->getComponent()).c_str() ) << endl;
        return;
      }

      bool     isHorizontal = true;
      Segment* segment      = dynamic_cast<Horizontal*>((*ihook)->getComponent());
      if ( !segment ) {
        isHorizontal = false;
        segment      = dynamic_cast<Vertical*>((*ihook)->getComponent());
        if ( !segment )
          continue;
      }

      AutoSegment* autoSegment = Session::lookup ( segment );
      if ( not autoSegment or processeds.find(autoSegment) != processeds.end() )
        continue;
      processeds.insert ( autoSegment );

      size_t     depth  = Session::getRoutingGauge()->getLayerDepth(segment->getLayer());
      DbU::Unit  length;
      if ( autoSegment->isLocal() ) {
        length = segment->getLength();
        lengths[depth] += length;
        if ( abs(length) >= DbU::lambda(50.0) )
          cerr << Error("Supicious length:%.2f of %s."
                       ,DbU::getLambda(length),getString(autoSegment).c_str()) << endl;
      } else {
        if ( isHorizontal ) {
          if ( isSourceHook )
            lengths[depth] += _gcell->getBoundingBox().getXMax() - segment->getSourceX();
          else
            lengths[depth] += segment->getTargetX() - _gcell->getBoundingBox().getXMin();
        } else {
          if ( isSourceHook )
            lengths[depth] += _gcell->getBoundingBox().getYMax() - segment->getSourceY();
          else
            lengths[depth] += segment->getTargetY() - _gcell->getBoundingBox().getYMin();
        }
      }
    }
  }


  Box  AutoContact::getNativeConstraintBox () const
  {
  //if ( _fixed ) return Box(getCenter());

    Component* component = getAnchor();
    if ( component == NULL ) return _gcell->getBoundingBox ();
    
    DbU::Unit   xMin;
    DbU::Unit   xMax;
    DbU::Unit   yMin;
    DbU::Unit   yMax;
    Vertical*   vertical;
    Horizontal* horizontal;
    RoutingPad* routingPad;

    if ( (horizontal = dynamic_cast<Horizontal*>(component))) {
      xMin = horizontal->getSourcePosition().getX();
      xMax = horizontal->getTargetPosition().getX();
      yMin = yMax
           = horizontal->getTargetPosition().getY();
    } else if ( (vertical = dynamic_cast<Vertical*>(component)) ) {
      yMin = vertical->getSourcePosition().getY();
      yMax = vertical->getTargetPosition().getY();
      xMin = xMax
           = vertical->getTargetPosition().getX();
    } else if ( (routingPad = dynamic_cast<RoutingPad*>(component)) ) {
      Entity* entity = routingPad->getOccurrence().getEntity();

    // Assumes there is no rotation in the Transformation.
      if ( dynamic_cast<Horizontal*>(entity) ) {
        xMin = routingPad->getSourcePosition().getX();
        xMax = routingPad->getTargetPosition().getX();
        yMin = yMax
             = routingPad->getTargetPosition().getY();
      } else if ( dynamic_cast<Vertical*>(entity) ) {
        yMin = routingPad->getSourcePosition().getY();
        yMax = routingPad->getTargetPosition().getY();
        xMin = xMax
             = routingPad->getTargetPosition().getX();
      } else {
        xMin = xMax = routingPad->getPosition().getX();
        yMin = yMax = routingPad->getPosition().getY();
      }
    } else {
      xMin = xMax = component->getPosition().getX();
      yMin = yMax = component->getPosition().getY();
    }

    order ( xMin, xMax );
    order ( yMin, yMax );

    return Box ( xMin, yMin, xMax, yMax );
  }


  Interval  AutoContact::getUConstraints ( unsigned int direction ) const
  {
    if ( direction == Constant::Horizontal ) {
      return Interval ( getCBYMin(), getCBYMax() );
    }
    return Interval ( getCBXMin(), getCBXMax() );
  }


  AutoContacts  AutoContact::getCollapseds ( unsigned int direction )
  {
    return AutoContacts_Collapsed ( this, direction );
  }


  void  AutoContact::invalidate ()
  {
    if ( !isInvalidated() ) {
      ltrace(110) << "AutoContact::invalidate() - " << this << endl;
      setInvalidated ( true );
      Session::invalidate ( this );
      getGCell()->invalidate ();
    }
  }


  void  AutoContact::revalidate ()
  {
    updateGeometry ();
    if ( _invalidTopology )
      revalidateTopology ();
  }


  void  AutoContact::updateGeometry ()
  {
    DebugSession::open ( getNet(), 80 );

    ltrace(110) << "AutoContact::updateGeometry() " << this << endl;
    ltracein(110);

    _contact->invalidate ( false );
    setInvalidated ( false );

    if ( isFixed() ) {
      FixedJunctionBox junctionBox ( this, false );
      junctionBox.resize ();
    } else {
      JunctionBox junctionBox ( this, false );
      junctionBox.resize ();
      junctionBox.updateContacts ( _subContacts );
    }
#if defined(CHECK_DATABASE)
    checkTopology();
#endif

    ltraceout(110);

    DebugSession::close ();
  }


  void  AutoContact::setGCell ( GCell* gcell )
  {
    invalidate ();
    if ( _gcell ) _gcell->removeContact ( this );

    _gcell = gcell;
    if ( _gcell ) {
      _gcell->addContact ( this );
      _contact->setPosition ( _gcell->getCenter() );
      _dxMin = 0;
      _dyMin = 0;
      _dxMax = _gcell->getXMax()-_gcell->getX();
      _dyMax = _gcell->getYMax()-_gcell->getY();
    } else {
      cerr << Bug("NULL GCell for %p:%s.",_contact,_getString().c_str()) << endl;
    }
  }


  void  AutoContact::breakUp ()
  {
    if ( isFixed() ) {
      FixedJunctionBox(this,false).breakUp();
    } else {
      JunctionBox(this,false).breakUp();
    }
    ltrace(110) << "AutoContact::breakUp() succeded" << endl;
  }


  void  AutoContact::split ()
  {
    DebugSession::open ( getNet() );
    JunctionBox(this,false).split();
    DebugSession::close ();
  }


  void  AutoContact::checkTopology ()
  {
    ltrace(110) << "checkTopology() " << this << endl;

    if ( isFixed() ) {
      FixedJunctionBox(this,true).checkTopology();
    } else {
      JunctionBox(this,true).checkTopology();
    }
  }


  void  AutoContact::revalidateTopology ()
  {
    ltrace(110) << "revalidateTopology() " << this << endl;

    JunctionBox(this,true).revalidateTopology();
  }


  bool  AutoContact::isAlignate ( unsigned int direction ) const
  {
    return (_hAlignate && (direction == Constant::Horizontal))
        || (_vAlignate && (direction == Constant::Vertical  ));
  }


  bool  AutoContact::isHExtended ()
  { return JunctionBox(this,true).isHExtended(); }


  bool  AutoContact::isVExtended ()
  { return JunctionBox(this,true).isVExtended(); }


  void  AutoContact::computeAlignate ()
  {
    if ( !isFixed() )
      JunctionBox(this,true).computeAlignate();
  }


  bool  AutoContact::canGoOutsideGCell ( const AutoSegment* segment )
  { return JunctionBox(this,true).canGoOutside(segment); }


  bool  AutoContact::canHDesalignate ()
  { return JunctionBox(this,true).canHDesalignate(); }


  bool  AutoContact::canVDesalignate ()
  { return JunctionBox(this,true).canVDesalignate(); }


  bool  AutoContact::hDesalignate ()
  {
    bool desalignate = JunctionBox(this,true).canHDesalignate();

    if ( desalignate ) setHAlignate ( false );
    return desalignate;
  }


  bool  AutoContact::vDesalignate ()
  {
    bool desalignate = JunctionBox(this,true).canVDesalignate();

    if ( desalignate ) setVAlignate ( false );
    return desalignate;
  }


  void  AutoContact::restoreHConnexity ( DbU::Unit x, bool split )
  { JunctionBox(this,true).restoreHConnexity ( x, split ); }


  void  AutoContact::restoreVConnexity ( DbU::Unit y, bool split )
  { JunctionBox(this,true).restoreVConnexity ( y, split ); }


  bool  AutoContact::canMoveUp ( AutoSegment* moved ) const
  {
    ltrace(200) << "AutoContact::canMoveUp() " << this << endl;
    size_t viaDepth = 100;

    RoutingGauge* rg         = Session::getRoutingGauge();
    size_t        movedDepth = rg->getLayerDepth(moved->getLayer());

    Component* anchor = getAnchor ();
    if ( anchor ) {
      viaDepth = rg->getLayerDepth(anchor->getLayer());
      ltrace(200) << "| Anchor depth: " << viaDepth << endl;
    }

    forEach ( Segment*, isegment, _contact->getSlaveComponents().getSubSet<Segment*>() ) {
      if ( *isegment == moved->base() ) continue;

      size_t depth = rg->getLayerDepth(isegment->getLayer());
      if ( viaDepth == 100 ) viaDepth = depth;
      else
        if ( viaDepth != depth ) return false;

      ltrace(200) << "| Segment depth: " << depth << endl;
    }

    return ( movedDepth+1 == viaDepth );
  }


  void  AutoContact::setConstraintBox ( const Box& box )
  {
    setCBXMin ( box.getXMin() );
    setCBXMax ( box.getXMax() );
    setCBYMin ( box.getYMin() );
    setCBYMax ( box.getYMax() );
    ltrace(110) << "setConstraintBox() - " << this << " " << getConstraintBox() << endl;
    ltrace(110) << "* " << _gcell << endl;
  }


  bool  AutoContact::restrictConstraintBox ( DbU::Unit    constraintMin
                                           , DbU::Unit    constraintMax
                                           , unsigned int direction
                                           , bool         warnOnError )
  {
    if ( direction & Constant::Horizontal ) {
      if ( (constraintMin > getCBYMax()) or (constraintMax < getCBYMin()) ) {
        if ( Session::getDemoMode() or not warnOnError ) return false;

        cerr << Error ( "Incompatible DY restriction on %s", _getString().c_str() ) << endl;
        if ( constraintMin > getCBYMax() )
          cerr << Error ( "(constraintMin > CBYMax : %.2lf > %.2lf)"
                        , DbU::getLambda(constraintMin)
                        , DbU::getLambda(getCBYMax()) )
               << endl;
        if ( constraintMax < getCBYMin() )
          cerr << Error ( "(constraintMax < CBYMin : %.2lf < %.2lf)"
                        , DbU::getLambda(constraintMax)
                        , DbU::getLambda(getCBYMin()) )
               << endl;
        return false;
      }
      setCBYMin ( max(getCBYMin(),constraintMin) );
      setCBYMax ( min(getCBYMax(),constraintMax) );
    } else if ( direction & Constant::Vertical ) {
      if ( (constraintMin > getCBXMax()) || (constraintMax < getCBXMin()) ) {
        if ( Session::getDemoMode() or not warnOnError ) return false;

        cerr << Error ( "Incompatible DX restriction on %s", _getString().c_str() ) << endl;
        if ( constraintMin > getCBXMax() )
          cerr << Error ( "(constraintMin > CBXMax : %.2lf > %.2lf)"
                        , DbU::getLambda(constraintMin)
                        , DbU::getLambda(getCBXMax()) )
               << endl;
        if ( constraintMax < getCBXMin() )
          cerr << Error ( "(constraintMax < CBXMin : %.2lf < %.2lf)"
                        , DbU::getLambda(constraintMax)
                        , DbU::getLambda(getCBXMin()) )
               << endl;
        return false;
      }
      setCBXMin ( max(getCBXMin(),constraintMin) );
      setCBXMax ( min(getCBXMax(),constraintMax) );
    }
    ltrace(110) << "restrictConstraintBox() - " << this << " " << getConstraintBox() << endl;
    return true;
  }


  void  AutoContact::restoreNativeConstraintBox ()
  {
    setConstraintBox ( getNativeConstraintBox() );
  }

                 
  Box& AutoContact::intersectConstraintBox ( Box& box ) const
  {
    return box = box.getIntersection ( getConstraintBox() );
  }


  Box  AutoContact::getBoundingBox () const
  {
    return _gcell->getBoundingBox ();
  }


  void  AutoContact::translate ( const DbU::Unit& tx, const DbU::Unit& ty )
  {
    cerr << Warning("Calling AutoContact::translate() is likely a bug.") << endl;
    _contact->translate ( tx, ty );
  }


  string  AutoContact::_getString () const
  {
    string  s = _contact->_getString();
    s.insert ( 1, "id: " );
    s.insert ( 4, getString(_id) );
    s.insert ( s.size()-1, (_fixed     )?" F":" -" );
    s.insert ( s.size()-1, (_isTerminal)? "t": "-" );
    s.insert ( s.size()-1, (_hAlignate) ? "h": "-" );
    s.insert ( s.size()-1, (_vAlignate) ? "v": "-" );
    s.insert ( s.size()-1, (_invalid)   ? "i": "-" );

    // Point p = _contact->getCenter();
    // s.insert ( s.size()-1, " [" );
    // s.insert ( s.size()-1, DbU::getValueString(p.getX()) );
    // s.insert ( s.size()-1, ":" );
    // s.insert ( s.size()-1, DbU::getValueString(p.getY()) );
    // s.insert ( s.size()-1, "]" );
    return s;
  }


  Record* AutoContact::_getRecord () const
  {
    Record* record = _contact->_getRecord ();
    record->add ( getSlot ( "_gcell"        , _gcell             ) );
    record->add ( getSlot ( "_constraintBox", getConstraintBox() ) );
    record->add ( getSlot ( "_fixed"        , _fixed             ) );
    record->add ( getSlot ( "_isTerminal"   , _isTerminal        ) );
    record->add ( getSlot ( "_hAlignate"    , _hAlignate         ) );
    record->add ( getSlot ( "_vAlignate"    , _vAlignate         ) );
    record->add ( getSlot ( "_invalid"      , _invalid           ) );
    return record;
  }


} // End of Katabatic namespace.
