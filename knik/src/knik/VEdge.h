#ifndef _KNIK_VEDGE_H
#define _KNIK_VEDGE_H

#include "knik/Edge.h"

namespace Knik {

    class VEdge : public Edge {
    // ************************
        // Types
        // *****
        public:
            typedef Edge Inherit;

        // Constructors & Destructors
        // **************************
        protected:
            VEdge ( Vertex* from, Vertex* to );
            VEdge ( Vertex* from, Vertex* to, unsigned capacity );
            ~VEdge ();

        public:
            static VEdge* create ( Vertex* from, Vertex* to );
            static VEdge* create ( Vertex* from, Vertex* to, unsigned capacity );
            //void destroy();
            void _postCreate ( bool fenceCapacity );
            //void _preDestroy();
            
        // Others
        // ******
        public:
            virtual Box        computeBoundingBox() const;
            virtual Point      getReferencePoint () const;
            virtual DbU::Unit  getWidth () const;
            virtual float      getNormalisedLength() const;

        // Predicates
        // **********
        public:
            //virtual void  createSplitter  ( Net* net );
            //virtual Hook* getSplitterHook ( Vertex* vertex );
            virtual bool  isVertical()   const { return true; };
            virtual bool  isHorizontal() const { return false;  };
    };
}  // end namespace

#endif
