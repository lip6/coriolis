#ifndef _KNIK_EDGE_H
#define _KNIK_EDGE_H

#include <algorithm>

#include "hurricane/ExtensionGo.h"
#include "hurricane/Segment.h"
#include "hurricane/Error.h"

//#define __USE_CONGESTION__
//#define __USE_STATIC_PRECONGESTION__
//#define __USE_DYNAMIC_PRECONGESTION__
namespace Knik {

  using namespace Hurricane;

    class Vertex;

    class Edge : public ExtensionGo {
    // *************************
        // Types
        // *****
        public:
            typedef ExtensionGo Inherit;

        // Attributes
        // **********
        protected:
            static const Name  _extensionName;
            static float _h;
            static float _k;

            Box       _boundingBox;
            Vertex*   _from;
            Edge*     _nextFrom;
            Vertex*   _to;
            Edge*     _nextTo;
            int       _connexID;
            float     _cost;
            unsigned  _capacity;
            unsigned  _realOccupancy;
            float     _estimateOccupancy; // used as estimateOccupancy in routing and historic cost during ripup & reroute
            float     _normalisedLength;
            unsigned  _netStamp;
            bool      _isCongested;
            vector<Segment*> _segments;
                 
        // Constructors & Destructors
        // **************************
        protected:
            Edge ( Vertex* from, Vertex* to );
            Edge ( Vertex* from, Vertex* to, unsigned capacity );
            ~Edge ();

        public: 
            //static Edge* create ( Fence* fence, Vertex* from, Vertex* to );
            void  destroy();
            void _postCreate();
            void _preDestroy();

        // Modifiers
        // *********
        public:
            void incOccupancy     ();
            void decOccupancy     ();
            void insertSegment    ( Segment* segment )   { assert(segment); incOccupancy(); _segments.push_back(segment); };
            void setNextFrom      ( Edge* edge )         { _nextFrom = edge; };
            void setNextTo        ( Edge* edge )         { _nextTo = edge; };
            void setConnexID      ( int connexID )       { _connexID = connexID; };
          //void setCapacity      ( unsigned capacity )  { _capacity = capacity; };
            void setCapacity      ( unsigned capacity );
            void increaseCapacity ( int capacity );
            void setCost          ( float cost )         { _cost = cost; };
            void incCost          ( float inc )          { _cost += inc; };
            void setNetStamp      ( unsigned netStamp )  { _netStamp = netStamp; };
            void setHParameter    ( float h )            { _h = h; };
            void setKParameter    ( float k )            { _k = k; };
            void removeSegment    ( Segment* segment );
            void addSubEstimateOccupancy ( float increment, bool add );

        // Accessors
        // *********
        public:
            static const Name& staticGetName () { return _extensionName; };
                   const Name& getName       () const { return _extensionName; };
            Vertex*   getFrom             () const { return _from; };
            Edge*     getNextFrom         () const { return _nextFrom; };
            Vertex*   getTo               () const { return _to; };
            Edge*     getNextTo           () const { return _nextTo; };
            int       getConnexID         () const { return _connexID; };
            unsigned  getCapacity         () const { return _capacity; };
            float     getEstimateOccupancy() const { return _estimateOccupancy; };
            unsigned  getNetStamp         () const { return _netStamp; };
            float     getHParameter       ()       { return _h; };
            float     getKParameter       ()       { return _k; };
            unsigned  getOverflow         () const { return (_realOccupancy>_capacity)?_realOccupancy-_capacity:0; };
            Vertex*   getOpposite ( const Vertex* v ) const { if (v == _from) return _to;
                                                              if (v == _to)   return _from;
                                                              assert ( (v==_from) || (v==_to) );
                                                              return NULL; /* to avoid warning, never reached */ };
            GenericCollection<Segment*> getSegments() { return getCollection ( _segments ); } ;
            unsigned  getRealOccupancy    () const;
            Segment*  getSegmentFor       ( Net*net );
            float     getCost             ( Edge* arrivalEdge );
            float     getConstCost     () const { return _cost; };
            DbU::Unit      getXTo      () const;
            DbU::Unit      getYTo      () const;
            DbU::Unit      getXFrom    () const;
            DbU::Unit      getYFrom    () const;
            Box            getBoundingBox() const { return _boundingBox; };
            virtual Box        computeBoundingBox() const = 0;
            virtual Point      getReferencePoint () const = 0;
            virtual DbU::Unit  getWidth () const = 0;
            virtual float      getNormalisedLength() const = 0;
            //virtual void       createSplitter  ( Net* net )       = 0;
            //virtual Hook*      getSplitterHook ( Vertex* vertex ) = 0;

        // Predicates
        // **********
        public:
            virtual bool isVertical  () const = 0;
            virtual bool isHorizontal() const = 0;
            bool         isCongested () { return _isCongested; } 
            bool         hasInfo     () const;

        // ExtensionGo methods
        // **************
        public:
            void  translate ( const DbU::Unit& dx, const DbU::Unit& dy );
            Cell* getCell () const;

            void invalidate ( bool propagateFlag );

        // Others
        // ******
        public:
            Record* _getRecord() const;
            string  _getString() const;
    };
} // namespace Knik


INSPECTOR_P_SUPPORT(Knik::Edge);


#endif  // _KNIK_EDGE_H
