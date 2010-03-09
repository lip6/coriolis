#include "knik/VEdge.h"
#include "knik/Vertex.h"
#include "knik/Graph.h"

namespace Knik {

VEdge::VEdge ( Vertex* from, Vertex* to )
// **************************************
    : Inherit (from, to)
{
  DbU::Unit thickness  = DbU::lambda( (_realOccupancy>_capacity)?5.0:2.5 );

  Point fromPoint = getFrom()->getPosition();
  Point toPoint = getTo()->getPosition();

  _boundingBox = Box ( fromPoint.getX() - thickness, fromPoint.getY(), toPoint.getX() + thickness, toPoint.getY() );
}

VEdge::VEdge ( Vertex* from, Vertex* to, unsigned capacity )
// *********************************************************
    : Inherit (from, to, capacity)
{
  DbU::Unit thickness  = DbU::lambda( (_realOccupancy>_capacity)?5.0:2.5 );

  Point fromPoint = getFrom()->getPosition();
  Point toPoint = getTo()->getPosition();

  _boundingBox = Box ( fromPoint.getX() - thickness, fromPoint.getY(), toPoint.getX() + thickness, toPoint.getY() );
}

VEdge* VEdge::create ( Vertex* from, Vertex* to )
// **********************************************
{
    if ( !from || !to )
        throw Error ("VEdge::create(): cannot create VEdge with NULL vertex.");

    VEdge* vEdge = new VEdge ( from, to );

    vEdge->_postCreate(true);

    return vEdge;
}

VEdge* VEdge::create ( Vertex* from, Vertex* to, unsigned capacity )
// *****************************************************************
{
    if ( !from || !to )
        throw Error ("VEdge::create(): cannot create VEdge with NULL vertex.");

    VEdge* vEdge = new VEdge ( from, to, capacity );

    vEdge->_postCreate(false);

    return vEdge;
}

void VEdge::_postCreate( bool fenceCapacity )
// ******************************************
{
    Inherit::_postCreate ( fenceCapacity );
}

VEdge::~VEdge()
// **********
{
}

Box VEdge::computeBoundingBox() const
// **********************************
{
    DbU::Unit thickness  = DbU::lambda( (_realOccupancy>_capacity)?2.5:2.5 );

    Point fromPoint = getFrom()->getPosition();
    Point toPoint = getTo()->getPosition();

    return Box ( fromPoint.getX() - thickness, fromPoint.getY(), toPoint.getX() + thickness, toPoint.getY() );
}

Point  VEdge::getReferencePoint() const
// ************************************
{
  return Point ( getBoundingBox().getXMin(), getBoundingBox().getYMin() );
}

DbU::Unit  VEdge::getWidth() const
// *******************************
{
  return getBoundingBox().getWidth();
}

float VEdge::getNormalisedLength() const
// *************************************
{
    return _from->getRoutingGraph()->getVEdgeNormalisedLength();
}

//void VEdge::createSplitter ( Net* net )
//// ************************************
//{
//    if ( !_splitter )
//    {
//        DbU::Unit _size = DbU::lambda(3);
//        _splitter  = Splitter::createVertical ( net, _fence->getGrid()->getLayer(), _fence, DbU::lambda(2), _size, _size );
//        _fence->attachSplitter ( _splitter );
//        // when attaching a splitter : fence's occupancity is incremented. do the same for the edge
//        _realOccupancy++;
//    }
//}

//Hook* VEdge::getSplitterHook ( Vertex* vertex )
//// ********************************************
//{
//    assert ( _splitter );
//    assert ( (vertex == _from) || (vertex == _to) );
//    if ( vertex == _from )
//        return _splitter->getBottomHook();
//    else
//        return _splitter->getTopHook();
//}

} // end namespace
