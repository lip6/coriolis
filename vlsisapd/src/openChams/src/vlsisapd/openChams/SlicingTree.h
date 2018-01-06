
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                            Eric Lao              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/SlicingTree.h"            |
// +-----------------------------------------------------------------+

#ifndef __OPENCHAMS_SLICINGTREE_H__
#define __OPENCHAMS_SLICINGTREE_H__

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


namespace OpenChams {
  
  enum Flags { UnknownType      = 1 << 0
             , HorizontalSNode  = 1 << 1
             , VerticalSNode    = 1 << 2
             , DeviceSNode      = 1 << 3
             , RoutingSNode     = 1 << 4
             , UnknownAlignment = 1 << 5
             , AlignLeft        = 1 << 6
             , AlignRight       = 1 << 7
             , AlignCenter      = 1 << 8
             , AlignTop         = 1 << 9
             , AlignBottom      = 1 << 10
             }; 

// -----------------------------------------------------------------------------------------------//
// Class : SlicingNode
// -----------------------------------------------------------------------------------------------//


  class SlicingNode
  {
    public:
               SlicingNode( SlicingNode* parent = NULL );
      virtual ~SlicingNode(){};

    public:
      virtual        unsigned int getType          () const { return UnknownType; }
      virtual        void         print            () const {};
              inline void         recursiveDestroy ();
                     void         setParent        ( SlicingNode* parent );
                     SlicingNode* getParent        () const;
              inline bool         isRoot           () const;
      virtual        void         setAlignment     ( std::string alignment );
                     std::string  alignmentToString() const;
    //Error Methods
      virtual std::string          getInstanceName   () const;
      virtual unsigned int         getAlignment      () const;
      virtual bool                 getPreset         () const;
      virtual double               getX              () const;
      virtual double               getY              () const;
      virtual int                  getNFing          () const;
      virtual double               getStart          () const;
      virtual double               getStep           () const;
      virtual double               getCount          () const; 

      virtual void                 setInstanceName   ( std::string instanceName );
      virtual void                 setPreset         ( std::string preset       );
      virtual void                 setX              ( std::string x            );
      virtual void                 setY              ( std::string y            );
      virtual void                 setNFing          ( std::string nfing        );
      virtual void                 setStart          ( std::string start        );
      virtual void                 setStep           ( std::string step         );
      virtual void                 setCount          ( std::string count        );

      virtual void                 setPreset        ( bool   preset );
      virtual void                 setX             ( double x      );
      virtual void                 setY             ( double y      );
      virtual void                 setNFing         ( int    nfing  );
      virtual void                 setStart         ( double start  );
      virtual void                 setStep          ( double step   );
      virtual void                 setCount         ( double count  );

      virtual const std::vector<SlicingNode*>       getChildren       () const;
      virtual const std::list<std::pair <int,int> > getSymmetries     () const;

      virtual double               getToleranceRatioH() const;
      virtual double               getToleranceRatioW() const;
      virtual double               getToleranceBandH () const;
      virtual double               getToleranceBandW () const;

      virtual void                 setToleranceRatioH( std::string value );
      virtual void                 setToleranceRatioW( std::string value );
      virtual void                 setToleranceBandH ( std::string value );
      virtual void                 setToleranceBandW ( std::string value );

      virtual void                 setToleranceRatioH( double value );
      virtual void                 setToleranceRatioW( double value );
      virtual void                 setToleranceBandH ( double value );
      virtual void                 setToleranceBandW ( double value );

      virtual void                 push_back         ( SlicingNode* node );
      virtual void                 addSymmetry       ( std::string source, std::string target );
      virtual void                 addSymmetry       ( int         source, int         target );
      virtual double               getValue          () const;
      virtual void                 setValue          ( std::string value );
      virtual void                 toXML             ( std::ofstream& file, unsigned indent ){}; 

