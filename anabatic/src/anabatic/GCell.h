// -*- mode: C++; explicit-buffer-name: "GCell.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/GCell.h"                            |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_GCELL_H
#define  ANABATIC_GCELL_H

#include <vector>
#include <string>
#include <set>
#include "hurricane/Name.h"
#include "hurricane/Box.h"
#include "hurricane/Cell.h"
#include "hurricane/ExtensionGo.h"
namespace Hurricane {
  class Contact;
}
#include "anabatic/Edge.h"


namespace Anabatic {

  using std::string;
  using std::vector;
  using Hurricane::StaticObservable;
  using Hurricane::BaseObserver;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Interval;
  using Hurricane::Box;
  using Hurricane::Net;
  using Hurricane::Entity;
  using Hurricane::Contact;
  using Hurricane::Cell;

  class AnabaticEngine;


// -------------------------------------------------------------------
// Class  :  "GCell".

  class GCell : public ExtensionGo {
    public:
      typedef ExtensionGo  Super;
    public:
      class Observable : public StaticObservable<1> {
        public:
          enum Indexes { Vertex = 0
                       };
        public:
          inline             Observable ();
        private:
                             Observable ( const StaticObservable& );
                 Observable& operator=  ( const StaticObservable& );
      };
    public:
      static        Box             getBorder            ( const GCell*, const GCell* );
    public:                         
      static        GCell*          create               ( AnabaticEngine* );
    public:
      inline        bool            isHFlat              () const;
      inline        bool            isVFlat              () const;
      inline        bool            isFlat               () const;
      inline        bool            isDevice             () const;
      inline        bool            isChannel            () const;
      inline        bool            isStrut              () const;
      inline        bool            isMatrix             () const;
      inline        AnabaticEngine* getAnabatic          () const;
      inline        DbU::Unit       getXMin              () const;
      inline        DbU::Unit       getYMin              () const;
      inline        DbU::Unit       getXMax              ( int shrink=0 ) const;
      inline        DbU::Unit       getYMax              ( int shrink=0 ) const;
      inline        Interval        getSide              ( Flags direction ) const;
      inline        Point           getCenter            () const;
      inline const  vector<Edge*>&  getWestEdges         () const;
      inline const  vector<Edge*>&  getEastEdges         () const;
      inline const  vector<Edge*>&  getNorthEdges        () const;
      inline const  vector<Edge*>&  getSouthEdges        () const;
      inline        Edges           getEdges             () const;
      inline        GCell*          getWest              () const;
      inline        GCell*          getEast              () const;
      inline        GCell*          getSouth             () const;
      inline        GCell*          getNorth             () const;
                    GCell*          getWest              ( DbU::Unit y ) const;
                    GCell*          getEast              ( DbU::Unit y ) const;
                    GCell*          getSouth             ( DbU::Unit x ) const;
                    GCell*          getNorth             ( DbU::Unit x ) const;
                    GCell*          getUnder             ( DbU::Unit x, DbU::Unit y ) const;
      inline        GCell*          getUnder             ( Point p ) const;
                    GCell*          hcut                 ( DbU::Unit y );
                    GCell*          vcut                 ( DbU::Unit x );
                    bool            doGrid               ();
                    Contact*        getGContact          ( Net* );
    // Misc. functions.
      inline const  Flags&          flags                () const;
      inline        Flags&          flags                ();
                    void            _add                 ( Edge* edge, Flags side );
                    void            _remove              ( Edge* edge, Flags side=Flags::AllSides );
                    void            _destroyEdges        ();
    private:                        
                    void            _revalidate          ();
                    void            _moveEdges           ( GCell* dest, size_t ibegin, Flags flags );
    public:                                
    // Observers.
      inline        void            setObserver          ( size_t slot, BaseObserver* );
      template<typename OwnerT>
      inline        OwnerT*         getObserver          ( size_t slot );
      inline        void            notify               ( unsigned int flags );
    // ExtensionGo support.                
      inline  const Name&           staticGetName        (); 
      virtual const Name&           getName              () const;
      virtual       void            translate            ( const DbU::Unit&, const DbU::Unit& );
      virtual       Box             getBoundingBox       () const;
    public:                                              
    // Inspector support.                                
      virtual       string          _getTypeName         () const;
      virtual       string          _getString           () const;
      virtual       Record*         _getRecord           () const;
    protected:                                           
                                    GCell                ( AnabaticEngine*, DbU::Unit xmin, DbU::Unit ymin );
      virtual                      ~GCell                ();
                    GCell*          _create              ( DbU::Unit xmin, DbU::Unit ymin );
      virtual       void            _postCreate          ();
      virtual       void            _preDestroy          ();
    private:                        
                                    GCell                ( const GCell& );
                    GCell&          operator=            ( const GCell& );
    private:
      static  Name             _extensionName;
              Observable       _observable;
              AnabaticEngine*  _anabatic;
              Flags            _flags;
              vector<Edge*>    _westEdges;
              vector<Edge*>    _eastEdges;
              vector<Edge*>    _southEdges;
              vector<Edge*>    _northEdges;
              DbU::Unit        _xmin;
              DbU::Unit        _ymin;
              vector<Contact*> _contacts;
  };


