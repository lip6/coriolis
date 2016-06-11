
#include <cmath>
#include <sstream>
#include <algorithm>
#include <memory>

#include "hurricane/DebugSession.h"
#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/Name.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Component.h"
#include "hurricane/Net.h"
#include "hurricane/DeepNet.h"
#include "hurricane/Cell.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Breakpoint.h"

#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/RoutingLayerGauge.h"

#include "knik/Configuration.h"
#include "knik/Graph.h"
#include "knik/Vertex.h"
#include "knik/Edge.h"
#include "knik/HEdge.h"
#include "knik/VEdge.h"
#include "knik/KnikEngine.h"

#include "knik/flute.h"

//#define __USE_SLICINGTREE__
#define __USE_MATRIXVERTEX__

#define EPSILON 10e-4
#define HISTORIC_INC 1.5 // define the increment of historic cost for ripup & reroute

namespace Knik {

  using Hurricane::tab;
  using Hurricane::ForEachIterator;

int  depthMaterialize;
unsigned countDijkstra    = 0;
unsigned countMonotonic   = 0;
unsigned countMaterialize = 0;
bool debugging;
Name debugName = Name("");

bool __ripupMode__;

extern float __edge_capacity_percent__;
extern unsigned __congestion__;
extern unsigned __precongestion__;

using namespace CRL;

STuple::STuplePQIter STuple::_stuplePQEnd;
Name STuple::CostProperty::_name = "Knik::CostProperty";

struct segmentStat {
    unsigned nbDep;
    unsigned nbTot;
    unsigned sumOv;

    segmentStat(unsigned dep, unsigned tot, unsigned ov) { nbDep=dep; nbTot=tot; sumOv=ov; };
    segmentStat() {};
    ~segmentStat() {};

    void incNbDep() { nbDep++; };
    void incNbTot() { nbTot++; };
    void incSumOv ( unsigned inc ) { sumOv+=inc; };

    unsigned getNbDep() const { return nbDep; };
    unsigned getNbTot() const { return nbTot; };
    unsigned getSumOv() const { return sumOv; };
};

Graph::Graph ( KnikEngine* engine, RoutingGrid* routingGrid, bool benchMode, bool useSegments )
// ********************************************************************************************
    : _cell ( engine->getCell() )
    , _engine ( engine )
    , _benchMode ( benchMode )
    , _useSegments ( useSegments )
    , _slicingTree ( NULL )
    , _matrixVertex ( NULL )
    , _routingGrid ( routingGrid )
    , _working_net ( NULL )
    , _vertexes_to_route()
    , _lowerLeftVertex ( NULL )
    , _all_vertexes()
    , _all_edges()
    , _nbSplitters ( 0 )
    , _vtuplePriorityQueue()
    , _stuplePriorityQueue()
    , _searchingArea()
    , _xSize ( 0 )
    , _ySize ( 0 )
    //, _estimateOccupancyWindow ( NULL )
    //, _occupancyWindow ( NULL )
    , _maxEstimateOccupancy ( 0.0 )
    , _maxXEstimateOccupancy ( 0.0 )
    , _maxYEstimateOccupancy ( 0.0 )
    , _maxOccupancy ( 0 )
    , _maxXOccupancy ( 0 )
    , _maxYOccupancy ( 0 )
    , _hEdgeNormalisedLength ( 1.0 ) // au cas ou
    , _vEdgeNormalisedLength ( 1.0 ) // au cas ou
{
    __ripupMode__ = false;
}

Graph* Graph::create ( KnikEngine* engine, RoutingGrid* routingGrid, bool benchMode, bool useSegments )
// ****************************************************************************************************
{
    Graph* _graph = new Graph ( engine, routingGrid, benchMode, useSegments );

    _graph->_postCreate();

    return _graph;
}

void Graph::_postCreate()
// **********************
{
    _netStamp = 1; // initializing netStamp here instead of in Knik::Route function (for reroute, it seems good)

  //Cell* cell = _nimbus->getCell();
  //DisplaySlot* vertexDS = DisplaySlot::create( cell, Name("KnikVertexDS"),192, 0 ,192,"FFFFFFFFFFFFFFFF",1,192, 0 ,192,"FFFFFFFFFFFFFFFF",1 );
  //DisplaySlot* edgeDS   = DisplaySlot::create( cell, Name("KnikEdgeDS")  ,153,100, 17,"55AA55AA55AA55AA",1,153,100, 17,"55AA55AA55AA55AA",1 );

  // XXX On supprime tout ce qui concerne NIMBUS et on considère qu'il existe toujours une _routingGrid XXX
    if ( !_routingGrid ) {
#ifdef __USE_MATRIXVERTEX__
      _matrixVertex = MatrixVertex::create(this);
      if( !_matrixVertex )
        throw Error ("Graph::_postCreate(): cannot create MatrixVertex");
      _lowerLeftVertex = _matrixVertex->createRegularMatrix ();
      cmess2 << "     - Created vertex matrix (without RoutingGrid)" << endl;
#else
      throw Error ("Graph::_postCreate(): cannot use another method than MatrixVertex");
#endif
    // il faut définir les normalisedLength, pour l'instant on le fait pas
    } else {
      cmess2 << "     - RoutingGrid size [" << _routingGrid->getNbXTiles() << "x" << _routingGrid->getNbYTiles() << "]." << endl;
#ifdef __USE_MATRIXVERTEX__
      _matrixVertex = MatrixVertex::create(this);
      if( !_matrixVertex )
        throw Error ("Graph::_postCreate(): cannot create MatrixVertex");

      _lowerLeftVertex = _matrixVertex->createRegularMatrix ( _routingGrid );
    // initialisation des normalisedLength
      float hEdgeLength = _routingGrid->getTileWidth();
      float vEdgeLength = _routingGrid->getTileHeight();
      _hEdgeNormalisedLength = hEdgeLength <= vEdgeLength ? 1.0 : hEdgeLength / vEdgeLength;
      _vEdgeNormalisedLength = hEdgeLength <= vEdgeLength ? vEdgeLength / hEdgeLength : 1.0;
      cmess2 << "     - Created vertex matrix (from RoutingGrid)." << endl;
#else
      throw Error ("Graph::_postCreate(): cannot use another method than MatrixVertex");
#endif
    }

    if (_lowerLeftVertex->getHEdgeOut())
      cmess1 << Dots::asUInt ("     - Global router H edges capacity"     ,_lowerLeftVertex->getHEdgeOut()->getCapacity()) << endl;
    else
      cerr << Warning( "Knik::Graph: Design has only one column, H edge capacity is zero." ) << endl;

    if (_lowerLeftVertex->getVEdgeOut())
      cmess1 << Dots::asUInt ("     - Global router V edges capacity"     ,_lowerLeftVertex->getVEdgeOut()->getCapacity()) << endl;
    else
      cerr << Warning( "Knik::Graph: Design has only one row, V edge capacity is zero." ) << endl;
    
//    #ifdef __USE_MATRIXVERTEX__
//        _matrixVertex = MatrixVertex::create(this);
//        if ( _routingGrid ) {  
//            _matrixVertex->createXRegular ( _routingGrid );
//            _matrixVertex->createYRegular ( _routingGrid );
//        }
//        else { 
//            _matrixVertex->createXIrregular ( _nimbus );
//            _matrixVertex->createYIrregular ( _nimbus );
//        }
//    #endif
//    #ifdef __USE_SLICINGTREE__
//        _slicingTree = SlicingTree::create ( _nimbus );
//    #endif
//
//    unsigned compteur = 0;
//    for_each_gcell ( gcell, _nimbus->getRoutingLeaves() )
//    {
//        compteur++;
//        Vertex* vertex = Vertex::create ( gcell, this );
//        if ( !_lowerLeftVertex )
//            _lowerLeftVertex = vertex;
//        else {
//            if ( vertex->getX() <= _lowerLeftVertex->getX() ) {
//                if ( vertex->getY() <= _lowerLeftVertex->getY() )
//                    _lowerLeftVertex = vertex;
//            }
//        }
//        _all_vertexes.push_back ( vertex );
//        #ifdef __USE_MATRIXVERTEX__
//            _matrixVertex->setVertex ( gcell->getCenter(), vertex );
//        #endif
//        #ifdef __USE_SLICINGTREE__
//            SlicingTreeNode* terminalNode = _slicingTree->getTerminalNode ( gcell->getCenter() );
//            terminalNode->setVertex ( vertex );
//        #endif
//        end_for;
//    }
//    cmess2 << "           - Parcours des gcells pour créer les vertex terminé : " << compteur << " vertex" << endl;
//
//    Vertex* currentVertex = _lowerLeftVertex;
//    while ( currentVertex ) {
//        Vertex* firstLineVertex = currentVertex;
//        while ( currentVertex ) {
//            GCell* source   = currentVertex->getGCell();
//            Fence* fenceUP  = source->getUpFence();
//            GCell* targetUP = source->getUpOfMe();
//            if ( targetUP )
//                createVEdge ( currentVertex, getVertex ( targetUP->getCenter() ), fenceUP );
//            Fence* fenceRIGHT  = source->getRightFence();
//            GCell* targetRIGHT = source->getRightOfMe();
//            if ( targetRIGHT ) {
//                Vertex* rightVertex = getVertex ( targetRIGHT->getCenter() );
//                createHEdge ( currentVertex, rightVertex, fenceRIGHT );
//                currentVertex = rightVertex;
//            }
//            else
//                break;
//        }
//        Edge* vEdgeOut = firstLineVertex->getVEdgeOut();
//        if ( vEdgeOut )
//            currentVertex = vEdgeOut->getOpposite ( firstLineVertex );
//        else
//            break;
//    }
//    cmess2 << "           - Parcours des gcells puis des fences pour créer les edges terminé" << endl;

    for ( unsigned i = 0 ; i < _all_vertexes.size() ; i++ ) {
        _all_vertexes[i]->sortEdges();
    }
    cmess2 << "     - Edge sorting completed." << endl;

    STuple::setSTuplePQEnd ( _stuplePriorityQueue.end() );

//    cmess2 << "Petites stats :" << endl
//         << "    gcell : " << sizeof(GCell) << endl
//         << "    fence : " << sizeof(Fence) << endl
//         << "    vertex : " << sizeof(Vertex) << endl
//         << "    edge : " << sizeof (Edge) << endl
//         << "    net : " << sizeof(Net) << endl
//         << "    pin : " << sizeof(Pin) << endl
//         << "    splitter : " << sizeof(Splitter) << endl
//         << "    splitterContact : " << sizeof(SplitterContact) << endl;

}

void Graph::destroy()
// *****************
{
    _preDestroy();
    delete this;
}

void Graph::_preDestroy()
// *********************
{
    // Destruction of all VTuples
    for ( VTuplePQIter pqit = _vtuplePriorityQueue.begin() ; pqit != _vtuplePriorityQueue.end() ; pqit++ ) {
        (*pqit)->destroy();
    }

    // Destrucion of all Edges and Vertexes
    Vertex* currentVertex = _lowerLeftVertex;
    while ( currentVertex ) {
        Vertex* nextVVertex;
        if ( Edge* vEdgeOut = currentVertex->getVEdgeOut() )
            nextVVertex = vEdgeOut->getOpposite ( currentVertex );
        else
            nextVVertex = NULL;

        while ( currentVertex ) {
            if ( Edge* vEdgeOut = currentVertex->getVEdgeOut() )
                vEdgeOut->destroy();

            Vertex* nextHVertex;
            if ( Edge* hEdgeOut = currentVertex->getHEdgeOut() ) {
                nextHVertex = hEdgeOut->getOpposite ( currentVertex );
                hEdgeOut->destroy();
            }
            else
                nextHVertex = NULL;
            currentVertex->destroy();
            currentVertex = nextHVertex;
        }
        currentVertex = nextVVertex;
    }

    //// Destruction of all Edges
    //for ( unsigned i = 0 ; i < _all_edges.size() ; i++ ) {
    //    _all_edges[i]->destroy();
    //}

    //// Destruction of all Vertexes
    //for ( unsigned i = 0 ; i < _all_vertexes.size() ; i++ ) {
    //    _all_vertexes[i]->destroy();
    //}

    #ifdef __USE_MATRIXVERTEX__
        _matrixVertex->destroy();
    #endif
    #ifdef __USE_SLICINGTREE__
        _slicingTree->destroy();
    #endif
}

Vertex* Graph::getPredecessor ( const Vertex* vertex )
// ***************************************************
{
    assert( vertex->getPredecessor() );
    return vertex->getPredecessor()->getOpposite ( vertex );
}


Vertex* Graph::getCentralVertex()
// ******************************
{
    assert ( _vertexes_to_route.begin() != _vertexes_to_route.end() );
    // This function skims the _vertexes_to_route set and returns the most centered vertex.
    //
    // first pass : builds the bounding box of all vertexes
    Box vertexesBBox;
    VertexSetIter vsit = _vertexes_to_route.begin();
    while ( vsit != _vertexes_to_route.end() ) {
        vertexesBBox.merge((*vsit)->getPosition());
        vsit++;
    }
    // second pass : finds the most centered vertex
    Point   boxCenter    = vertexesBBox.getCenter();
    vsit = _vertexes_to_route.begin();
    Vertex* mostCentered = (*vsit);
    DbU::Unit    minDistance  = boxCenter.manhattanDistance ( mostCentered->getPosition() );
    vsit++;
    while ( vsit != _vertexes_to_route.end() ) {
        Vertex* currentVertex   = (*vsit);
        DbU::Unit    currentDistance = boxCenter.manhattanDistance( currentVertex->getPosition() );
        if ( currentDistance < minDistance ) {
            mostCentered = currentVertex;
            minDistance  = currentDistance;
        }
        vsit++;
    }

    return mostCentered;
}

Vertex* Graph::getVertex ( Point p )
// *********************************
{
    Vertex* vertex;
    #ifdef __USE_MATRIXVERTEX__
    vertex = _matrixVertex->getVertex(p);
        assert(vertex);
    #endif
    #ifdef __USE_SLICINGTREE__
        vertex = _slicingTree->getVertex ( p );
        assert ( vertex );
    //#else   // Ce n'est plus une option !
    //    GCell* gcell =  _nimbus->getRoutingLeafContaining ( p );
    //    assert ( gcell );
    //    vertex = getVertexOfGCell ( gcell );
    //    assert ( vertex );
    #endif

    return vertex;
}

Vertex* Graph::getVertex ( DbU::Unit x, DbU::Unit y )
// **************************************************
{
  return getVertex ( Point(x,y) );
}

unsigned Graph::getGridLength ( Segment* segment )
// ***********************************************
{
    // new time-optimized version 02/02/09
    unsigned sourceColId = _matrixVertex->getColumnIndex ( segment->getSourceX() );
    unsigned targetColId = _matrixVertex->getColumnIndex ( segment->getTargetX() );
    unsigned sourceRowId = _matrixVertex->getLineIndex ( segment->getSourceY() );
    unsigned targetRowId = _matrixVertex->getLineIndex ( segment->getTargetY() );
    if ( sourceColId == targetColId )
        return targetRowId - sourceRowId;
    else
        return targetColId - sourceColId;
    //if ( isAGlobalRoutingSegment(segment) ) {
    //    if ( Horizontal* horiz = dynamic_cast<Horizontal*>(segment) ) {
    //        unsigned sourceId = _matrixVertex->getColumnIndex ( horiz->getSourceX() );
    //        unsigned targetId = _matrixVertex->getColumnIndex ( horiz->getTargetX() );
    //        assert ( sourceId != targetId );
    //        return targetId - sourceId; // On considere que les segments sont bien orientes !!!!
    //    }
    //    else if ( Vertical* verti = dynamic_cast<Vertical*>(segment) ) {
    //        unsigned sourceId = _matrixVertex->getLineIndex ( verti->getSourceY() );
    //        unsigned targetId = _matrixVertex->getLineIndex ( verti->getTargetY() );
    //        assert ( sourceId != targetId );
    //        return targetId - sourceId; // meme hypothese !!!
    //    }
    //    else
    //        throw Error ( "Graph::getGridLength(): segment is neither horizontal nor vertical." );
    //}
    return 0;
}

unsigned Graph::getCongestEdgeNb ( Segment* segment )
// **************************************************
{
    if ( !isAGlobalRoutingSegment(segment) ) {
        string message = "getCongestEdgeNb only treats global routing segment for now : ";
        message += getString(segment);
        throw Warning ( message );
    }
    unsigned nbEdge = 0;
    if ( dynamic_cast<Horizontal*>(segment) ) {
        Vertex* current = getVertex ( segment->getSource()->getCenter() );
        Vertex* target  = getVertex ( segment->getTarget()->getCenter() );
        while ( current != target ) {
            Edge* edge = current->getHEdgeOut();
            if ( !edge ) throw Error ("Graph::getCongestEdgeNb(): Wow! NULL horizontal Edge.");
            if ( edge->isCongested() )
                nbEdge++;
            current = edge->getTo();
        }
    }
    else if ( dynamic_cast<Vertical*>(segment) ) {
        Vertex* current = getVertex ( segment->getSource()->getCenter() );
        Vertex* target  = getVertex ( segment->getTarget()->getCenter() );
        while ( current != target ) {
            Edge* edge = current->getVEdgeOut();
            if ( !edge ) throw Error ("Graph::getCongestEdgeNb(): Wow! NULL vertical Edge.");
            if ( edge->isCongested() )
                nbEdge++;
            current = edge->getTo();
        }
    }
    else
            throw Error ( "Graph::getCongestEdgeNb(): segment is neither horizontal nor vertical." );
    
    return nbEdge++;
    
}

Segment* Graph::createSegment ( Contact* initialContact, Contact* reachedContact )
// *******************************************************************************
{
    //cerr << "Graph::createSegment: " << initialContact << " -- " << reachedContact << endl;
    if ( initialContact->getX() == reachedContact->getX() ) {
      const Layer* layer  = Configuration::getGMetalV();
      DbU::Unit   xCoord = initialContact->getX();
      DbU::Unit   width  = DbU::lambda(2);

      if ( initialContact->getY() <= reachedContact->getY() )
        return Vertical::create ( initialContact, reachedContact, layer, xCoord, width );
      else
        return Vertical::create ( reachedContact, initialContact, layer, xCoord, width );
    }
    else if ( initialContact->getY() == reachedContact->getY() ) {
      const Layer* layer  = Configuration::getGMetalH();
      DbU::Unit   yCoord = initialContact->getY();
      DbU::Unit   height = DbU::lambda(2);
      
      if ( initialContact->getX() <= reachedContact->getX() )
        return Horizontal::create ( initialContact, reachedContact, layer, yCoord, height );
      else
        return Horizontal::create ( reachedContact, initialContact, layer, yCoord, height );
    }
    else
      throw Error ( "Graph::createSegment(): Contacts are not aligneds." );
}

void Graph::sortHVertexes ( Vertex*& from, Vertex*& to )
// *****************************************************
{
    // This function sorts 2 vertexes horizontally:
    //   from will be the leftest one
    assert (from);
    assert (to);
    if ( from->getX() > to->getX() ) {
        Vertex* temp = from;
        from = to;
        to   = temp;
    }
}

void Graph::sortVVertexes ( Vertex*& from, Vertex*& to )
// *****************************************************
{
    // This function sorts 2 vertexes vertically:
    //   from will be the downest one
    assert (from);
    assert (to);
    if ( from->getY() > to->getY() ) {
        Vertex* temp = from;
        from = to;
        to   = temp;
    }
}

Vertex* Graph::createVertex ( Point position, DbU::Unit halfWidth, DbU::Unit halfHeight )
// **************************************************************************************
{
    Vertex* vertex = Vertex::create ( this, position, halfWidth, halfHeight );
    assert ( vertex );
    _all_vertexes.push_back ( vertex );
    return vertex;
}

