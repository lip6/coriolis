// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :            C. Alexandre, J.-M. Caba              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/MetaTransistor.h"           |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Cell.h"


namespace Analog {

  using namespace Hurricane;


  class MetaTransistor : public Cell {
    public:
      typedef Cell Super;
    public:
      static  MetaTransistor* create       ( Library* , const Name& );
      inline  Net*            getSource    () const;
      inline  Net*            getDrain     () const;
      inline  Net*            getGate      () const;
      inline  Net*            getBulk      () const;
      inline  float           getWE        () const;
      inline  float           getLE        () const;
      inline  float           getIDS       () const;
      inline  float           getVGS       () const;
      inline  float           getVDS       () const;
      inline  float           getVBS       () const;
      inline  float           getVG        () const;
      inline  float           getVD        () const;
      inline  float           getVS        () const;
      inline  float           getVB        () const;
      inline  float           getVEG       () const;
      inline  float           getVTH       () const;
      inline  float           getWmin      () const;
      inline  float           getWmax      () const;
      inline  int             getNfing     () const;
      inline  void            setM         ( unsigned int m );
      inline  void            setWE        ( float we );
      inline  void            setLE        ( float le );
      inline  void            setIDS       ( float ids );
      inline  void            setVGS       ( float vgs );
      inline  void            setVDS       ( float vds );
      inline  void            setVBS       ( float vbs );
      inline  void            setVG        ( float vg );
      inline  void            setVD        ( float vd );
      inline  void            setVS        ( float vs );
      inline  void            setVB        ( float vb );
      inline  void            setVEG       ( float veg );
      inline  void            setVTH       ( float vth );
      inline  void            setWmin      ( float wmin );
      inline  void            setWmax      ( float wmax );
      inline  void            setNfing     ( int nfing );
      virtual std::string     _getTypeName () const;
      virtual Record*         _getRecord   () const;
    protected:                          
             void             _postCreate  ();
    private:
                              MetaTransistor ( Library* , const Name& );
    private:
      Net*          _drain;
      Net*          _source;
      Net*          _gate;
      Net*          _bulk;
      Net*          _anonymous;
      unsigned int  _m;
      float         _we , _le;
      float         _ids;
      float         _vgs, _vds, _vbs;
      float         _vg , _vd , _vb , _vs;
      float         _veg;
      float         _vth;
      float         _wmin;
      float         _wmax;
      int           _nfing;
  };

  
  inline Net*   MetaTransistor::getSource  () const           { return _source; }
  inline Net*   MetaTransistor::getDrain   () const           { return _drain; }
  inline Net*   MetaTransistor::getGate    () const           { return _gate; }
  inline Net*   MetaTransistor::getBulk    () const           { return _bulk; }
  inline float  MetaTransistor::getWE      () const           { return _we; }
  inline float  MetaTransistor::getLE      () const           { return _le; }
  inline float  MetaTransistor::getIDS     () const           { return _ids; }
  inline float  MetaTransistor::getVGS     () const           { return _vgs; }
  inline float  MetaTransistor::getVDS     () const           { return _vds; }
  inline float  MetaTransistor::getVBS     () const           { return _vbs; }
  inline float  MetaTransistor::getVG      () const           { return _vg; }
  inline float  MetaTransistor::getVD      () const           { return _vd; }
  inline float  MetaTransistor::getVS      () const           { return _vs; }
  inline float  MetaTransistor::getVB      () const           { return _vb; }
  inline float  MetaTransistor::getVEG     () const           { return _veg; }
  inline float  MetaTransistor::getVTH     () const           { return _vth; }
  inline float  MetaTransistor::getWmin    () const           { return _wmin; }
  inline float  MetaTransistor::getWmax    () const           { return _wmax; }
  inline int    MetaTransistor::getNfing   () const           { return _nfing; }
  inline void   MetaTransistor::setM       ( unsigned int m ) { _m = m; }
  inline void   MetaTransistor::setWE      ( float we )       { _we = we; }
  inline void   MetaTransistor::setLE      ( float le )       { _le = le; }
  inline void   MetaTransistor::setIDS     ( float ids )      { _ids = ids; }
  inline void   MetaTransistor::setVGS     ( float vgs )      { _vgs = vgs; }
  inline void   MetaTransistor::setVDS     ( float vds )      { _vds = vds; }
  inline void   MetaTransistor::setVBS     ( float vbs )      { _vbs = vbs; }
  inline void   MetaTransistor::setVG      ( float vg )       { _vg = vg; }
  inline void   MetaTransistor::setVD      ( float vd )       { _vd = vd; }
  inline void   MetaTransistor::setVS      ( float vs )       { _vs = vs; }
  inline void   MetaTransistor::setVB      ( float vb )       { _vb = vb; }
  inline void   MetaTransistor::setVEG     ( float veg )      { _veg = veg; }
  inline void   MetaTransistor::setVTH     ( float vth )      { _vth = vth; }
  inline void   MetaTransistor::setWmin    ( float wmin )     { _wmin = wmin; }
  inline void   MetaTransistor::setWmax    ( float wmax )     { _wmax = wmax; }
  inline void   MetaTransistor::setNfing   ( int nfing )      { _nfing = nfing; }
  

}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::MetaTransistor);