      virtual std::string          presetToString    () const;
      virtual std::string          xToString         () const;
      virtual std::string          yToString         () const;
      virtual std::string          nfingToString     () const;
      virtual std::string          startToString     () const;
      virtual std::string          stepToString      () const;
      virtual std::string          countToString     () const;
      virtual std::string          valueToString     () const;

      virtual std::string          toleranceRatioHToString() const;
      virtual std::string          toleranceRatioWToString() const;
      virtual std::string          toleranceBandHToString () const;
      virtual std::string          toleranceBandWToString () const;

      virtual bool                 hasSameParentToleranceRatioH () const;
      virtual bool                 hasSameParentToleranceRatioW () const;
      virtual bool                 hasSameParentToleranceBandH () const;
      virtual bool                 hasSameParentToleranceBandW () const;

      virtual bool                    checkInitialPlacement( int& cpt ) const;
      virtual std::list<SlicingNode*> getLeaves            ()           const;

    protected:
      SlicingNode* _parent;
      unsigned int _alignment;
  };

  inline void SlicingNode::recursiveDestroy(){ delete(this); }
  inline bool SlicingNode::isRoot          () const { return (_parent == NULL); }


// -----------------------------------------------------------------------------------------------//
// Class : DeviceSNode
// -----------------------------------------------------------------------------------------------//


  class DSlicingNode : public SlicingNode
  { 
    private:
                           DSlicingNode( std::string instanceName, SlicingNode* parent = NULL );
                          ~DSlicingNode(){};

    public:
      inline static DSlicingNode* create ( std::string instanceName, SlicingNode* parent = NULL );
      inline std::string          getInstanceName  () const;
      inline unsigned int         getAlignment     () const;
      inline bool                 getPreset        () const;
      inline double               getX             () const;
      inline double               getY             () const;
      inline int                  getNFing         () const;
      inline double               getStart         () const;
      inline double               getStep          () const;
      inline double               getCount         () const; 
      inline unsigned int         getType          () const;

             void                 setInstanceName  ( std::string instanceName );
             void                 setPreset        ( std::string preset       );
             void                 setX             ( std::string x            );
             void                 setY             ( std::string y            );
             void                 setNFing         ( std::string nfing        );
             void                 setStart         ( std::string start        );
             void                 setStep          ( std::string step         );
             void                 setCount         ( std::string count        );

      inline void                 setPreset        ( bool      preset );
      inline void                 setX             ( double    x      );
      inline void                 setY             ( double    y      );
      inline void                 setNFing         ( int       nfing  );
      inline void                 setStart         ( double    start  );
      inline void                 setStep          ( double    step   );
      inline void                 setCount         ( double    count  );

             void                 print            () const;
             void                 toXML            ( std::ofstream& file, unsigned indent ); 
      inline std::string          presetToString   () const;
      inline std::string          xToString        () const;
      inline std::string          yToString        () const;
      inline std::string          nfingToString    () const;
      inline std::string          startToString    () const;
      inline std::string          stepToString     () const;
      inline std::string          countToString    () const;

             bool                 checkInitialPlacement ( int& cpt ) const;

    private:
      std::string  _instanceName;
      bool         _preset;
      double       _x;
      double       _y;
      int          _nfing;
      double       _start;
      double       _step;
      double       _count;
  };
  
  inline DSlicingNode*        DSlicingNode::create          ( std::string instanceName, SlicingNode* parent ){ return new DSlicingNode( instanceName ); } 
  inline std::string          DSlicingNode::getInstanceName () const { return _instanceName ; }
  inline unsigned int         DSlicingNode::getAlignment    () const { return _alignment    ; }
  inline bool                 DSlicingNode::getPreset       () const { return _preset       ; }
  inline double               DSlicingNode::getX            () const { return _x            ; }
  inline double               DSlicingNode::getY            () const { return _y            ; }
  inline int                  DSlicingNode::getNFing        () const { return _nfing        ; }
  inline double               DSlicingNode::getStart        () const { return _start        ; }
  inline double               DSlicingNode::getStep         () const { return _step         ; }
  inline double               DSlicingNode::getCount        () const { return _count        ; }
  inline unsigned int         DSlicingNode::getType         () const { return DeviceSNode   ; }

