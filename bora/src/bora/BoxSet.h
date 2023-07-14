// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      B o r a   -  A n a l o g   S l i c i n g   T r e e         |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./bora/BoxSet.h"                          |
// +-----------------------------------------------------------------+
//
// A dimension defined by a width and a height for slicing nodes.
// - HBoxSet are used for HSlicingNode: 
//     -> width  = max(children's widths), height = sum(children's heights)
// - VBoxSet are used for VSlicingNode:
//     -> width  = sum(children's widths), height = max(children's heights)
// - DBoxSet are used for DSlicingNode:
//     -> width  = device's width, height = device's height
// - RHBoxSet are used for RHSlicingNode:
//     -> height = routing space's height
// - RVBoxSet are used for RVSlicingNode:
//     -> width  = routing space's width


#ifndef BORA_BOX_SET_H
#define BORA_BOX_SET_H

#include <iostream>
#include <vector>
#include "hurricane/DbU.h"
#include "bora/Constants.h"
namespace Hurricane {
  class Cell;
}
namespace CRL {
  class RoutingGauge;
}

namespace Bora {

  using Hurricane::DbU;
  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "Bora::BoxSet".


  class BoxSet
  {
    public:
                                           BoxSet            ( DbU::Unit height=0, DbU::Unit width=0 );
                                           BoxSet            ( BoxSet* boxSet );
      virtual                             ~BoxSet            ();
      inline         DbU::Unit             getHeight         () const;
      inline         DbU::Unit             getWidth          () const; 
      inline         unsigned int          getCpt            () const;
      inline         void                  incrementCpt      ();
      virtual        BoxSet*               clone             () = 0;
      inline         double                getRatio          ();
      inline         double                getArea           ();
                     void                  preDestroy        ();
      virtual        void                  destroy           ();
      virtual        unsigned int          getType           () const;
      virtual        double                getOccupationArea () const;
      virtual const  std::vector<BoxSet*>& getSet            () const;
      virtual        int                   getNFing          () const;
      virtual        size_t                getIndex          () const;
      virtual        void                  print             () const;
      virtual        double                getDevicesArea    () const = 0;
      virtual        void                  setHeight         ( DbU::Unit );
      virtual        void                  setWidth          ( DbU::Unit );
      virtual        std::string           _getTypeName      () const;
      virtual        std::string           _getString        () const;
    protected:
      DbU::Unit     _height;
      DbU::Unit     _width;
      unsigned int  _cpt;
  };


  inline DbU::Unit    BoxSet::getHeight         () const { return _height; }
  inline DbU::Unit    BoxSet::getWidth          () const { return _width; }
  inline unsigned int BoxSet::getCpt            () const { return _cpt; }
  inline void         BoxSet::incrementCpt      ()       { _cpt++; }
  inline double       BoxSet::getRatio          ()       { return (double)_width/(double)_height; }
  inline double       BoxSet::getArea           ()       { return _width*_height; }
  inline unsigned int BoxSet::getType           () const { return UnknownType; }
  inline double       BoxSet::getOccupationArea () const { return 100; }


// -------------------------------------------------------------------
// Class  :  "Bora::HVBoxSet".

  
  class HVBoxSet: public BoxSet
  {
    protected:
                                           HVBoxSet          ( const std::vector<BoxSet*>& , DbU::Unit height=0, DbU::Unit width=0 );
                                           HVBoxSet          ( HVBoxSet* );
      virtual                             ~HVBoxSet          ();
      inline const   std::vector<BoxSet*>& getSet            () const;
      inline         double                getOccupationArea () const;
                     double                getDevicesArea    () const;
      virtual inline void                  calculateHeight   () = 0;
      virtual inline void                  calculateWidth    () = 0;
      virtual        std::string           _getTypeName      () const;
    protected:
      std::vector<BoxSet*>  _dimensionSet;
  };


  inline const std::vector<BoxSet*>& HVBoxSet::getSet            () const { return _dimensionSet; }
  inline       double                HVBoxSet::getOccupationArea () const { return getDevicesArea()/(_height * _width)*100; }


// -------------------------------------------------------------------
// Class  :  "Bora::HBoxSet".

  
  class HBoxSet: public HVBoxSet
  {
    protected:
                                 HBoxSet         ( const std::vector<BoxSet*>& , DbU::Unit height, DbU::Unit width );
                                 HBoxSet         ( HBoxSet* boxSet );
                                ~HBoxSet         ();
    public:
      static        HBoxSet*     create          ( const std::vector<BoxSet*>& , DbU::Unit height=0, DbU::Unit width=0  );
                    HBoxSet*     clone           ();
             inline unsigned int getType         () const;
                    void         calculateHeight ();
                    void         calculateWidth  ();
      static inline int          getCount        ();
      static inline void         printCount      ();
      static inline void         printCountAll   ();
                    void         destroy         ();
      virtual       std::string  _getTypeName    () const;
    private:
      static int _count;
      static int _countAll;
  };


