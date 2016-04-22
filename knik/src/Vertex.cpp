#include <cmath>
#include <algorithm>
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "knik/Vertex.h"
#include "knik/Edge.h"
#include "knik/Graph.h"

namespace {
using namespace Knik;
struct EdgeToYComp : public binary_function<const Edge*, const Edge*, bool> { 
public:
    bool operator() ( Edge* edge1, Edge* edge2 ) const
    {
        return ( edge1->getTo()->getY() < edge2->getTo()->getY() ); 
    }
};
struct EdgeToXComp : public binary_function<const Edge*, const Edge*, bool> { 
public:
    bool operator() ( Edge* edge1, Edge* edge2 ) const
    {
        return ( edge1->getTo()->getX() > edge2->getTo()->getX() ); 
    }
};
struct EdgeFromYComp : public binary_function<const Edge*, const Edge*, bool> { 
public:
    bool operator() ( Edge* edge1, Edge* edge2 ) const
    {
        return ( edge1->getFrom()->getY() > edge2->getFrom()->getY() ); 
    }
};
struct EdgeFromXComp : public binary_function<const Edge*, const Edge*, bool> { 
public:
    bool operator() ( Edge* edge1, Edge* edge2 ) const
    {
        return ( edge1->getFrom()->getX() <  edge2->getFrom()->getX() ); 
    }
};
}

namespace Knik {

extern unsigned __congestion__;
extern unsigned __precongestion__;

  const Name Vertex::_extensionName = "Knik::Vertex";

// ********************************************************************
// VERTEX_Edges declaration
// ********************************************************************
class VERTEX_Edges : public Collection<Edge*> {
    public:
        typedef Collection<Edge*> Inherit;
    public:
        class Locator : public Hurricane::Locator<Edge*> {
                public:
                    typedef Hurricane::Locator<Edge*> Inherit;

                private:
                    const Vertex* _vertex;
                          Edge*   _edge;
                          int     _direction;

                public:
                    Locator(const Vertex* vertex = NULL, Edge* edge = NULL, int direction = 0);
                    Locator(const Locator& locator);

                    Locator& operator=(const Locator& locator);

                    virtual Edge* getElement() const;
                    virtual Hurricane::Locator<Edge*>* getClone() const;

                    virtual bool isValid() const;

                    virtual void progress();

                    virtual string _getString() const;
            };

    private:
        const Vertex* _vertex;
              Edge*   _edge;
              int     _direction;

    public: 
        VERTEX_Edges(const Vertex* vertex);
        VERTEX_Edges(const VERTEX_Edges& edges);

