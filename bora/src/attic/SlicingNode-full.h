// -*- C++ -*-
//
// This file is part of the Chams Software.
// Copyright (c) UPMC 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                      C H A M S                                  |
// |      H o r u s  -  S l i c i n g  T r e e   V i e w e r         |
// |                                                                 |
// |  Authors     :                         Eric LAO                 |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricaneAMS/layout/SlicingNode.h"      |
// +-----------------------------------------------------------------+


#ifndef DEF_SLICINGNODE
#define DEF_SLICINGNODE

#include <typeinfo>
#include <iterator>     
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <iomanip>
#include <string>
#include <tuple>

#include "hurricane/Cell.h"
#include "hurricane/Net.h"
#include "hurricane/Contact.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Name.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/UpdateSession.h" 
#include "hurricane/DbU.h"
#include "hurricane/Property.h"

#include "hurricaneAMS/technology/AnalogTechnology.h"
#include "hurricaneAMS/analogic/LayoutGenerator.h"
#include "hurricaneAMS/analogic/Device.h"
#include "hurricaneAMS/devices/TransistorFamily.h"
#include "hurricaneAMS/devices/Transistor.h"
#include "hurricaneAMS/devices/SimpleCurrentMirror.h"
#include "hurricaneAMS/devices/DifferentialPair.h"
#include "hurricaneAMS/devices/CommonSourcePair.h"

#include "anabatic/Session.h"
#include "anabatic/GCell.h"
#include "anabatic/Dijkstra.h"
#include "anabatic/Matrix.h"
#include "crlcore/RoutingGauge.h"
#include "katana/KatanaEngine.h"
#include "vlsisapd/openChams/SlicingTree.h"




const unsigned int presetMask           =     0x1;
const unsigned int setMask              =     0x2;
const unsigned int placedMask           =     0x4;
const unsigned int alignmentMask        =   0x1F8;
const unsigned int slicingTypeMask      =  0x3E00;
const unsigned int overCellMask         =  0x8000;
const unsigned int routingEstimatedMask = 0x10000;
const unsigned int routingCreatedMask   = 0x20000;
const unsigned int symmetricMask        = 0xC0000;

enum Flags { Preset            = 1 << 0
           , Set               = 1 << 1
           , Placed            = 1 << 2
           , UnknownAlignment  = 1 << 3

           , AlignLeft         = 1 << 4
           , AlignRight        = 1 << 5
           , AlignCenter       = 1 << 6
           , AlignTop          = 1 << 7

           , AlignBottom       = 1 << 8
           , UnknownType       = 1 << 9
           , HorizontalSNode   = 1 << 10
           , VerticalSNode     = 1 << 11

           , DeviceSNode       = 1 << 12
           , RoutingSNode      = 1 << 13
           , None              = 1 << 14
           , MX                = 1 << 15

           , MY                = 1 << 16
           , FixedRoutingSNode = 1 << 17
           , OverCell          = 1 << 19

           , RoutingEstimated  = 1 << 20
           , RoutingCreated    = 1 << 21
             
           , HSymmetry         = 1 << 23
           , VSymmetry         = 1 << 24

};

const unsigned int sideMask         =  0x1F;
const unsigned int targetMask       =  0x20;
const unsigned int notPassableMask  = 0x3C0; // North - South - East - West
const unsigned int notPassableNMask =  0x40; // North 
const unsigned int notPassableSMask =  0x80; // South 
const unsigned int notPassableEMask = 0x100; // East
const unsigned int notPassableWMask = 0x200; // West

enum FunctionFlags { NoFlags = 0
                   , ShowDiff = (1<<0)
                   };

// Make sure these restrictions are the same than in the class Device
enum Restriction { NorthBlocked = (1<<0)
                 , SouthBlocked = (1<<1)
                 , EastBlocked  = (1<<2)
                 , WestBlocked  = (1<<3)
                 , AllBlocked   = NorthBlocked|SouthBlocked|EastBlocked|WestBlocked
};
// -----------------------------------------------------------------------------------------------//
// Class : BoxSet
// Description: 
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
// -----------------------------------------------------------------------------------------------//

  
class ChannelRouting 
{
  private:
                           ChannelRouting();
                          ~ChannelRouting();

  public:
    static ChannelRouting* create       ();
    void                   destroy      ();
    int                    getMaxCount  () const; 
    void                   insertChannel( DbU::Unit xy1, DbU::Unit xy2 );
    void                   insertChannel( DbU::Unit xy1, DbU::Unit xy2, unsigned int w );
    void                   reset        ();
    void                   print        () const;

  private:
    std::vector<DbU::Unit> _limits;
    std::vector<int>       _counts;
      
};


class BoxSet
{
  public:
                                         BoxSet ( DbU::Unit height = 0, DbU::Unit width = 0 );
                                         BoxSet ( BoxSet* boxSet );
                                virtual ~BoxSet ();
  
    inline         DbU::Unit             getHeight         () const;
    inline         DbU::Unit             getWidth          () const; 
    inline         unsigned int          getCpt            () const;
    inline         void                  incrementCpt      ();
    virtual        BoxSet*               clone             () = 0;
    inline         double                getRatio          ();
    inline         double                getArea           ();

                   void                  preDestroy        (){}
    virtual        void                  destroy           ();

    virtual inline unsigned int          getType           () const;
    virtual inline double                getOccupationArea () const;
    virtual const  std::vector<BoxSet*>& getSet            () const;
    virtual        int                   getNFing          () const;
    virtual        void                  print             () const;
    virtual        double                getDevicesArea    () const = 0;
 
    virtual        void                  setHeight ( DbU::Unit height ){};
    virtual        void                  setWidth  ( DbU::Unit width  ){};
  protected:
    DbU::Unit    _height;
    DbU::Unit    _width;
    unsigned int _cpt;

};

inline DbU::Unit    BoxSet::getHeight         () const { return _height;     }
inline DbU::Unit    BoxSet::getWidth          () const { return _width ;     }
inline unsigned int BoxSet::getCpt            () const { return _cpt;        }
inline void         BoxSet::incrementCpt      ()       { _cpt++;             }
inline double       BoxSet::getRatio          ()       { return _width/_height; }
inline double       BoxSet::getArea           ()       { return _width*_height; }


inline unsigned int BoxSet::getType           () const { return UnknownType; }
inline double       BoxSet::getOccupationArea () const { return 100;         }


// -----------------------------------------------------------------------------------------------//
// Class : HVBoxSet
// -----------------------------------------------------------------------------------------------//

  
class HVBoxSet: public BoxSet
{
  protected:
                                         HVBoxSet ( std::vector<BoxSet*> dimensionSet, DbU::Unit height = 0, DbU::Unit width = 0 );
                                         HVBoxSet ( HVBoxSet* boxSet );
                                virtual ~HVBoxSet ();

    inline const   std::vector<BoxSet*>& getSet            () const;
    inline         double                getOccupationArea () const;
                   double                getDevicesArea    () const;

    virtual inline void                  calculateHeight   () = 0;
    virtual inline void                  calculateWidth    () = 0;

  protected:
    std::vector<BoxSet*> _dimensionSet;
};

inline const std::vector<BoxSet*>& HVBoxSet::getSet            () const { return _dimensionSet; }
inline       double                HVBoxSet::getOccupationArea () const { return getDevicesArea()/(_height * _width)*100; }


// -----------------------------------------------------------------------------------------------//
// Class : HBoxSet
// -----------------------------------------------------------------------------------------------//

  
class HBoxSet: public HVBoxSet
{
  protected:
                        HBoxSet ( std::vector<BoxSet*> dimensionSet, DbU::Unit height = 0, DbU::Unit width = 0 );
                        HBoxSet ( HBoxSet* boxSet );
                       ~HBoxSet ();
      
  public:
    static HBoxSet*     create          ( std::vector<BoxSet*> dimensionSet, DbU::Unit height = 0, DbU::Unit width = 0  );
           HBoxSet*     clone           ();
    inline unsigned int getType         () const;
           void         calculateHeight ();
           void         calculateWidth  ();

