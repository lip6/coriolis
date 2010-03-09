
#ifndef _KNIK_STUPLE_H
#define _KNIK_STUPLE_H

#include "hurricane/Segment.h"
#include "hurricane/Net.h"
#include "hurricane/Property.h"

namespace Knik {

    class STuple {
    // ***********
        // Types
        // *****
        public: struct STupleCompare : public binary_function<const STuple*, const STuple*, bool> {
            bool operator()(const STuple* stuple1, const STuple* stuple2) const {
                if ( stuple1->getCost() >  stuple2->getCost() )  return true;
                if ( stuple1->getCost() == stuple2->getCost() ) {
                    assert ( stuple1 != stuple2 );
                    Segment* segment1 = stuple1->getSegment();
                    Segment* segment2 = stuple2->getSegment();
                    assert ( segment1 );
                    assert ( segment2 );
                    if ( segment1->getSourceX() <  segment2->getSourceX() ) return true;
                    if ( segment1->getSourceX() == segment2->getSourceX() ) {
                        if ( segment1->getSourceY() <  segment2->getSourceY() ) return true;
                        if ( segment1->getSourceY() == segment2->getSourceY() ) {
                            // nous avons deux segemnts de meme cout exactement superposés
                            // il n'est pas possible qu'ils aient le meme net !
                            if (getString(segment1->getNet()->getName()) <  getString(segment2->getNet()->getName()) ) return true;
                            if (getString(segment1->getNet()->getName()) == getString(segment2->getNet()->getName()) ) assert(false);
                        }
                    }
                }
                return false;
            }
        };

        typedef multiset<STuple*, STupleCompare>           STuplePriorityQueue;
        typedef multiset<STuple*, STupleCompare>::iterator STuplePQIter;

        public: class CostProperty : public PrivateProperty {
            public: typedef PrivateProperty Inherit;
        
            public:  static Name  _name;
            private: STuplePQIter _pqIter;
        
            protected: CostProperty()
                       : Inherit()
                       {
                       };
        
            public: static CostProperty* create() {
                        CostProperty* property = new CostProperty();
                        property->_postCreate();
        
                        return property;
                    };
        
            public: virtual Name         getName()   const { return _name; };
            public: const   STuplePQIter getPQIter() const { return _pqIter; };
        
            public: void setPQIter ( const STuplePQIter& pqIter ) { _pqIter = pqIter; };
        
            public: virtual string _getTypeName() const { return _TName("CostProperty"); };
            public: virtual string _getString() const {
                        string s = Inherit::_getString();
                        s += " ";
                        if ( getOwner() ) s += getString(getOwner()) + " ";
                        //s += getString(_cost);
                        return s;
                    };
            public: virtual Record* _getRecord() const {
                        Record* r =  Inherit::_getRecord();
                        //if (r)  r->add(getSlot("Cost", _cost));
                        return  r;
                    };
        
        };


        // Attributes
        // **********      
        private:
            CostProperty* _costProperty;
        public:
            static STuplePQIter _stuplePQEnd;
            Segment* _segment;
            float    _cost;

        // Constructor
        // ***********
        private:
            STuple ( Segment* segment, float cost )
                : _segment (segment)
                , _cost (cost)
                {
                    // si le segment a uneproperty on la recupere sinon on en crée une une à _stuplePQEnd
                    _costProperty = STuple::CostProperty::create();
                }
        public:
            static STuple* create ( Segment* segment, float cost ) {
                if(!segment)
                    throw Error ( "STuple::create(): NULL segment." );
                STuple* stuple = new STuple ( segment, cost );
                return stuple;
            }

        // Destructor
        // **********
        public:
            void destroy()
            {
                if ( _costProperty ) {
                    assert ( (*_costProperty->getPQIter()) == this );
                    _costProperty->setPQIter(_stuplePQEnd);
                }
                delete ( this );
            }
                 
        // Methods
        // *******
        public:
            CostProperty* getCostProperty() const { return _costProperty; }
            float         getCost()         const { return _cost; }
            Segment*      getSegment()      const { return _segment; };
            string       _getTypeName()     const { return _TName("STuple"); }
            string       _getString()       const { string s = "<STuple: ";
                                                    if (_segment) s += " " + _segment->_getString();
                                                    else          s += " NULL";
                                                    s += " ";
                                                    s += getString(_cost);
                                                    s += ">"; return s;}
            Record*      _getRecord()       const { Record* record = new Record ( this->_getString() );
                                                    return record; }

            void        setCostProperty ( CostProperty* costProperty ) { _costProperty = costProperty; }
            void        setCost         ( float cost )                 { _cost = cost; }
            // no setSegment : cannot be changed
            static void setSTuplePQEnd  ( STuplePQIter stuplePQEnd )   { _stuplePQEnd = stuplePQEnd; }
    };

} // end namespace


INSPECTOR_P_SUPPORT(Knik::STuple);
INSPECTOR_P_SUPPORT(Knik::STuple::CostProperty);

#endif