  inline unsigned int  HBoxSet::getType       () const { return HorizontalSNode; }
  inline int           HBoxSet::getCount      ()       { return _count; }
  inline void          HBoxSet::printCount    ()       { std::cerr << "HBoxSet::Count           = " << _count    << std::endl; }
  inline void          HBoxSet::printCountAll ()       { std::cerr << "HBoxSet::CountAll        = " << _countAll << std::endl; }


// -------------------------------------------------------------------
// Class  :  "Bora::VBoxSet".

  
  class VBoxSet: public HVBoxSet
  {
    protected:
                                  VBoxSet         ( const std::vector<BoxSet*>& , DbU::Unit height, DbU::Unit width );
                                  VBoxSet         ( VBoxSet* boxSet );
                                 ~VBoxSet         ();
    public:
      static        VBoxSet*      create          ( const std::vector<BoxSet*>& , DbU::Unit height=0, DbU::Unit width=0  );
                    VBoxSet*      clone           ();
             inline unsigned int  getType         () const;
                    void          calculateHeight ();
                    void          calculateWidth  ();
      static inline int           getCount        ();
      static inline void          printCount      ();
      static inline void          printCountAll   ();
                    void          destroy         ();
      virtual       std::string   _getTypeName    () const;
    private:
      static int  _count;
      static int  _countAll;
  };


  inline unsigned int VBoxSet::getType       () const { return VerticalSNode; }
  inline int          VBoxSet::getCount      ()       { return _count; }
  inline void         VBoxSet::printCount    ()       { std::cerr << "VBoxSet::Count           = " << _count    << std::endl; }
  inline void         VBoxSet::printCountAll ()       { std::cerr << "VBoxSet::CountAll        = " << _countAll << std::endl; }


// -------------------------------------------------------------------
// Class  :  "Bora::DBoxSet".


  class DBoxSet: public BoxSet
  {
    protected:
                                   DBoxSet        ( DbU::Unit height, DbU::Unit width, size_t index );
                                   DBoxSet        ( DBoxSet* boxSet );
                                  ~DBoxSet        ();
    public:   
      static         DBoxSet*      create         ( Cell* , int index, CRL::RoutingGauge* rg=NULL );
                     DBoxSet*      clone          ();
              inline unsigned int  getType        () const;
              inline double        getDevicesArea () const;
              inline size_t        getIndex       () const;
              inline int           getNFing       () const;
      static  inline int           getCount       ();
      static  inline void          printCount     ();
      static  inline void          printCountAll  ();
                     void          destroy        ();
      virtual        std::string   _getTypeName   () const;
    private:
             size_t  _index;    
      static int     _count;
      static int     _countAll;
  };


  inline unsigned int DBoxSet::getType        () const { return DeviceSNode; }
  inline double       DBoxSet::getDevicesArea () const { return _height*_width; }
  inline int          DBoxSet::getNFing       () const { return _index; }
  inline size_t       DBoxSet::getIndex       () const { return _index; }
  inline int          DBoxSet::getCount       ()       { return _count; }
  inline void         DBoxSet::printCount     ()       { std::cerr << "DBoxSet::Count           = " << _count    << std::endl; }
  inline void         DBoxSet::printCountAll  ()       { std::cerr << "DBoxSet::CountAll        = " << _countAll << std::endl; }


// -------------------------------------------------------------------
// Class  :  "Bora::RHVBoxSet".


