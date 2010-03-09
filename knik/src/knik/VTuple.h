
#ifndef _KNIK_VTUPLE_H
#define _KNIK_VTUPLE_H

#include "knik/Vertex.h"

namespace Knik {
    class VTuple {
    // **********
        // Attributes
        // **********      
        private:
            Vertex* _vertex;
            float   _distance; // Should be a cost : distance / future cost

        // Constructor
        // ***********
        private:
            VTuple ( Vertex* vertex, float distance )
                : _vertex(vertex)
                , _distance(distance)
                {}
        public:
            static VTuple* create ( Vertex* vertex, float distance ) {
                if(!vertex)
                    throw Error ( "VTuple::create(): NULL vertex." );
                VTuple* vtuple = new VTuple ( vertex, distance );
                vertex->setVTuple ( vtuple );
                return vtuple;
            }

        // Destructor
        // **********
        public:
            void destroy()
            {
                if ( _vertex ) {
                    assert ( this->getVertex()->getVTuple() == this );
                    _vertex->setVTuple ( NULL );
                }
                delete ( this );
            }
                 
        // Methods
        // *******
        public:
            Vertex* getVertex()   const { return _vertex; }
            float   getDistance() const { return _distance; }
            string  _getString()   const { string s = "<VTuple: ";
                                          s += getString(_distance);
                                          if (_vertex) s += " " + getString(_vertex);
                                          else         s += " NULL";
                                          s += ">"; return s;}

            void setAll      ( Vertex* vertex, float distance ) { _vertex   = vertex; _distance = distance; }
            void setDistance ( float distance )                 { _distance = distance; }
    };

} // end namespace

#endif
