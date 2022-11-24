#ifndef _KNIK_MATRIXVERTEX_H
#define _KNIK_MATRIXVERTEX_H

#include <utility>
#include <vector>

#include "hurricane/Point.h"
#include "hurricane/Error.h"

#include "knik/RoutingGrid.h"

namespace Knik {

  using namespace std;

  class Graph;
  class Vertex;

    class MatrixVertex {
    // *****************
        // Types
        // *****
            struct IndexComp : public binary_function<const pair<DbU::Unit,unsigned>, const pair<DbU::Unit,unsigned>, bool> {
                bool operator() ( const pair<DbU::Unit,unsigned>& pair1, const pair<DbU::Unit,unsigned>& pair2 )const {
                    return (pair1.first < pair2.first);
                }
            };
            typedef vector< pair<DbU::Unit,unsigned> >::iterator  pairIterator;

        // Attributes
        // **********
        protected:
            bool      _xInit;
            bool      _yInit;
            bool      _xRegular;
            bool      _yRegular;
            unsigned  _nbXTiles;
            unsigned  _nbYTiles;
            DbU::Unit _tileWidth;
            DbU::Unit _tileHeight;
            Box       _boundingBox;
            Graph*    _routingGraph;
            vector< vector<Vertex*> >     _matrix;
            vector< pair<DbU::Unit,unsigned> > _linesIndexes;
            vector< pair<DbU::Unit,unsigned> > _columnsIndexes;

        // Constructors & Destructors
        // **************************
        protected:
            MatrixVertex ( Graph* routingGraph );
            ~MatrixVertex();
        public:
            static MatrixVertex* create ( Graph* routingGraph );
            void  destroy();
            void _postCreate ();
            void _preDestroy();

        // Modifiers
        // *********
        public:
            //void createXRegular   ( RoutingGrid* routingGrid );
            //void createYRegular   ( RoutingGrid* routingGrid );
            Vertex* createRegularMatrix ( RoutingGrid* routingGrid ); // used for ispd global routing benchmarks
            Vertex* createRegularMatrix (); // used for other benchmarks
            void setVertex        ( pair<unsigned int,unsigned int> indexes, Vertex* vertex );
            void setVertex        ( Point point, Vertex* vertex );
        private:
            //void createMatrix ();

        // Accessors
        // *********
        public:
            DbU::Unit  getTileWidth () const { return _tileWidth; }
            DbU::Unit  getTileHeight () const { return _tileHeight; }
            size_t getXSize () const { return _matrix[0].size(); };
            size_t getYSize () const { return _matrix.size(); };
            unsigned int getLineIndex   ( DbU::Unit y );
            unsigned int getColumnIndex ( DbU::Unit x );
            pair<unsigned int,unsigned int> getIJ ( DbU::Unit x, DbU::Unit y );
            pair<unsigned int,unsigned int> getIJ ( const Point& point );
            Vertex* getVertex ( pair<unsigned int,unsigned int> indexes );
            Vertex* getVertex ( Point point );
            Vertex* getVertex ( DbU::Unit x, DbU::Unit y );
            Vertex* getVertexFromIndexes ( unsigned lineIdx, unsigned columnIdx );
            bool isLineIndexValid   ( int lineIdx )   { if ( (lineIdx >= 0)  &&(lineIdx < (int)_matrix.size()) )      return true; else return false; };
            bool isColumnIndexValid ( int columnIdx ) { if ( (columnIdx >= 0)&&(columnIdx < (int)_matrix[0].size()) ) return true; else return false; };

        // Others
        // ******
        public:
            void print();

    };

} // end namespace

#endif