  inline       bool            GCell::isHFlat       () const { return getYMin() == getYMax(); }
  inline       bool            GCell::isVFlat       () const { return getXMin() == getXMax(); }
  inline       bool            GCell::isFlat        () const { return isHFlat() or isVFlat(); }
  inline       bool            GCell::isDevice      () const { return _flags & Flags::DeviceGCell; }
  inline       bool            GCell::isChannel     () const { return _flags & Flags::ChannelGCell; }
  inline       bool            GCell::isStrut       () const { return _flags & Flags::StrutGCell; }
  inline       bool            GCell::isMatrix      () const { return _flags & Flags::MatrixGCell; }
  inline       AnabaticEngine* GCell::getAnabatic   () const { return _anabatic; }
  inline       DbU::Unit       GCell::getXMin       () const { return _xmin; }
  inline       DbU::Unit       GCell::getYMin       () const { return _ymin; }
  inline       Edges           GCell::getEdges      () const { return new GCell_Edges(this); }
  inline const vector<Edge*>&  GCell::getWestEdges  () const { return _westEdges; }
  inline const vector<Edge*>&  GCell::getEastEdges  () const { return _eastEdges; }
  inline const vector<Edge*>&  GCell::getNorthEdges () const { return _northEdges; }
  inline const vector<Edge*>&  GCell::getSouthEdges () const { return _southEdges; }
  inline       GCell*          GCell::getWest       () const { return  _westEdges.empty() ? NULL :  _westEdges[0]->getOpposite(this); }
  inline       GCell*          GCell::getEast       () const { return  _eastEdges.empty() ? NULL :  _eastEdges[0]->getOpposite(this); }
  inline       GCell*          GCell::getSouth      () const { return _southEdges.empty() ? NULL : _southEdges[0]->getOpposite(this); }
  inline       GCell*          GCell::getNorth      () const { return _northEdges.empty() ? NULL : _northEdges[0]->getOpposite(this); }
  inline       GCell*          GCell::getUnder      ( Point p ) const { return getUnder(p.getX(),p.getY()); }
  inline const Flags&          GCell::flags         () const { return _flags; }
  inline       Flags&          GCell::flags         () { return _flags; }

  inline DbU::Unit  GCell::getXMax ( int shrink ) const
  { return _eastEdges.empty() ?       getCell()->getAbutmentBox().getXMax() - shrink
                              : _eastEdges[0]->getOpposite(this)->getXMin() - shrink; }

  inline DbU::Unit  GCell::getYMax ( int shrink ) const
  { return _northEdges.empty() ?        getCell()->getAbutmentBox().getYMax() - shrink 
                               : _northEdges[0]->getOpposite(this)->getYMin() - shrink; }

  inline Point  GCell::getCenter () const
  { return Point( (getXMin()+getXMax())/2, (getYMin()+getYMax())/2); }

  inline Interval  GCell::getSide ( Flags direction ) const
  {
    if (direction.isset(Flags::Vertical)) return Interval( getYMin(), getYMax() );
    return Interval( getXMin(), getXMax() );
  }

  inline void  GCell::setObserver ( size_t slot, BaseObserver* observer )
  { _observable.setObserver( slot, observer ); }

  template<typename OwnerT>
  inline OwnerT* GCell::getObserver ( size_t slot )
  { return _observable.getObserver<OwnerT>(slot); }

  inline void  GCell::notify ( unsigned int flags )
  { _observable.notify( flags ); }

  inline GCell::Observable::Observable () : StaticObservable<1>() { }


// -------------------------------------------------------------------
// Class  :  "GCellSet".

  typedef  std::set< GCell*, Entity::CompareById >  GCellSet;


}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::GCell);

#endif  // ANABATIC_GCELL_H
