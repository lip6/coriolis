#include "knik/HEdge.h"
#include "knik/Vertex.h"
#include "knik/Graph.h"

namespace Knik {

HEdge::HEdge ( Vertex* from, Vertex* to )
// **************************************
    : Inherit (from, to)
{
  DbU::Unit thickness  = DbU::lambda( (_realOccupancy>_capacity)?5.0:2.5 );

  Point fromPoint = getFrom()->getPosition();
  Point toPoint = getTo()->getPosition();

  _boundingBox = Box ( fromPoint.getX(), fromPoint.getY() - thickness, toPoint.getX(), toPoint.getY() + thickness );
}

HEdge::HEdge ( Vertex* from, Vertex* to, unsigned capacity )
// *********************************************************
    : Inherit (from, to, capacity)
{
  DbU::Unit thickness  = DbU::lambda( (_realOccupancy>_capacity)?5.0:2.5 );

  Point fromPoint = getFrom()->getPosition();
  Point toPoint = getTo()->getPosition();

  _boundingBox = Box ( fromPoint.getX(), fromPoint.getY() - thickness, toPoint.getX(), toPoint.getY() + thickness );
}

HEdge* HEdge::create ( Vertex* from, Vertex* to )
// **********************************************
{
    if ( !from || !to )
        throw Error ("HEdge::create(): cannot create HEdge with NULL vertex.");

    HEdge* hEdge = new HEdge ( from, to );

    hEdge->_postCreate(true);

    return hEdge;
}

HEdge* HEdge::create ( Vertex* from, Vertex* to, unsigned capacity )
// *****************************************************************
{
    if ( !from || !to )
        throw Error ("HEdge::create(): cannot create HEdge with NULL vertex.");

    HEdge* hEdge = new HEdge ( from, to, capacity );

    hEdge->_postCreate(false);

    return hEdge;
}

void HEdge::_postCreate ( bool fenceCapacity )
// *******************************************
{
    Inherit::_postCreate ( fenceCapacity );
}

HEdge::~HEdge()
// **********
{
}

Box  HEdge::computeBoundingBox() const
// ***********************************
{
  DbU::Unit thickness  = DbU::lambda( (_realOccupancy>_capacity)?2.5:2.5 );

  Point fromPoint = getFrom()->getPosition();
  Point toPoint = getTo()->getPosition();

  return Box ( fromPoint.getX(), fromPoint.getY() - thickness, toPoint.getX(), toPoint.getY() + thickness );
}

Point  HEdge::getReferencePoint() const
// ************************************
{
  return Point ( getBoundingBox().getXMax(), getBoundingBox().getYMin() );
}

DbU::Unit  HEdge::getWidth() const
// *******************************
{
  return getBoundingBox().getHeight();
}

float HEdge::getNormalisedLength() const
// *************************************
{
    return _from->getRoutingGraph()->getHEdgeNormalisedLength();
}

//void HEdge::createSplitter ( Net* net )
//// ************************************
//{
//    if ( !_splitter )
//    {
//        DbU::Unit _size = DbU::lambda(3);
//        _splitter  = Splitter::createHorizontal ( net, _fence->getGrid()->getLayer(), _fence, DbU::lambda(2), _size, _size );
//        _fence->attachSplitter ( _splitter );
//        // when attaching a splitter : fence's occupancity is incremented. do the same for the edge
//        _realOccupancy++;
//    }
//}

//Hook* HEdge::getSplitterHook ( Vertex* vertex )
//// ********************************************
//{
//    assert ( _splitter );
//    assert ( (vertex == _from) || (vertex == _to) );
//    if ( vertex == _from )
//        return _splitter->getLeftHook();
//    else
//        return _splitter->getRightHook();
//}

} // end namespace
