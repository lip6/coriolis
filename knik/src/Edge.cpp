
#include "crlcore/Utilities.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Warning.h"

#include "knik/Edge.h"
#include "knik/Vertex.h"
#include "knik/Graph.h"

namespace Knik {

extern unsigned __congestion__;
extern unsigned __precongestion__;
extern bool __ripupMode__;

extern float __edge_cost__;

const Name  Edge::_extensionName = "Knik::Edge";

Edge::Edge ( Vertex* from, Vertex* to )
// ************************************
    : Inherit (from->getCell())
    , _from (from)
    , _nextFrom (NULL)
    , _to (to)
    , _nextTo (NULL)
    , _connexID (-1)
    , _cost (0.0)
    , _capacity (0)
    , _realOccupancy (0)
    , _estimateOccupancy (0.0)
    , _netStamp (0)
    , _segments()
{
    /*_cost = v1->getGCell()->getCenter().ManhattanDistance (v2->getGCell()->getCenter());*/
}

Edge::Edge ( Vertex* from, Vertex* to, unsigned capacity )
// *******************************************************
    : Inherit (from->getCell())
    , _boundingBox()
    , _from (from)
    , _nextFrom (NULL)
    , _to (to)
    , _nextTo (NULL)
    , _connexID (-1)
    , _cost (0.0)
    , _capacity (capacity)
    , _realOccupancy (0)
    , _estimateOccupancy (0.0)
    , _netStamp (0)
    , _isCongested (false)
    , _segments()
{
//cerr << "    Edge::Edge() capacity:" << _capacity << endl;
}

void Edge::_postCreate ( bool capacity )
// *************************************
{
    Inherit::_postCreate();

    _segments.reserve(_capacity);
}

Edge::~Edge()
// **********
{
}

void Edge::destroy()
// ****************
{
    _preDestroy();
    delete this;
}

void Edge::_preDestroy()
// ********************
{
    Inherit::_preDestroy();
}

void Edge::increaseCapacity ( int capacity )
// *****************************************
{
  if ( (int)_capacity + capacity < 0 ) _capacity = 0;
  else
    _capacity += capacity;

//if ( _capacity < 2 ) _capacity = 0;

  // cerr << "Increase Edge Capacity " << _from->getPosition()
  //      << " to " << _to->getPosition() << ":" << _capacity << endl;

  if ( _capacity == 0 )
    ltrace(300) << Warning("%s has reached NULL capacity.",getString(this).c_str()) << endl;
}

void Edge::incOccupancy ()
// ***********************
{
    _realOccupancy++;
    if ( !_isCongested ) {
        if ( _realOccupancy > _capacity ) {
            _boundingBox = computeBoundingBox();
            _isCongested = true;
            // reste a mettre les segments traversant l'edge dans la pile ou s'ils y sont deja a les mettre a jour
        }
    }

}