  void Graph::createHEdge ( Vertex* from, Vertex* to, size_t reserved )
// ********************************************************************
{
    size_t capacity = 0;
    if ( _routingGrid ) {
        capacity = _routingGrid->getHCapacity();
      //cerr << "createHEdge capacity:" << capacity << " reserved:" << reserved << endl;
    } else {
        vector<RoutingLayerGauge*> rtLGauges = _engine->getRoutingGauge()->getLayerGauges();
        for ( vector<RoutingLayerGauge*>::iterator it = rtLGauges.begin() ; it != rtLGauges.end() ; it++ ) {
            RoutingLayerGauge* routingLayerGauge = (*it);
            if (routingLayerGauge->getType() != Constant::Default) continue;
            if (routingLayerGauge->getDepth() > _engine->getAllowedDepth()) continue;

            if (routingLayerGauge->getDirection() != Constant::Horizontal)
                continue;

            capacity += routingLayerGauge->getTrackNumber ( from->getYMin(), from->getYMax() ) - 1;
        }
      //cerr << "createHEdge capacity:" << capacity << " reserved:" << reserved << endl;
    }
    Edge* newEdge = HEdge::create ( from, to, capacity-reserved );

    _all_edges.push_back ( newEdge );

    newEdge->setCost(1);

    Edge* previousEdge = from->getHEdgeOut();
    if ( previousEdge )
        newEdge->setNextFrom ( previousEdge );
    from->setHEdgeOut ( newEdge );

    previousEdge = to->getHEdgeIn();
    if ( previousEdge )
        newEdge->setNextTo ( previousEdge );
    to->setHEdgeIn ( newEdge );
}

void Graph::createVEdge ( Vertex* from, Vertex* to, size_t reserved )
// ******************************************************************
{
    size_t capacity = 0;
    if ( _routingGrid )
        capacity = _routingGrid->getVCapacity();
    else {
        vector<RoutingLayerGauge*> rtLGauges = AllianceFramework::get()->getRoutingGauge()->getLayerGauges();
        for ( vector<RoutingLayerGauge*>::iterator it = rtLGauges.begin() ; it != rtLGauges.end() ; it++ ) {
            RoutingLayerGauge* routingLayerGauge = (*it);
            if (routingLayerGauge->getType() != Constant::Default) continue;
            if (routingLayerGauge->getDepth() > _engine->getAllowedDepth()) continue;

            if (routingLayerGauge->getDirection() != Constant::Vertical)
                continue;

            capacity += routingLayerGauge->getTrackNumber ( from->getXMin(), from->getXMax() ) - 1;
        }
      //cerr << "createVEdge capacity:" << capacity << " reserved:" << reserved << endl;
    }
    Edge* newEdge = VEdge::create ( from, to, capacity-reserved );

    _all_edges.push_back ( newEdge );

    newEdge->setCost(1);

    Edge* previousEdge = from->getVEdgeOut();
    if ( previousEdge )
        newEdge->setNextFrom ( previousEdge );
    from->setVEdgeOut ( newEdge );

    previousEdge = to->getVEdgeIn();
    if ( previousEdge )
        newEdge->setNextTo ( previousEdge );
    to->setVEdgeIn ( newEdge );
}

void Graph::initConnexComp ( Vertex* vertex, int newConnexID )
// ***********************************************************
{
    initConnexComp ( vertex, NULL, newConnexID );
}

void Graph::initConnexComp ( Vertex* vertex, Edge* arrivalEdge, int newConnexID )
// ******************************************************************************
{
    // This recursive function initializes all vertexes of same connexID and connected through edges of same connexID,
    //  which means that the vertex's distance is set to 0 and the vertex is inserted in the VTuplePriorityQueue
    //  If newConnexID is different from -1, then the connexID is set to newConnexID
    int vertexConnex = vertex->getConnexID();
    assert ( vertexConnex != -1 );
    for_each_edge ( edge, vertex->getAdjacentEdges() ) {
        if ( edge == arrivalEdge ) {
            continue;
        }
        if ( (edge->getNetStamp() == _netStamp) && (edge->getConnexID() == vertexConnex) ) {
            initConnexComp ( edge->getOpposite(vertex), edge, newConnexID );
            if ( newConnexID != -1 ) {
                edge->setConnexID(newConnexID);
            }
        }
        end_for;
    }
    
    vertex->setDistance(0);
    if ( newConnexID != -1 ) {
        vertex->setConnexID ( newConnexID );
    }

    VTuple* vtuple = vertex->getVTuple();
    if ( vtuple ) {
        //cerr << "    Vertex " << vertex << " already has a vtuple : " << vtuple << endl;
        assert ( vtuple->getVertex() == vertex );
        increaseVTuplePriority ( vtuple, 0 );
    }
    else {
        vtuple = VTuple::create ( vertex, 0 );
        //cerr << "    Vertex " << vertex << " has now a new vtuple : " << vtuple << endl;
        addVTupleToPriorityQueue ( vtuple );
    }
}

void Graph::UpdateConnexComp ( VertexList reachedVertexes, Vertex* firstVertex )
// *****************************************************************************
{
    // XXX PLUTOT QUE DE PASSER UNE LISTE DE VERTEX EN ARG, ON DEVRAIT PASSER SEULEMENT UN VERTEX XXX
    // XXX proviens du fait qu'au depart on voulait reellement passer une liste mais du fait de la possiblité de chemins paralleles on evite XXX
    int firstConnexID = firstVertex->getConnexID();

    VertexListIter lvit = reachedVertexes.begin();
    // Pour essayer d'éviter le bug des chemins parallèles si 2 vertex sont atteints avec la meme distance, on ne va updater que le premier
    //while ( lvit != reachedVertexes.end() ) {
    Vertex* currentVertex = (*lvit);
    int     currentConnexID = currentVertex->getConnexID(); 

    //cerr << "Gonna updateConnexComp : first: " << firstVertex << ", reached: " << currentVertex << endl;

    //cerr << "  vertexes_to_route :" << endl;
    //for ( VertexSetIter vsit = _vertexes_to_route.begin() ; vsit != _vertexes_to_route.end() ; vsit++ )
    //     cerr << "     " << (*vsit) << endl;

    // the vertex must be removed from the _vertexes_to_route
    // XXX Woowoo il ne faut pas faire un erase du vertex atteint, mais rechercher le représentant de la composante connexe atteinte dans les _vertexes_to_route et faire un erase dessus !
    Vertex* toErase = NULL;
    for ( VertexSetIter vsit = _vertexes_to_route.begin() ; vsit != _vertexes_to_route.end() ; vsit++ ) {
        if ( (*vsit)->getConnexID() == currentConnexID )
            toErase = (*vsit);
    }

    //cerr << "  gonna erase : " << toErase << endl;
    // on veut updater toute la composante connexe représenter par le currentVertex
#ifndef NDEBUG
    unsigned deleteVertex = _vertexes_to_route.erase ( toErase );
    assert ( deleteVertex == 1 );
#else
    _vertexes_to_route.erase ( toErase );
#endif
    // the connexe component corresponding to the vertex must be initialize with the firstConnexID
    initConnexComp ( currentVertex, firstConnexID );
    
    // create the new connex component and initializes it :
    while ( Edge* predecessor = currentVertex->getPredecessor() ) {
        currentVertex->setPredecessor ( NULL );
        predecessor->setConnexID ( firstConnexID );
        currentVertex = predecessor->getOpposite ( currentVertex );

        // si jamais on réatteint un vertex dejà mis à jour.
        if ( currentVertex->getConnexID() == firstConnexID )
            break;

        currentVertex->setDistance(0);
        currentVertex->setConnexID(firstConnexID);

        VTuple* vtuple = currentVertex->getVTuple();
        if ( vtuple  ) {
            increaseVTuplePriority ( vtuple, 0);
        }
        else {
            vtuple = VTuple::create ( currentVertex, 0 );
            addVTupleToPriorityQueue ( vtuple );
        }
    }
    //}
}

// VTuplePriorityQueue Utility Methods
// ***********************************
Vertex* Graph::extractMinFromPriorityQueue()
// *****************************************
{
    if ( _vtuplePriorityQueue.begin() == _vtuplePriorityQueue.end() )
        return NULL;
    
    VTuple*  vtuple  = *(_vtuplePriorityQueue.begin());
    Vertex* vertex = vtuple->getVertex();

    _vtuplePriorityQueue.erase ( _vtuplePriorityQueue.begin() );
    vtuple->destroy();
    vertex->setVTuple ( NULL );

    return vertex;
}

Vertex* Graph::getMinFromPriorityQueue()
// *************************************
{
    if ( _vtuplePriorityQueue.begin() == _vtuplePriorityQueue.end() )
        return NULL;

    VTuple*  vtuple  = *(_vtuplePriorityQueue.begin());
    Vertex* vertex = vtuple->getVertex();

    return vertex;
}

void Graph::PopMinFromPriorityQueue()
// **********************************
{
    if ( _vtuplePriorityQueue.begin() != _vtuplePriorityQueue.end() ) {
        VTuple*  vtuple  = *(_vtuplePriorityQueue.begin());
        Vertex* vertex = vtuple->getVertex();

        _vtuplePriorityQueue.erase ( _vtuplePriorityQueue.begin() );
        vtuple->destroy();
        vertex->setVTuple ( NULL );
    }
}

void Graph::addVTupleToPriorityQueue ( VTuple* vtuple )
// *************************************************
{
//cerr << "addVTupleToPriorityQueue: "
//     << (void*)vtuple << " " << (void*)vtuple->getVertex() << ":" << vtuple->getVertex() << endl;

  assert ( vtuple );
  assert ( vtuple->getVertex()->getVTuple() == vtuple );
  assert ( _vtuplePriorityQueue.find ( vtuple ) == _vtuplePriorityQueue.end() );
  if (debugging)
    cerr << "    ADDING vtuple to priority queue : " << vtuple->_getString() << endl;
  _vtuplePriorityQueue.insert ( vtuple );
//pair<VTuplePQIter,bool> p = _vtuplePriorityQueue.insert ( vtuple );
//assert ( p.second );
}

void Graph::increaseVTuplePriority ( VTuple* vtuple, float distance )
// ***************************************************************
{
    assert ( vtuple );
    //if ( debugging ) 
    //    cerr << "    " << vtuple->getVertex() << " : " << vtuple->getDistance() << " > " << distance << endl;
    assert ( vtuple->getDistance() > distance );
    // Copy removeVTupleFromPriorityQueue without vtuple->destroy()
    VTuplePQIter pqit = _vtuplePriorityQueue.find ( vtuple );
    if ( pqit != _vtuplePriorityQueue.end() ) {
        assert ( vtuple->getVertex() == (*pqit)->getVertex() );
        _vtuplePriorityQueue.erase ( pqit );
    }
    // end copy
    vtuple->setDistance ( distance );
    _vtuplePriorityQueue.insert ( vtuple );
}

void Graph::printVTuplePriorityQueue()
// ***********************************
{
  cdebug_tabw(139,1);
  cdebug_log(139,0) << "VTuplePriorityQueue:" << endl;
  unsigned int i=0;
  for ( auto iv : _vtuplePriorityQueue ) {
    cdebug_log(139,0) << setw(3) << i << "| " << iv->getVertex() << " : " << iv->getDistance() << endl;
    ++i;
  }
  cdebug_tabw(139,-1);
}

void Graph::clearPriorityQueue()
// *****************************
{
    for ( VTuplePQIter pqit = _vtuplePriorityQueue.begin() ; pqit != _vtuplePriorityQueue.end() ; pqit++ ) {
        (*pqit)->destroy();
    }
    _vtuplePriorityQueue.clear();
}

// STuplePriorityQueue Utility Methods
// ***********************************
Segment* Graph::extractMaxFromSTuplePQ()
// *************************************
{
    if ( _stuplePriorityQueue.begin() == STuple::_stuplePQEnd )
        return NULL;
    
    STuple*  stuple = *(_stuplePriorityQueue.begin());
    Segment* segment = stuple->getSegment();

    _stuplePriorityQueue.erase ( _stuplePriorityQueue.begin() );
    stuple->destroy();

    return segment;
}

STuple* Graph::getMaxFromSTuplePQ()
// ********************************
{
    if ( _stuplePriorityQueue.begin() == STuple::_stuplePQEnd )
        return NULL;

    STuple* stuple  = *(_stuplePriorityQueue.begin());

    return stuple;
}

void Graph::popMaxFromSTuplePQ()
// *********************************
{
    if ( _stuplePriorityQueue.begin() != STuple::_stuplePQEnd ) {
        STuple* stuple = *(_stuplePriorityQueue.begin());
#ifndef NDEBUG
        STuple::CostProperty* costProperty = stuple->getCostProperty();
        assert ( costProperty );
        assert ( (*costProperty->getPQIter()) == stuple );
#endif
        stuple->destroy();
        _stuplePriorityQueue.erase ( _stuplePriorityQueue.begin());
    }
}

void Graph::addToSTuplePQ ( STuple* stuple )
// *****************************************
{
  assert( stuple );
  STuple::CostProperty* costProperty = stuple->getCostProperty();
  assert( costProperty );
  costProperty->setPQIter ( _stuplePriorityQueue.insert( stuple ) );
#ifndef NDEBUG
  STuple::STuplePQIter pqit = costProperty->getPQIter();
#endif
  assert( (*pqit) == stuple );
}

void Graph::updateSTupleCost ( STuple* stuple, unsigned cost )
// ***********************************************************
{
    assert ( stuple );
    STuple::CostProperty* costProperty = stuple->getCostProperty();
    assert ( costProperty );
    STuple::STuplePQIter pqit = costProperty->getPQIter();
    assert ( (*pqit) == stuple );
    _stuplePriorityQueue.erase ( pqit );
    stuple->setCost ( cost );
    costProperty->setPQIter ( _stuplePriorityQueue.insert ( stuple ) );
}

void Graph::printSTuplePQ()
// ************************
{
    cmess1 << "*** printing STuplePriorityQueue ***" << endl;
    STuple::STuplePQIter pqit = _stuplePriorityQueue.begin();
    while ( pqit != STuple::_stuplePQEnd ) {
        cmess1 << getString(*pqit) << endl;
        pqit++;
    }
    cmess1 << "***********************************" << endl;
}

void Graph::clearSTuplePQ()
// ************************
{
    for ( STuple::STuplePQIter pqit = _stuplePriorityQueue.begin() ; pqit != STuple::_stuplePQEnd ; pqit++ ) {
        (*pqit)->destroy();
    }
    _stuplePriorityQueue.clear();
}

void Graph::testSTuplePQ()
// ***********************
{
//    unsigned count = 0;
//    Component* compo1 = NULL;
//    Component* compo2 = NULL;
//    for_each_component ( component, _nimbus->getCell()->getComponents() ) {
//        if ( dynamic_cast<Segment*>(component) ) {
//            STuple::CostProperty* costProperty = STuple::CostProperty::create (count );
//            component->put ( costProperty );
//            STuple* stuple = STuple::create ( costProperty );
//            addToSTuplePQ ( stuple );
//            if ( count == 4 ) compo1 = component;
//            if ( count == 8 ) compo2 = component;
//            count++;
//            if ( count >= 10 )   
//                break;
//        }
//        end_for;
//    }
//    printSTuplePQ();
//    STuple::CostProperty* costProperty = dynamic_cast<STuple::CostProperty*>(compo1->getProperty ( "Knik::CostProperty" ));
//    updateSTupleCost ( (*costProperty->getPQIter()), 18 );
//    cmess2 << "updateSTupleCost ( (*costProperty->getPQIter()), 18 )" << endl;
//    printSTuplePQ();
//    costProperty = dynamic_cast<STuple::CostProperty*>(compo2->getProperty ( "Knik::CostProperty"));
//    updateSTupleCost ( (*costProperty->getPQIter()), 2 );
//    cmess2 << "updateSTupleCost ( (*costProperty->getPQIter()), 2 )" << endl;
//    printSTuplePQ();
//    popMaxFromSTuplePQ();
//    cmess2 << "popMaxFromSTuplePQ()" << endl;
//    printSTuplePQ();
//    cmess2 << "getString ( getMaxFromSTuplePQ() )" << endl;
//    cmess2 << getString ( getMaxFromSTuplePQ() ) << endl;
//    printSTuplePQ();
//    cmess2 << "getString ( extractMaxFromSTuplePQ() )" << endl;
//    cmess2 << getString ( extractMaxFromSTuplePQ() ) << endl;
//    printSTuplePQ();
//    clearSTuplePQ();
//    cmess2 << "clearSTuplePQ()" << endl;
//    printSTuplePQ();
}

int Graph::countVertexes ( Net* net )
// **********************************
{
    assert ( net );
    
    _working_net = net;
    forEach ( Component*, component, net->getComponents() ) {
        if ( RoutingPad* routingPad = dynamic_cast<RoutingPad*>(*component) ) {
            //if ( routingPad->getCenter().getY() < 0 ) {

            //    CEditor* editor = getCEditor ( getCell() );
            //    editor->Select ( routingPad );
            //    editor->Refresh();
            //    editor->Stop ( "RoutingPad" );
            //    editor->Unselect( routingPad );
            //}


            Vertex* vertex = getVertex ( routingPad->getCenter() );
            assert(vertex);
            if ( _vertexes_to_route.find ( vertex ) == _vertexes_to_route.end() ) {
                _vertexes_to_route.insert ( vertex );
            }
        }
    }
    return _vertexes_to_route.size();
}

int Graph::initRouting ( Net* net )
// ********************************
{
    assert ( net );
    _working_net = net;

    //cerr  << "[DEBUG]: Net: " << _working_net << endl;
    //cerr  << "[DEBUG]: vertexes_to_route size: " << _vertexes_to_route.size() << endl;

    //CEditor* editor = getCEditor( _nimbus->getCell() );
    //editor->Refresh();
    //editor->Stop ("Going to init");

  //if (_working_net->getName() == "read0") DebugSession::open( 130, 140 );
    cdebug_log(139,1) << "Graph::initRouting() " << _working_net << endl;

    int currentConnexID = 0;
    vector<Contact*> vContacts;
    vector<RoutingPad*> vRoutingPads;
    unsigned compoSize = net->getComponents().getSize();
    vContacts.reserve ( compoSize );
    vRoutingPads.reserve ( compoSize );
    for_each_component ( component, net->getComponents() ) {
        if ( dynamic_cast<RoutingPad*>(component) )
            vRoutingPads.push_back ( static_cast<RoutingPad*>(component) );
        if ( dynamic_cast<Contact*>(component) ) {
            Contact* contact = static_cast<Contact*>(component);
            if ( isAGlobalRoutingContact ( contact ) )
                vContacts.push_back ( contact );
        }
        end_for;
    }
    //cerr << "   vContacts size :    " << vContacts.size() << endl;
    //cerr << "   vRoutingPads size : " << vRoutingPads.size() << endl;
    UpdateSession::open();
    for ( unsigned index = 0; index < vContacts.size() ; index++ ) {
        Contact* contact = vContacts[index];
        //cerr << "        [0;32mcontact : " << contact << "[0m" << endl;
        Vertex* contactVertex = getVertex ( contact->getCenter() );
        Contact* vContact = contactVertex->getContact();
        if ( (vContact == contact) && (contactVertex->getNetStamp() == _netStamp) ) {
            //cerr << "        [0;32mon a deja un contact associe avec le meme netStamp ![0m" << endl;
            continue; // on la déjà traité à travers le parcourt de composante connexe
        }
        else {
            //cerr << "            [0;32mrebuild connexe component " << contact << "[0m" << endl;
            rebuildConnexComponent ( contact, currentConnexID, NULL );
            //cerr << "            [0;32mdone[0m" << endl;
            if ( _vertexes_to_route.find ( contactVertex ) == _vertexes_to_route.end() ) {
                //cerr << "            [0;32mAdding vertex to _vertexes_to_route: continue[0m" << endl;
                _vertexes_to_route.insert ( contactVertex );
                _searchingArea.merge ( contactVertex->getBox() );
                contactVertex->setDistance((float)(HUGE_VAL));
                contactVertex->setPredecessor(NULL);
                currentConnexID++;
            }
        }
    }

    //cerr << "   traitement pour les contacts : OK" << endl;
    //OoOoooOoooOoO A QUOI SERT CE BOOLEEN OoOoooOoooOoO  ==>> pour le //5 le connexID ne doit pas avoir été augmenté ! ou alors connexID-1 !!
    //bool useConnexID = false;
    for ( unsigned index = 0 ; index < vRoutingPads.size() ; index ++ ) {
        RoutingPad* routingPad = vRoutingPads[index];
        cdebug_log(139,0) << routingPad << endl;
        Vertex*  rpVertex = getVertex ( routingPad->getCenter() );
        cdebug_log(139,0) << rpVertex << endl;
        Contact* rpContact = rpVertex->getContact();
        if ( rpContact && (rpContact->getNet() == routingPad->getNet()) ) {
            // s'il existe deja un contact pour ce vertex pour ce net, on s'y attache
            //cerr << "            [0;34mContact already exists: attaching hooks[0m" << endl;
            // le contact a deja ete cree, il suffit d'ajouter le routingPad à la ronde du contact.
            routingPad->getBodyHook()->detach();
            routingPad->getBodyHook()->attach ( rpVertex->getContact()->getBodyHook() );
        }
        else {
            // sinon on crée un contact et on s'y attache
            Contact* contact = Contact::create ( _working_net
                                               , DataBase::getDB()->getTechnology()->getLayer("metal2")
                                               , rpVertex->getPosition().getX()
                                               , rpVertex->getPosition().getY()
                                               , rpVertex->getHalfWidth()/5
                                               , rpVertex->getHalfHeight()/5
                                               );
            rpVertex->setContact ( contact );
            routingPad->getBodyHook()->detach();
            routingPad->getBodyHook()->attach ( contact->getBodyHook() );
            if ( _vertexes_to_route.find ( rpVertex ) == _vertexes_to_route.end() ) {
                //cerr << "            [0;32mAdding vertex to _vertexes_to_route: continue[0m" << endl;
                _vertexes_to_route.insert ( rpVertex );
                _searchingArea.merge ( rpVertex->getBox() );
                rpVertex->setConnexID ( currentConnexID );
                rpVertex->setNetStamp ( _netStamp );
                rpVertex->setDistance((float)(HUGE_VAL));
                rpVertex->setPredecessor(NULL);
                currentConnexID++;
            }
        }
    }

    if (_searchingArea.getHeight() <= _matrixVertex->getTileHeight())
      _searchingArea.inflate( 0, _matrixVertex->getTileHeight() );

    if (_searchingArea.getWidth() <= _matrixVertex->getTileWidth())
      _searchingArea.inflate( _matrixVertex->getTileWidth(), 0 );

    // for ripup & reroute purpose
    if ( __ripupMode__ )
        _searchingArea = _cell->getAbutmentBox(); // recherche sur toute la surface : trop long pour gros circuits
        //_searchingArea.inflate((_searchingArea.getWidth()*10)/100, (_searchingArea.getHeight()*10)/100); // raté ça ne marche pas avec les nets plats

    //cerr << "   traitement pour les routingPads : OK" << endl;
    UpdateSession::close();
    //cerr  << "[DEBUG]: vertexes_to_route size (after init): " << _vertexes_to_route.size() << endl;
    //editor->Refresh();
    //editor->Stop("initDone");
    // 8/10/2007
    // {
    // Expand seaching area
    //int bottomLineIdx  = (int)_matrixVertex->getLineIndex   ( _searchingArea.getYMin() +1 );
    //int topLineIdx     = (int)_matrixVertex->getLineIndex   ( _searchingArea.getYMax() -1 );
    //int leftColumnIdx  = (int)_matrixVertex->getColumnIndex ( _searchingArea.getXMin() +1 );
    //int rightColumnIdx = (int)_matrixVertex->getColumnIndex ( _searchingArea.getXMax() -1 );
    //if ( _matrixVertex->isLineIndexValid ( bottomLineIdx - 2 ) )
    //    bottomLineIdx -= 2;
    //else if ( _matrixVertex->isLineIndexValid ( bottomLineIdx - 1 ) )
    //    bottomLineIdx -= 1;

    //if ( _matrixVertex->isLineIndexValid ( topLineIdx + 2 ) )
    //    topLineIdx += 2;
    //else if ( _matrixVertex->isLineIndexValid ( topLineIdx + 1 ) )
    //    topLineIdx += 1;

    //if ( _matrixVertex->isColumnIndexValid ( leftColumnIdx - 2 ) )
    //    leftColumnIdx -= 2;
    //else if ( _matrixVertex->isColumnIndexValid ( leftColumnIdx - 1 ) )
    //    leftColumnIdx -= 1;

    //if ( _matrixVertex->isColumnIndexValid ( rightColumnIdx + 2 ) )
    //    rightColumnIdx += 2;
    //else if ( _matrixVertex->isColumnIndexValid ( rightColumnIdx + 1 ) )
    //    rightColumnIdx += 1;

    //_searchingArea.merge ( _matrixVertex->getVertexFromIndexes(bottomLineIdx, leftColumnIdx)->getGCell()->getBox() );
    //_searchingArea.merge ( _matrixVertex->getVertexFromIndexes(topLineIdx, rightColumnIdx)->getGCell()->getBox() );
    
    //cerr << "net " << net << " has " << _vertexes_to_route.size() << " vertexes to route." << endl;
    //}

    cdebug_tabw(139,-1);
  //DebugSession::close();

    return _vertexes_to_route.size();
}

void Graph::Dijkstra()
// *******************
{
//checkEmptyPriorityQueue();

  countDijkstra++;

// first we need to choose the closest to center vertex in _vertexes_to_route
  Vertex* centralVertex = getCentralVertex();
//ltrace(435) << "  most centered vertex is " << centralVertex << endl;
// we want to prepare all vertexes of the 'composante connexe'
//      set the distance to 0
//      insert each vertex in the vtuplePriorityQueue
  initConnexComp( centralVertex );
    
// create a copy of _vertexes_to_route vector fo method UpdateEstimateCongestion
//set<Vertex*,VertexPositionComp> copy_vertex = _vertexes_to_route; // This is no more useful

//#if defined ( __USE_DYNAMIC_PRECONGESTION__ )
  if ( !__ripupMode__ &&  (__precongestion__ == 2) )
    UpdateEstimateCongestion();
//#endif

//DebugSession::open( _working_net, 130, 140 );
//if (_working_net->getName() == "read0") DebugSession::open( 130, 140 );

  cdebug_log(139,1) << "Dijkstra for net: " << _working_net << endl;
  cdebug_log(139,0)   << "Stamp:" << _netStamp << endl;
  cdebug_log(139,0)   << "Search area : " << _searchingArea
                    << " h:" << DbU::getValueString(_searchingArea.getHeight()) << endl;
  cdebug_log(139,0)   << "Matrix tile height : " << DbU::getValueString(_matrixVertex->getTileHeight()) << endl;
  cdebug_log(139,0)   << "Central vertex : " << centralVertex << endl;
  cdebug_log(139,1) << "_vertexes_to_route.size(): " << _vertexes_to_route.size() << endl;
//Breakpoint::stop(1, "<center><b>Dijkstra</b><br>initialized</center>");

  while ( _vertexes_to_route.size() > 1 ) {
  // Now, let's expanse the top of the queue
    VertexList reachedVertexes;
    float      reachedDistance = (float)(HUGE_VAL);

  //checkGraphConsistency();
    if (cdebug.enabled(139)) {
      cdebug_log(139,0) << "_vertexes_to_route:" << endl;
      for ( auto iv : _vertexes_to_route )
        cdebug_log(139,0) << "| " << iv << endl;
    }

    cdebug_log(139,0) << "Source component" << endl;
    printVTuplePriorityQueue();
  //Breakpoint::stop(1, "<center><b>Dijkstra</b><br>source connexe component</center>");

    Vertex* firstVertex = getMinFromPriorityQueue();
    assert( firstVertex );
    Vertex* currentVertex     = firstVertex;
    int     firstVertexConnex = firstVertex->getConnexID();

    while ( currentVertex and (currentVertex->getDistance() < reachedDistance) ) {
      PopMinFromPriorityQueue();
      assert( not currentVertex->getVTuple() );

    // IMPORTANT : each currentVertex considered here has been getFromPriorityQueue() 
    // which means its NetStamp and ConnexID are set for the current _working_net :
    // no need to check netStamp
      int currentVertexConnex = currentVertex->getConnexID() ;
      if ( (currentVertexConnex != -1) and (currentVertexConnex != firstVertexConnex) ) {
        reachedDistance = currentVertex->getDistance();
        reachedVertexes.clear();
        reachedVertexes.push_back ( currentVertex );
        cdebug_log(139,0) << "Re-init reachedVertexes with " << currentVertex << endl;
        break;
      }

      Edge* arrivalEdgeCurrentVertex = currentVertex->getPredecessor();
      forEach ( Edge*, iedge, currentVertex->getAdjacentEdges() ) {
        if ( (iedge->getNetStamp() == _netStamp) && (iedge->getConnexID() == firstVertexConnex) ) {
        // already visited iedge
        //   ok because to reach a connex component the algorithm first reach a vertex !!
          continue;
        }
        iedge->setConnexID(-1);  // reinitialize connexID for edge (was done by CleanRoutingState)
        iedge->setNetStamp(_netStamp);
        
        Vertex* oppositeVertex = iedge->getOpposite ( currentVertex );
        assert( oppositeVertex );
        if (not _searchingArea.contains(oppositeVertex->getPosition()))
          continue;

        float newDistance          = currentVertex->getDistance()
                                   + iedge->getCost( arrivalEdgeCurrentVertex );
        bool  updateOppositeVertex = false;
      // reinitialize the oppositeVertex if its netStamp is < _netStamp
        if (oppositeVertex->getNetStamp() < _netStamp) {
        //oppositeVertex->setLocalRingHook(NULL);
          oppositeVertex->setContact(NULL);
          oppositeVertex->setConnexID(-1);
          updateOppositeVertex = true;
        }

        int   oppositeConnex       = oppositeVertex->getConnexID();
        float oppositeDistance     = oppositeVertex->getDistance();
        if ( updateOppositeVertex or (newDistance + EPSILON < oppositeDistance) ) {
          assert( oppositeConnex != firstVertexConnex );
          oppositeVertex->setPredecessor( *iedge );
          oppositeVertex->setDistance   ( newDistance );
          oppositeVertex->setNetStamp   ( _netStamp );

          VTuple* oppositeVTuple = oppositeVertex->getVTuple();
          if (oppositeVTuple) {
            cdebug_log(139,0) << "Increasing priority for:" << endl;
            cdebug_log(139,0) << "* " << oppositeVertex << endl;
            cdebug_log(139,0) << "* " << oppositeVTuple << endl;
            increaseVTuplePriority( oppositeVTuple, newDistance );
          // Du fait de la reinit ce n'est plus seulement un increase!
          // Non, c'est bon si on garde le CleanRoutingState (avec clearPriorityQueue)
          } else {
            VTuple* newOppositeVTuple = VTuple::create ( oppositeVertex, newDistance );
            cdebug_log(139,0) << "Creating New VTuple for Vertex:" << endl;
            cdebug_log(139,0) << "* " << oppositeVertex    << ":" << newDistance << endl;
            cdebug_log(139,0) << "* " << newOppositeVTuple << endl;
            addVTupleToPriorityQueue( newOppositeVTuple );
          }

          cdebug_log(139,0) << "Updated distance " << newDistance << " on: " << (*iedge) << endl;
          printVTuplePriorityQueue();
        //Breakpoint::stop(1, "<center><b>Dijkstra</b><br>distance has been updated</center>");
        }

        if ( (oppositeConnex != -1) and (oppositeConnex != firstVertexConnex) ) {
        // verifier si la newDistance est inférieure a la reachedDistance,
        // si oui vider la liste des reachedVertex et ajouter l'oppositeVertex
        // si == ajouter l'oppositeVertex a la liste (a condition qu'il n'y soit pas déjà)
        // si > rien a faire (a verifier que la pile de priorité se comporte bien)
          if (newDistance < reachedDistance) {
            reachedDistance = newDistance;
            reachedVertexes.clear();
            reachedVertexes.push_back( oppositeVertex );
            cdebug_log(139,0) << "Re-init (< distance) reachedVertexes with " << oppositeVertex << endl;
          } else if (newDistance == reachedDistance) {
          // on pourrait simplifier grandement tout ca : 1 seul vertex atteint sauvergardé!
          // Conclusion qu'il ait le meme connexID ou pas, on ne fait rien, on en a deja atteint
          // un avec la meme distance ...
            bool  foundVertex = false;
            for ( auto iv : reachedVertexes ) {
            // the following test depends on the fact we authorize multiple representant (vertex)
            // of the same connexe component in reachedVertexes list
            //if (iv->getConnexID() == oppositeConnex) 
              if (iv == oppositeVertex) {
                foundVertex = true;
                break;
              }
            }
            if (not foundVertex) {
              reachedVertexes.push_back( oppositeVertex );
              cdebug_log(139,0) << "Re-init (== distance) reachedVertexes with " << oppositeVertex << endl;
            }
          } else {
          // Nothing to do?
          }
        }
      }

      currentVertex = getMinFromPriorityQueue();
    }

    if (reachedVertexes.empty()) {
      ostringstream message;
      message << "In Graph::Dijkstra():\n";
      message << "        Unable to reach target on net " << _working_net->getName() << ".";
      for ( auto iv : _vertexes_to_route ) message << "\n        | " << iv;
      throw Error( message.str() );
    }
    assert( reachedDistance < (float)(HUGE_VAL) );

    cdebug_log(139,0) << "Updating two connex components:" << endl;
    cdebug_log(139,0) << "1. " << (*(reachedVertexes.begin())) << endl;
    cdebug_log(139,0) << "2. " << firstVertex << endl;
    UpdateConnexComp( reachedVertexes, firstVertex );
  }

//cerr << "check before materialize _vertexes_to_route.size = " << _vertexes_to_route.size() << endl;
//checkGraphConsistency();
  MaterializeRouting ( *(_vertexes_to_route.begin()) );

//_vertexes_to_route.clear();   // no more useful
//_vertexes_to_route = copy_vertex ;

  cdebug_tabw(139,-2);
//DebugSession::close();
}

void Graph::Monotonic()
// ********************
{
    // This function execute an algorithm of monotonic routing
    //   it assumes that _vertexes_to_route.sze is 2 (only 2 pin nets can be route with monotonic routing)
    assert ( _vertexes_to_route.size() == 2 );

    countMonotonic++;

    //cerr << "Monotonic for net " << _working_net << " : " << _netStamp << endl;
    //cerr << "    check start monotonic" << endl;
    //checkGraphConsistency();

    Vertex* source = (*_vertexes_to_route.begin());
    Vertex* target = (*_vertexes_to_route.rbegin());
    assert ( source != target );
    DbU::Unit sourceX = source->getPosition().getX();
    DbU::Unit targetX = target->getPosition().getX();

    if ( sourceX > targetX ) {
        Vertex* temp = source;
        source = target;
        target = temp;
    }
    else if ( sourceX == targetX ) {
        if ( source->getPosition().getY() > target->getPosition().getY() ) {
            Vertex* temp = source;
            source = target;
            target = temp;
        }
    }

    // POUR SIMPLIFIER : PAS DE TRAITEMENT D'UN GRAPHE DE ROUTAGE IRREGULIER POUR L'INSTANT
    sourceX = source->getPosition().getX();
    targetX = target->getPosition().getX();
    DbU::Unit sourceY = source->getPosition().getY();
    DbU::Unit targetY = target->getPosition().getY();
    source->setDistance ( 0 );
    if ( sourceY <= targetY ) {
        // 1st case : monotonic directions : top and right
        // marquing all vertexes which y-coordinates are equal to sourceY
        Vertex* predecessor = source;
        Edge* rightEdge = predecessor->getHEdgeOut();
        while ( rightEdge ) {
            Vertex* currentVertex = rightEdge->getOpposite ( predecessor );
            if ( currentVertex->getPosition().getX() <= targetX ) {
                currentVertex->setDistance ( predecessor->getDistance() + rightEdge->getCost( predecessor->getPredecessor() ) );
                currentVertex->setPredecessor ( rightEdge );
                predecessor = currentVertex;
                rightEdge = predecessor->getHEdgeOut();
            }
            else
                break;
        }
        // marquing all vertexes which x-coordinates are equal to sourceX
        predecessor = source;
        Edge* topEdge = predecessor->getVEdgeOut();
        while ( topEdge ) {
            //while ( topEdge->getNextFrom() != NULL )
            //    topEdge = topEdge->getNextFrom();
            Vertex* currentVertex = topEdge->getOpposite ( predecessor );
            if ( currentVertex->getPosition().getY() <= targetY ) {
                currentVertex->setDistance ( predecessor->getDistance() + topEdge->getCost( predecessor->getPredecessor() ) );
                currentVertex->setPredecessor ( topEdge );
                predecessor = currentVertex;
                topEdge = predecessor->getVEdgeOut();
            }
            else
                break;
        }
        // marquing all others vertexes by column
        predecessor = source;
        rightEdge = predecessor->getHEdgeOut();
        while ( rightEdge ) {
            Vertex* baseColumn = rightEdge->getOpposite ( predecessor );
            if ( baseColumn->getPosition().getX() <= targetX ) {
                Vertex* vertPred = baseColumn;
                Edge* topEdge = vertPred->getVEdgeOut();
                while ( topEdge ) {
                    Vertex* currentVertex = topEdge->getOpposite ( vertPred );
                    if ( currentVertex->getPosition().getY() <= targetY ) {
                        float vertDistance = (float)(HUGE_VAL);
                        float horzDistance = (float)(HUGE_VAL);

                        vertDistance = vertPred->getDistance() + topEdge->getCost ( vertPred->getPredecessor() );
                        Edge* leftEdge = currentVertex->getHEdgeIn();
                        if ( leftEdge ) {
                            Vertex* horzPred = leftEdge->getOpposite ( currentVertex );
                            horzDistance = horzPred->getDistance() + leftEdge->getCost ( horzPred->getPredecessor() );
                        }

                        if ( vertDistance < horzDistance ) {
                            currentVertex->setDistance ( vertDistance );
                            currentVertex->setPredecessor ( topEdge );
                        }
                        else {
                            currentVertex->setDistance ( horzDistance );
                            currentVertex->setPredecessor ( leftEdge );
                        }

                        vertPred = currentVertex;
                        topEdge = vertPred->getVEdgeOut();
                    }
                    else
                        break;
                }
                predecessor = baseColumn;
                rightEdge = predecessor->getHEdgeOut();
            }
            else
                break;
        }
    }
    else {
        // 2nd case : monotonic directions : down and right
        // marquing all vertexes which y-coordinates are equal to sourceY
        Vertex* predecessor = source;
        Edge* rightEdge = predecessor->getHEdgeOut();
        while ( rightEdge ) {
            Vertex* currentVertex = rightEdge->getOpposite ( predecessor );
            if ( currentVertex->getPosition().getX() <= targetX ) {
                currentVertex->setDistance ( predecessor->getDistance() + rightEdge->getCost ( predecessor->getPredecessor() ) );
                currentVertex->setPredecessor ( rightEdge );
                predecessor = currentVertex;
                rightEdge = predecessor->getHEdgeOut();
            }
            else
                break;
        }
        // marquing all vertexes which x-coordinates are equal to sourceX
        predecessor = source;
        Edge* bottomEdge = predecessor->getVEdgeIn();
        while ( bottomEdge ) {
            //while ( topEdge->getNextFrom() != NULL )
            //    topEdge = topEdge->getNextFrom();
            Vertex* currentVertex = bottomEdge->getOpposite ( predecessor );
            if ( currentVertex->getPosition().getY() >= targetY ) {
                currentVertex->setDistance ( predecessor->getDistance() + bottomEdge->getCost ( predecessor->getPredecessor() ) );
                currentVertex->setPredecessor ( bottomEdge );
                predecessor = currentVertex;
                bottomEdge = predecessor->getVEdgeIn();
            }
            else
                break;
        }
        // marquing all others vertexes by column
        predecessor = source;
        rightEdge = predecessor->getHEdgeOut();
        while ( rightEdge ) {
            Vertex* baseColumn = rightEdge->getOpposite ( predecessor );
            if ( baseColumn->getPosition().getX() <= targetX ) {
                Vertex* vertPred = baseColumn;
                Edge* bottomEdge = vertPred->getVEdgeIn();
                while ( bottomEdge ) {
                    Vertex* currentVertex = bottomEdge->getOpposite ( vertPred );
                    if ( currentVertex->getPosition().getY() >= targetY ) {
                        float vertDistance = (float)(HUGE_VAL);
                        float horzDistance = (float)(HUGE_VAL);

                        vertDistance = vertPred->getDistance() + bottomEdge->getCost ( vertPred->getPredecessor() );
                        Edge* leftEdge = currentVertex->getHEdgeIn();
                        if ( leftEdge ) {
                            Vertex* horzPred = leftEdge->getOpposite ( currentVertex );
                            horzDistance = horzPred->getDistance() + leftEdge->getCost (horzPred->getPredecessor() );
                        }

                        if ( vertDistance < horzDistance ) {
                            currentVertex->setDistance ( vertDistance );
                            currentVertex->setPredecessor ( bottomEdge );
                        }
                        else {
                            currentVertex->setDistance ( horzDistance );
                            currentVertex->setPredecessor ( leftEdge );
                        }

                        vertPred = currentVertex;
                        bottomEdge = vertPred->getVEdgeIn();
                        if (!bottomEdge)
                            break;
                        currentVertex = bottomEdge->getOpposite ( vertPred );
                    }
                    else
                        break;
                }
                predecessor = baseColumn;
                rightEdge = predecessor->getHEdgeOut();
            }
            else
                break;
        }
    }

    // On crée la nouvelle composante connexe à partir du chemin monotone trouvé :
    int sourceID = source->getConnexID();
    Vertex* currentVertex = target;

    while ( Edge* predecessor = currentVertex->getPredecessor() ) {
        currentVertex->setConnexID ( sourceID );
        currentVertex->setNetStamp ( _netStamp );
        predecessor->setConnexID ( sourceID );
        predecessor->setNetStamp ( _netStamp );
        currentVertex = predecessor->getOpposite ( currentVertex );
    }
    source->setNetStamp ( _netStamp );

    assert ( currentVertex == source );

    MaterializeRouting ( source );
    //#if defined ( __USE_DYNAMIC_PRECONGESTION__ )
    if ( __precongestion__ == 2 )
        UpdateEstimateCongestion();
    //#endif
}

FTree* Graph::createFluteTree()
// ****************************
{ 
    int  accuracy = 3;                         // accuracy for flute (by default 3)
    int  d        = _vertexes_to_route.size(); // degre du net, ie nombre de routingPads
    int *x = new int [d];                      // x coordinates of the vertexes
    int *y = new int [d];                      // y coordinates of the vertexes
    FTree* flutetree = new FTree;              // the flute Steiner Tree

    //cout << "Net : " << _working_net << endl;
    // scans _working_net to find x,y coordinates and fill x, y and d
    // parcours des _vertexes_to_route
    VertexSetIter vsit = _vertexes_to_route.begin();
    int cpt = 0;
    while ( vsit != _vertexes_to_route.end() ) {
        Point position = (*vsit)->getPosition();
        x[cpt] = position.getX();
        y[cpt] = position.getY();
        vsit++;
        cpt++;
    }

    assert ( d == cpt );

    *flutetree = flute ( d, x, y, accuracy );
    //printtree ( flutetree );
    //plottree ( flutetree );
    //cout << endl;
    return flutetree;
}

void Graph::UpdateEstimateCongestion ( bool create )
// *************************************************
{
    if ( _vertexes_to_route.size() < 2 )
       return;
    //cerr << "Running FLUTE for net : " << _working_net << endl;
    auto_ptr<FTree> flutetree ( createFluteTree() );

    //parcours des branches du FTree pour créer la congestion estimée
    for ( int i = 0 ; i < 2*flutetree->deg-2 ; i++ ) {
//        int sourceX = flutetree->branch[i].x;
//        int sourceY = flutetree->branch[i].y;
//        int targetX = flutetree->branch[flutetree->branch[i].n].x;
//        int targetY = flutetree->branch[flutetree->branch[i].n].y;
        Vertex* source = getVertex ( flutetree->branch[i].x                     , flutetree->branch[i].y );
        Vertex* target = getVertex ( flutetree->branch[flutetree->branch[i].n].x, flutetree->branch[flutetree->branch[i].n].y );
        assert ( source );
        assert ( target );
        //Si source et target alignée -> ajoute 1 a toutes les edges sur le chemin
        if ( source->isVerticallyAligned ( target ) ) {
            sortVVertexes ( source, target );
            Vertex* currentVertex = source;
            while ( currentVertex != target ) {
                Edge* edge = currentVertex->getVEdgeOut();
                assert ( edge );
                edge->addSubEstimateOccupancy ( 1.0, create );
                currentVertex = edge->getOpposite ( currentVertex );
                assert ( currentVertex );
            }
        }
        else if ( source->isHorizontallyAligned ( target ) ) {
            sortHVertexes ( source, target );
            Vertex* currentVertex = source;
            while( currentVertex != target ) {
                Edge* edge = currentVertex->getHEdgeOut();
                assert ( edge );
                edge->addSubEstimateOccupancy ( 1.0, create );
                currentVertex = edge->getOpposite ( currentVertex );
                assert ( currentVertex );
            }
        }
        else {
            sortHVertexes ( source, target );
            if ( source->getY() < target->getY() ) {
                // 1st case :
                //  +-------T       +-------T               T
                //  |       |       |                       |
                //  |       |   =   |  0.5      +      0.5  |
                //  |       |       |                       |
                //  S-------+       S               S-------+
                Vertex* currentVertex = source;
                while ( currentVertex->getY() < target->getY() ) {
                    Edge* edge = currentVertex->getVEdgeOut();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( currentVertex );
                }
                while ( currentVertex != target ) {
                    Edge* edge = currentVertex->getHEdgeOut();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( currentVertex );
                }
                currentVertex = source;
                while ( currentVertex->getX() < target->getX() ) {
                    Edge* edge = currentVertex->getHEdgeOut();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( currentVertex );
                }
                while ( currentVertex != target ) {
                    Edge* edge = currentVertex->getVEdgeOut();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( edge );
                }
            }
            else {
                // 2nd case :
                //  S-------+       S               S-------+
                //  |       |       |                       |
                //  |       |   =   |  0.5      +      0.5  |
                //  |       |       |                       |
                //  +-------T       +-------T               T
                Vertex* currentVertex = source;
                while ( currentVertex->getY() > target->getY() ) {
                    Edge* edge = currentVertex->getVEdgeIn();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( currentVertex );
                }
                while ( currentVertex != target ) {
                    Edge* edge = currentVertex->getHEdgeOut();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( currentVertex );
                }
                currentVertex = source;
                while ( currentVertex->getX() < target->getX() ) {
                    Edge* edge = currentVertex->getHEdgeOut();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( currentVertex );
                }
                while ( currentVertex != target ) {
                    Edge* edge = currentVertex->getVEdgeIn();
//                    assert ( edge );
                    edge->addSubEstimateOccupancy ( 0.5, create );
                    currentVertex = edge->getOpposite ( currentVertex );
//                    assert ( edge );
                }
            }
        }
    }
}

void Graph::UpdateMaxEstimateCongestion()
// **************************************
{
    Vertex* currentVertex = _lowerLeftVertex;
    while ( currentVertex ) {
        Vertex* firstLineVertex = currentVertex;
        while ( currentVertex ) {
            Edge* hEdgeOut = currentVertex->getHEdgeOut();
            unsigned max     = 0;
            float    maxEsti = 0.0;
            if ( hEdgeOut ) {
                unsigned maxX     = hEdgeOut->getRealOccupancy();
                float    maxXEsti = hEdgeOut->getEstimateOccupancy();
                max     += maxX;
                maxEsti += maxXEsti;
                if ( maxX > _maxXOccupancy )
                    _maxXOccupancy = maxX;
                if ( maxXEsti > _maxXEstimateOccupancy )
                    _maxXEstimateOccupancy = maxXEsti;
            }
            Edge* vEdgeOut = currentVertex->getVEdgeOut();
            if ( vEdgeOut ) {
                unsigned maxY     = vEdgeOut->getRealOccupancy();
                float    maxYEsti = vEdgeOut->getEstimateOccupancy();
                max     += maxY;
                maxEsti += maxYEsti;
                if ( maxY > _maxYOccupancy )
                    _maxYOccupancy = maxY;
                if ( maxYEsti > _maxYEstimateOccupancy )
                    _maxYEstimateOccupancy = maxYEsti;
            }
            if ( max > _maxOccupancy )
                _maxOccupancy = max;
            if ( maxEsti > _maxEstimateOccupancy )
                _maxEstimateOccupancy = maxEsti;

            if ( hEdgeOut )
                currentVertex = hEdgeOut->getOpposite ( currentVertex );
            else
                break;
        }
        Edge* vEdgeOut = firstLineVertex->getVEdgeOut();
        if ( vEdgeOut )
            currentVertex = vEdgeOut->getOpposite ( firstLineVertex );
        else
            break;
    }
}


Edge* Graph::getEdge ( unsigned col1, unsigned row1, unsigned col2, unsigned row2 )
// ********************************************************************************
{
  Edge* edge = NULL;

  if ( col1 == col2 ) {
    if ( row1 == row2 ) 
      throw Error ( "Graph::UpdateEdgeCapacity(): the two specified vertices must be different." );

    Vertex* bottomVertex = NULL;
    Vertex* topVertex    = NULL;

    if ( ( row1 < row2 ) && ( row2 == row1 + 1 ) ) {
      bottomVertex = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col1, row1) );
      topVertex    = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col1, row2) );
    } else if ( row1 == row2 + 1 ) {
      bottomVertex = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col1, row2) );
      topVertex    = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col1, row1) );
    } else
      throw Error ( "Graph::UpdateEdgeCapacity(): the two specified vertices must be contiguous." );

    edge = bottomVertex->getVEdgeOut();
    assert ( edge->getOpposite(bottomVertex) == topVertex );
  } else if ( row1 == row2 ) {
    Vertex* leftVertex  = NULL;
    Vertex* rightVertex = NULL;

    if ( ( col1 < col2 ) && ( col2 == col1 + 1 ) ) {
      leftVertex  = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col1, row1) );
      rightVertex = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col2, row1) );
    } else if ( col1 == col2 + 1 ) {
      leftVertex  = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col2, row1) );
      rightVertex = _matrixVertex->getVertex ( pair<unsigned int, unsigned int>(col1, row1) );
    } else
      throw Error ( "Graph::UpdateEdgeCapacity(): the two specified vertices must be contiguous." );

    edge = leftVertex->getHEdgeOut();
    assert ( edge->getOpposite(leftVertex) == rightVertex );
  } else
    throw Error ( "Graph::UpdateEdgeCapacity(): the two specified vertices must be vertically or horizontally aligned." );

  return edge;
}