    static inline int   getCount        ();
    static inline void  printCount      ();
    static inline void  printCountAll   ();
                  void  destroy         ();

  private:
    static int _count;
    static int _countAll;
};

inline unsigned int HBoxSet::getType       () const { return HorizontalSNode; }
inline int          HBoxSet::getCount      ()       { return _count         ; }
inline void         HBoxSet::printCount    ()       { std::cerr << "HBoxSet::Count           = " << _count    << std::endl; }
inline void         HBoxSet::printCountAll ()       { std::cerr << "HBoxSet::CountAll        = " << _countAll << std::endl; }


// -----------------------------------------------------------------------------------------------//
// Class : VBoxSet
// -----------------------------------------------------------------------------------------------//

  
class VBoxSet: public HVBoxSet
{
  protected:
                        VBoxSet ( std::vector<BoxSet*> dimensionSet, DbU::Unit height = 0, DbU::Unit width = 0 );
                        VBoxSet ( VBoxSet* boxSet );
                       ~VBoxSet ();

  public:
    static VBoxSet*     create          ( std::vector<BoxSet*> dimensionSet, DbU::Unit height = 0, DbU::Unit width = 0  );
           VBoxSet*     clone           ();
    inline unsigned int getType         () const;
           void         calculateHeight ();
           void         calculateWidth  ();

    static inline int   getCount        ();
    static inline void  printCount      ();
    static inline void  printCountAll   ();
                  void  destroy         ();

  private:
    static int _count;
    static int _countAll;
};

inline unsigned int VBoxSet::getType       () const { return VerticalSNode; }
inline int          VBoxSet::getCount      ()       { return _count       ; }
inline void         VBoxSet::printCount    ()       { std::cerr << "VBoxSet::Count           = " << _count    << std::endl; }
inline void         VBoxSet::printCountAll ()       { std::cerr << "VBoxSet::CountAll        = " << _countAll << std::endl; }


// -----------------------------------------------------------------------------------------------//
// Class : DBoxSet
// -----------------------------------------------------------------------------------------------//


class DBoxSet: public BoxSet
{
  protected:
                        DBoxSet  ( DbU::Unit height, DbU::Unit width, int nfing = 1 );
                        DBoxSet  ( DBoxSet* boxSet );
                       ~DBoxSet ();

  public:
    static DBoxSet*     create         ( DbU::Unit height, DbU::Unit width, int nfing = 1 );
           DBoxSet*     clone          ();
    inline unsigned int getType        () const;
    inline double       getDevicesArea () const;

           inline int   getNFing       () const;
    static inline int   getCount       ();
    static inline void  printCount     ();
    static inline void  printCountAll  ();
                  void  destroy        ();

  private:
    int        _nfing;    
    static int _count;
    static int _countAll;
};

inline unsigned int DBoxSet::getType        () const { return DeviceSNode   ; }
inline double       DBoxSet::getDevicesArea () const { return _height*_width; }
inline int          DBoxSet::getNFing       () const { return _nfing        ; }
inline int          DBoxSet::getCount       ()       { return _count        ; }
inline void         DBoxSet::printCount     ()       { std::cerr << "DBoxSet::Count           = " << _count    << std::endl; }
inline void         DBoxSet::printCountAll  ()       { std::cerr << "DBoxSet::CountAll        = " << _countAll << std::endl; }


// -----------------------------------------------------------------------------------------------//
// Class : RHVBoxSet
// -----------------------------------------------------------------------------------------------//


class RHVBoxSet: public BoxSet
{
  public:
                        RHVBoxSet ( DbU::Unit height = 0, DbU::Unit width = 0);
                        RHVBoxSet ( RHVBoxSet* boxSet );
                       ~RHVBoxSet ();

    inline DbU::Unit    getHeight      () const;
    inline DbU::Unit    getWidth       () const;
    inline unsigned int getType        () const;
    inline double       getDevicesArea () const;
      
    static inline int   getCount       ();
    static inline void  printCount     ();
    static inline void  printCountAll  ();
                  void  destroy        ();
    virtual       void  setHeight      ( DbU::Unit height ){ _height = 0; };
    virtual       void  setWidth       ( DbU::Unit width  ){ _width  = 0; };

                  void  print          () const;

  protected:
    static int _count;
    static int _countAll;
};
 
inline DbU::Unit    RHVBoxSet::getHeight      () const { return _height     ; } 
inline DbU::Unit    RHVBoxSet::getWidth       () const { return _width      ; }
inline unsigned int RHVBoxSet::getType        () const { return RoutingSNode; }
inline double       RHVBoxSet::getDevicesArea () const { return 0           ; }
inline int          RHVBoxSet::getCount       ()       { return _count      ; }
inline void         RHVBoxSet::printCount     ()       { std::cerr << "RHVBoxSet::Count         = " << _count << std::endl; }
inline void         RHVBoxSet::printCountAll  ()       { std::cerr << "RHVBoxSet::CountAll      = " << _countAll << std::endl; }


// -----------------------------------------------------------------------------------------------//
// Class : RHBoxSet
// -----------------------------------------------------------------------------------------------//


class RHBoxSet: public RHVBoxSet
{
  protected:
    RHBoxSet  ( DbU::Unit height = 0 );
    RHBoxSet  ( RHBoxSet* boxSet     );
    ~RHBoxSet ();

  public:
    static RHBoxSet* create    ( DbU::Unit height );
           RHBoxSet* clone     ();
    inline void      setHeight ( DbU::Unit height );
};

inline void RHBoxSet::setHeight ( DbU::Unit height ){ _height = height; }


// -----------------------------------------------------------------------------------------------//
// Class : RVBoxSet
// -----------------------------------------------------------------------------------------------//

class RVBoxSet: public RHVBoxSet
{
  protected:
    RVBoxSet  ( DbU::Unit width = 0 );
    RVBoxSet  ( RVBoxSet* boxSet );
    ~RVBoxSet ();

  public:
    static RVBoxSet* create   ( DbU::Unit width );
           RVBoxSet* clone    ();
    inline void      setWidth ( DbU::Unit width );
};

inline void RVBoxSet::setWidth ( DbU::Unit width ){ _width = width; }


// -----------------------------------------------------------------------------------------------//
// Class : NodeSets
// Description:
// Contains all the possible dimensions (BoxSet) for a SlicingNode
// -----------------------------------------------------------------------------------------------//
  
// To compare BoxSets
class compBoxSet
{
  public:
    bool operator() ( const  BoxSet* lhs, const  BoxSet* rhs) {
      return ( (lhs->getHeight() < rhs->getHeight()) || 
             ( (lhs->getHeight() == rhs->getHeight()) && (lhs->getWidth() < rhs->getWidth()))
             ); 
    }
};


class NodeSets 
{
  public:
    NodeSets  ( double start = 0, double step = 0, double count = 0 );
    NodeSets  ( const NodeSets* other );
    ~NodeSets ();
     
    static NodeSets*                      create( Cell* cell, double start, double step, double count, CRL::RoutingGauge* rg = NULL );
    static NodeSets*                      create();

    BoxSet* operator[] (size_t i);
    BoxSet* at         (size_t i);

    inline std::vector<BoxSet*>::iterator begin       ();
    inline std::vector<BoxSet*>::iterator end         ();
    inline std::vector<BoxSet*>           getNodeSets () const;
    inline size_t                         size        () const;
    inline bool                           empty       () const;
           void                           clear       ();
    inline void                           sort        ();

           BoxSet*                        getPairH    ( DbU::Unit height ) const;
           BoxSet*                        getPairHW   ( DbU::Unit height, DbU::Unit width ) const;
           BoxSet*                        getPairHW   ( unsigned int index ) const;
           int                            findIndex   ( DbU::Unit height, DbU::Unit width ) const;
           std::vector<BoxSet*>::iterator find        ( DbU::Unit height, DbU::Unit width );
           std::vector<BoxSet*>::iterator find        ( BoxSet* boxSet );
           std::vector<BoxSet*>::iterator find        ( int     nfing  );

