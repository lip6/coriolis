
#ifndef _KNIK_ROUTINGGRID_H
#define _KNIK_ROUTINGGRID_H


#include "hurricane/DbU.h"

namespace Knik {

    using namespace Hurricane;
    class RoutingGrid {
    // Attributes
    // **********
        private: 
                unsigned   _nbXTiles;
                unsigned   _nbYTiles;
                Box        _boundingBox;
                DbU::Unit  _tileWidth;
                DbU::Unit  _tileHeight;
                unsigned   _hcapacity;
                unsigned   _vcapacity;

    // Constructors & Destructors
    // **************************
        protected:
            RoutingGrid ( unsigned   nbXTiles
                        , unsigned   nbYTiles
                        , const Box& boundingBox
                        , DbU::Unit  tileWidth
                        , DbU::Unit  tileHeight
                        , unsigned   hcapacity
                        , unsigned   vcapacity )
              : _nbXTiles   (nbXTiles)
              , _nbYTiles   (nbYTiles)
              , _boundingBox(boundingBox)
              , _tileWidth  (tileWidth)
              , _tileHeight (tileHeight)
              , _hcapacity  (hcapacity)
              , _vcapacity  (vcapacity)
              { };
            ~RoutingGrid () {};
        public:
            static RoutingGrid* create ( unsigned   nbXTiles
                                       , unsigned   nbYTiles
                                       , const Box& boundingBox
                                       , DbU::Unit  tileWidth
                                       , DbU::Unit  tileHeight
                                       , unsigned   hcapacity
                                       , unsigned   vcapacity )
            {
              RoutingGrid* _routingGrid = new RoutingGrid ( nbXTiles
                                                          , nbYTiles
                                                          , boundingBox
                                                          , tileWidth
                                                          , tileHeight
                                                          , hcapacity
                                                          , vcapacity );
              return _routingGrid;
            };

    // Accessors
    // *********
        public:
            unsigned   getNbXTiles()    { return _nbXTiles;   };
            unsigned   getNbYTiles()    { return _nbYTiles;   };
            const Box& getBoundingBox() const { return _boundingBox; }
            DbU::Unit  getLowerLeftX()  { return _boundingBox.getXMin(); };
            DbU::Unit  getLowerLeftY()  { return _boundingBox.getYMin(); };
            DbU::Unit  getTileWidth()   { return _tileWidth;  };
            DbU::Unit  getTileHeight()  { return _tileHeight; };
            unsigned   getHCapacity()   { return _hcapacity;  };
            unsigned   getVCapacity()   { return _vcapacity;  };

    // Modifiers
    // *********
    }; // class RoutingGrid

} // namespace Knik

#endif




