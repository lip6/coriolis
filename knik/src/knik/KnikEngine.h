
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
// |                 Knik - Global Router                            |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./KnikEngine.h"                                |
// | *************************************************************** |
// |  Date        :  30/10/2006                                      |
// |                                                                 |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/Timer.h"
#include "hurricane/Property.h"
#include "hurricane/Net.h"
#include "hurricane/RoutingPad.h"

//#include "CEditor.h"
#include "crlcore/ToolEngine.h"

#ifndef _KNIK_H
#define _KNIK_H


namespace Knik {


using std::string;
using std::vector;
using std::set;
using std::map;
using std::pair;
using std::binary_function;
using Hurricane::_TName;
using Hurricane::Record;
using Hurricane::Timer;
using Hurricane::Name;
using Hurricane::DbU;
using Hurricane::Point;
using Hurricane::Box;
using Hurricane::Net;
using Hurricane::Segment;
using Hurricane::Contact;
using Hurricane::Cell;


class Vertex;
class Graph;
class RoutingGrid;

class KnikEngine : public CRL::ToolEngine {
// *******************************

// Type Definition
// ***************
struct NetRecord {
// ***************
    public: Net*     _net;
            long int _exArea; // extendedArea represents (boundingBoxWidth + 1) * (boundingBoxHeigth + 1)

    public: NetRecord ( Net* net, long int exArea ) : _net ( net ), _exArea ( exArea ) {}
            NetRecord ( const NetRecord & record ) : _net ( record._net ), _exArea ( record._exArea ) {}
            bool operator== ( const NetRecord & record ) const {
                return (record._net == _net) && (record._exArea == _exArea);
            }
};

struct SegRecord {
// ***************
    public: unsigned _NbTotEdges;
            unsigned _NbOvEdges;
            unsigned _SumOv;
            unsigned _MaxOv;

            SegRecord (): _NbTotEdges (0), _NbOvEdges (0), _SumOv (0), _MaxOv (0) {};

            void incNbTotEdges ()              { _NbTotEdges++; };
            void incNbOvEdges  ()              { _NbOvEdges++; };
            void incSumOv      ( unsigned ov ) { _SumOv += ov; };
            void UpdateMaxOv   ( unsigned ov ) { _MaxOv = ov > _MaxOv ? ov : _MaxOv; };

            unsigned getNbTotEdges () { return _NbTotEdges; };
            unsigned getNbOvEdges  () { return _NbOvEdges; };
            unsigned getSumOv      () { return _SumOv; };
            unsigned getMaxOv      () { return _MaxOv; };
};

// Comparison methods
// ******************
struct NetSurfacesComp : public binary_function<const NetRecord, const NetRecord, bool> { 
    public: bool operator() ( const NetRecord &record1, const NetRecord &record2 ) const {
                if  (record1._exArea <  record2._exArea) return true;
                if  (record1._exArea == record1._exArea) {
                    if (getString(record1._net->getName()) <  getString(record2._net->getName())) return true;
                    if (record1._net == record2._net) assert ( false );
                }
                return false;
            }
};

struct NetNameComp : public binary_function<const NetRecord, const NetRecord, bool> {
    public: bool operator() ( const NetRecord &record1, const NetRecord &record2 ) const {
                if ( record1._net->getName() < record2._net->getName() ) return true;
                if ( record1._net == record2._net ) assert (false ); 
                return false;
            }
}; 

typedef CRL::ToolEngine      Inherit;
typedef vector<NetRecord> NetVector;

// Attributes
// **********
    private:
        static const Name  _toolName;
        static float       _edgeCapacityPercent;
        Graph*             _routingGraph;
        RoutingGrid*       _routingGrid;
        Timer              _timer;
        NetVector          _nets_to_route;
        bool               _benchMode;
        bool               _useSegments;
        bool               _routingDone;
        unsigned           _rerouteIteration;
        map<Segment*,SegRecord> _segmentOverEdges;
        vector<pair<Segment*,SegRecord*> > _sortSegmentOv;
        set<Segment*> _segmentsToUnroute;

// Constructors & Destructors
// **************************
    protected: 
        KnikEngine ( Cell* cell, unsigned congestion, unsigned precongestion, bool benchMode, bool useSegments, float edgeCost );
        ~KnikEngine();

    public:
        static KnikEngine* create ( Cell* cell, unsigned congestion = 1, unsigned precongestion = 2, bool benchMode = false, bool useSegments = true, float edgeCost = 3.0 );
        void  _postCreate();
        void   destroy();
        void  _preDestroy();

// Modifiers
// *********
    private:
        void     MakeRoutingLeaves();
        void     unroute ( Segment* segment, set<Segment*> &segmentsToUnroute, Contact* fromContact = NULL );
//    private: unsigned getGeometry  ( Net* net, set<Vertex*,VertexPositionComp> &vertexes, Box &vertexCenterBoundingBox );
//    private: void     RouteFlatNet ( Net* net, set<Vertex*,VertexPositionComp> vertexes, Box vertexCenterBoundingBox );
//    private: void     RouteLNet    ( Net* net, set<Vertex*,VertexPositionComp> vertexes );
//    private: void     RestructureNet ( Net* net );
//    private: void     createLimitedZone ( Net* net, set<Vertex*,VertexPositionComp> gcells, Box vertexCenterBoundingBox, unsigned netStamp );
        string   adaptString ( string s );
    public:
        static void  setEdgeCapacityPercent ( float ecp ) { _edgeCapacityPercent = ecp; };
        void     initGlobalRouting(); // Making it public, so it can be called earlier and then capacities on edges can be ajusted
        void     run();
        void     Route();
        void     createRoutingGrid ( unsigned   nbXTiles
                                   , unsigned   nbYTiles
                                   , const Box& boundingBox
                                   , DbU::Unit  tileWidth
                                   , DbU::Unit  tileHeight
                                   , unsigned   hcapacity
                                   , unsigned   vcapacity );
        void     updateEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, unsigned capacity );
        void     increaseEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, int capacity );
        void     insertSegment ( Segment* segment );
        bool     analyseRouting();
        void     unrouteOvSegments();
        void     reroute();
        void     unrouteSelected();
        // for ispd07 reload
        void     createRoutingGraph();
        void     addRoutingPadToGraph ( Hurricane::RoutingPad* routingPad );
        Vertex*  getVertex ( Point );
        Vertex*  getVertex ( DbU::Unit x, DbU::Unit y );
            
// Others
// ******
    public:
        static  KnikEngine* get                       ( const Cell* );
        static  const Name& staticGetName             () { return _toolName; };
        static  float       getEdgeCapacityPercent    () { return _edgeCapacityPercent; };
                const Name& getName                   () const { return _toolName; };
                void        printTime                 ();
                void        computeOverflow           ();
                void        computeSymbolicWireLength ();
      //        void        showOccupancy             ();
      //        void        showEstimateOccupancy     ();
                void        getHorizontalCutLines     ( vector<DbU::Unit>& horizontalCutLines );
                void        getVerticalCutLines       ( vector<DbU::Unit>& verticalCutLines );
                void        saveSolution              ( const string& fileName="" );
                void        loadSolution              ( const string& fileName="" );
                string      _getSolutionName          () const;
        virtual Record*     _getRecord                () const;
        virtual string      _getTypeName              () const { return _TName ( "KnikEngine" ); };
};


}

#endif