           void                           print       () const;
           bool                           compare     ( NodeSets nodeSets2, unsigned int flags=NoFlags ) const;
           void                           push_back   ( BoxSet* boxSet );
           void                           push_back   ( std::vector<BoxSet*> vect, DbU::Unit height, DbU::Unit width, unsigned int type );
           void                           push_back   ( DbU::Unit height, DbU::Unit width );
           NodeSets*                      clone       ();

    inline double                         getStartParameter () const;
    inline double                         getStepParameter  () const;
    inline double                         getCountParameter () const;
    

  private:
    std::vector<BoxSet*> _nodeSets;
    int                  _start;
    int                  _step;
    int                  _count;
};

inline std::vector<BoxSet*>::iterator NodeSets::begin             ()       { return _nodeSets.begin(); }
inline std::vector<BoxSet*>::iterator NodeSets::end               ()       { return _nodeSets.end  (); }
inline std::vector<BoxSet*>           NodeSets::getNodeSets       () const { return _nodeSets;         }
inline size_t                         NodeSets::size              () const { return _nodeSets.size() ; }
inline bool                           NodeSets::empty             () const { return _nodeSets.empty(); }
inline void                           NodeSets::sort              ()       { std::sort( _nodeSets.begin(),_nodeSets.end(), compBoxSet() ); }
inline double                         NodeSets::getStartParameter () const { return _start; }
inline double                         NodeSets::getStepParameter  () const { return _step ; }
inline double                         NodeSets::getCountParameter () const { return _count; }


// -----------------------------------------------------------------------------------------------//
// Class : SlicingNode
// Description:
// Node used for building a slicing tree used for device's placement.
// Step 1: Build  - build the slicing tree
// Step 2: Update - possible dimensions (updateGlobalSize)
// Step 3: Select - select the desired dimension (setGlobalSize)
// -----------------------------------------------------------------------------------------------//


class SlicingNode
{
  protected:
    SlicingNode ( unsigned int type       
                , NodeSets*    nodeSets  = NodeSets::create()
                , unsigned int alignment = UnknownAlignment
                , BoxSet*      boxSet    = NULL
                );
    virtual ~SlicingNode ();

  public:
    inline  bool                    isPreset              () const;
    inline  bool                    isSet                 () const;
    inline  bool                    isPlaced              () const;
    inline  bool                    isAlignLeft           () const;
    inline  bool                    isAlignRight          () const;
    inline  bool                    isAlignCenter         () const;
    inline  bool                    isAlignTop            () const;
    inline  bool                    isAlignBottom         () const;
    inline  bool                    isHorizontal          () const;
    inline  bool                    isVertical            () const;
    inline  bool                    isDevice              () const;
    inline  bool                    isRouting             () const;
            bool                    isSameSize            ( BoxSet* boxSet ) const;
            bool                    isSameSize            ( DbU::Unit height, DbU::Unit width ) const;
            bool                    isEmpty               () const;
    inline  bool                    isOverCell            () const;
    inline  bool                    isRoutingCreated      () const;
    virtual inline bool             isRoutingEstimated    () const;
            bool                    isHSymmetry           () const;
            bool                    isVSymmetry           () const;
    virtual inline bool             isAnalog              () const; 
    virtual inline bool             isDigital             () const; 

    inline unsigned int             getPreset            () const;
    inline unsigned int             getSet               () const;
    inline unsigned int             getPlaced            () const;
    inline unsigned int             getAlignment         () const;
    inline unsigned int             getType              () const;
    virtual DbU::Unit               getHeight            () const;
    virtual DbU::Unit               getWidth             () const;
            DbU::Unit               getRootHeight         () const;
            DbU::Unit               getRootWidth          () const;
    inline BoxSet*                  getBoxSet            () const;
    inline DbU::Unit                getX                 () const;
    inline DbU::Unit                getY                 () const;
           DbU::Unit                getRootX             () const;
           DbU::Unit                getRootY             () const;
    inline DbU::Unit                getXCenter           () const;
    inline DbU::Unit                getYCenter           () const;
    inline NodeSets*                getNodeSets          () const;
    inline SlicingNode*             getParent            () const;
    inline BoxSet*                  getPairH             ( DbU::Unit h ) const; 
    inline BoxSet*                  getPairHW            ( DbU::Unit height, DbU::Unit width ) const;
    inline BoxSet*                  getPairHW            ( int index ) const;
           SlicingNode*             getRoot              ();
           int                      getCpt               ();
           double                   getRatio             ();
           double                   getArea              ();
    static inline AnalogTechnology* getTechnology        (); 
    static inline Cell*             getCell              (); 
    inline Anabatic::GCell*         getGCell             () const; 
    inline CRL::RoutingGauge*       getRoutingGauge      () const;
    inline SlicingNode*             getMaster            () const;
           DbU::Unit                getVPitch            ();     
           DbU::Unit                getHPitch            ();       

    inline void                     setPreset            ( unsigned int preset       );
    inline void                     setSet               ( unsigned int set          );
    virtual inline void             setPlaced            ( unsigned int placed       );
    inline void                     setAlignment         ( unsigned int alignment    );
    inline void                     setType              ( unsigned int type         );
           void                     setBoxSet            ( BoxSet*      boxSet       );
           void                     setBoxSet            ( DbU::Unit height, DbU::Unit width );
           void                     setBoxSet            ( size_t       index        );
           void                     _setBoxSet           ( BoxSet*      boxSet       );
           void                     setX                 ( DbU::Unit    x            );
           void                     setY                 ( DbU::Unit    y            );
    inline void                     setNodeSets          ( NodeSets*    nodeSets     );
    inline void                     setParent            ( SlicingNode* parent       );
    inline void                     setPairH             ( DbU::Unit    height       );
    inline void                     setPairHW            ( DbU::Unit height, DbU::Unit width );

    inline void                     setOverCell         ( unsigned int overCell         );
    inline void                     setRoutingCreated   ( unsigned int routingCreated   );
    inline void                     setRoutingEstimated ( unsigned int RoutingEstimated );
    static void                     setRoutingGauge     ( CRL::RoutingGauge* rg         );
           void                     setCell             ( Cell* cell                    ); 
           void                     setKatana           ( Katana::KatanaEngine* katana          ); 
    virtual inline void             setSymmetryFlag     ( unsigned int flag   );
    virtual inline void             setMaster           ( SlicingNode* master );
    virtual void                    setHeight           ( DbU::Unit height ){};
    virtual void                    setWidth            ( DbU::Unit width  ){};
      
    inline  void                    clearNodeSets        ();
            void                    clearParentsNodeSets ();
    inline  void                    removeParent         ();
    virtual void                    resetSlicingTree     (); // see notes in .cpp
    virtual void                    _resetSlicingTree    ();

    virtual void                    print                () const;
    virtual void                    printLine            () const;
    virtual void                    printParent          () const;
            void                    printBoxSet          () const;
    virtual void                    place                ( DbU::Unit x = 0 , DbU::Unit y = 0 ); 
    virtual void                    replace              ( DbU::Unit x = 0 , DbU::Unit y = 0 ); 
    virtual void                    _place               ( DbU::Unit x = 0 , DbU::Unit y = 0, bool replace = false ); 
    virtual void                    updateGlobalSize     (){}; 
    virtual void                    setGlobalSize        ( DbU::Unit height, DbU::Unit width );
    virtual void                    setGlobalSize        ( size_t index );
    virtual void                   _setGlobalSize        ( BoxSet* boxSet );
    virtual inline bool             recursiveCheckPreset () const;
    virtual inline bool             recursiveCheckSet    () const;
    virtual inline bool             recursiveCheckPlaced () const;
    virtual inline int              getLeafNumber        () const;
    virtual void                    preDestroy           ();
    virtual void                    destroy              ();
    virtual void                    preRecursiveDestroy  ();
    virtual void                    recursiveDestroy     ();