 void Edge::decOccupancy ()
// ************************
{
    if ( _realOccupancy > 0 )
        _realOccupancy--;
    if ( _isCongested && (_realOccupancy <= _capacity) ) {
        _boundingBox = computeBoundingBox();
        _isCongested = false;
    }
}

void Edge::removeSegment ( Segment* segment )
// ******************************************
{
    assert(segment);
    decOccupancy();
    vector<Segment*>::iterator vsit = find(_segments.begin(),_segments.end(),segment);
    if ( vsit != _segments.end() )
        _segments.erase ( vsit );
    else {
        cerr << segment << "  " << this << endl;
        Breakpoint::stop(0, "Shootdown -h now");
        throw Error ( "Edge::removeSegment(): I cannot believe this segment is not in _segments!" );
    }
}

void Edge::addSubEstimateOccupancy ( float increment, bool add )
// *************************************************************
{
    if ( add )
        _estimateOccupancy += increment;
    else
        _estimateOccupancy -= increment;

    return;
}

DbU::Unit Edge::getXTo() const
// ***************************
{
    return _to->getX();
}

DbU::Unit Edge::getYTo() const
// ***************************
{
    return _to->getY();
}

DbU::Unit Edge::getXFrom() const
// *****************************
{
    return _from->getX();
}

DbU::Unit Edge::getYFrom() const
// *****************************
{
    return _from->getY();
}

void Edge::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
// **************************************************************
{
    _from->translate ( dx, dy );
    _to->translate ( dx, dy );
}

Cell* Edge::getCell() const
// ************************
{
    return _from->getCell();
}

float Edge::getCost ( Edge* arrivalEdge )
// **************************************
{
// 20/10/2010: Check for null capacity, which may occurs after back-annotation
// by Kite.
  if ( _capacity == 0.0 ) return (float)(HUGE);

//#ifdef __USE_CONGESTION__
    if ( __congestion__ ) {
        // definition de la fonction de cout :
        // on ramene l'occupation de l'edge à un pourcentage de la capacité :
        float edge_capacity  = (float)1.0;
        float edge_occupancy;
        //float h = 9.0;
        //float k = 30.0; // environ 3*h pour etre sur d'avoir cost(0) = 1 !!
        //now defined as edge's attributes for ripup & reroute

        if ( !__ripupMode__ ) {
            //#if defined ( __USE_STATIC_PRECONGESTION__ ) || defined ( __USE_DYNAMIC_PRECONGESTION__ )
            if ( __precongestion__ )
                edge_occupancy = ((float)getRealOccupancy() + getEstimateOccupancy()) / (float)_capacity;
            //#else
            else
                edge_occupancy = (float)getRealOccupancy() / (float)_capacity;
            //#endif
            _cost = 1.0 + (9.0 / (1.0 + exp(-30.0 * (edge_occupancy - edge_capacity)))); // plutot que 1.0 on devrait avoir un getLength renvoyant la longueur de l'arete en unités normalisées (pas de grille)
        }
        else {
            // dans ce mode l'estimation de congestion est utilisé comme historique de congestion
            edge_occupancy = (float)getRealOccupancy() / (float)_capacity;
            float historicCost;
            if ( edge_occupancy < 1 )
                historicCost = getEstimateOccupancy() * edge_occupancy;
            else
                historicCost = getEstimateOccupancy() * exp(log(8)*(edge_occupancy - edge_capacity));
            _cost = 1.0 + (19.0 / (1.0 + exp(-60.0 * (edge_occupancy - edge_capacity)))) + historicCost;
        }
    }
//#else
    else
        _cost = 1.0;
//#endif
    // Prise en compte des vias !
    if ( arrivalEdge ) {
        if ( arrivalEdge->isVertical() && isHorizontal() )
            _cost += __edge_cost__;
        if ( arrivalEdge->isHorizontal() && isVertical() )
            _cost += __edge_cost__;
    }

    //if ( _from->getRoutingGraph()->getRipupMode() )
    //   if ( _isCongested ) 
    //       _cost += 1000;

    return _cost;
}

unsigned Edge::getRealOccupancy() const
// ************************************
{
    // pour debug plus facile :   // Attention ce n'est plus vrai lorsque l'edge gère elle même son occupation!
    //assert ( _realOccupancy == _fence->getOccupancy() );

    return _realOccupancy;
}

Segment* Edge::getSegmentFor ( Net* net )
// **************************************
{
   for_each_segment ( segment, getCollection ( _segments ) ) {
        if ( segment->getNet() == net )
            return segment;
        end_for;
   } 
   return NULL;
}

bool Edge::hasInfo() const
// ***********************
{
   return (_netStamp == _from->getRoutingGraph()->getNetStamp())&&(_connexID != -1);
}

// void Edge::_Draw ( View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation )
// //******************************************************************************************************************
// {
//     Point fromPos = transformation.getPoint ( _from->getPosition() );
//     Point toPos   = transformation.getPoint ( _to->getPosition() );
//     //view->DrawLine ( fromPos, toPos );
//     // new version to represent occupation
//     DbU::Unit halfWidth  = _from->getHalfWidth()  < _to->getHalfWidth()  ? _from->getHalfWidth()  : _to->getHalfWidth();
//     DbU::Unit halfHeight = _from->getHalfHeight() < _to->getHalfHeight() ? _from->getHalfHeight() : _to->getHalfHeight();
//     bool  overCap = false;
//     float relOccupancy = 0.0;
//     if ( _realOccupancy > _capacity ) {
//         overCap = true;
//         relOccupancy = 1.0;
//     }
//     else
//         relOccupancy = (float)_realOccupancy / (float)_capacity;
//     if ( fromPos.getY() == toPos.getY() ) {
//         DbU::Unit medFromX = fromPos.getX() + halfWidth;
//         DbU::Unit medFromY = fromPos.getY() - halfHeight;
//         DbU::Unit medToX   = fromPos.getX() + halfWidth;
//         DbU::Unit medToY   = fromPos.getY() + halfHeight;
//         view->DrawLine ( medFromX, medFromY, medToX, medToY );
//         DbU::Unit cover = DbU::lambda ( getValue(medToY - medFromY) * relOccupancy );
//         view->FillRectangle ( medFromX - 100, medFromY          , medFromX + 100, medFromY + (cover/2) );
//         view->FillRectangle ( medToX - 100  , medToY - (cover/2), medToX + 100  , medToY               );
//         if (overCap)
//             view->FillRectangle ( medFromX-150, medFromY+halfHeight-150, medFromX+150, medFromY+halfHeight+150 );
//     }
//     else {
//         DbU::Unit medFromX = fromPos.getX() - halfWidth;
//         DbU::Unit medFromY = fromPos.getY() + halfHeight;
//         DbU::Unit medToX   = fromPos.getX() + halfWidth;
//         DbU::Unit medToY   = fromPos.getY() + halfHeight;
//         view->DrawLine ( medFromX, medFromY, medToX, medToY );
//         DbU::Unit cover = DbU::lambda ( getValue(medToX - medFromX) * relOccupancy );
//         view->FillRectangle ( medFromX          , medFromY - 100, medFromX + (cover/2), medFromY + 100 );
//         view->FillRectangle ( medToX - (cover/2), medToY - 100  , medToX              , medToY + 100   );
//         if (overCap)
//             view->FillRectangle ( medFromX+halfWidth-150, medFromY-150, medFromX+halfWidth+150, medFromY+150 );
//     }
//     // old version
//     //if ( _estimateOccupancy > 0 )
//     //    view->DrawString ( getString(_estimateOccupancy), (fromPos.getX()+toPos.getX())/2 , (fromPos.getY()+toPos.getY())/2 );
//     if ( (_netStamp == _from->getRoutingGraph()->getNetStamp()) && (_connexID != -1) ) {
//         view->DrawString ( getString(_connexID), (fromPos.getX()+toPos.getX())/2 , (fromPos.getY()+toPos.getY())/2+10 );
//     }
//     //if ( _nextTo ) {
//     //    Point nextToFromPos = transformation.getPoint ( _nextTo->getFrom()->getPosition() );
//     //    Point nextToToPos   = transformation.getPoint ( _nextTo->getTo()->getPosition() );
//     //    DbU::Unit origX = fromPos.getX() + (toPos.getX() - fromPos.getX())/3;
//     //    DbU::Unit origY = fromPos.getY() + (toPos.getY() - fromPos.getY())/3;
//     //    DbU::Unit destX = nextToFromPos.getX() + (nextToToPos.getX() - nextToFromPos.getX())/3;
//     //    DbU::Unit destY = nextToFromPos.getY() + (nextToToPos.getY() - nextToFromPos.getY())/3;
//     //    view->DrawLine ( origX, origY, destX, destY );
//     //    view->DrawCircle ( destX, destY, DbU::lambda ( 3 ) );
//     //}
//     //if ( _nextFrom ) {
//     //    Point nextFromFromPos = transformation.getPoint ( _nextFrom->getFrom()->getPosition() );
//     //    Point nextFromToPos   = transformation.getPoint ( _nextFrom->getTo()->getPosition() );
//     //    DbU::Unit origX = fromPos.getX() + (toPos.getX() - fromPos.getX())*2/3;
//     //    DbU::Unit origY = fromPos.getY() + (toPos.getY() - fromPos.getY())*2/3;
//     //    DbU::Unit destX = nextFromFromPos.getX() + (nextFromToPos.getX() - nextFromFromPos.getX())*2/3;
//     //    DbU::Unit destY = nextFromFromPos.getY() + (nextFromToPos.getY() - nextFromFromPos.getY())*2/3;
//     //    view->DrawLine ( origX, origY, destX, destY );
//     //    view->FillCircle ( destX, destY, DbU::lambda ( 3 ) );
//     //}
// }

// void Edge::_Highlight ( View* view, const Box& updateArea, const Transformation& transformation )
// // **********************************************************************************************
// {
//     view->DrawLine ( transformation.getPoint ( _from->getPosition() )
//                    , transformation.getPoint (   _to->getPosition() ) );
// }

// bool Edge::_IsInterceptedBy ( View* view, const Point& point, const DbU::Unit& aperture ) const
// // ***************************************************************************************
// {
//     Box area ( point );
//     area.inflate(aperture);

//     return getBoundingBox().intersect ( area );
// }

void Edge::invalidate ( bool propagateFlag )
// *****************************************
{
    Inherit::invalidate ( false );
}

string Edge::_getString() const
// ****************************
{
    return "<" + _TName ( "Edge" )
         + ":" + getString ( _connexID )
         + " " + getString ( _netStamp )
         + " " + getString ( _from )
         + " " + getString ( _to ) + ">";
}

Record* Edge::_getRecord() const
// ***********************
{
    Record* record = Inherit::_getRecord();

    if ( !record )
        record = new Record ( getString ( this ) );

    record->add ( getSlot ( "from"     , _from      ) );
    record->add ( getSlot ( "to"       , _to        ) );
    record->add ( getSlot ( "nextFrom" , _nextFrom  ) );
    record->add ( getSlot ( "nextTo"   , _nextTo    ) );
    record->add ( getSlot ( "connexID" , _connexID  ) );
    record->add ( getSlot ( "cost"     , _cost      ) );
    record->add ( getSlot ( "netStamp" , _netStamp  ) );
    record->add ( getSlot ( "capacity" , _capacity  ) );
    record->add ( getSlot ( "occupancy", _realOccupancy ) );
    record->add ( getSlot ( "estimate occupancy", _estimateOccupancy ) );
    record->add ( getSlot ( "segments" , &_segments ) );

    return record;
}

} // end namespace