void Graph::UpdateEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, unsigned cap )
// ********************************************************************************************************
{
  getEdge ( col1, row1, col2, row2 )->setCapacity ( cap );
}


void Graph::increaseEdgeCapacity ( unsigned col1, unsigned row1, unsigned col2, unsigned row2, int cap )
// *****************************************************************************************************
{
  getEdge ( col1, row1, col2, row2 )->increaseCapacity ( cap );
}


void Graph::updateEdgesOccupancy ( Segment* segment, bool add )
// ************************************************************
{
    if ( Horizontal* horiz = dynamic_cast<Horizontal*>(segment) ) {
        Vertex* currentVertex = _matrixVertex->getVertex ( Point ( horiz->getSourceX(), horiz->getY() ) );
        Vertex* targetVertex  = _matrixVertex->getVertex ( Point ( horiz->getTargetX(), horiz->getY() ) );
        while ( currentVertex != targetVertex ) {
            Edge* edge = currentVertex->getHEdgeOut();
            assert ( edge );
            if ( add )
                edge->insertSegment ( segment );
            else
                edge->removeSegment ( segment );
            currentVertex = edge->getOpposite ( currentVertex );
            assert ( currentVertex );
        }
    }
    else if ( Vertical* verti = dynamic_cast<Vertical*>(segment) ) {
        Vertex* currentVertex = _matrixVertex->getVertex ( Point ( verti->getX(), verti->getSourceY() ) );
        Vertex* targetVertex  = _matrixVertex->getVertex ( Point ( verti->getX(), verti->getTargetY() ) );
        while ( currentVertex != targetVertex ) {
            Edge* edge = currentVertex->getVEdgeOut();
            assert ( edge );
            if ( add )
                edge->insertSegment ( segment );
            else
                edge->removeSegment ( segment );
            currentVertex = edge->getOpposite ( currentVertex );
            assert ( currentVertex );
        }
    }
}