    virtual OpenChams::SlicingNode* toOpenChams          (){ return NULL; }; 
    virtual std::list<SlicingNode*> getLeaves            () const;
    virtual bool                    checkInitialPlacement( int& cpt ) const; // see notes in .cpp
    void                            updateCellAbutmentBox();
    virtual void                    setGCell             ( Anabatic::GCell* gcell );
    virtual bool                    isSymmetric          ( SlicingNode* node, unsigned int symmetryType, unsigned int flags=NoFlags  ) const = 0;
    virtual bool                    isSame               ( SlicingNode* node, unsigned int flags=NoFlags ) const = 0;

    static  void                    createRoutingPads    (); 
    static  void                    clearRoutingPads     (); 
    virtual void                    restrictDevices      ();
    virtual void                    setVertexRestriction ( Net* net, Katana::KatanaEngine* katana );
    virtual void                    estimateChannelsSize ();
    virtual void                    expandRoutingChannel ();
    virtual void                    _expandRoutingChannel();
    virtual void                    expandRoutingChannel ( DbU::Unit height, DbU::Unit width );
    virtual void                    clearGlobalRouting   (); 
    virtual void                    clearGCells          (); 
    virtual void                    adjustBorderChannels (){}; 

            bool                    containAxis          ( DbU::Unit axis, unsigned int symmetryType ); 
    virtual void                    updateMasterSize     ( DbU::Unit size ) {}


  // Error Methods
    virtual DbU::Unit                        getToleranceRatioH           () const;
    virtual DbU::Unit                        getToleranceRatioW           () const;
    virtual void                             setToleranceRatioH           ( DbU::Unit tolerance );
    virtual void                             setToleranceRatioW           ( DbU::Unit tolerance );
    virtual DbU::Unit                        getToleranceBandH            () const;
    virtual DbU::Unit                        getToleranceBandW            () const;
    virtual void                             setToleranceBandH            ( DbU::Unit tolerance );
    virtual void                             setToleranceBandW            ( DbU::Unit tolerance );
    virtual void                             recursiveSetToleranceRatioH  ( DbU::Unit tolerance );
    virtual void                             recursiveSetToleranceRatioW  ( DbU::Unit tolerance );
    virtual void                             recursiveSetToleranceBandH   ( DbU::Unit tolerance );
    virtual void                             recursiveSetToleranceBandW   ( DbU::Unit tolerance );
    virtual void                             _recursiveSetToleranceRatioH ( DbU::Unit tolerance );
    virtual void                             _recursiveSetToleranceRatioW ( DbU::Unit tolerance );
    virtual void                             _recursiveSetToleranceBandH  ( DbU::Unit tolerance );
    virtual void                             _recursiveSetToleranceBandW  ( DbU::Unit tolerance );
    virtual void                             setTolerances                ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw );
    virtual void                             recursiveSetTolerances       ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw );
    virtual void                             _recursiveSetTolerances      ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw );

    virtual bool                             hasEmptyChildrenNodeSets () const;
    virtual const std::vector<SlicingNode*>& getChildren              () const;
    virtual SlicingNode*                     getChild                 ( int index ) const;
    virtual int                              getChildIndex            ( SlicingNode* node ) const;
    virtual size_t                           getNbChild               () const;
    virtual void                             createChild              ( unsigned int type, unsigned int alignment );
    virtual void                             createChild              ( NodeSets*    nodeSets  
                                                                      , unsigned int alignment = UnknownAlignment
                                                                      , Instance*    instance  = NULL
                                                                      , BoxSet*      boxSet    = NULL
                                                                      );
    virtual void                             createChild              ( int childIndex, int copyIndex, unsigned int tr ); 
    virtual void                             createRouting            ( DbU::Unit space );
    virtual void                             insertNode               ( SlicingNode* node, int index ); 
    virtual void                             push_back                ( SlicingNode* node, bool reset = false ); 
    virtual void                             push_front               ( SlicingNode* node ); 
    virtual void                             removeNode               ( SlicingNode* node );
    virtual void                             removeAllNodes           ();     

    virtual std::list<std::pair<int,int> >   getSymmetries            () const;
    virtual void                             setSymmetries            ( std::list<std::pair< int,int> > sym );
    virtual bool                             isSymmetry               ( int index, std::pair<int,int>& symmetry );
    virtual bool                             isSymmetry               ( int index );
    virtual void                             addSymmetry              ( int childIndex, int copyIndex, bool reset = true );
    virtual void                             removeSymmetry           ( int index );
    virtual void                             normalizeSymmetries      ();

    virtual void                             printChildren            () const;
    virtual void                             printChildrenLine        () const;
    virtual SlicingNode*                     clone                    ( unsigned int tr = None ) = 0;
    virtual double                           getDevicesArea           () const = 0;
    virtual double                           getOccupationArea        () const;
    virtual void                             setNFing                 ( int nfing );
    virtual int                              getNFing                 () const;
    virtual double                           getStartParameter        () const;
    virtual double                           getStepParameter         () const;
    virtual double                           getCountParameter        () const;
    virtual Instance*                        getInstance              () const;
    virtual void                             setInstance              ( Instance* instance );

    virtual bool                             checkCellInstances       ( Cell*     cell     );
    virtual SlicingNode*                     findInstance             ( Instance* instance );
    virtual SlicingNode*                     findSlicingNode          ( Anabatic::GCell* gcell );
    virtual void                             updateGCellPosition      ();
    virtual void                             updateGContacts          ();
    virtual void                             updateMatrixGCellPosition();
    virtual void                             updateMatrixGContacts    ();
    virtual int                              getNbDevices             ();
 
  // Methods to be tested
    virtual void                             createSlicingRouting      ();
    virtual void                             resetSlicingRouting       ();
    virtual void                             destroySlicingRouting     ();

    virtual int                              getRoutingIndex           ( SlicingNode* node ) const;
    virtual SlicingNode*                     getSlicingRouting         ( int index ) const;
    SlicingNode*                             findHorizontalParent      () const;
    SlicingNode*                             findVerticalParent        () const;
    int                                      getIndexInHorizontalParent() const;
    int                                      getIndexInVerticalParent  () const;

    
    virtual bool                             checkSymmetryNet     ( unsigned int type, Net* net1, Net* net2 = NULL ) const;
    virtual void                             addSymmetryNet       ( unsigned int type, Net* net1, Net* net2 = NULL );
    virtual void                             updateNetConstraints ();
    virtual void                             updateSymNetAxis     ();

    virtual string                           _getString      () const;
    virtual string                           _getTypeName    () const;
    static  void                             addRailSegments ( Hurricane::Segment* s );
    static  bool                             isRailSegments  ( Hurricane::Segment* s );
    static  bool                             isRailSegments  ( Hurricane::Plug* p    );
            void                             createRailCell  ();
    virtual void                             flattenDigitalNets();
    
    virtual void updateWireOccupation ( Anabatic::VertexSet );
    virtual void resetWireOccupation  ();
    virtual void addWireOccupation    ( DbU::Unit min, DbU::Unit max );
    virtual int  getMaxWireOccupation ();
  protected:
    static CRL::RoutingGauge*          _rg;
    static Cell*                       _cell;
    static Katana::KatanaEngine*       _katana;
  //static Cell*                       _railCell;
    static vector<Segment*>            _railSegments;

    SlicingNode*              _parent;
    unsigned int              _flags;
    NodeSets*                 _nodeSets;    
    DbU::Unit                 _x;         
    DbU::Unit                 _y;       
    BoxSet*                   _boxSet;
    Anabatic::GCell*          _gcell;
    SlicingNode*              _master;
};

