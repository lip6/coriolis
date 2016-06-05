#ifndef _KNIK_VERTEX_H
#define _KNIK_VERTEX_H

#include "hurricane/ExtensionGo.h"
#include "hurricane/Hook.h"
#include "hurricane/Contact.h"

#include "knik/Edges.h"

namespace Knik {

    class Graph;
    class Edge;
    class VTuple;

    class Vertex : public ExtensionGo {
    // ***************************
        // Types
        // *****
        public:
            typedef ExtensionGo Inherit;
            enum    Flags       { Blocked=0x0001 };
            //enum EdgeDirection { EAST=0, NORTH=1, WEST=2, SOUTH=3, UNDEFINED=4 };

        // Attributes
        // **********
        private:
            static const Name    _extensionName;
                   Graph*        _routingGraph;
                   Edge*         _firstEdges[4]; // dans l'ordre : _hEdgeOut, _vEdgeOut, _hEdgeIn et _vEdgeIn
                   Edge*         _predecessor;
                   Contact*      _contact;
                   Point         _position;
                   VTuple*       _vtuple;
                   float         _distance;
                   int           _connexID;      // XXX limiter le nombre de bits du connexID pour associer aux 3 booléens ?
                   unsigned      _netStamp;
                   DbU::Unit     _halfWidth;     // this corresponds to the half width of dual bin of the vertex
                   DbU::Unit     _halfHeight;    // this corresponds to the half height of dual bin of the vertex
                   unsigned int  _flags;
                 
        // Constructors & Destructors
        // **************************
        protected:
            Vertex ( Graph* routingGraph, Point position, DbU::Unit halfWidth, DbU::Unit halfHeight );
            ~Vertex ();

        public:
            static Vertex* create ( Graph* routingGraph, Point position, DbU::Unit halfWidth, DbU::Unit halfHeight );
            void destroy();
            void _postCreate();
            void _preDestroy();

        // Modifiers
        // *********
        public:
            void     setHEdgeOut       ( Edge* edge )        { _firstEdges[0] = edge; };
            void     setVEdgeOut       ( Edge* edge )        { _firstEdges[1] = edge; };
            void     setHEdgeIn        ( Edge* edge )        { _firstEdges[2] = edge; };
            void     setVEdgeIn        ( Edge* edge )        { _firstEdges[3] = edge; };
            void     setPredecessor    ( Edge* edge )        { _predecessor = edge; };
            void     setContact        ( Contact* contact )  { _contact = contact; };
            void     setConnexID       ( int connexID )      { _connexID = connexID; };
            void     setDistance       ( float distance )    { _distance = distance; };
            void     setNetStamp       ( unsigned netStamp ) { _netStamp = netStamp; };
            void     setVTuple         ( VTuple* vtuple )    { _vtuple = vtuple; };
            void     attachToLocalRing ( Component* component );
            void     sortEdges         ();
            void     setBlocked        () { _flags |=  Blocked; }
            void     resetBlocked      () { _flags &= ~Blocked; }

        // Accessors
        // *********
        public:
            static  const Name& staticGetName      () { return _extensionName; };
            virtual const Name& getName            () const { return _extensionName; };
                    Edge*       getHEdgeOut        () const { return _firstEdges[0]; };
                    Edge*       getVEdgeOut        () const { return _firstEdges[1]; };
                    Edge*       getHEdgeIn         () const { return _firstEdges[2]; };
                    Edge*       getVEdgeIn         () const { return _firstEdges[3]; };
                    Edges       getAdjacentEdges   () const;
                    Edge*       getPredecessor     () const { return _predecessor; };
                    Contact*    getContact         () const { return _contact; };
                    int         getConnexID        () const { return _connexID; };
                    float       getDistance        () const { return _distance; };
                    unsigned    getNetStamp        () const { return _netStamp; };
                    Point       getPosition        () const { return _position; };
                    VTuple*     getVTuple          () const { return _vtuple; };
                    Graph*      getRoutingGraph    () const { return _routingGraph; };
                    DbU::Unit   getX               () const { return _position.getX(); };
                    DbU::Unit   getY               () const { return _position.getY(); };
                    DbU::Unit   getXMin            () const { return _position.getX() - _halfWidth;  };
                    DbU::Unit   getYMin            () const { return _position.getY() - _halfHeight; };
                    DbU::Unit   getXMax            () const { return _position.getX() + _halfWidth;  };
                    DbU::Unit   getYMax            () const { return _position.getY() + _halfHeight; };
                    DbU::Unit   getHalfWidth       () const { return _halfWidth; };
                    DbU::Unit   getHalfHeight      () const { return _halfHeight; };
                    Box         getBox             () const { return Box(getXMin(),getYMin(),getXMax(),getYMax()); };
                    Edge*       getFirstEdges      ( int i ) const { return _firstEdges[i]; };
                    Contact*    getContact         ( Edge* arrivalEdge );
                    Edge*       getHEdgeLeadingTo  ( Vertex* to );
                    Edge*       getVEdgeLeadingTo  ( Vertex* to );
                    Edge*       getHEdgeComingFrom ( Vertex* from );
                    Edge*       getVEdgeComingFrom ( Vertex* from );
                    Edge*       getBestHEdgeOut    ( DbU::Unit yDest );
                    Edge*       getBestVEdgeOut    ( DbU::Unit xDest );
                    Edge*       getBestHEdgeIn     ( DbU::Unit yDest );
                    Edge*       getBestVEdgeIn     ( DbU::Unit xDest );
        
        // Predicates
        // **********
        public:
            bool isBlocked             () const           { return _flags & Blocked; };
            bool isVerticallyAligned   ( Vertex* vertex ) { return ( getX() == vertex->getX() ); };
            bool isHorizontallyAligned ( Vertex* vertex ) { return ( getY() == vertex->getY() ); };
            bool hasInfo               () const;

        // ExtensionGo methods
        // **************
        public:
            void  translate      ( const DbU::Unit& dx, const DbU::Unit& dy );
            Cell* getCell        () const;
            Box   getBoundingBox () const { Box area ( getPosition() ); return area.inflate(DbU::lambda(1)); };
            void  invalidate     ( bool propagateFlag );

        // Others
        // ******
        public:
            Record* _getRecord() const;
            string  _getString() const;
    };

struct VertexPositionComp : public binary_function<const Vertex*, const Vertex*, bool> { 
    public:
        bool operator() ( Vertex* vertex1, Vertex* vertex2 ) const {
            return ( vertex1->getX() <  vertex2->getX() ) 
              || ( ( vertex1->getX() == vertex2->getX() ) && ( vertex1->getY() < vertex2->getY() ) );
        }
};

} // namespace Knik

INSPECTOR_P_SUPPORT(Knik::Vertex);

#endif // _KNIK_VERTEX_H
