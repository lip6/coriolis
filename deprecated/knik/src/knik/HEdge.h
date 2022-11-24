#ifndef _KNIK_HEDGE_H
#define _KNIK_HEDGE_H

#include "knik/Edge.h"

namespace Knik {

    class HEdge : public Edge {
    // ************************
        // Types
        // *****
        public:
            typedef Edge Inherit;

        // Constructors & Destructors
        // **************************
        protected:
            HEdge ( Vertex* from, Vertex* to );
            HEdge ( Vertex* from, Vertex* to, unsigned capacity );
            ~HEdge ();

        public:
            static HEdge* create ( Vertex* from, Vertex* to );
            static HEdge* create ( Vertex* from, Vertex* to, unsigned capacity );
            //void destroy();
            void _postCreate ();
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
            virtual bool  isVertical()   const { return false; };
            virtual bool  isHorizontal() const { return true;  };
    };
}  // end namespace

#endif
