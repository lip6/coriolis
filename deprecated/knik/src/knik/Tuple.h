
#ifndef _KNIK_TUPLE_H
#define _KNIK_TUPLE_H

#include "knik/Vertex.h"

namespace Knik {
    class Tuple {
    // **********
        // Attributes
        // **********      
        private:
            Vertex* _vertex;
            float   _distance; // Should be a cost : distance / future cost

        // Constructor
        // ***********
        private:
            Tuple ( Vertex* vertex, float distance )
                : _vertex(vertex)
                , _distance(distance)
                {}
        public:
            static Tuple* create ( Vertex* vertex, float distance ) {
                if(!vertex)
                    throw Error ( "Tuple::create(): NULL vertex." );
                Tuple* tuple = new Tuple ( vertex, distance );
                vertex->setTuple ( tuple );
                return tuple;
            }

        // Destructor
        // **********
        public:
            void destroy()
            {
                if ( _vertex ) {
                    assert ( this->getVertex()->getTuple() == this );
                    _vertex->setTuple ( NULL );
                }
                delete ( this );
            }
                 
        // Methods
        // *******
        public:
            Vertex* getVertex()   const { return _vertex; }
            float   getDistance() const { return _distance; }
            string  _getString()   const { string s = "<Tuple: ";
                                          s += getString(_distance);
                                          if (_vertex) s += " " + getString(_vertex);
                                          else         s += " NULL";
                                          s += ">"; return s;}

            void setAll      ( Vertex* vertex, float distance ) { _vertex   = vertex; _distance = distance; }
            void setDistance ( float distance )                 { _distance = distance; }
    };

} // end namespace

#endif