inline bool            SlicingNode::isPreset      () const { return (_flags & Preset)     ; }
inline bool            SlicingNode::isSet         () const { return (_flags & Set)        ; }
inline bool            SlicingNode::isPlaced      () const { return (_flags & Placed)     ; }
inline bool            SlicingNode::isAlignLeft   () const { return (_flags & AlignLeft)  ; }
inline bool            SlicingNode::isAlignRight  () const { return (_flags & AlignRight) ; }
inline bool            SlicingNode::isAlignCenter () const { return (_flags & AlignCenter); }
inline bool            SlicingNode::isAlignTop    () const { return (_flags & AlignTop)   ; }
inline bool            SlicingNode::isAlignBottom () const { return (_flags & AlignBottom); }
inline bool            SlicingNode::isHorizontal  () const { return (_flags & HorizontalSNode) ; }
inline bool            SlicingNode::isVertical    () const { return (_flags & VerticalSNode)   ; }
inline bool            SlicingNode::isDevice      () const { return (_flags & DeviceSNode)     ; }
inline bool            SlicingNode::isRouting     () const { return (_flags & RoutingSNode)    ; }

inline bool            SlicingNode::isHSymmetry   () const { return (_flags & HSymmetry); }
inline bool            SlicingNode::isVSymmetry   () const { return (_flags & VSymmetry); }

inline bool            SlicingNode::isOverCell            () const { return (_flags & OverCell        ); }
inline bool            SlicingNode::isRoutingCreated      () const { return (_flags & RoutingCreated  ); }
inline bool            SlicingNode::isRoutingEstimated    () const { return true; }

inline bool            SlicingNode::isAnalog              () const { return false; }
inline bool            SlicingNode::isDigital             () const { return false; }

inline unsigned int      SlicingNode::getPreset      () const { return (_flags & presetMask     ); }
inline unsigned int      SlicingNode::getSet         () const { return (_flags & setMask        ); }
inline unsigned int      SlicingNode::getPlaced      () const { return (_flags & placedMask     ); }
inline unsigned int      SlicingNode::getAlignment   () const { return (_flags & alignmentMask  ); }
inline unsigned int      SlicingNode::getType        () const { return (_flags & slicingTypeMask); }
inline BoxSet*           SlicingNode::getBoxSet      () const { return _boxSet;   }
inline DbU::Unit         SlicingNode::getX           () const { return _x;        }
inline DbU::Unit         SlicingNode::getY           () const { return _y;        }
inline DbU::Unit         SlicingNode::getXCenter     () const { return getX() + getWidth()/2;  }
inline DbU::Unit         SlicingNode::getYCenter     () const { return getY() + getHeight()/2; }
inline NodeSets*         SlicingNode::getNodeSets    () const { return _nodeSets; }
inline SlicingNode*      SlicingNode::getParent      () const { return _parent;   }
inline Cell*              SlicingNode::getCell        ()       { return _cell;      } 
inline Anabatic::GCell*   SlicingNode::getGCell       () const { return _gcell;     } 
inline CRL::RoutingGauge* SlicingNode::getRoutingGauge() const { return _rg;        } 
inline SlicingNode*       SlicingNode::getMaster      () const { return _master;    }

inline BoxSet*         SlicingNode::getPairH       ( DbU::Unit height )                  const { return _nodeSets->getPairH(height);         }  
inline BoxSet*         SlicingNode::getPairHW      ( DbU::Unit height, DbU::Unit width ) const { return _nodeSets->getPairHW(height, width); }  
inline BoxSet*         SlicingNode::getPairHW      ( int index ) const { return _nodeSets->getPairHW(index); }  
inline void            SlicingNode::setPreset     ( unsigned int preset               ) { _flags    = ((_flags & ~(presetMask)      ) | preset   ); } 
inline void            SlicingNode::setSet        ( unsigned int set                  ) { _flags    = ((_flags & ~(setMask)         ) | set      ); } 
inline void            SlicingNode::setPlaced     ( unsigned int placed               ) { _flags    = ((_flags & ~(placedMask)      ) | placed   ); } 
inline void            SlicingNode::setAlignment  ( unsigned int alignment            ) { _flags    = ((_flags & ~(alignmentMask)   ) | alignment); }
inline void            SlicingNode::setType       ( unsigned int type                 ) { _flags    = ((_flags & ~(slicingTypeMask) ) | type     ); }
inline void            SlicingNode::setNodeSets   ( NodeSets*    nodeSets             ) { _nodeSets = nodeSets; }  
inline void            SlicingNode::setParent     ( SlicingNode* parent               ) { _parent   = parent;   }
inline void            SlicingNode::setPairH      ( DbU::Unit    height               ) { _boxSet   = this->getPairH(height);          }
inline void            SlicingNode::setPairHW     ( DbU::Unit height, DbU::Unit width ) { _boxSet   = this->getPairHW(height, width);  }
//inline void            SlicingNode::setRoutingGauge ( CRL::RoutingGauge* rg   ) { _rg     = rg; }
inline void            SlicingNode::setSymmetryFlag ( unsigned int     flag   ) { _flags  = ((_flags & ~(symmetricMask) ) | flag ); }
inline void            SlicingNode::setMaster       ( SlicingNode*     master ) { _master = master; }

inline void            SlicingNode::setOverCell             ( unsigned int overCell    )     { _flags    = ((_flags & ~(overCellMask        )) | overCell)        ; }
inline void            SlicingNode::setRoutingCreated       ( unsigned int routingCreated   ){ _flags    = ((_flags & ~(routingCreatedMask  )) | routingCreated  ); }
inline void            SlicingNode::setRoutingEstimated     ( unsigned int routingEstimated ){ _flags    = ((_flags & ~(routingEstimatedMask)) | routingEstimated); }

inline void            SlicingNode::clearNodeSets () { _nodeSets->clear(); }
inline void            SlicingNode::removeParent  () { _parent   = NULL;  }
    
inline bool            SlicingNode::recursiveCheckPreset () const { return isPreset(); }
inline bool            SlicingNode::recursiveCheckSet    () const { return isSet   (); }
inline bool            SlicingNode::recursiveCheckPlaced () const { return isPlaced(); }
inline int             SlicingNode::getLeafNumber        () const { return 1  ; }


// -----------------------------------------------------------------------------------------------//
// Class : HVSlicingNode
// -----------------------------------------------------------------------------------------------//

class RHVSlicingNode;

class HVSlicingNode: public SlicingNode
{
  public:
    typedef  SlicingNode  Super;
  protected:
                                            HVSlicingNode ( unsigned int type, unsigned int alignment = AlignLeft );
                                   virtual ~HVSlicingNode ();
    
  public:
    inline DbU::Unit                        getToleranceRatioH          () const;
    inline DbU::Unit                        getToleranceRatioW          () const;
           void                             setToleranceRatioH          ( DbU::Unit tolerance );
           void                             setToleranceRatioW          ( DbU::Unit tolerance );
    inline DbU::Unit                        getToleranceBandH           () const;
    inline DbU::Unit                        getToleranceBandW           () const;
           void                             setToleranceBandH           ( DbU::Unit tolerance );
           void                             setToleranceBandW           ( DbU::Unit tolerance );
           void                             recursiveSetToleranceRatioH ( DbU::Unit tolerance );
           void                             recursiveSetToleranceRatioW ( DbU::Unit tolerance );
           void                             recursiveSetToleranceBandH  ( DbU::Unit tolerance );
           void                             recursiveSetToleranceBandW  ( DbU::Unit tolerance );

           void                             _recursiveSetToleranceRatioH ( DbU::Unit tolerance );
           void                             _recursiveSetToleranceRatioW ( DbU::Unit tolerance );
           void                             _recursiveSetToleranceBandH  ( DbU::Unit tolerance );
           void                             _recursiveSetToleranceBandW  ( DbU::Unit tolerance );
           void                             setTolerances                ( DbU::Unit trh
                                                                         , DbU::Unit trw
                                                                         , DbU::Unit tbh
                                                                         , DbU::Unit tbw 
                                                                         );
           void                             recursiveSetTolerances       ( DbU::Unit trh
                                                                         , DbU::Unit trw
                                                                         , DbU::Unit tbh
                                                                         , DbU::Unit tbw 
                                                                         );
           void                             _recursiveSetTolerances      ( DbU::Unit trh
                                                                         , DbU::Unit trw
                                                                         , DbU::Unit tbh
                                                                         , DbU::Unit tbw 
                                                                         );