  inline void                 DSlicingNode::setPreset       ( bool      preset ) { _preset = preset; }
  inline void                 DSlicingNode::setX            ( double    x      ) { _x      = x     ; }
  inline void                 DSlicingNode::setY            ( double    y      ) { _y      = y     ; }
  inline void                 DSlicingNode::setNFing        ( int       nfing  ) { _nfing  = nfing ; }
  inline void                 DSlicingNode::setStart        ( double    start  ) { _start  = start ; }
  inline void                 DSlicingNode::setStep         ( double    step   ) { _step   = step  ; }
  inline void                 DSlicingNode::setCount        ( double    count  ) { _count  = count ; }
  
  inline std::string          DSlicingNode::presetToString  () const { return _preset ? "true" : "false"; } 
  inline std::string          DSlicingNode::xToString       () const { return std::to_string(_x    ); } 
  inline std::string          DSlicingNode::yToString       () const { return std::to_string(_y    ); } 
  inline std::string          DSlicingNode::nfingToString   () const { return std::to_string(_nfing); } 
  inline std::string          DSlicingNode::startToString   () const { return std::to_string(_start); } 
  inline std::string          DSlicingNode::stepToString    () const { return std::to_string(_step ); } 
  inline std::string          DSlicingNode::countToString   () const { return std::to_string(_count); } 

// -----------------------------------------------------------------------------------------------//
// Class : HVSlicingNode
// -----------------------------------------------------------------------------------------------//


  class HVSlicingNode : public SlicingNode
  { 
    protected:
                          HVSlicingNode( SlicingNode* parent = NULL );
                 virtual ~HVSlicingNode(){};

    public:
      inline unsigned int                    getAlignment      () const;
      inline double                          getToleranceRatioH() const;
      inline double                          getToleranceRatioW() const;
      inline double                          getToleranceBandH () const;
      inline double                          getToleranceBandW () const;

      inline const std::vector<SlicingNode*>       getChildren  () const;
      inline const std::list<std::pair <int,int> > getSymmetries() const;

             void                            setToleranceRatioH( std::string value );
             void                            setToleranceRatioW( std::string value );
             void                            setToleranceBandH ( std::string value );
             void                            setToleranceBandW ( std::string value );

             void                            setToleranceRatioH( double      value );
             void                            setToleranceRatioW( double      value );
             void                            setToleranceBandH ( double      value );
             void                            setToleranceBandW ( double      value );

             void                            push_back         ( SlicingNode* node );
             void                            addSymmetry       ( std::string source, std::string target );
             void                            addSymmetry       ( int         source, int         target );
             void                            recursiveDestroy  ();
             void                            print             () const;

      inline std::string                     toleranceRatioHToString      () const;
      inline std::string                     toleranceRatioWToString      () const;
      inline std::string                     toleranceBandHToString       () const;
      inline std::string                     toleranceBandWToString       () const;

             bool                            hasSameParentToleranceRatioH () const;
             bool                            hasSameParentToleranceRatioW () const;
             bool                            hasSameParentToleranceBandH  () const;
             bool                            hasSameParentToleranceBandW  () const;

             bool                            checkInitialPlacement        ( int& cpt ) const;
             std::list<SlicingNode*>         getLeaves                    ()           const;

    protected:
      double                          _toleranceRatioH;
      double                          _toleranceRatioW;
      double                          _toleranceBandH;
      double                          _toleranceBandW;
      std::list<std::pair <int,int> > _symmetries;
      std::vector<SlicingNode*>       _children;
  };
  
  inline unsigned int                    HVSlicingNode::getAlignment      () const { return _alignment      ; }
  inline double                          HVSlicingNode::getToleranceRatioH() const { return _toleranceRatioH; }
  inline double                          HVSlicingNode::getToleranceRatioW() const { return _toleranceRatioW; }
  inline double                          HVSlicingNode::getToleranceBandH () const { return _toleranceBandH ; }
  inline double                          HVSlicingNode::getToleranceBandW () const { return _toleranceBandW ; }

