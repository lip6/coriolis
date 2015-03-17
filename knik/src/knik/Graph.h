// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2006-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K n i k  -  G l o b a l   R o u t e r                    |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./knik/Graph.h"                                |
// +-----------------------------------------------------------------+


#ifndef KNIK_GRAPH_H
#define KNIK_GRAPH_H

#include <math.h>
#include "hurricane/RoutingPad.h"
#include "knik/Vertex.h"
#include "knik/Vertexes.h"
#include "knik/Edge.h"
#include "knik/MatrixVertex.h"
#include "knik/VTuple.h"
#include "knik/STuple.h"
#include "knik/SlicingTree.h"
#include "knik/RoutingGrid.h"

struct FTree;

namespace Knik {

  class KnikEngine;

    class Graph {
    friend class Vertex;
    // Types
    // *****
        public:
                struct VTupleCompare : public binary_function<const VTuple*, const VTuple*, bool>
                {
                    // Used to sort VTuplePriorityQueue
                    bool operator()(const VTuple* vtuple1, const VTuple* vtuple2) const
                    {
                        if ( vtuple1->getDistance() <  vtuple2->getDistance() )  return true;
                        if ( vtuple1->getDistance() == vtuple2->getDistance() ) {
                            if ( vtuple1->getVertex()->getX() <  vtuple2->getVertex()->getX() ) return true;
                            if ((vtuple1->getVertex()->getX() == vtuple2->getVertex()->getX()) &&
                                (vtuple1->getVertex()->getY() <  vtuple2->getVertex()->getY())) return true;
                        }
                        return false;
                    }
                };

                typedef set<VTuple*, VTupleCompare>                VTuplePriorityQueue;
                typedef set<VTuple*, VTupleCompare>::iterator      VTuplePQIter;
                typedef set<Vertex*,VertexPositionComp>            VertexSet;
                typedef set<Vertex*,VertexPositionComp>::iterator  VertexSetIter;
                typedef list<Vertex*>                              VertexList;
                typedef list<Vertex*>::iterator                    VertexListIter;
                typedef vector<Vertex*>                            VertexVector;
                typedef vector<Edge*>                              EdgeVector;

    // Attributes
    // **********
        private: 
                Cell*               _cell;
                KnikEngine*         _engine;
                bool                _benchMode;
                bool                _useSegments;
                SlicingTree*        _slicingTree;
                MatrixVertex*       _matrixVertex;
                RoutingGrid*        _routingGrid;
                Net*                _working_net;
                VertexSet           _vertexes_to_route;
                Vertex*             _lowerLeftVertex;
                VertexVector        _all_vertexes; 
                EdgeVector          _all_edges;
                unsigned            _nbSplitters;
                VTuplePriorityQueue _vtuplePriorityQueue;
                STuple::STuplePriorityQueue _stuplePriorityQueue;
                Box                 _searchingArea;
                unsigned int        _xSize;
                unsigned int        _ySize;
              //DensityWindow*      _estimateOccupancyWindow;
              //DensityWindow*      _occupancyWindow;
                float               _maxEstimateOccupancy;
                float               _maxXEstimateOccupancy;
                float               _maxYEstimateOccupancy;
                unsigned            _maxOccupancy;
                unsigned            _maxXOccupancy;
                unsigned            _maxYOccupancy;
                unsigned            _netStamp;
                float               _hEdgeNormalisedLength;
                float               _vEdgeNormalisedLength;

    // Constructors & Destructors
    // **************************
        protected:
            Graph  ( KnikEngine*, RoutingGrid*, bool benchMode, bool useSegments );
            ~Graph() {};
        public:
            static Graph* create ( KnikEngine*, RoutingGrid*, bool benchMode, bool useSegments );
            void   destroy();
            void  _postCreate();
            void  _preDestroy();

    // Accessors
    // *********
        public:
            KnikEngine* getEngine               () { return _engine; }
            Cell*       getCell                 () { return _cell; };
            unsigned    getNbSplitters          () { return _nbSplitters; };
            unsigned    getNetStamp             () { return _netStamp; };
            Net*        getWorkingNet           () { return _working_net; };
            Vertex*     getPredecessor          ( const Vertex* vertex );
            Edge*       getEdgeBetween          ( Vertex* vertex1, Vertex* vertex2 );
            Edge*       getEdge                 ( unsigned col1, unsigned row1, unsigned col2, unsigned row2 );
            Vertex*     getCentralVertex        ();
            Vertex*     getVertex               ( Point );
            Vertex*     getVertex               ( DbU::Unit x, DbU::Unit y );
            Vertexes    getVertexes             ()    { return VectorCollection<Vertex*>(_all_vertexes); };
            Vertex*     getLowerLeftVertex      () { return _lowerLeftVertex; };
            unsigned    getGridLength           ( Segment* segment );
            unsigned    getCongestEdgeNb        ( Segment* segment );
            size_t      getXSize                () const { return (_matrixVertex) ? _matrixVertex->getXSize() : 0; };
            size_t      getYSize                () const { return (_matrixVertex) ? _matrixVertex->getYSize() : 0; };
            float       getHEdgeNormalisedLength() const { return _hEdgeNormalisedLength; };
            float       getVEdgeNormalisedLength() const { return _vEdgeNormalisedLength; };

