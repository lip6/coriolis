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
#include "hurricane/Name.h"
#include "hurricane/Box.h"
#include "hurricane/Cell.h"
#include "hurricane/ExtensionGo.h"
#include "anabatic/Edge.h"


namespace Anabatic {

  using std::string;
  using std::vector;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Interval;
  using Hurricane::Box;
  using Hurricane::Cell;

  class AnabaticEngine;


  class GCell : public ExtensionGo {
    public:
      typedef ExtensionGo  Super;
    public:
      static        Box             getBorder            ( const GCell*, const GCell* );
    public:                         
      static        GCell*          create               ( AnabaticEngine* );
      virtual       void            destroy              ();
    public:
      inline        AnabaticEngine* getAnabatic          () const;
      inline        DbU::Unit       getXMin              () const;
      inline        DbU::Unit       getYMin              () const;
      inline        DbU::Unit       getXMax              () const;
      inline        DbU::Unit       getYMax              () const;
      inline        Interval        getSide              ( Flags direction ) const;
      inline        Point           getCenter            () const;
                    GCell*          hcut                 ( DbU::Unit y );
                    GCell*          vcut                 ( DbU::Unit x );
                    bool            doGrid               ();
      inline const  Flags&          flags                () const;
      inline        Flags&          flags                ();
                    void            _add                 ( Edge* edge, Flags side );
                    void            _remove              ( Edge* edge, Flags side=Flags::AllSides );
    private:                        
                    void            _revalidate          ();
                    void            _moveEdges           ( GCell* dest, size_t ibegin, Flags flags );
    public:                                
    // ExtensionGo support.                
      inline  const Name&         staticGetName        (); 
      virtual const Name&         getName              () const;
      virtual       void          translate            ( const DbU::Unit&, const DbU::Unit& );
      virtual       Box           getBoundingBox       () const;
    public:                                            
    // Inspector support.                              
      virtual       string        _getTypeName         () const;
      virtual       string        _getString           () const;
      virtual       Record*       _getRecord           () const;
    protected:                                         
                                  GCell                ( AnabaticEngine*, DbU::Unit xmin, DbU::Unit ymin );
      virtual                    ~GCell                ();
                    GCell*        _create              ( DbU::Unit xmin, DbU::Unit ymin );
      virtual       void          _postCreate          ();
      virtual       void          _preDestroy          ();
    private:
                                  GCell                ( const GCell& );
                    GCell&        operator=            ( const GCell& );
    private:
      static  Name            _extensionName;
              AnabaticEngine* _anabatic;
              Flags           _flags;
              vector<Edge*>   _westEdges;
              vector<Edge*>   _eastEdges;
              vector<Edge*>   _southEdges;
              vector<Edge*>   _northEdges;
              DbU::Unit       _xmin;
              DbU::Unit       _ymin;
  };


  inline AnabaticEngine* GCell::getAnabatic          () const { return _anabatic; }
  inline DbU::Unit       GCell::getXMin              () const { return _xmin; }
  inline DbU::Unit       GCell::getYMin              () const { return _ymin; }
  inline const Flags&    GCell::flags                () const { return _flags; }
  inline Flags&          GCell::flags                () { return _flags; }

  inline DbU::Unit  GCell::getXMax () const
  { return _eastEdges.empty() ? getCell()->getAbutmentBox().getXMax()
                              : _eastEdges[0]->getOpposite(this)->getXMin(); }

  inline DbU::Unit  GCell::getYMax () const
  { return _northEdges.empty() ? getCell()->getAbutmentBox().getYMax()
                               : _northEdges[0]->getOpposite(this)->getYMin(); }

  inline Point  GCell::getCenter () const
  { return Point( (getXMin()+getXMax())/2, (getYMin()+getYMax())/2); }

  inline Interval  GCell::getSide ( Flags direction ) const
  {
    if (direction.isset(Flags::Vertical)) return Interval( getYMin(), getYMax() );
    return Interval( getXMin(), getXMax() );
  }


}  // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::GCell);

#endif  // ANABATIC_GCELL_H