  inline const std::vector<SlicingNode*>      HVSlicingNode::getChildren  () const { return _children       ; }
  inline const std::list<std::pair<int,int> > HVSlicingNode::getSymmetries() const { return _symmetries     ; }
  
  inline std::string                     HVSlicingNode::toleranceRatioHToString() const { return std::to_string(_toleranceRatioH); } 
  inline std::string                     HVSlicingNode::toleranceRatioWToString() const { return std::to_string(_toleranceRatioW); } 
  inline std::string                     HVSlicingNode::toleranceBandHToString () const { return std::to_string(_toleranceBandH); } 
  inline std::string                     HVSlicingNode::toleranceBandWToString () const { return std::to_string(_toleranceBandW); } 


// -----------------------------------------------------------------------------------------------//
// Class : HSlicingNode
// -----------------------------------------------------------------------------------------------//


  class HSlicingNode: public HVSlicingNode
  {
    private:
       HSlicingNode( SlicingNode* parent = NULL );
      ~HSlicingNode(){};
      
    public:
      inline static HSlicingNode* create  ( SlicingNode* parent = NULL );          
      inline unsigned int         getType () const;
             void                 print   () const;
             void                 toXML   ( std::ofstream& file, unsigned indent ); 
  };

  inline HSlicingNode* HSlicingNode::create ( SlicingNode* parent ){ return new HSlicingNode(parent); } 
  inline unsigned int  HSlicingNode::getType() const               { return HorizontalSNode   ; } 


// -----------------------------------------------------------------------------------------------//
// Class : VSlicingNode
// -----------------------------------------------------------------------------------------------//


  class VSlicingNode: public HVSlicingNode
  {
    private:
       VSlicingNode( SlicingNode* parent = NULL );
      ~VSlicingNode(){};
      
    public:
      inline static VSlicingNode* create  ( SlicingNode* parent = NULL );  
      inline unsigned int         getType () const;
             void                 print   () const;
             void                 toXML   ( std::ofstream& file, unsigned indent ); 
  };

  inline VSlicingNode* VSlicingNode::create ( SlicingNode* parent ){ return new VSlicingNode(parent); } 
  inline unsigned int  VSlicingNode::getType() const               { return VerticalSNode     ; } 


// -----------------------------------------------------------------------------------------------//
// Class : RoutingSNode
// -----------------------------------------------------------------------------------------------//
    
  
  class RSlicingNode: public SlicingNode
  {
    private:
      RSlicingNode( std::string value, SlicingNode* parent = NULL );
      RSlicingNode( double value, SlicingNode* parent = NULL );
      ~RSlicingNode(){};

    public:
      inline static RSlicingNode* create           ( std::string value, SlicingNode* parent = NULL ); 
      inline static RSlicingNode* create           ( double   value, SlicingNode* parent = NULL ); 
      inline void                 recursiveDestroy ();
      inline double               getValue         () const;
             void                 setValue         ( std::string value );
      inline unsigned int         getType          () const;
             void                 print            () const;
             void                 setAlignment     ( std::string alignment) {};
             void                 toXML            ( std::ofstream& file, unsigned indent ); 
      inline std::string          valueToString    () const;
      
    private:
      double _value;
  };

  inline RSlicingNode* RSlicingNode::create        ( std::string value, SlicingNode* parent ){ return new RSlicingNode( value, parent ); }
  inline RSlicingNode* RSlicingNode::create        ( double value, SlicingNode* parent ){ return new RSlicingNode( value, parent ); }
  inline double        RSlicingNode::getValue      () const{ return _value      ; }
  inline unsigned int  RSlicingNode::getType       () const{ return RoutingSNode; } 
  inline std::string   RSlicingNode::valueToString () const { return std::to_string(_value); }


} // OpenChams namespace

#endif