void Graph::rebuildConnexComponent ( Contact* contact, int connexID, Segment* arrivalSegment )
// *******************************************************************************************
{
    //CEditor* editor = getCEditor ( _nimbus->getCell() );
    Vertex* contactVertex = getVertex ( contact->getCenter() );
    contactVertex->setContact  ( contact );
    contactVertex->setConnexID ( connexID );
    contactVertex->setNetStamp ( _netStamp );
    contactVertex->setDistance((float)(HUGE_VAL));
    contactVertex->setPredecessor(NULL);
    //cerr << "from :" << contact << endl;
    //cerr << "arrivalSegment: " << arrivalSegment << endl;
    for_each_component ( component, contact->getSlaveComponents() ) {
        if ( dynamic_cast<Segment*>(component) ) {
            Segment* segment = static_cast<Segment*>(component);
            //cerr << " a segment " << segment << endl;
            //editor->Stop("Guess what to do");
            if ( segment == arrivalSegment ) continue;
            //cerr << "    not arrivalSegment -> setNetStampConnexID" << endl;
            setNetStampConnexID ( segment, connexID );
            //cerr << "    done" << endl;
            if ( !dynamic_cast<Contact*>( segment->getOppositeAnchor(contact) ) )
              throw Error ("Graph::rebuildConnexComponent(): For the moment we do not consider Segments that anchors on something else than a Contact.");
            Contact* oppositeContact = static_cast<Contact*>(segment->getOppositeAnchor(contact));
            //cerr << "    now recursive call on oppositeContact: " << oppositeContact << endl;
            //editor->Stop("See what has been done");
            rebuildConnexComponent ( oppositeContact, connexID, segment );
            //cerr << "    done" << endl;
        }
        end_for;
    }
}