           bool                             hasEmptyChildrenNodeSets () const;
    inline const std::vector<SlicingNode*>& getChildren              () const;
    inline SlicingNode*                     getChild                 ( int          index ) const;
           int                              getChildIndex            ( SlicingNode* node  ) const;
    inline size_t                           getNbChild               () const;
           void                             createChild              ( unsigned int type, unsigned int alignment );
           void                             createChild              ( NodeSets*    nodeSets  
                                                                     , unsigned int alignment = UnknownAlignment
                                                                     , Instance*    instance  = NULL
                                                                     , BoxSet*      boxSet    = NULL
                                                                     );
           void                             createChild              ( int childIndex, int copyIndex, unsigned int tr );
           void                             insertNode               ( SlicingNode* node, int index );
           void                             push_back                ( SlicingNode* node, bool reset = true );  
           void                             push_front               ( SlicingNode* node ); 
           void                             removeNode               ( SlicingNode* node ); 
    inline void                             removeAllNodes           ();

    inline std::list<std::pair< int,int> >  getSymmetries            () const;
    inline void                             setSymmetries            ( std::list<std::pair< int,int> > sym );
           bool                             isSymmetry               ( int index, std::pair<int,int>& symmetry );
           bool                             isSymmetry               ( int index );
           void                             addSymmetry              ( int childIndex, int copyIndex, bool reset = true );
           void                             removeSymmetry           ( int index );
           void                             normalizeSymmetries      (); // see notes in .cpp

           void                             preDestroy               ();
           void                             destroy                  ();
           void                             preRecursiveDestroy      ();
           void                             recursiveDestroy         ();

           void                             _resetSlicingTree        ();
           void                             print                    () const;
           void                             printChildren            () const;
           void                             printLine                () const;
           void                             printChildrenLine        () const;
           bool                             recursiveCheckPreset     () const;
           bool                             recursiveCheckSet        () const;
           bool                             recursiveCheckPlaced     () const;
           int                              getLeafNumber            () const;
           double                           getDevicesArea           () const;
           double                           getOccupationArea        () const;
           void                             setGlobalSize            ( DbU::Unit height, DbU::Unit width );
           void                             setGlobalSize            ( size_t index );
           void                             _setGlobalSize           ( BoxSet* boxSet );
           std::list<SlicingNode*>          getLeaves                () const;
           bool                             checkInitialPlacement    ( int& cpt ) const; // see notes in .cpp
        
           bool                             isSymmetric              ( SlicingNode* node, unsigned int symmetryType, unsigned int flags=NoFlags  ) const;
           bool                             isSame                   ( SlicingNode* node, unsigned int flags=NoFlags ) const;
           void                             restrictDevices          ();
           void                             setVertexRestriction     ( Net* net, Katana::KatanaEngine* katana );
           void                             estimateChannelsSize     ();
           void                             expandRoutingChannel     ();
           void                             _expandRoutingChannel    ();
           void                             expandRoutingChannel     ( DbU::Unit height, DbU::Unit width );
           bool                             isRoutingEstimated       () const;

           void                             updateGCellPosition      ();
           void                             updateGContacts          ();
           int                              getNbDevices             ();
           virtual string                   _getString               () const;
           virtual string                   _getTypeName             () const;
           void                             setSymmetryFlag          ( unsigned int flag   );
           void                             setMaster                ( SlicingNode* master );

  // Start here
           bool                             checkCellInstances    ( Cell* cell );
           SlicingNode*                     findInstance          ( Instance* instance );
           SlicingNode*                     findSlicingNode       ( Anabatic::GCell* gcell );

           void                             resetSlicingRouting   ();
           void                             destroySlicingRouting ();

           int                              getRoutingIndex       ( SlicingNode* node ) const;
           SlicingNode*                     getSlicingRouting     ( int index ) const;
           void                             clearGCells           (); 
    bool checkSymmetryNet     ( unsigned int type, Net* net1, Net* net2 = NULL ) const;
    void addSymmetryNet       ( unsigned int type, Net* net1, Net* net2 = NULL );
    void updateNetConstraints ();
    void updateSymNetAxis     ();
    void flattenDigitalNets   ();
    
    void updateWireOccupation ( Anabatic::VertexSet );
    void resetWireOccupation  ();

  protected:
    std::vector<SlicingNode*>       _children;
    DbU::Unit                       _toleranceRatioH;
    DbU::Unit                       _toleranceRatioW;
    DbU::Unit                       _toleranceBandH;
    DbU::Unit                       _toleranceBandW;
    std::list<std::pair <int,int> > _symmetries;
    std::vector<RHVSlicingNode*>    _slicingRouting;
    std::vector<std::tuple<unsigned int, Hurricane::Net*, Hurricane::Net*> > _netsymmetries;
};

inline DbU::Unit HVSlicingNode::getToleranceRatioH () const { return _toleranceRatioH; }
inline DbU::Unit HVSlicingNode::getToleranceRatioW () const { return _toleranceRatioW; }
inline DbU::Unit HVSlicingNode::getToleranceBandH  () const { return _toleranceBandH;  }
inline DbU::Unit HVSlicingNode::getToleranceBandW  () const { return _toleranceBandW;  }

inline const std::vector<SlicingNode*>& HVSlicingNode::getChildren    () const            { return _children; }
inline SlicingNode*                     HVSlicingNode::getChild       ( int index ) const { return _children[index]; }
inline size_t                           HVSlicingNode::getNbChild     () const            { return _children.size(); }
inline void                             HVSlicingNode::removeAllNodes ()                  { _children.clear(); }

inline std::list<std::pair<int,int> >   HVSlicingNode::getSymmetries  () const                               { return _symmetries; }
inline void                             HVSlicingNode::setSymmetries  ( std::list<std::pair<int,int> > sym ) { _symmetries = sym;  }

// -----------------------------------------------------------------------------------------------//
// Class : HSlicingNode
// -----------------------------------------------------------------------------------------------//


class HSlicingNode: public HVSlicingNode
{
  public:
    typedef  HVSlicingNode  Super;
  private:
                         HSlicingNode  ( unsigned int type, unsigned int alignment = UnknownAlignment );
                        ~HSlicingNode ();

  public:
    static HSlicingNode* create              ( unsigned int alignment  = AlignLeft );
    void                 createRouting       ( DbU::Unit space );

    void                 print               () const;
    HSlicingNode*        clone               ( unsigned int tr = None );
    void                 place               ( DbU::Unit x = 0, DbU::Unit y = 0 );
    void                 replace             ( DbU::Unit x = 0, DbU::Unit y = 0 );
    void                 _place              ( DbU::Unit x = 0, DbU::Unit y = 0, bool replace = false );
    void                 updateGlobalSize    (); 

    static inline int    getCount            ();
    static inline void   printCount          ();
    static inline void   printCountAll       ();
    void                 preDestroy          ();
    void                 destroy             ();
    void                 preRecursiveDestroy ();
    void                 recursiveDestroy    ();
    
    OpenChams::SlicingNode*   toOpenChams          (); 

    DbU::Unit                 getHeight            () const;
    DbU::Unit                 getWidth             () const;

    void                      createSlicingRouting ();
    void                      setGCell             ( Anabatic::GCell* gcell );
    void                      adjustBorderChannels (); 
    virtual string            _getString           () const;
    virtual string            _getTypeName         () const;

  private:
    static int _count;
    static int _countAll;
};

inline int  HSlicingNode::getCount      () { return _count; }
inline void HSlicingNode::printCount    () { std::cerr << "HSlicingNode::Count      = " << _count    << std::endl; }
inline void HSlicingNode::printCountAll () { std::cerr << "HSlicingNode::CountAll   = " << _countAll << std::endl; }