        VERTEX_Edges& operator=(const VERTEX_Edges& edges);
        virtual Collection<Edge*>* getClone() const;
        virtual Hurricane::Locator<Edge*>* getLocator() const;
        virtual string _getString() const;
};


Vertex::Vertex ( Graph* routingGraph, Point position, DbU::Unit halfWidth, DbU::Unit halfHeight )
// **********************************************************************************************
    : Inherit (routingGraph->getCell())
    , _routingGraph (routingGraph)
    , _predecessor (NULL)
    , _contact (NULL)
    , _position (position)
    , _vtuple (NULL)
    , _distance ((float)(HUGE_VAL))
    , _connexID (-1)
    , _netStamp (0)
    , _halfWidth  (halfWidth)
    , _halfHeight (halfHeight)
    , _flags(0)
{
    _firstEdges[0] = NULL;
    _firstEdges[1] = NULL;
    _firstEdges[2] = NULL;
    _firstEdges[3] = NULL;
}

Vertex* Vertex::create ( Graph* routingGraph, Point position, DbU::Unit halfWidth, DbU::Unit halfHeight )
// *******************************************************************************************************
{
    Vertex* vertex = new Vertex ( routingGraph, position, halfWidth, halfHeight );

    vertex->_postCreate();

    return vertex;
}

void Vertex::_postCreate()
// ***********************
{
    Inherit::_postCreate();
}

Vertex::~Vertex()
// **************
{
}

void Vertex::destroy()
// ******************
{
    _preDestroy();
    delete this;
}

void Vertex::_preDestroy()
// **********************
{
    Inherit::_preDestroy();
}
 
void Vertex::attachToLocalRing ( Component* component )
// ****************************************************
{
    // XXX plus utilisée ? XXX
    //assert ( component );

    //Hook* hook = component->getBodyHook();
    //hook->detach();
    //if ( Hook* previousHook = getLocalRingHook() ) {
    //    //cerr << "Vertex::attachToLocalRingHook : going to attach " << previousHook << " to " << hook << endl;
    //    hook->attach ( previousHook );
    //}
    //setLocalRingHook ( hook );
}

void Vertex::sortEdges()
// *********************
{
    vector<Edge*> hEdgesOut;
    vector<Edge*> vEdgesOut;
    vector<Edge*> hEdgesIn;
    vector<Edge*> vEdgesIn;

    Edge* currentEdge;
    //ltracein(425);
    if ( (currentEdge = getHEdgeOut()) ) {  // si pas de HEdgeOut : rien a faire ^^
        while ( currentEdge ) {
            //ltrace(425) << "Pushing_back edge " << getString(currentEdge) << endl;
            hEdgesOut.push_back ( currentEdge );
            currentEdge = currentEdge->getNextFrom();
        }
        //ltrace(425) << "sorting vector..." << endl;
        sort ( hEdgesOut.begin(), hEdgesOut.end(), EdgeToYComp() );
        //ltrace(425) << "setting " << getString(hEdgesOut[0]) << " as hEdgeOut for vertex " << getString(this) << endl;
        setHEdgeOut ( hEdgesOut[0] );
        for ( unsigned i = 1 ; i < hEdgesOut.size() ; i++ ) {
            //ltrace(425) << "next : " << getString(hEdgesOut[i]) << endl;
            hEdgesOut[i-1]->setNextFrom ( hEdgesOut[i] );
        }
        hEdgesOut[hEdgesOut.size()-1]->setNextFrom ( NULL ); // si jamais avant en milieu de chaine ...
    }
    //ltraceout(425);
    if ( (currentEdge = getVEdgeOut()) ) {
        while ( currentEdge ) {
            vEdgesOut.push_back ( currentEdge );
            currentEdge = currentEdge->getNextFrom();
        }
        sort ( vEdgesOut.begin(), vEdgesOut.end(), EdgeToXComp() );
        setVEdgeOut ( vEdgesOut[0]);
        for ( unsigned i = 1 ; i < vEdgesOut.size() ; i++ ) {
            vEdgesOut[i-1]->setNextFrom ( vEdgesOut[i] );
        }
        vEdgesOut[vEdgesOut.size()-1]->setNextFrom ( NULL );
    }
    if ( (currentEdge = getHEdgeIn()) ) {
        while ( currentEdge ) {
            hEdgesIn.push_back ( currentEdge );
            currentEdge = currentEdge->getNextTo();
        }
        sort ( hEdgesIn.begin(), hEdgesIn.end(), EdgeFromYComp() );
        setHEdgeIn ( hEdgesIn[0] );
        for ( unsigned i = 1 ; i < hEdgesIn.size() ; i++ ) {
            hEdgesIn[i-1]->setNextTo ( hEdgesIn[i] );
        }
        hEdgesIn[hEdgesIn.size()-1]->setNextTo ( NULL );
    }
    if ( (currentEdge = getVEdgeIn()) ) {
        while ( currentEdge ) {
            vEdgesIn.push_back ( currentEdge );
            currentEdge = currentEdge->getNextTo();
        }
        sort ( vEdgesIn.begin(), vEdgesIn.end(), EdgeFromXComp() );
        setVEdgeIn ( vEdgesIn[0] );
        for ( unsigned i = 1 ; i < vEdgesIn.size() ; i++ ) {
            vEdgesIn[i-1]->setNextTo ( vEdgesIn[i] );
        }
        vEdgesIn[vEdgesIn.size()-1]->setNextTo ( NULL );
    }
    
}

Edges Vertex::getAdjacentEdges() const
// ***********************************
{
    return VERTEX_Edges ( this );
}

Contact* Vertex::getContact ( Edge* arrivalEdge )
// **********************************************
{

    if ( _contact ) {
        // il y deja un contact associe au vertex   XXX on verifie que le contact appartient bien au workingNet du routingGraph XXX
        if ( _routingGraph->getWorkingNet() == _contact->getNet() )
            return _contact;
        _contact = NULL;
    }

    //assert ( arrivalEdge ); // ca change si on se decide quele vertex pointe sur le contact et pas le hook d'un routingPad ou autre
    if ( arrivalEdge ) {
        int arrivalConnexID = arrivalEdge->getConnexID();
        bool createContact = false;
        bool straightOn = false;
        if ( arrivalEdge->isVertical() ) {
            if ( _firstEdges[0] && ( _firstEdges[0]->getConnexID() == arrivalConnexID ) && ( _firstEdges[0]->getNetStamp() == _netStamp ) )
                createContact = true;
            else if ( _firstEdges[2] && ( _firstEdges[2]->getConnexID() == arrivalConnexID ) && ( _firstEdges[2]->getNetStamp() == _netStamp ) )
                createContact = true;
            else if (  ( _firstEdges[1] == arrivalEdge )
                    && ( _firstEdges[3] && ( _firstEdges[3]->getConnexID() == arrivalConnexID ) && ( _firstEdges[3]->getNetStamp() == _netStamp ) ) )
                straightOn = true;
            else if (  ( _firstEdges[3] == arrivalEdge )
                    && ( _firstEdges[1] && ( _firstEdges[1]->getConnexID() == arrivalConnexID ) && ( _firstEdges[1]->getNetStamp() == _netStamp ) ) )
                straightOn = true;
            else
                throw Error ( "Vertex::getContact(): problem with edges configuration *1." );
        }
        else {
            if ( _firstEdges[1] && ( _firstEdges[1]->getConnexID() == arrivalConnexID ) && ( _firstEdges[1]->getNetStamp() == _netStamp ) )
                createContact = true;
            else if ( _firstEdges[3] && ( _firstEdges[3]->getConnexID() == arrivalConnexID ) && ( _firstEdges[3]->getNetStamp() == _netStamp ) )
                createContact = true;
            else if (  ( _firstEdges[0] == arrivalEdge )
                    && ( _firstEdges[2] && ( _firstEdges[2]->getConnexID() == arrivalConnexID ) && ( _firstEdges[2]->getNetStamp() == _netStamp ) ) )
                straightOn = true;
            else if (  ( _firstEdges[2] == arrivalEdge )
                    && ( _firstEdges[0] && ( _firstEdges[0]->getConnexID() == arrivalConnexID ) && ( _firstEdges[0]->getNetStamp() == _netStamp ) ) )
                straightOn = true;
            else
                throw Error ( "Vertex::getContact(): problem with Edges configuration *2." );
        }
        if ( createContact ) {
            Contact* contact = Contact::create ( _routingGraph->getWorkingNet()
                                             //, getLayer(1)
                                               , DataBase::getDB()->getTechnology()->getLayer("metal2")
                                               , _position.getX()
                                               , _position.getY()
                                               , _halfWidth/5
                                               , _halfHeight/5
                                               );
            // on a besoin de tester si par routage précédent il n'y a pas un segment du net qui traverse le vertex ....
            Segment* segment0 = NULL;
            Segment* segment1 = NULL;
            Segment* segment2 = NULL;
            Segment* segment3 = NULL;
            if ( _firstEdges[0] )
                segment0 = _firstEdges[0]->getSegmentFor ( _routingGraph->getWorkingNet() );
            if ( _firstEdges[1] )
                segment1 = _firstEdges[1]->getSegmentFor ( _routingGraph->getWorkingNet() );
            if ( _firstEdges[2] )
                segment2 = _firstEdges[2]->getSegmentFor ( _routingGraph->getWorkingNet() );
            if ( _firstEdges[3] )
                segment3 = _firstEdges[3]->getSegmentFor ( _routingGraph->getWorkingNet() );

            if ( (segment0 != NULL) && (segment0 == segment2) ) {
                if ( !dynamic_cast<Contact*>(segment0->getSource()) )    throw Error ("Vertex::getContact(): does not consider Segment not based on Contact (source).");
                if ( !dynamic_cast<Contact*>(segment0->getTarget()) )    throw Error ("Vertex::getContact(): does not consider Segment not based on Contact (target).");
                Contact* targetContact = static_cast<Contact*>(segment0->getTarget());
                _routingGraph->removeSegment(segment0);
                segment0->getTargetHook()->detach();
                segment0->getTargetHook()->attach(contact->getBodyHook());
                _routingGraph->insertSegment(segment0);
                Segment* newSeg = _routingGraph->createSegment(contact, targetContact);
                _routingGraph->insertSegment(newSeg);
            }
            if ( (segment1 != NULL) && (segment1 == segment3) ) {
                if ( !dynamic_cast<Contact*>(segment1->getSource()) )    throw Error ("Vertex::getContact(): does not consider Segment not based on Contact (Source).");
                if ( !dynamic_cast<Contact*>(segment1->getTarget()) )    throw Error ("Vertex::getContact(): does not consider Segment not based on Contact (Target).");
                Contact* targetContact = static_cast<Contact*>(segment1->getTarget());
                _routingGraph->removeSegment(segment1);
                segment1->getTargetHook()->detach();
                segment1->getTargetHook()->attach(contact->getBodyHook());
                _routingGraph->insertSegment(segment1);
                Segment* newSeg = _routingGraph->createSegment(contact, targetContact);
                _routingGraph->insertSegment(newSeg);
            }
            return contact;
        }
        if ( straightOn )
            return NULL;
    }

    throw Error ( "Vertex::getContact(): what are you doing here O_o?" );
}

Edge* Vertex::getHEdgeLeadingTo ( Vertex* to )
// *******************************************
{
    Edge* edge = getHEdgeOut();
    while ( edge ) {
        if ( edge->getTo() == to )
            break;
        edge = edge->getNextFrom();
    }
    return edge;
}

Edge* Vertex::getVEdgeLeadingTo ( Vertex* to )
// *******************************************
{
    Edge* edge = getVEdgeOut();
    while ( edge ) {
        if ( edge->getTo() == to ) 
            break;
        edge = edge->getNextFrom();
    }
    return edge;
}

Edge* Vertex::getHEdgeComingFrom ( Vertex* from )
// **********************************************
{
    Edge* edge = getHEdgeIn();
    while ( edge ) {
        if ( edge->getFrom() == from ) 
            break;
        edge = edge->getNextTo();
    }
    return edge;
}

Edge* Vertex::getVEdgeComingFrom ( Vertex* from )
// **********************************************
{
    Edge* edge = getVEdgeIn();
    while ( edge ) {
        if ( edge->getFrom() == from ) 
            break;
        edge = edge->getNextTo();
    }
    return edge;
}

Edge* Vertex::getBestHEdgeOut ( DbU::Unit yDest )
// *****************************************
{
    Edge* bestEdge    = getHEdgeOut();
    Edge* currentEdge = bestEdge;

    while ( (currentEdge = currentEdge->getNextFrom()) ) {
        if ( abs(currentEdge->getTo()->getY() - yDest) < abs(bestEdge->getTo()->getY() - yDest) )
            bestEdge = currentEdge;
    }

    return bestEdge;
}

Edge* Vertex::getBestVEdgeOut ( DbU::Unit xDest )
// *****************************************
{
    Edge* bestEdge    = getVEdgeOut();
    Edge* currentEdge = bestEdge;

    while ( (currentEdge = currentEdge->getNextFrom()) ) {
        if ( abs(currentEdge->getTo()->getX() - xDest) < abs(bestEdge->getTo()->getX() - xDest) )
            bestEdge = currentEdge;
    }

    return bestEdge;
}

Edge* Vertex::getBestHEdgeIn ( DbU::Unit yDest )
// ****************************************
{
    Edge* bestEdge    = getHEdgeIn();
    Edge* currentEdge = bestEdge;

    while ( (currentEdge = currentEdge->getNextTo()) ) {
        if ( abs(currentEdge->getFrom()->getY() - yDest) < abs(bestEdge->getFrom()->getY() - yDest) )
            bestEdge = currentEdge;
    }

    return bestEdge;
}

Edge* Vertex::getBestVEdgeIn ( DbU::Unit xDest )
// ****************************************
{
    Edge* bestEdge    = getVEdgeIn();
    Edge* currentEdge = bestEdge;

    while ( (currentEdge = currentEdge->getNextTo()) ) {
        if ( abs(currentEdge->getFrom()->getX() - xDest) < abs(bestEdge->getFrom()->getX() - xDest) )
            bestEdge = currentEdge;
    }

    return bestEdge;
}

bool Vertex::hasInfo() const
// *************************
{
   return (_netStamp == _routingGraph->getNetStamp());
}

void Vertex::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
// ****************************************************************
{
    _position.translate ( dx, dy );
}

Cell* Vertex::getCell() const
// **************************
{
    return _routingGraph->getCell();
}

// void Vertex::_Draw ( View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation )
// // *******************************************************************************************************************
// {
//     DbU::Unit size = DbU::lambda(5);
//     view->FillCircle ( transformation.getPoint ( getPosition() ), size );
//     //if ( _distance < (float)(HUGE_VAL) )
//     if ( _netStamp == _routingGraph->getNetStamp() &&  _distance < (float)(HUGE_VAL) )
//         view->DrawString ( getString(_distance), getXMax() - (_halfWidth), getYMax() - (_halfHeight/2) );
//     if ( _connexID != -1 )
//         view->DrawString ( getString(_connexID)+","+getString(_netStamp), getXMax() - (_halfWidth), getYMin() + (_halfHeight/2) );
//     //if ( _predecessor ) {
//     //    Vertex* oppositeVertex = _predecessor->getOpposite ( this );
//     //    Point position = transformation.getPoint ( _position );
//     //    Point oppPos   = transformation.getPoint ( oppositeVertex->getPosition() );
//     //    DbU::Unit  origX = position.getX();
//     //    DbU::Unit  origY = position.getY();
//     //    DbU::Unit  destX = position.getX() + (4*(oppPos.getX() - position.getX()))/5;
//     //    DbU::Unit  destY = position.getY() + (4*(oppPos.getY() - position.getY()))/5;
//     //    view->DrawLine ( origX, origY, destX, destY );
//     //}
// }

// void Vertex::_Highlight ( View* view, const Box& updateArea, const Transformation& transformation )
// // ************************************************************************************************
// {
//     DbU::Unit size = DbU::lambda(5);
//     view->FillCircle ( transformation.getPoint ( getPosition() ), size );
// }

// bool Vertex::_IsInterceptedBy ( View* view, const Point& point, const DbU::Unit& aperture ) const
// // *****************************************************************************************
// {
//     Box area ( point );
//     area.inflate(aperture);

//     return getBoundingBox().intersect ( area );
// //    return false;
// }

void Vertex::invalidate ( bool propagateFlag )
// *******************************************
{
    Inherit::invalidate ( false );

    // Invalider toutes les Edges associées ?
}

string Vertex::_getString() const
// ******************************
{
    return "<" + _TName ( "Vertex" )
         + " id:" + getString ( _connexID )
         + " s:"  + getString ( _netStamp )
         + " "    + getString ( DbU::getValueString(_position.getX()) )
         + " "    + getString ( DbU::getValueString(_position.getY()) ) + ">";
}

Record* Vertex::_getRecord() const
// *************************
{
    Record* record = Inherit::_getRecord();

    if ( !record )
        record = new Record ( getString ( this ) );

    record->add ( getSlot ( "hEdgeOut", getHEdgeOut() ) );
    record->add ( getSlot ( "vEdgeOut", getVEdgeOut() ) );
    record->add ( getSlot ( "hEdgeIn", getHEdgeIn() ) );
    record->add ( getSlot ( "vEdgeIn", getVEdgeIn() ) );
    record->add ( getSlot ( "predecessor", _predecessor ) );
    //if ( _localRingHook)  record->add ( getSlot ( "localRingHook", _localRingHook ) );
    record->add ( getSlot ( "contact", _contact ) );
    record->add ( getSlot ( "connexID", _connexID ) );
    record->add ( getSlot ( "position", _position ) );
    record->add ( getSlot ( "distance", _distance ) );
    record->add ( getSlot ( "netStamp", _netStamp ) );
    record->add ( getSlot ( "halfWidth", _halfWidth ) );
    record->add ( getSlot ( "halfHeight", _halfHeight ) );

    return record;
}


// ************************************************************************************************
// VERTEX_Edges implementation
// ************************************************************************************************
VERTEX_Edges::VERTEX_Edges ( const Vertex* vertex )
// ************************************************
    : Inherit()
    , _vertex(vertex)
    , _edge(NULL)
    , _direction(0)
{
    // _direction represents one of the four direction in the routing graph. these ditections are set as follow :
    // 0 = East
    // 1 = North
    // 2 = West
    // 3 = South
    for ( _direction = 0 ; _direction < 4 ; _direction++ ) {
        if ( (_edge = _vertex->getFirstEdges(_direction)) )
            return;
    }
}

VERTEX_Edges::VERTEX_Edges ( const VERTEX_Edges& edges )
// *****************************************************
    : Inherit()
    , _vertex(edges._vertex)
    , _edge(edges._edge)
    , _direction(edges._direction)
{
}

VERTEX_Edges& VERTEX_Edges::operator=(const VERTEX_Edges& edges)
// *************************************************************
{
    _vertex    = edges._vertex;
    _edge      = edges._edge;
    _direction = edges._direction;
    return *this;
}

Collection<Edge*>* VERTEX_Edges::getClone() const
// **********************************************
{
    return new VERTEX_Edges(*this);
}

Locator<Edge*>* VERTEX_Edges::getLocator() const
// *********************************************
{
    return new Locator(_vertex, _edge, _direction);
}

string VERTEX_Edges::_getString() const
// ************************************
{
    string s = "<" + _TName("Vertex::Edges");
    if (_vertex) s += " " + getString(_vertex);
    s += ">";
    return s;
}

// ************************************************************************************************
// VERTEX_Edges::Locator implementation
// ************************************************************************************************
VERTEX_Edges::Locator::Locator(const Vertex* vertex, Edge* edge, int direction)
// ****************************************************************************
    : Inherit()
    , _vertex(vertex)
    , _edge(edge)
    , _direction(direction)
{
}

VERTEX_Edges::Locator::Locator(const Locator& locator)
// ***************************************************
    : Inherit()
    , _vertex(locator._vertex)
    , _edge(locator._edge)
    , _direction(locator._direction)
{
}

VERTEX_Edges::Locator& VERTEX_Edges::Locator::operator=(const Locator& locator)
// ****************************************************************************
{
    _vertex    = locator._vertex;
    _edge      = locator._edge;
    _direction = locator._direction;
    return *this;
}

Edge* VERTEX_Edges::Locator::getElement() const
// ********************************************
{
    return _edge;
}

Locator<Edge*>* VERTEX_Edges::Locator::getClone() const
// ****************************************************
{
    return new Locator(*this);
}

bool VERTEX_Edges::Locator::isValid() const
// ****************************************
{
    return (_edge != NULL );
}

void VERTEX_Edges::Locator::progress()
// ***********************************
{
    if ( !_edge )
        return;

    if ( _direction == 0 || _direction == 1 )
        _edge = _edge->getNextFrom();
    else
        _edge = _edge->getNextTo();

    while ( !_edge ) {
        if ( _direction == 3 )
            return;
        _direction++;
        _edge = _vertex->getFirstEdges(_direction);
    }
}

string VERTEX_Edges::Locator::_getString() const
// *********************************************
{
    string s = "<" + _TName("Vertex::Edges::Locator");
    if (_vertex) s += " " + getString(_vertex);
    s += ">";
    return s;
}

} // namespace Knik