void Graph::setNetStampConnexID ( Segment* segment, int connexID ) 
// ***************************************************************
{
    unsigned int sourceColumnIdx = _matrixVertex->getColumnIndex ( segment->getSourceX() );
    unsigned int sourceLineIdx   = _matrixVertex->getLineIndex   ( segment->getSourceY() );
    unsigned int targetColumnIdx = _matrixVertex->getColumnIndex ( segment->getTargetX() );
    unsigned int targetLineIdx   = _matrixVertex->getLineIndex   ( segment->getTargetY() );

    if ( sourceColumnIdx == targetColumnIdx ) { // vertical segment
        for ( unsigned line = sourceLineIdx ; line < targetLineIdx ; line++ ) {
            Vertex* vertex = _matrixVertex->getVertexFromIndexes ( line, sourceColumnIdx );
            assert(vertex);
            vertex->setNetStamp ( _netStamp );
            vertex->setConnexID ( connexID );
            vertex->setDistance((float)(HUGE_VAL));
            Edge* edge = vertex->getVEdgeOut();
            assert(edge);
            edge->setNetStamp ( _netStamp );
            edge->setConnexID ( connexID );
        }
        Vertex* vertex = _matrixVertex->getVertexFromIndexes ( targetLineIdx, targetColumnIdx );
        assert(vertex);
        vertex->setNetStamp ( _netStamp );
        vertex->setConnexID ( connexID );
        vertex->setDistance((float)(HUGE_VAL));
        return;
    }
    if ( sourceLineIdx == targetLineIdx ) { // horizontal segment
        for ( unsigned column = sourceColumnIdx ; column < targetColumnIdx ; column++ ) {
            Vertex* vertex = _matrixVertex->getVertexFromIndexes ( sourceLineIdx, column );
            assert(vertex);
            vertex->setNetStamp ( _netStamp );
            vertex->setConnexID ( connexID );
            vertex->setDistance((float)(HUGE_VAL));
            Edge* edge = vertex->getHEdgeOut();
            assert(edge);
            edge->setNetStamp ( _netStamp );
            edge->setConnexID ( connexID );
        }
        Vertex* vertex = _matrixVertex->getVertexFromIndexes ( targetLineIdx, targetColumnIdx );
        assert(vertex);
        vertex->setNetStamp ( _netStamp );
        vertex->setConnexID ( connexID );
        vertex->setDistance((float)(HUGE_VAL));
        return;
    }
    throw Error ( "Graph::setNetStampConnexId(): what sort of segment is that, a diagonal one?" );
}

