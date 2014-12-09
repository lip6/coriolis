#include <algorithm>

#include "hurricane/Cell.h"
#include "hurricane/Box.h"

#include "crlcore/AllianceFramework.h"
#include "crlcore/CellGauge.h"

#include "knik/Graph.h"
#include "knik/MatrixVertex.h"
#include "knik/KnikEngine.h"

namespace Knik {
using namespace std;
using namespace CRL;

MatrixVertex::MatrixVertex ( Graph* routingGraph )
// ***********************************************
    : _xInit(false)
    , _yInit(false)
    , _xRegular(false)
    , _yRegular(false)
    , _nbXTiles(0)
    , _nbYTiles(0)
    , _tileWidth(0)
    , _tileHeight(0)
    , _boundingBox(0,0,1,1)
    , _routingGraph(routingGraph)
{
}

MatrixVertex* MatrixVertex::create ( Graph* routingGraph )
// *******************************************************
{
    MatrixVertex* matrixVertex = new MatrixVertex(routingGraph);

    return matrixVertex;
}

MatrixVertex::~MatrixVertex()
// **************************
{
}

void MatrixVertex::destroy()
// ************************
{
    _preDestroy();
    delete ( this );
}

void MatrixVertex::_preDestroy()
// ****************************
{
}

//void MatrixVertex::createXRegular ( RoutingGrid* routingGrid )
//// ***********************************************************
//{
//
//    if ( _xInit )
//        throw Error ("MatrixVertex::createXRegular(): cannot initialize X vector twice.");
//    _lowerLeftX = routingGrid->getLowerLeftX();
//    _nbXTiles   = routingGrid->getNbXTiles();
//    _tileWidth  = routingGrid->getTileWidth();
//    _xRegular   = true;
//    _xInit      = true;
//
//    if ( _xInit && _yInit )
//        createMatrix();
//}

//void MatrixVertex::createYRegular ( RoutingGrid* routingGrid )
//// ***********************************************************
//{
//    if ( _yInit )
//        throw Error ("MatrixVertex::createYRegular(): cannot initialize Y vector twice.");
//    _boundingBox.getYMin() = routingGrid->getLowerLeftY();
//    _nbYTiles   = routingGrid->getNbYTiles();
//    _tileHeight = routingGrid->getTileHeight();
//    _yRegular   = true;
//    _yInit      = true;
//
//    if ( _xInit && _yInit )
//        createMatrix();
//}

Vertex* MatrixVertex::createRegularMatrix ( RoutingGrid* routingGrid )
// *******************************************************************
{
    if ( _xInit || _yInit )
        throw Error ("MatrixVertex::createRegularMatrix(): cannot initialize matrix twice.");

    _boundingBox = routingGrid->getBoundingBox();
    _nbXTiles    = routingGrid->getNbXTiles();
    _nbYTiles    = routingGrid->getNbYTiles();
    _tileWidth   = routingGrid->getTileWidth();
    _tileHeight  = routingGrid->getTileHeight();
    _xInit       = true;    // XXX Nécessaire pour les fonctions comme getLineIndex
    _yInit       = true;
    _xRegular    = true;    // XXX Nécessaire pour les fonctions comme getLineIndex
    _yRegular    = true;

    DbU::Unit halfWidth  = _tileWidth / 2;
    DbU::Unit halfHeight = _tileHeight / 2;

    // On cree les vecteurs de vertex en meme temps que les vertex et aussi les edges !
    for ( unsigned j = 0 ; j < _nbYTiles ; j++ ) {
        vector<Vertex*> vect;
        for ( unsigned i = 0 ; i < _nbXTiles ; i++ ) {
          Point position ( _boundingBox.getXMin()+(i*_tileWidth)+halfWidth, _boundingBox.getYMin()+(j*_tileHeight)+halfHeight );
            // on cree le vertex
            Vertex* vertex = _routingGraph->createVertex ( position, halfWidth, halfHeight );
            assert ( vertex );
            // on l'ajoute dans la matrice
            vect.push_back ( vertex );
            // si i > 0 alors on peut creer une edge horizontale entre matrix[i-1][j] et matrix[i][j] c'est a dire vect[i-1] et vect[i]
            if ( i > 0 )  { 
                Vertex* from = vect[i-1];
                assert(from);
                Vertex* to = vect[i];
                assert(to);
                _routingGraph->createHEdge ( from, to );
            }
            // si j > 0 alors on peut creer une edge verticale entre matrix[i][j-1] et matrix[i][j] c'est a dire _matrix[j-1][i] et vect[i]
            if ( j > 0 ) { //  _matrix est un vecteur de vecteur represantant les lignes -> _matrix[ligne][colonne]
                Vertex* from = _matrix[j-1][i];
                assert(from);
                Vertex* to = vect[i];
                assert(to);
                _routingGraph->createVEdge ( from, to );
            }
        }
        _matrix.push_back ( vect );
    }
    return _matrix[0][0];
}

Vertex* MatrixVertex::createRegularMatrix ()
// *****************************************
{
    if ( _xInit || _yInit )
        throw Error ("MatrixVertex::createRegularMatrix(): cannot initialize matrix twice.");

    Cell* cell = _routingGraph->getCell();
    DbU::Unit sliceHeight = AllianceFramework::get()->getCellGauge()->getSliceHeight();
    DbU::Unit cellWidth  = cell->getAbutmentBox().getWidth();
    DbU::Unit cellHeight = cell->getAbutmentBox().getHeight();
    _boundingBox = cell->getAbutmentBox();
    _nbXTiles   = (unsigned int)ceil(float(cellWidth)  / float(sliceHeight));
    _nbYTiles   = (unsigned int)ceil(float(cellHeight) / float(sliceHeight));
    _tileWidth  = sliceHeight;
    _tileHeight = sliceHeight;
    DbU::Unit _latestTileWidth  = cellWidth - ((_nbXTiles - 1) * _tileWidth);
    DbU::Unit _latestTileHeight = cellHeight - ((_nbYTiles - 1) * _tileHeight);
    _xInit      = true;    // XXX Nécessaire pour les fonctions comme getLineIndex
    _yInit      = true;
    _xRegular   = true;    // XXX Nécessaire pour les fonctions comme getLineIndex
    _yRegular   = true;

//     cerr << "Resume :" << endl
//          << "    - this             : " << (void*)this       << endl
//          << "    - cellWidth        : " << cellWidth         << endl
//          << "    - cellHeight       : " << cellHeight        << endl
//          << "    - boundingBox      : " << _boundingBox      << endl
//          << "    - nbXTiles         : " << _nbXTiles         << endl
//          << "    - nbYTiles         : " << _nbYTiles         << endl
//          << "    - tileWidth        : " << _tileWidth        << endl
//          << "    - tileHieght       : " << _tileHeight       << endl
//          << "    - latestTileWidth  : " << _latestTileWidth  << endl
//          << "    - latestTileHeight : " << _latestTileHeight << endl;

    // On cree les vecteurs de vertex en meme temps que les vertex et aussi les edges !
    size_t hreserved = KnikEngine::get( cell )->getHEdgeReservedLocal();
    size_t vreserved = KnikEngine::get( cell )->getVEdgeReservedLocal();
    for ( unsigned j = 0 ; j < _nbYTiles ; j++ ) {
        vector<Vertex*> vect;
        for ( unsigned i = 0 ; i < _nbXTiles ; i++ ) {
            DbU::Unit halfWidth  = (i == _nbXTiles - 1)?_latestTileWidth/2:_tileWidth/2;
            DbU::Unit halfHeight = (j == _nbYTiles - 1)?_latestTileHeight/2:_tileHeight/2;
            Point position ( _boundingBox.getXMin()+(i*_tileWidth)+halfWidth, _boundingBox.getYMin()+(j*_tileHeight)+halfHeight );
            // on cree le vertex
            Vertex* vertex = _routingGraph->createVertex ( position, halfWidth, halfHeight );
            assert ( vertex );
            //cerr << ". .. " << vertex << endl;
            // on l'ajoute dans la matrice
            vect.push_back ( vertex );
            // si i > 0 alors on peut creer une edge horizontale entre matrix[i-1][j] et matrix[i][j] c'est a dire vect[i-1] et vect[i]
            if ( i > 0 )  { 
                Vertex* from = vect[i-1];
                assert(from);
                Vertex* to = vect[i];
                assert(to);
                _routingGraph->createHEdge ( from, to, hreserved );
            }
            // si j > 0 alors on peut creer une edge verticale entre matrix[i][j-1] et matrix[i][j] c'est a dire _matrix[j-1][i] et vect[i]
            if ( j > 0 ) { //  _matrix est un vecteur de vecteur represantant les lignes -> _matrix[ligne][colonne]
                Vertex* from = _matrix[j-1][i];
                assert(from);
                Vertex* to = vect[i];
                assert(to);
                _routingGraph->createVEdge ( from, to, vreserved );
            }
        }
        _matrix.push_back ( vect );
    }
    //cerr << "---------------------------" << endl;
    //print();
    return _matrix[0][0];
}

//void MatrixVertex::createXIrregular ( NimbusEngine* nimbus )
//// *********************************************************
//{
//    if ( _xInit )
//        throw Error ("MatrixVertex::createXIrregular(): cannot initialize X vector twice.");
//    GCell*   gcell = nimbus->getGrid()->getLowerLeftCorner ( nimbus->getDepth() );
//    assert(gcell);
//    unsigned index = 0;
//    _columnsIndexes.push_back ( pair<DbU::Unit,unsigned>(gcell->getXMin(),index++) );
//    while ( gcell ) {
//        //if ( gcell->getRightFence() ) pour la dernière gcell, on veut récupérer la fence virtuelle de droite
//        _columnsIndexes.push_back ( pair<DbU::Unit,unsigned>(gcell->getXMax(),index++) );
//        gcell = gcell->getRightOfMe();
//    }
//    _nbXTiles = index-1;
//    //assert ( is_sorted(vertiCutLines.begin(), vertiCutLines.end()) );
//    //for ( unsigned i = 0 ; i < vertiCutLines.size() ; i++ )
//    //    _columnsIndexes.push_back ( pair<DbU::Unit,unsigned> (vertiCutLines[i], i) );
//    assert ( is_sorted(_columnsIndexes.begin(), _columnsIndexes.end(), IndexComp()) );
//    //_nbXTiles = _columnsIndexes.size()-1;
//    _xRegular = false;
//    _xInit    = true;
//
//    if ( _xInit && _yInit )
//        createMatrix();
//}

//void MatrixVertex::createYIrregular ( NimbusEngine* nimbus )
//// *********************************************************
//{
//    if ( _yInit )
//        throw Error ("MatrixVertex::createYIrregular(): cannot initialize Y vector twice.");
//
//    GCell* gcell = nimbus->getGrid()->getLowerLeftCorner ( nimbus->getDepth() );
//    assert(gcell);
//    unsigned index = 0;
//    _linesIndexes.push_back ( pair<DbU::Unit,unsigned>(gcell->getYMin(),index++) );
//    while ( gcell ) {
//        _linesIndexes.push_back ( pair<DbU::Unit,unsigned>(gcell->getYMax(),index++) );
//        gcell = gcell->getUpOfMe();
//    }
//    assert ( is_sorted(_linesIndexes.begin(), _linesIndexes.end(), IndexComp()) );
//    _nbYTiles = index-1;
//    _yRegular = false;
//    _yInit    = true;
//
//    if ( _xInit && _yInit )
//        createMatrix();
//}

//void MatrixVertex::createMatrix()
//// ******************************
//{
//    assert ( _xInit && _yInit );
//    for ( unsigned j = 0 ; j < _nbYTiles ; j++ ) {
//        vector<Vertex*> vect;
//        for ( unsigned i = 0 ; i < _nbXTiles ; i++ ) {
//            Vertex* vertex (NULL);
//            vect.push_back ( vertex );
//        }
//        _matrix.push_back ( vect );
//    }
//}

void MatrixVertex::setVertex ( pair<unsigned int,unsigned int> indexes, Vertex* vertex )
// *************************************************************************************   
{
    _matrix[indexes.second][indexes.first] = vertex;
}

void MatrixVertex::setVertex ( Point point, Vertex* vertex )
// *********************************************************
{
    pair<unsigned int,unsigned int> indexes = getIJ ( point );
    setVertex ( indexes, vertex );
}

unsigned int MatrixVertex::getLineIndex ( DbU::Unit y )
// ***********************************************
{
    assert(_yInit );

    if ( _yRegular ) {
//       cerr << "y:" << DbU::getValueString(y-_boundingBox.getYMin()) << "/" << DbU::getValueString(_tileHeight)
//            << "=" << (DbU::getLambda(y-_boundingBox.getYMin())/_tileHeight)
//            << "<=>" << (unsigned int)floor((y-_boundingBox.getYMin())/_tileHeight) << endl;
      if ( (y < _boundingBox.getYMin()) or (y > _boundingBox.getYMax()) )
        throw Error ("MatrixVertex::getLineIndex(): search value (%s) is out of bounds [%s,%s]."
                    ,DbU::getValueString(y).c_str()
                    ,DbU::getValueString(_boundingBox.getYMin()).c_str()
                    ,DbU::getValueString(_boundingBox.getYMax()).c_str());

      unsigned int index = (unsigned int)floor((y-_boundingBox.getYMin())/_tileHeight);
      if ( y == _boundingBox.getYMax() ) --index;

      return index;
    }

    assert(is_sorted(_linesIndexes.begin(), _linesIndexes.end()));
    if ( _linesIndexes.empty() )
      throw Error ( "MatrixVertex::getLineIndex(): Indexes map is empty." );

    pair<pairIterator,pairIterator> result = equal_range (_linesIndexes.begin(), _linesIndexes.end()
                                                         , pair<DbU::Unit,unsigned>(y,0), MatrixVertex::IndexComp());
    if ( result.second == _linesIndexes.begin() )
        throw Error ("MatrixVertex::getLineIndex(): search value (%s) is lower than lowest bound (%s)."
                    ,DbU::getValueString(y).c_str()
                    ,DbU::getValueString((*_linesIndexes.begin()).first).c_str());
    if ( result.second == _linesIndexes.end() )
        throw Error ("MatrixVertex::getLineIndex(): search value (%s) is upper than uppest bound (%s)."
                    ,DbU::getValueString(y).c_str()
                    ,DbU::getValueString((*_linesIndexes.rbegin()).first).c_str());

    return ((*result.second).second-1);
}

unsigned int MatrixVertex::getColumnIndex ( DbU::Unit x )
// *************************************************
{
    assert(_xInit );

    if ( _xRegular ) {
//       cerr << "x:" << DbU::getValueString(x-DbU::lambda(_boundingBox.getXMin())) << "/" << _tileWidth << "=" << (DbU::getLambda(x-DbU::lambda(_lowerLeftX))/_tileWidth)
//            << "<=>" << (unsigned int)floor(DbU::getLambda(x-DbU::lambda(_boundingBox.getXMin()))/_tileWidth) << endl;
      if ( (x < _boundingBox.getXMin()) or (x > _boundingBox.getXMax()) )
          throw Error ("MatrixVertex::getColumnIndex(): search value (%s) is out of bounds [%s,%s]."
                      ,DbU::getValueString(x).c_str()
                      ,DbU::getValueString(_boundingBox.getXMin()).c_str()
                      ,DbU::getValueString(_boundingBox.getXMax()).c_str());

        unsigned int index = (unsigned int)floor((x-_boundingBox.getXMin())/_tileWidth);
        if ( x == _boundingBox.getXMax() ) --index;

        return index;
    }

    assert(is_sorted(_columnsIndexes.begin(),_columnsIndexes.end()));
    if ( _columnsIndexes.empty() )
      throw Error ( "MatrixVertex::getColumnIndex(): Indexes map is empty." );

    pair<pairIterator,pairIterator> result = equal_range (_columnsIndexes.begin(), _columnsIndexes.end()
                                                         , pair<DbU::Unit,unsigned>(x,0), MatrixVertex::IndexComp());
    if ( result.second == _columnsIndexes.begin() )
        throw Error ("MatrixVertex::getColumnIndex(): search value is lower than lowest bound.");
    if ( result.second == _columnsIndexes.end() )
        throw Error ("MatrixVertex::getColumnIndex(): search value is upper than uppest bound.");

    return ((*result.second).second-1);
}

pair<unsigned int,unsigned int> MatrixVertex::getIJ ( DbU::Unit x, DbU::Unit y )
// *******************************************************************
{
  return pair<unsigned int,unsigned int> (getColumnIndex(x),getLineIndex(y));
}

pair<unsigned int,unsigned int> MatrixVertex::getIJ ( const Point& point )
// ***********************************************************************
{
  return pair<unsigned int,unsigned int> (getColumnIndex(point.getX()),getLineIndex(point.getY()));
}

Vertex* MatrixVertex::getVertex ( pair<unsigned int,unsigned int> indexes )
// ************************************************************************
{
    return _matrix[indexes.second][indexes.first];
}

Vertex* MatrixVertex::getVertex ( Point point )
// ********************************************
{
    pair<unsigned int,unsigned int> indexes = getIJ ( point );
    return getVertex ( indexes );
}

Vertex* MatrixVertex::getVertex ( DbU::Unit x, DbU::Unit y )
// *********************************************************
{
    pair<unsigned int,unsigned int> indexes = getIJ ( x, y );
    return getVertex ( indexes );
}

Vertex* MatrixVertex::getVertexFromIndexes ( unsigned lineIdx, unsigned columnIdx )
// ********************************************************************************
{
    return _matrix[lineIdx][columnIdx];
}

void MatrixVertex::print()
// ***********************
{
    //cerr << "\'_linesIndexes\';" << endl;
    //for ( unsigned i = 0 ; i < _linesIndexes.size() ; ) {
    //    cerr << "\'<" << _linesIndexes[i].first << "," << _linesIndexes[i].second << ">\'";
    //    if ( ++i %10 )
    //        cerr << ",";
    //    else
    //        cerr << ";" << endl;
    //}
    //cerr << ";" << endl;
    //cerr << "\'_columnsIndexes\';" << endl;
    //for ( unsigned i = 0 ; i < _columnsIndexes.size() ; ) {
    //    cerr << "\'<" << _columnsIndexes[i].first << "," << _columnsIndexes[i].second << ">\'";
    //    if ( ++i %10 )
    //        cerr << ",";
    //    else
    //        cerr << ";" << endl;
    //}
    //cerr << ";" << endl;
    for ( unsigned j = 0 ; j < _nbYTiles ; j++ )
        for ( unsigned i = 0 ; i < _nbXTiles ; i++ )
            cerr << i << "," << j << " " << _matrix[j][i] << endl;
}

} // end namespace