    // Modifiers
    // *********
        private:
            void   initConnexComp     ( Vertex* vertex, int newConnexID = -1 );
            void   initConnexComp     ( Vertex* vertex, Edge* arrivalEdge, int newConnexID );
            void   UpdateConnexComp   ( VertexList reachedVertexes, Vertex* firstVertex );
            void   MaterializeRouting ( Vertex* vertex );
            void   MaterializeRouting ( Vertex* vertex, Edge* arrivalEdge, Contact* initialContact = NULL );

            void   updateEdgesOccupancy   ( Segment* segment, bool add );
            void   rebuildConnexComponent ( Contact* contact, int connexID, Segment* arrivalSegment ); 
            void   setNetStampConnexID    ( Segment* segment, int connexID );
        public:
            Vertex* createVertex     ( Point position, DbU::Unit halfWidth, DbU::Unit halfHeight );
            void   createHEdge       ( Vertex* from, Vertex* to, size_t reserved=0 );
            void   createVEdge       ( Vertex* from, Vertex* to, size_t reserved=0 );
            void   resetVertexes     ()                    { _vertexes_to_route.clear(); };
            void   setNetStamp       ( unsigned netStamp ) { _netStamp = netStamp; };
            void   incNetStamp       ()                    { _netStamp++; };
            int    countVertexes     ( Net* net );
            int    initRouting       ( Net* net );
            void   Dijkstra          ();
            void   Monotonic         ();
            FTree* createFluteTree   ();
            void   CleanRoutingState ();
            void   UpdateEstimateCongestion ( bool create = false );
            void   UpdateMaxEstimateCongestion ();
            void   UpdateEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, unsigned cap );
            void   increaseEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, int cap );
            void   insertSegment ( Segment* segment );
            void   removeSegment ( Segment* segment );
            void   increaseEdgesCostParameters() { Edge* edge = _all_edges[0]; assert(edge);
                                                   edge->setHParameter(edge->getHParameter()*100);
                                                   edge->setKParameter(edge->getKParameter()*100); };

    // Predicates
    // **********
        private:
            bool   hasGlobalRouting        ( RoutingPad* routingPad );
            bool   isAGlobalRoutingContact ( Contact* contact );
            bool   isAGlobalRoutingSegment ( Segment* segment );

    // Others
    // ******
        private:
            void           sortHVertexes ( Vertex *& from, Vertex *& to );
            void           sortVVertexes ( Vertex *& from, Vertex *& to );
            Segment*       createSegment ( Contact* initialContact, Contact* reachedContact );
            float          getSegmentCost ( Segment* segment );
          //DensityWindow* createEstimateOccupancyWindow();
          //DensityWindow* createOccupancyWindow();
        public:
            unsigned       analyseRouting ( set<Segment*>& segmentsToUnroute );
            //void           printStats();
            void           checkGraphConsistency();
            void           checkEmptyPriorityQueue();
          //void           UpdateEstimateOccupancyWindow();
          //void           UpdateOccupancyWindow();
            void           getHorizontalCutLines ( vector<DbU::Unit>& horizontalCutLines );
            void           getVerticalCutLines   ( vector<DbU::Unit>& verticalCutLines );

            string        _getString() const;
            string        _getTypeName() const { return _TName("Graph"); };
            Record*       _getRecord() const;

            void           testSTuplePQ();

    // VTuplePriorityQueue Utility Methods
    // **********************************
        public:
            Vertex* extractMinFromPriorityQueue();
            Vertex* getMinFromPriorityQueue();
            void    PopMinFromPriorityQueue();
            //VTuple*  getTopVTupleFromPriorityQueue();
            //void    PopTopVTupleFromPriorityQueue();
            void    addVTupleToPriorityQueue ( VTuple* vtuple );
            void    increaseVTuplePriority ( VTuple* vtuple, float distance );
            void    printVTuplePriorityQueue();
            void    clearPriorityQueue();

    // STuplePriorityQueue Utility Methods
    // **********************************
        public:
            Segment*  extractMaxFromSTuplePQ();
            STuple*   getMaxFromSTuplePQ();
            void      popMaxFromSTuplePQ();
            void      addToSTuplePQ ( STuple* stuple );
            void      updateSTupleCost ( STuple* stuple, unsigned cost );
            void      printSTuplePQ();
            void      clearSTuplePQ();
    }; // class Graph

} // namespace Knik

INSPECTOR_P_SUPPORT(Knik::Graph);

#endif