void Graph::insertSegment ( Segment* segment )
// *******************************************
{
    updateEdgesOccupancy ( segment, true );
}

void Graph::removeSegment ( Segment* segment )
// *******************************************
{
    updateEdgesOccupancy ( segment, false );
}

float Graph::getSegmentCost ( Segment* segment )
// *********************************************
{
    unsigned nbDep = 0;
    unsigned nbTot = 0;
    unsigned sumDep = 0;
    Vertex* currentVertex = getVertex ( segment->getSourcePosition() );
    Vertex* targetVertex = getVertex ( segment->getTargetPosition() );
    if ( segment->getSourceY() == segment->getTargetY() ) { // which means : if it's a horizontal segment
        while ( currentVertex != targetVertex ) { // attention pour faire simple on code pour un grpahe regulier
            Edge* edge = currentVertex->getHEdgeOut(); // il faudrait choisir la bonne HEdgeOut pour un graphe irregulier
            unsigned ov = edge->getOverflow();
            if ( ov ) {
                nbDep++;
                sumDep += ov;
            }
            nbTot++;
            currentVertex = edge->getOpposite ( currentVertex );
        }
    }
    else {
        while ( currentVertex != targetVertex ) {
            Edge* edge = currentVertex->getVEdgeOut();
            unsigned ov = edge->getOverflow();
            if ( ov ) {
                nbDep++;
                sumDep += ov;
            }
            nbTot++;
            currentVertex = edge->getOpposite ( currentVertex );
        }
    }
    if ( !nbTot )
        return 0;
    return (float(nbDep)/float(nbTot))*sumDep;
}