  class RHVBoxSet: public BoxSet
  {
    public:
                                  RHVBoxSet      ( DbU::Unit height=0, DbU::Unit width=0 );
                                  RHVBoxSet      ( RHVBoxSet* boxSet );
                                 ~RHVBoxSet      ();
             inline DbU::Unit     getHeight      () const;
             inline DbU::Unit     getWidth       () const;
             inline unsigned int  getType        () const;
             inline double        getDevicesArea () const;
      static inline int           getCount       ();
      static inline void          printCount     ();
      static inline void          printCountAll  ();
                    void          destroy        ();
      virtual       void          setHeight      ( DbU::Unit height ) { _height = 0; };
      virtual       void          setWidth       ( DbU::Unit width  ) { _width  = 0; };
                    void          print          () const;
      virtual       std::string   _getTypeName   () const;
    protected:
      static int  _count;
      static int  _countAll;
  };

 
  inline DbU::Unit    RHVBoxSet::getHeight      () const { return _height; } 
  inline DbU::Unit    RHVBoxSet::getWidth       () const { return _width; }
  inline unsigned int RHVBoxSet::getType        () const { return RoutingSNode; }
  inline double       RHVBoxSet::getDevicesArea () const { return 0; }
  inline int          RHVBoxSet::getCount       ()       { return _count; }
  inline void         RHVBoxSet::printCount     ()       { std::cerr << "RHVBoxSet::Count         = " << _count << std::endl; }
  inline void         RHVBoxSet::printCountAll  ()       { std::cerr << "RHVBoxSet::CountAll      = " << _countAll << std::endl; }


// -------------------------------------------------------------------
// Class  :  "Bora::RHBoxSet".


  class RHBoxSet: public RHVBoxSet
  {
    protected:
                           RHBoxSet     ( DbU::Unit height=0 );
                           RHBoxSet     ( RHBoxSet* );
                          ~RHBoxSet     ();
    public:                             
      static  RHBoxSet*    create       ( DbU::Unit height );
              RHBoxSet*    clone        ();
      inline  void         setHeight    ( DbU::Unit height );
      virtual std::string  _getTypeName () const;
  };


  inline void RHBoxSet::setHeight ( DbU::Unit height ) { _height = height; }


// -------------------------------------------------------------------
// Class  :  "Bora::RVBoxSet".


  class RVBoxSet: public RHVBoxSet
  {
    protected:
                           RVBoxSet     ( DbU::Unit width=0 );
                           RVBoxSet     ( RVBoxSet* boxSet );
                          ~RVBoxSet     ();
    public:                             
      static  RVBoxSet*    create       ( DbU::Unit width );
              RVBoxSet*    clone        ();
      inline  void         setWidth     ( DbU::Unit width );
      virtual std::string  _getTypeName () const;
  };


  inline void RVBoxSet::setWidth ( DbU::Unit width ) { _width = width; }


// -------------------------------------------------------------------
// Class  :  "Bora::compBoxSet".

  
  class compBoxSet
  {
    public:
      inline bool  operator() ( const  BoxSet* lhs, const  BoxSet* rhs ) const;
  };

  
  inline bool  compBoxSet::operator() ( const  BoxSet* lhs, const  BoxSet* rhs ) const
  {
    return ( (lhs->getHeight() <  rhs->getHeight()) or 
           ( (lhs->getHeight() == rhs->getHeight()) and (lhs->getWidth() < rhs->getWidth())) ); 
  }


}  // Bora namespace.

GETSTRING_POINTER_SUPPORT(Bora::BoxSet)
GETSTRING_POINTER_SUPPORT(Bora::HVBoxSet)
GETSTRING_POINTER_SUPPORT(Bora::HBoxSet)
GETSTRING_POINTER_SUPPORT(Bora::VBoxSet)
GETSTRING_POINTER_SUPPORT(Bora::DBoxSet)
GETSTRING_POINTER_SUPPORT(Bora::RHVBoxSet)
GETSTRING_POINTER_SUPPORT(Bora::RHBoxSet)
GETSTRING_POINTER_SUPPORT(Bora::RVBoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::BoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::HVBoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::HBoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::VBoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::DBoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::RHVBoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::RHBoxSet)
GETSTRING_REFERENCE_SUPPORT(Bora::RVBoxSet)

IOSTREAM_POINTER_SUPPORT(Bora::BoxSet)
IOSTREAM_POINTER_SUPPORT(Bora::HVBoxSet)
IOSTREAM_POINTER_SUPPORT(Bora::HBoxSet)
IOSTREAM_POINTER_SUPPORT(Bora::VBoxSet)
IOSTREAM_POINTER_SUPPORT(Bora::DBoxSet)
IOSTREAM_POINTER_SUPPORT(Bora::RHVBoxSet)
IOSTREAM_POINTER_SUPPORT(Bora::RHBoxSet)
IOSTREAM_POINTER_SUPPORT(Bora::RVBoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::BoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::HVBoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::HBoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::VBoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::DBoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::RHVBoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::RHBoxSet)
IOSTREAM_REFERENCE_SUPPORT(Bora::RVBoxSet)

#endif  // BORA_BOX_SET_H