// -----------------------------------------------------------------------------------------------//
// Class : VSlicingNode
// -----------------------------------------------------------------------------------------------//


class VSlicingNode: public HVSlicingNode
{
  public:
    typedef  HVSlicingNode  Super;
  private:
                         VSlicingNode  ( unsigned int type, unsigned int alignment = UnknownAlignment);
                        ~VSlicingNode (); 

  public:
    static VSlicingNode* create              ( unsigned int alignment = AlignBottom );
    void                 createRouting       ( DbU::Unit space );

    void                 print               () const;
    VSlicingNode*        clone               ( unsigned int tr = None );
    void                 place               ( DbU::Unit x = 0, DbU::Unit y = 0 );
    void                 replace             ( DbU::Unit x = 0, DbU::Unit y = 0 );
    void                 _place              ( DbU::Unit x = 0, DbU::Unit y = 0, bool replace = false );
    void                 updateGlobalSize    ();

    static inline int    getCount            ();
    static inline void   printCount          ();
    static inline void   printCountAll       ();
    void                 preDestroy          ();
    void                 destroy             ();
    void                 preRecursiveDestroy ();
    void                 recursiveDestroy    ();

    OpenChams::SlicingNode*   toOpenChams          (); 
    DbU::Unit                 getHeight            () const;
    DbU::Unit                 getWidth             () const;

    void                      createSlicingRouting ();
    void                      setGCell             ( Anabatic::GCell* gcell );
    void                      adjustBorderChannels (); 
    virtual string            _getString           () const;
    virtual string            _getTypeName         () const;


  private:
    static int _count;
    static int _countAll;
};

inline int  VSlicingNode::getCount      () { return _count; }
inline void VSlicingNode::printCount    () { std::cerr << "VSlicingNode::Count      = " << _count    << std::endl; }
inline void VSlicingNode::printCountAll () { std::cerr << "VSlicingNode::CountAll   = " << _countAll << std::endl; }


// -----------------------------------------------------------------------------------------------//
// Class : DSlicingNode
// -----------------------------------------------------------------------------------------------//


class DSlicingNode: public SlicingNode
{
  public:
    typedef  SlicingNode  Super;
  private:
                          DSlicingNode ( unsigned int type
                                       , NodeSets*    nodeSets  
                                       , unsigned int alignment = UnknownAlignment
                                       , Instance*    instance  = NULL
                                       , BoxSet*      boxSet    = NULL
                                       );
                         ~DSlicingNode ();

  public:
    static DSlicingNode* create                   ( NodeSets*    nodeSets  
                                                  , unsigned int alignment = UnknownAlignment
                                                  , Instance*    instance  = NULL
                                                  , BoxSet*      boxSet    = NULL
                                                  );
           void             print                 () const;
           DSlicingNode*    clone                 ( unsigned int tr = None );
    inline double           getDevicesArea        () const;
           void             setNFing              ( int nfing );
           int              getNFing              () const;
    inline Instance*        getInstance           () const;
    inline void             setInstance           ( Instance* instance );
           void             place                 ( DbU::Unit x = 0, DbU::Unit y = 0 );
           void             _place                ( DbU::Unit x = 0, DbU::Unit y = 0, bool replace = false );

    static inline int       getCount              ();
    static inline void      printCount            ();
    static inline void      printCountAll         ();
    void                    preDestroy            ();
    void                    destroy               ();
    void                    preRecursiveDestroy   ();
    void                    recursiveDestroy      ();

    OpenChams::SlicingNode* toOpenChams           (); 
    double                  getStartParameter     () const;
    double                  getStepParameter      () const;
    double                  getCountParameter     () const;
    bool                    checkInitialPlacement ( int& cpt ) const;// see notes in .cpp

    void                    setGCell                ( Anabatic::GCell* gcell );
    bool                    isSame                  ( SlicingNode* node ) const;    
    bool                    isSymmetric             ( SlicingNode* node, unsigned int symmetryType, unsigned int flags=NoFlags ) const;
    bool                    isSame                  ( SlicingNode* node, unsigned int glags=NoFlags ) const;

    void                    updateGCellPosition     ();
    void                    updateGContacts         ();
    void                    updateMatrixGCellPosition();
    void                    updateMatrixGContacts    ();
    void                    restrictDevices         ();
    void                    printLine               () const;
    bool                    isAnalog                () const;
    bool                    isDigital               () const;
    bool                    checkCellInstances      ( Cell* cell );

    virtual string          _getString              () const;
    virtual string          _getTypeName            () const;

  private:
    Instance*  _instance;
    static int _count;
    static int _countAll;
};

inline double    DSlicingNode::getDevicesArea () const               { return this->getHeight()*this->getWidth(); }
inline Instance* DSlicingNode::getInstance    () const               { return _instance; }
inline void      DSlicingNode::setInstance    ( Instance* instance ) { _instance = instance; }
inline int       DSlicingNode::getCount       ()                     { return _count  ; }
inline void      DSlicingNode::printCount     ()                     { std::cerr << "DSlicingNode::Count      = " << _count    << std::endl; }
inline void      DSlicingNode::printCountAll  ()                     { std::cerr << "DSlicingNode::CountAll   = " << _countAll << std::endl; }


// -----------------------------------------------------------------------------------------------//
// Class : RHVSlicingNode
// -----------------------------------------------------------------------------------------------//


class RHVSlicingNode: public SlicingNode
{
  protected:
                       RHVSlicingNode      ();
                      ~RHVSlicingNode      ();

  public:
    void               print               () const;
    void               _setGlobalSize      ( DbU::Unit height, DbU::Unit width = 0 );
    inline double      getDevicesArea      () const;
    inline bool        isRoutingEstimated  () const;

  // Error Message Methods
    unsigned int       getAlignment        () const;
    NodeSets*          getNodeSets         () const;
    BoxSet*            getPairH            ( DbU::Unit height ) const; 
    BoxSet*            getPairHW           ( DbU::Unit height, DbU::Unit width ) const; 
    void               setPairH            ( DbU::Unit height ); 
    void               setPairHW           ( DbU::Unit height, DbU::Unit width ); 
  
    static inline int  getCount            ();
    static inline void printCount          ();
    static inline void printCountAll       ();
    void               preDestroy          ();
    void               destroy             ();
    void               preRecursiveDestroy ();
    void               recursiveDestroy    ();

    void               resetSize           ();
    void               setHeight           ( DbU::Unit height ){};
    void               setWidth            ( DbU::Unit width  ){};

    bool               isSame              ( SlicingNode* node, unsigned int flags=NoFlags ) const;
    bool               isSymmetric         ( SlicingNode* node, unsigned int symmetryType, unsigned int flags=NoFlags ) const;
    void               estimateChannelsSize (){}
    void               _expandRoutingChannel(){};
    void               expandRoutingChannel ( DbU::Unit height, DbU::Unit width ){}

    void               _place               ( DbU::Unit x = 0, DbU::Unit y = 0, bool replace = false );

    void                        setRailInstance ( Hurricane::Instance* i );
    inline Hurricane::Instance* getRailInstance () const ;
    
    void addWireOccupation    ( DbU::Unit min,  DbU::Unit max );
    void resetWireOccupation  ();
    int  getMaxWireOccupation ();
    
  protected:
    Hurricane::Instance* _railInstance;
    static int          _count;   
    static int          _countAll;   
    DbU::Unit           _minHeight;
    DbU::Unit           _minWidth;
    ChannelRouting*     _wireOccupation;
    
};