unsigned Graph::analyseRouting ( set<Segment*>& segmentsToUnroute )
// ****************************************************************
{
    // This function construct the list of segments that participate to congestion
    // empty Priority queue : easier than update each semgent when removing a semgent ...
    clearSTuplePQ();
    // 30/01/09 on  remplace le parcours  des nets/segments par un  parcours des
    // edges avec un map trié sur pointeur de segments et définissant un record:
    // nbDep nBTot + sumOv pour chaque segment.
    unsigned nbEdgesTot = 0;
    unsigned nbEdgesOv = 0;
    unsigned overflow = 0;
    unsigned maxOv = 0;
    unsigned wirelength = 0;
    unsigned viaWirelength = 0;
    map<Segment*, segmentStat> segmentsMap;
    for ( unsigned i = 0 ; i < _all_edges.size() ; i++ ) 
    {
        Edge* edge = _all_edges[i];
        nbEdgesTot++;
        if ( edge->isCongested() ) {
            nbEdgesOv++;
            unsigned edgeOv = 2*edge->getOverflow();
            overflow += edgeOv;
            maxOv = edgeOv > maxOv ? edgeOv : maxOv;
            edge->addSubEstimateOccupancy ( HISTORIC_INC, true ); // add historic cost for each overflowed edge
        }
        forEach ( Segment*, segment, edge->getSegments() ) {
            map<Segment*, segmentStat>::iterator it = segmentsMap.find(*segment);
            if ( it != segmentsMap.end() ) {
                (*it).second.incNbTot();
                if ( edge->isCongested() ) {
                    (*it).second.incNbDep();
                    (*it).second.incSumOv(edge->getOverflow());
                }
            }
            else {
                if ( edge->isCongested() )
                    segmentsMap[*segment]=segmentStat(1,1,edge->getOverflow());
                else
                    segmentsMap[*segment]=segmentStat(0,1,0);
            }
        }
    }

    unsigned nbDep = 0;
    unsigned nbTot = 0;
    float minimalCost = 0;
    float maximalCost = 0;
    for ( map<Segment*, segmentStat>::iterator it = segmentsMap.begin() ; it != segmentsMap.end() ; it++ ) {
      assert( (*it).second.getNbTot() != 0 );

      nbTot++;
      wirelength += getGridLength( (*it).first );
      float segmentCost = (float((*it).second.getNbDep()) / float((*it).second.getNbTot())) * (*it).second.getSumOv();
      if ( segmentCost ) {
        if (minimalCost == 0)
          minimalCost = segmentCost;
        minimalCost = segmentCost < minimalCost ? segmentCost : minimalCost;
        maximalCost = segmentCost > maximalCost ? segmentCost : maximalCost;

        STuple* stuple = STuple::create((*it).first, segmentCost);
        addToSTuplePQ(stuple);
        nbDep++;
      }
    }

    forEach ( Net*, net, _cell->getNets() ) {
        forEach ( Contact*, contact, net->getContacts() ) {
            const Layer* cLayer = contact->getLayer();
            if ( cLayer == Configuration::getGContact() || cLayer == Configuration::getGMetalV()  )
                viaWirelength += 3;
        }
    }
    //forEach ( Net*, net, _cell->getNets() ) {
    //    forEach ( Segment*, segment, net->getSegments() ) {
    //        if ( isAGlobalRoutingSegment ( *segment ) ) {
    //            float segmentCost = getSegmentCost ( *segment );
    //            if ( segmentCost ) {
    //                nbDep++;
    //                STuple* stuple = NULL;
    //                // si une CostProperty est attaché au segment et que son iterateur est différent de _stuplePQEnd
    //                //    on recupere le STuple
    //                // sinon on cree un stuple
    //                Hurricane::Property* property = segment->getProperty ( STuple::CostProperty::_name );
    //                if ( STuple::CostProperty* cprop = dynamic_cast<STuple::CostProperty*>(property) ) {
    //                    STuple::STuplePQIter pqit = cprop->getPQIter();
    //                    if ( pqit != STuple::_stuplePQEnd )
    //                        stuple = (*pqit);
    //                }
    //                if ( !stuple )
    //                    stuple = STuple::create ( *segment, segmentCost );
    //                addToSTuplePQ ( stuple );
    //            }
    //            nbTot++;
    //        }
    //    }
    //}

    cmess2 << "                     # of Overcapacity edges:" << nbEdgesOv
           <<                    "  (tot.:" << nbEdgesTot << ")" << endl;
    cmess2 << "                     # of Overflow:" << overflow
           <<                    "  Max. overflow:" << maxOv
           <<                    "  Avg. overflow:" << (float)overflow / (float)nbEdgesTot << endl;
    cmess2 << "                     # of Overflowed edges: " << nbDep << " (tot.:" << nbTot << ")" << endl; 
    cmess2 << "                     gHPWL:" << wirelength
           <<                    "  # of VIAs:" << viaWirelength/3
           <<                    "  Tot. wirelength:" << wirelength + viaWirelength << endl;


    // cmess1 << "     o  Analyse routing :" << endl
    //        << "        - Number of overcapacity edges : " << nbEdgesOv << " / " << nbEdgesTot << endl
    //        << "        - # of overflow : " << overflow << endl
    //        << "        - max of overflow : " << maxOv << endl
    //        << "        - avg overflow : " << (float)overflow / (float)nbEdgesTot << endl
    //        << "        - grid wirelength : " << wirelength << endl
    //        << "        - # of via : " << viaWirelength/3 << endl
    //        << "        - total wirelength : " << wirelength + viaWirelength << endl
    //        << "        - Overflowed segments : " << nbDep << " / " << nbTot << endl;
    // construct the list on segments to unroute
    if ( _stuplePriorityQueue.empty() ) {
    //cmess1 << "        - Nothing to reroute !" << endl;
    //cmess1 << "        - Maximum segment cost : 0" << endl;
    }
    else {
        STuple* topSTuple = getMaxFromSTuplePQ();
      //float maxCost = topSTuple->getCost();
      //cmess1 << "        - Maximum segment cost : " << maxCost << endl;
        float minCost = 0.20;
        if (_stuplePriorityQueue.size() <= 100)
            minCost = 0.0;
        //if ( maxCost > 1 )
        //    minCost = maxCost * 0.20;
        
        //cmess1 << "        - Minimum segment cost considered : " << minCost << endl;
        //cmess1 << "        - Minimal computed segment cost : " << minimalCost << endl
        //       << "        - Maximal computed segment cost : " << maximalCost << endl;
        segmentsToUnroute.insert ( topSTuple->getSegment() );
        //debugName = topSTuple->getSegment()->getNet()->getName();
        popMaxFromSTuplePQ();
        topSTuple = getMaxFromSTuplePQ();
        //bool fullSkim = true;
        //printSTuplePQ();
        while ( topSTuple != NULL ) {
            if ( topSTuple->getCost() >= minCost ) {
                segmentsToUnroute.insert ( topSTuple->getSegment() );
                popMaxFromSTuplePQ();
                topSTuple = getMaxFromSTuplePQ();
            }
            else {
                //fullSkim = false;
                break;
            }
        }
        //cerr << "        - fullSkim : " << fullSkim << endl;
        //printSTuplePQ();
    }
    return overflow;
}

void Graph::getHorizontalCutLines ( vector<DbU::Unit>& horizontalCutLines )
// ************************************************************************
{
    horizontalCutLines.clear();
    Vertex* currentVertex = _lowerLeftVertex;
    horizontalCutLines.push_back ( currentVertex->getYMin() );
    while ( currentVertex ) {
        horizontalCutLines.push_back ( currentVertex->getYMax() );
        Edge* edge = currentVertex->getVEdgeOut();
        if ( edge )
            currentVertex = edge->getTo();
        else
            break;
    }
}

void Graph::getVerticalCutLines ( vector<DbU::Unit>& verticalCutLines )
// ********************************************************************
{
    verticalCutLines.clear();
    Vertex* currentVertex = _lowerLeftVertex;
    verticalCutLines.push_back ( currentVertex->getXMin() );
    while ( currentVertex ) {
        verticalCutLines.push_back ( currentVertex->getXMax() );
        Edge* edge = currentVertex->getHEdgeOut();
        if ( edge)
            currentVertex = edge->getTo();
        else
            break;
    }
}

bool Graph::hasGlobalRouting ( RoutingPad* routingPad )
// ****************************************************
{
    Hook* rpHook = routingPad->getBodyHook();
    Vertex* rpVertex = _matrixVertex->getVertex ( routingPad->getX(), routingPad->getY() );
    Hook* currentHook = rpHook->getNextHook();
    while ( currentHook != rpHook ) {
        if ( dynamic_cast<Segment::TargetHook*>(currentHook) ) {
            Segment* seg = dynamic_cast<Segment*>(currentHook->getComponent());
            if ( getVertex ( seg->getTargetX(), seg->getTargetY() ) == rpVertex )
                return true;
        }
        if ( dynamic_cast<Segment::SourceHook*>(currentHook) ) {
            Segment* seg = dynamic_cast<Segment*>(currentHook->getComponent());
            if ( getVertex ( seg->getSourceX(), seg->getSourceY() ) == rpVertex )
                return true;
        }
        currentHook = currentHook->getNextHook();
    }
    return false;
}

bool Graph::isAGlobalRoutingContact ( Contact* contact )
// *****************************************************
{
    if ( contact->getLayer() == Configuration::getGMetalH () ) return true;
    if ( contact->getLayer() == Configuration::getGMetalV () ) return true;
    if ( contact->getLayer() == Configuration::getGContact() ) return true;
    return false;
}

bool Graph::isAGlobalRoutingSegment ( Segment* segment )
// *****************************************************
{
    if ( segment->getLayer() == Configuration::getGMetalH () ) return true;
    if ( segment->getLayer() == Configuration::getGMetalV () ) return true;
    return false;
}

static void setContactLayer(Contact* contact)
// ******************************************
{
	bool alu2Used = false;
	bool alu3Used = false;

	for_each_hook(hook, contact->getBodyHook()->getHooks()) {
		Component* component = hook->getComponent();
		if (dynamic_cast<Segment*>(component) || dynamic_cast<RoutingPad*>(component)) {
			const Layer* layer = component->getLayer();
            if      (layer == Configuration::getGMetalH() || layer == Configuration::getPinMetal() ) alu2Used = true;
            else if (layer == Configuration::getGMetalV() )   alu3Used = true;
            //else    assert(false);   // makes non ispd07 benchs failed....
		}
		end_for;
	}

	const Layer* layer = NULL;
    if ( alu2Used ) {
        if ( alu3Used )
          layer = Configuration::getGContact();
        else
            layer = Configuration::getGMetalH();
    }
    else if ( alu3Used ) {
      layer = Configuration::getGMetalV();
    }
    else
        assert ( false );

    contact->setLayer(layer);
}

void Graph::MaterializeRouting ( Vertex* vertex )
// **********************************************
{
    // This function materializes the routing of the connexe component passed as argument
    
    countMaterialize++;

    //if ( debugging ) {
    //    cerr << "    Materialize routing" << endl;
    //    CEditor* editor = getCEditor( _nimbus->getCell() );
    //    editor->Refresh();
    //    string stopMessage = "MaterializeRouting, starting vertex: ";
    //    stopMessage += getString ( vertex );
    //    editor->Stop(stopMessage);
    //}
    
    depthMaterialize = 0;
    //cerr << "INITIAL CALL of materialize routing for net : " << _working_net << endl;
    Edge*    arrivalEdge = NULL;
    Contact* initContact = NULL;
    if ( _useSegments ) {
        initContact = vertex->getContact ( arrivalEdge );
        assert ( initContact ); // must be != NULL because the source vertex must have a routingPad !
    }

    MaterializeRouting ( vertex, arrivalEdge, initContact );
}

void Graph::MaterializeRouting ( Vertex* vertex, Edge* arrivalEdge, Contact* initialContact )
// ******************************************************************************************
{
    depthMaterialize++;
    int vertexConnex = vertex->getConnexID();
    assert ( vertexConnex != -1 );
    //cerr << "--> check in MaterializeRouting " << endl;
    //checkGraphConsistency();
    //cerr << "    MR:" << depthMaterialize << ": vertex " << vertex << endl;
    
    //add for test ofnew MaterializeRouting 03/09/2997 d2
    if ( _useSegments ) {
        for ( unsigned i = 0 ; i < 4 ; i++ ) {
            if ( Edge* edge = vertex->getFirstEdges ( i ) ) {
                if ( edge == arrivalEdge )
                    continue;
                if ( (edge->getNetStamp() == _netStamp) && (edge->getConnexID() == vertexConnex) ) {
                    Vertex* oppositeVertex = edge->getOpposite ( vertex );
                    Contact* reachedContact = oppositeVertex->getContact(edge);
                    Edge* straightArrivalEdge = edge;
                    vector<Edge*> crossedEdges; 
                    crossedEdges.push_back(edge);
                    while ( reachedContact == NULL ) {
                        straightArrivalEdge = oppositeVertex->getFirstEdges(i);
                        if ( straightArrivalEdge == NULL )
                            throw Error ( "Graph:MaterializeRouting(): Cannot continue straight on." );
                        oppositeVertex = straightArrivalEdge->getOpposite ( oppositeVertex );
                        reachedContact = oppositeVertex->getContact ( straightArrivalEdge );
                        //straightArrivalEdge->incOccupancy(); // put the incOccupancy in insertSegment function
                        crossedEdges.push_back(straightArrivalEdge);
                    }
                    // test pour ne pas recreer le routage preexistant en cas de reroutage
                    bool alreadyExist = false;
                    for_each_component ( component, initialContact->getSlaveComponents() ) {
                        if ( dynamic_cast<Segment*>(component) ) {
                            Segment* seg = static_cast<Segment*>(component);
                            if ( seg->getOppositeAnchor(initialContact) == (Component*)(reachedContact) ) {
                                //cerr << "    [0;33msegment already exists[0;m" << endl;
                                alreadyExist = true;
                            }
                        }
                        end_for;
                    }
                    if ( !alreadyExist ) {
                        Segment* segment = createSegment ( initialContact, reachedContact );
                        assert ( segment );
                        for ( unsigned j = 0 ; j < crossedEdges.size(); j++ )
                            crossedEdges[j]->insertSegment(segment);
                    }
                    MaterializeRouting ( oppositeVertex, straightArrivalEdge, reachedContact );
                }
            }
        }
        setContactLayer(initialContact);
    }
    //else {
    //    for_each_edge ( edge, vertex->getAdjacentEdges() ) {
    //        //if ( edge == arrivalEdge)
    //        //    continue;
    //        //cerr << "    MR:" << depthMaterialize<< ":     edge " << edge << endl;
    //        if ( (edge->getNetStamp() == _netStamp) && (edge->getConnexID() == vertexConnex) ) {
    //            edge->createSplitter ( _working_net );
    //            _nbSplitters++;
    //            Hook* hook = edge->getSplitterHook ( vertex );
    //            assert (hook);
    //            if ( Hook* previousHook = vertex->getLocalRingHook() ) // XXX Attention puisqu'on vire le systeme le localRingHook au profit du contact
    //                hook->attach ( previousHook );
    //            vertex->setLocalRingHook ( hook );
    //            if ( edge != arrivalEdge ) {
    //                MaterializeRouting ( edge->getOpposite(vertex), edge);
    //                // cleaning for next routing
    //                edge->setSplitter(NULL);
    //            }
    //        end_for;
    //        }
    //    }
    //}
    // cleaning for next routing
    
    //vertex->setLocalRingHook(NULL);
    vertex->setContact(NULL);
    depthMaterialize--;
}

void Graph::CleanRoutingState()
// ****************************
{
    // Vide la pile
    clearPriorityQueue();

    //// Pour chacun des vertex, on efface le pointeur sur la ronde locale et on reinitialise le connexID, la distance et le predecessor
    //// le vtuple est aussi mis a NULL, c'est pourquoi il faut que la pile ait été vidée avant ( sinon assert )
    //// (XXX mieux vaudrait le faire uniquement pour les vertex touchés par le routage XXX)
    //for ( unsigned i = 0 ; i < _all_vertexes.size() ; i++ ) {
    //    Vertex* vertex = _all_vertexes[i];
    //    assert (vertex);
    //    vertex->setLocalRingHook(NULL);
    //    vertex->setConnexID(-1);
    //    vertex->setDistance((float)(HUGE_VAL));
    //    vertex->setPredecessor(NULL);
    //    assert ( vertex->getVTuple() == NULL );
    //}

    //// Pour chacune des edges, on efface le pointeur sur le splitter (potentiel) et on reinitialise le connexID
    //for ( unsigned i = 0 ; i < _all_edges.size() ; i++ ) {
    //    Edge* edge = _all_edges[i];
    //    assert ( edge );
    //    edge->setSplitter(NULL);
    //    edge->setConnexID(-1);
    //}

    // On vide le set des _vertexes_to_route
    _vertexes_to_route.clear();
    _working_net = NULL;
    _searchingArea.makeEmpty();
}

