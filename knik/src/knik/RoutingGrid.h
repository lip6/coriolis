
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
                DbU::Unit  _lowerLeftX;
                DbU::Unit  _lowerLeftY;
                DbU::Unit  _tileWidth;
                DbU::Unit  _tileHeight;
                unsigned   _hcapacity;
                unsigned   _vcapacity;

    // Constructors & Destructors
    // **************************
        protected:
            RoutingGrid ( unsigned nbXTiles, unsigned nbYTiles, DbU::Unit lowerLeftX, DbU::Unit lowerLeftY
                        , DbU::Unit tileWidth, DbU::Unit tileHeight, unsigned hcapacity, unsigned vcapacity )
                        : _nbXTiles ( nbXTiles )
                        , _nbYTiles ( nbYTiles )
                        , _lowerLeftX ( lowerLeftX )
                        , _lowerLeftY ( lowerLeftY )
                        , _tileWidth ( tileWidth )
                        , _tileHeight ( tileHeight )
                        , _hcapacity ( hcapacity )
                        , _vcapacity ( vcapacity )
                        {};
            ~RoutingGrid () {};
        public:
            static RoutingGrid* create ( unsigned nbXTiles, unsigned nbYTiles, DbU::Unit lowerLeftX
                                       , DbU::Unit lowerLeftY, DbU::Unit tileWidth, DbU::Unit tileHeight, unsigned hcapacity, unsigned vcapacity )
            {
                RoutingGrid* _routingGrid = new RoutingGrid ( nbXTiles, nbYTiles, lowerLeftX, lowerLeftY, tileWidth, tileHeight, hcapacity, vcapacity );
                return _routingGrid;
            };

    // Accessors
    // *********
        public:
            unsigned  getNbXTiles()   { return _nbXTiles;   };
            unsigned  getNbYTiles()   { return _nbYTiles;   };
            DbU::Unit getLowerLeftX() { return _lowerLeftX; };
            DbU::Unit getLowerLeftY() { return _lowerLeftY; };
            DbU::Unit getTileWidth()  { return _tileWidth;  };
            DbU::Unit getTileHeight() { return _tileHeight; };
            unsigned  getHCapacity()  { return _hcapacity;  };
            unsigned  getVCapacity()  { return _vcapacity;  };

    // Modifiers
    // *********
    }; // class RoutingGrid

} // namespace Knik

#endif