inline double RHVSlicingNode::getDevicesArea () const { return 0; }
inline int    RHVSlicingNode::getCount       ()       { return _count; }
inline void   RHVSlicingNode::printCount     ()       { std::cerr << "RHVSlicingNode::Count    = " << _count    << std::endl; }
inline void   RHVSlicingNode::printCountAll  ()       { std::cerr << "RHVSlicingNode::CountAll = " << _countAll << std::endl; }
inline bool   RHVSlicingNode::isRoutingEstimated() const { return (_flags & RoutingEstimated); }
//inline void   RHVSlicingNode::setSegment(Hurricane::Segment* s) { _segment = s; }
inline Hurricane::Instance* RHVSlicingNode::getRailInstance () const { return _railInstance; }

// -----------------------------------------------------------------------------------------------//
// Class : RHSlicingNode
// -----------------------------------------------------------------------------------------------//


class RHSlicingNode: public RHVSlicingNode
{
  private:
                            RHSlicingNode ( DbU::Unit height = 0 );
  //RHSlicingNode ( Hurricane::Horizontal* h );
                            RHSlicingNode ( Hurricane::Instance* i );
                           ~RHSlicingNode ();

  public:
    static RHSlicingNode*   create        ( DbU::Unit height = DbU::Unit(0) );
  //static RHSlicingNode*   create        ( Hurricane::Horizontal* h );
    static RHSlicingNode*   create        ( Hurricane::Net*   net
                                          , Hurricane::Layer* layer
                                          , int               npitch
                                          , string            cname
                                          , string            iname
                                          );
  //static RHSlicingNode*   create        ( OpenChams::SlicingNode* ocSlicingTree, CRL::RoutingGauge* rg );
           RHSlicingNode*   clone         ( unsigned int tr = None );

    OpenChams::SlicingNode* toOpenChams(); 
    DbU::Unit               getWidth   () const;
    void                    setHeight  ( DbU::Unit height );

    void                    estimateChannelsSize ();
    void                    _expandRoutingChannel();
    void                    expandRoutingChannel ( DbU::Unit height, DbU::Unit width );
    void                    setGCell             ( Anabatic::GCell* gcell );
    void                    updateMasterSize     ( DbU::Unit size ); 
    virtual string          _getTypeName         () const;

  // Error Message Methods
    void  setWidth ( DbU::Unit width );
};


// -----------------------------------------------------------------------------------------------//
// Class : RVSlicingNode
// -----------------------------------------------------------------------------------------------//


class RVSlicingNode: public RHVSlicingNode
{
  private:
                          RVSlicingNode ( DbU::Unit width = 0 );
  //RVSlicingNode ( Hurricane::Vertical* v );
                          RVSlicingNode ( Hurricane::Instance* i );
                         ~RVSlicingNode ();

  public:
    static RVSlicingNode* create ( DbU::Unit width = DbU::Unit(0) );
    static RVSlicingNode* create ( Hurricane::Vertical* v );
    static RVSlicingNode* create ( Hurricane::Net*   net
                                 , Hurricane::Layer* layer
                                 , int               npitch
                                 , string            cname
                                 , string            iname
                                 );
  //static RVSlicingNode* create ( OpenChams::SlicingNode* ocSlicingTree, CRL::RoutingGauge* rg );
           RVSlicingNode* clone  ( unsigned int tr = None );

    OpenChams::SlicingNode* toOpenChams();
    DbU::Unit               getHeight  () const;
    void                    setWidth   ( DbU::Unit width );

    void                    estimateChannelsSize ();
    void                    _expandRoutingChannel();
    void                    expandRoutingChannel ( DbU::Unit height, DbU::Unit width );
    void                    setGCell             ( Anabatic::GCell* gcell );
    void                    updateMasterSize     ( DbU::Unit size ); 
    virtual string          _getTypeName         () const;
 
  // Error Message Methods
    void  setHeight ( DbU::Unit height );
};


// -----------------------------------------------------------------------------------------------//
// Class : HVSetState
// Description:
// HVSetState, HSetState and VSetState are classes used in the algorithm to determine the possible
// dimensions for a slicing tree. They are not part of the slicing tree, they only help during the 
// use of updateGlobalSize. At the end of the algorithm, it should provide the resulting NodeSets.
//
// In a horizontal/vertical slicing node, the number of possible dimensions (EndCounter) to study is:
//   EndCounter = N(child0) * N(child1) * ... * N(childn)
// We call a "state" a dimension for the horizontal/vertical slicing node. "EndCounter" states needs
// to be studied.
// For each states, we check if these conditions are filled:
// - Horizontal case: toleranceBandW <= max(children's width )-min(children's width )
// - Vertical   case: toleranceBandH <= max(children's height)-min(children's height)
// When the condition is filled, we add the dimensions to the NodeSets and we proceed to the next combinations.
// -----------------------------------------------------------------------------------------------//

class HVSetState
{
  protected:
                      HVSetState ( HVSlicingNode* node );
             virtual ~HVSetState ();

  public:
    virtual DbU::Unit getCurrentH   () = 0;
    virtual DbU::Unit getCurrentW   () = 0;
    inline  bool      end           ();
    inline  int       getEndCounter ();
            NodeSets* getNodeSets   (); 
            bool      isSymmetry    ( int index, std::pair<int,int>& symmetry );
            bool      isSymmetry    ( int index );

    virtual void      print         ();
            void      initSet       ();
            void      initModulos   (); // see notes in .cpp
            void      next          (); // see notes in .cpp
    virtual void      push_back     () = 0;

  protected: 
    HVSlicingNode*      _HVSnode; 
    int                 _counter;
    std::vector<int>    _modulos;
    std::vector<size_t> _currentSet;
    std::vector<size_t> _nextSet;
    NodeSets*           _nodeSets;
};

inline bool HVSetState::end           ()                                          { return (_counter == _modulos.back()+1); }
inline int  HVSetState::getEndCounter ()                                          { return _modulos.back()+1; }
inline bool HVSetState::isSymmetry    ( int index, std::pair<int,int>& symmetry ) { return _HVSnode->isSymmetry(index, symmetry); }
inline bool HVSetState::isSymmetry    ( int index )                               { return _HVSnode->isSymmetry(index); }


// -----------------------------------------------------------------------------------------------//
// Class : HSetState
// -----------------------------------------------------------------------------------------------//


class HSetState: public HVSetState
{
  public:
                                   HSetState ( HSlicingNode* node );
                                  ~HSetState ();

    std::pair<DbU::Unit,DbU::Unit> getCurrentWs (); // see notes in .cpp
    DbU::Unit                      getCurrentH  (); // see notes in .cpp
    DbU::Unit                      getCurrentW  (); // see notes in .cpp
    void                           print        ();
    void                           next         ();
    void                           push_back    (); // see notes in .cpp
};


// -----------------------------------------------------------------------------------------------//
// Class : VSetState
// -----------------------------------------------------------------------------------------------//


class VSetState: public HVSetState
{
  public:
                                   VSetState ( VSlicingNode* node );
                                  ~VSetState ();
      
    std::pair<DbU::Unit,DbU::Unit> getCurrentHs (); // see notes in .cpp
    DbU::Unit                      getCurrentH  (); // see notes in .cpp
    DbU::Unit                      getCurrentW  (); // see notes in .cpp
    void                           print        ();
    void                           next         ();
    void                           push_back    (); // see notes in .cpp
};
  



GETSTRING_POINTER_SUPPORT(SlicingNode)
GETSTRING_POINTER_SUPPORT(HSlicingNode)
GETSTRING_POINTER_SUPPORT(VSlicingNode)
GETSTRING_POINTER_SUPPORT(DSlicingNode)
GETSTRING_POINTER_SUPPORT(RHSlicingNode)
GETSTRING_POINTER_SUPPORT(RVSlicingNode)
IOSTREAM_POINTER_SUPPORT(SlicingNode)
IOSTREAM_POINTER_SUPPORT(HSlicingNode)
IOSTREAM_POINTER_SUPPORT(VSlicingNode)
IOSTREAM_POINTER_SUPPORT(DSlicingNode)
IOSTREAM_POINTER_SUPPORT(RHSlicingNode)
IOSTREAM_POINTER_SUPPORT(RVSlicingNode)

#endif