void Graph::checkGraphConsistency()
// ********************************
{
    bool checkFailed = false;
    unsigned nbEdges= 0;
    for_each_edge ( edge, VectorCollection<Edge*>(_all_edges) ) {
        nbEdges++;
        if ( edge->getNetStamp() > _netStamp ) {
            cerr << "    - " << edge << " netStamp mismatch" << endl;
            checkFailed = true;
        }
        if ( edge->getNetStamp() == _netStamp ) {
            int edgeID = edge->getConnexID();
            if ( edgeID != -1 ) {
                if ( edge->getFrom()->getConnexID() == -1 ) {
                    cerr << "    - from : " << edge->getFrom() << " of " << edge << " : connexID mismatch" << endl;
                    checkFailed = true;
                }
                if ( edge->getTo()->getConnexID() == -1 ) {
                    cerr << "    - to : " << edge->getTo() << " of " << edge << " : connexID mismatch" << endl;
                    checkFailed = true;
                }
                if ( (edge->getFrom()->getConnexID() != edgeID) && (edge->getTo()->getConnexID() != edgeID) ) {
                    cerr << "    - " << edge << " : connexID mismatch" << endl;
                    checkFailed = true;
                }
            }
        }
        end_for
    }
    if ( checkFailed ) {
        cerr << " ************************************" << endl
             << " *  GRAPH CHECK CONSISTENCY FAILED  *" << endl
             << " ************************************" << endl;
    }
}

void Graph::checkEmptyPriorityQueue()
// **********************************
{
    bool checkFailed = false;
    // check empty priority queue
    if ( !_vtuplePriorityQueue.empty() ) {
        checkFailed = true;
        cerr << "    Priority Queue is not empty !!!!" << endl
             << "      stil in queue :" << endl;

        VTuplePQIter pqit = _vtuplePriorityQueue.begin();
        while ( pqit != _vtuplePriorityQueue.end() ) {
            cerr << "      " << (*pqit)->getVertex() << " : " << (*pqit)->getDistance() << endl;
            pqit++;
        }
    }
    //check null vtuple foreach vertex
    unsigned nbVertexes= 0;
    for_each_vertex ( vertex, VectorCollection<Vertex*>(_all_vertexes) ) {
        nbVertexes++;
        if ( vertex->getNetStamp() > _netStamp ) {
            cerr << "    - " << vertex << " netStamp mismatch" << endl;
            checkFailed = true;
        }
        if ( vertex->getVTuple() ) {
            cerr << "    - " << vertex << " still have a vtuple : " << vertex->getVTuple() << endl;
            checkFailed = true;
        }
        end_for
    }
    if ( checkFailed ) {
        cerr << " *********************************************" << endl
             << " *  GRAPH CHECK EMPTY PRIORITY QUEUE FAILED  *" << endl
             << " *********************************************" << endl;
    }

}

// DensityWindow* Graph::createEstimateOccupancyWindow()
// // **************************************************
// {
//     if ( _estimateOccupancyWindow )    return _estimateOccupancyWindow;

//     unsigned int max   = _ySize > _xSize ? _ySize : _xSize;
//     unsigned int scale = (unsigned) ceil(500.0/(double)max);

//     _estimateOccupancyWindow = new DensityWindow ( _xSize, _ySize, 3, "fire", scale );
//     _estimateOccupancyWindow->setName ( 0, "Global Estimate Occupancy" );
//     _estimateOccupancyWindow->setName ( 1, "Horizontal Estimate Occupancy" );
//     _estimateOccupancyWindow->setName ( 2, "Vertical Estimate Occupancy" );

//     return _estimateOccupancyWindow;
// }

// void Graph::UpdateEstimateOccupancyWindow()
// // ****************************************
// {
//     if ( !_estimateOccupancyWindow )
//         createEstimateOccupancyWindow();

//     _estimateOccupancyWindow->setSlice(0);
//     Vertex* currentVertex = _lowerLeftVertex;
//     int i = 0;
//     int j = 0;
//     while ( currentVertex ) {
//         Vertex* firstLineVertex = currentVertex;
//         i = 0;
//         while ( currentVertex ) {
//             float xEstimateOccupancy = 0.0;
//             Edge* hEdgeOut = currentVertex->getHEdgeOut();
//             if ( hEdgeOut )
//                 xEstimateOccupancy += hEdgeOut->getEstimateOccupancy();
//             float yEstimateOccupancy = 0.0;
//             if ( Edge* vEdgeOut = currentVertex->getVEdgeOut() )
//                 yEstimateOccupancy += vEdgeOut->getEstimateOccupancy();

//             int colorValue  = _maxEstimateOccupancy  != 0 ? (int)(255 * (xEstimateOccupancy + yEstimateOccupancy) / _maxEstimateOccupancy) : 0;
//             int colorXValue = _maxXEstimateOccupancy != 0 ? (int)(255 * xEstimateOccupancy / _maxXEstimateOccupancy) : 0;
//             int colorYValue = _maxYEstimateOccupancy != 0 ? (int)(255 * yEstimateOccupancy / _maxYEstimateOccupancy) : 0;
//             _estimateOccupancyWindow->DrawRectangle(i, j, i, j, colorValue , 0);
//             _estimateOccupancyWindow->DrawRectangle(i, j, i, j, colorXValue, 1);
//             _estimateOccupancyWindow->DrawRectangle(i, j, i, j, colorYValue, 2);
//             i++;
//             if ( hEdgeOut )
//                 currentVertex = hEdgeOut->getOpposite ( currentVertex );
//             else
//                 break;
//         }
//         j++;
//         Edge* vEdgeOut = firstLineVertex->getVEdgeOut();
//         if ( vEdgeOut )
//             currentVertex = vEdgeOut->getOpposite ( firstLineVertex );
//         else
//             break;
//     }
//     _estimateOccupancyWindow->getWindow()->show();
//     return;
// }

//void Graph::printStats()
//// *********************
//{
//    Vertex* currentVertex = _lowerLeftVertex;
//    int   i = 0;
//    int   j = 0;
//    int   nbEdgesTotal = 0;
//    int   nbEdgesOver  = 0;
//
//    unsigned overflow  = 0;
//    unsigned maxOver   = 0;
//    //float maxOver      = 0;
//    //float averageOver  = 0;
//    while ( currentVertex ) {
//        Vertex* firstLineVertex = currentVertex;
//        i = 0;
//        while ( currentVertex ) {
//            Edge* hEdgeOut = currentVertex->getHEdgeOut();
//            if ( hEdgeOut ) {
//                nbEdgesTotal++;
//                int ov = 2*(hEdgeOut->getRealOccupancy() - hEdgeOut->getCapacity()); // 2 = minimum sapcing + minimum width
//                if ( ov > 0 ) {
//                    nbEdgesOver++;
//                    overflow += ov;
//                    maxOver = ov > maxOver ? ov : maxOver;
//                    for_each_segment ( segment, hEdgeOut->getSegments() ) {
//                        Net* net = segment->getNet();
//                        map<Net*, unsigned>::iterator nuit = _netNbOverEdges.find ( net );
//                        if ( nuit != _netNbOverEdges.end() )
//                            (*nuit).second = (*nuit).second + 1;
//                        else
//                            _netNbOverEdges[net] = 1;
//                        end_for;
//                    }
//                }
//                //float tempOver = (float)hEdgeOut->getRealOccupancy() / (float)hEdgeOut->getCapacity();
//                //if ( tempOver > 0.8 ) {
//                //    nbEdgesOver++;
//                //    averageOver += tempOver;
//                //    if ( tempOver > maxOver )    maxOver = tempOver;
//                //}
//            }
//            if ( Edge* vEdgeOut = currentVertex->getVEdgeOut() ) {
//                nbEdgesTotal++;
//                int ov = 2*(vEdgeOut->getRealOccupancy() - vEdgeOut->getCapacity());
//                if ( ov > 0 ) {
//                    nbEdgesOver++;
//                    overflow += ov;
//                    maxOver = ov > maxOver ? ov : maxOver;
//                    for_each_segment ( segment, vEdgeOut->getSegments() ) {
//                        Net* net = segment->getNet();
//                        map<Net*, unsigned>::iterator nuit = _netNbOverEdges.find ( net );
//                        if ( nuit != _netNbOverEdges.end() )
//                            (*nuit).second = (*nuit).second + 1;
//                        else
//                            _netNbOverEdges[net] = 1;
//                        end_for;
//                    }
//                }
//                //float tempOver = (float)vEdgeOut->getRealOccupancy() / (float)vEdgeOut->getCapacity();
//                //if ( tempOver > 0.8 ) {
//                //    nbEdgesOver++;
//                //    averageOver += tempOver;
//                //    if ( tempOver > maxOver )    maxOver = tempOver;
//                //}
//            }
//
//            i++;
//            if ( hEdgeOut )
//                currentVertex = hEdgeOut->getOpposite ( currentVertex );
//            else
//                break;
//        }
//        j++;
//        Edge* vEdgeOut = firstLineVertex->getVEdgeOut();
//        if ( vEdgeOut )
//            currentVertex = vEdgeOut->getOpposite ( firstLineVertex );
//        else
//            break;
//    }
//    //averageOver = nbEdgesOver == 0 ? 0 : averageOver / (float)nbEdgesOver;
//    //
//
//    cerr << "        - Total number of edges : " << nbEdgesTotal << endl
//         << "        - Number of overcapacity edges : " << nbEdgesOver << endl
//         << "        - Total calls to Dijkstra : " << countDijkstra << endl
//         << "        - Total calls to Monotonic : " << countMonotonic << endl
//         << "        - Total calls to Materialize : " << countMaterialize << endl
//         << "        - Taille du Graphe de routage : " << _xSize << " x " << _ySize << endl
//         << endl
//         << "        - # of overflow : " << overflow << endl
//         << "        - max of overflow : " << maxOver << endl
//         << "        - # of net with overflow : " << _netNbOverEdges.size() << endl;
//    return;
//}

// DensityWindow* Graph::createOccupancyWindow()
// // ******************************************
// {
//     if ( _occupancyWindow )    return _occupancyWindow;

//     unsigned int max   = _ySize > _xSize ? _ySize : _xSize;
//     unsigned int scale = (unsigned) ceil(500.0/(double)max);

//     _occupancyWindow = new DensityWindow ( _xSize, _ySize, 3, "fire", scale );
//     _occupancyWindow->setName ( 0, "Global Occupancy" );
//     _occupancyWindow->setName ( 1, "Horizontal Occupancy" );
//     _occupancyWindow->setName ( 2, "Vertical Occupancy" );

//     return _occupancyWindow;
// }

// void Graph::UpdateOccupancyWindow()
// // ********************************
// {
//     if ( !_occupancyWindow )
//         createOccupancyWindow();

//     _occupancyWindow->setSlice(0);
//     Vertex* currentVertex = _lowerLeftVertex;
//     int i = 0;
//     int j = 0;
//     while ( currentVertex ) {
//         Vertex* firstLineVertex = currentVertex;
//         i = 0;
//         while ( currentVertex ) {
//             unsigned xOccupancy = 0;
//             Edge* hEdgeOut = currentVertex->getHEdgeOut();
//             if ( hEdgeOut )
//                 xOccupancy += hEdgeOut->getRealOccupancy();
//             unsigned yOccupancy = 0;
//             if ( Edge* vEdgeOut = currentVertex->getVEdgeOut() )
//                 yOccupancy += vEdgeOut->getRealOccupancy();
//             assert(_maxOccupancy);
//             assert(_maxXOccupancy);
//             assert(_maxYOccupancy);
//             _occupancyWindow->DrawRectangle(i, j, i, j, (255 * (xOccupancy + yOccupancy) / _maxOccupancy), 0);
//             _occupancyWindow->DrawRectangle(i, j, i, j, (255 * xOccupancy / _maxXOccupancy), 1);
//             _occupancyWindow->DrawRectangle(i, j, i, j, (255 * yOccupancy / _maxYOccupancy), 2);
//             i++;
//             if ( hEdgeOut )
//                 currentVertex = hEdgeOut->getOpposite ( currentVertex );
//             else
//                 break;
//         }
//         j++;
//         Edge* vEdgeOut = firstLineVertex->getVEdgeOut();
//         if ( vEdgeOut )
//             currentVertex = vEdgeOut->getOpposite ( firstLineVertex );
//         else
//             break;
//     }
//     _occupancyWindow->getWindow()->show();
//     return;
// }

Record* Graph::_getRecord() const
// ************************
{
    Record* record = new Record ( getString ( this ) );

    record->add ( getSlot ( "LowerLeftVertex", _lowerLeftVertex ) );
//    record->add ( getSlot ( "SlicingTree"    , _slicingTree ) );
    record->add ( getSlot ( "WorkingNet"     , _working_net ) );
    record->add ( getSlot ( "SearchingArea"  , _searchingArea ) );
    record->add ( getSlot ( "xSize"          , _xSize ) );
    record->add ( getSlot ( "ySize"          , _ySize ) );
    record->add ( getSlot ( "maxEstimateOccupancy", _maxEstimateOccupancy ) );
    record->add ( getSlot ( "maxXEstimateOccupancy", _maxXEstimateOccupancy ) );
    record->add ( getSlot ( "maxYEstimateOccupancy", _maxYEstimateOccupancy ) );

    return record;
}

string Graph::_getString() const
// *****************************
{
    return   "<" + _TName ( "RoutingGraph" )
           //+ ":" + getString ( _slicingTree )
           + "," + getString ( _xSize )
           + "," + getString ( _ySize )
           + ">";
}

} // namespace Knik
