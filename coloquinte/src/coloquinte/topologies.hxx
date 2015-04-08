
#include "common.hxx"

#ifndef COLOQUINTE_TOPOLOGIES
#define COLOQUINTE_TOPOLOGIES

namespace coloquinte{
namespace steiner_lookup{

template<int pin_cnt>
struct Hconnectivity{
    // The edges and the couple of pins connected to the extreme ones are represented by one char each
    // The first 4 bits represent the first pin minus one, the next 4 bits the second pin minus one
    std::uint8_t connexions[pin_cnt-3];
    std::uint8_t extremes;

    int_t get_wirelength(std::array<point<int_t>, pin_cnt> const sorted_points) const;
    std::array<std::pair<index_t, index_t>, pin_cnt-1> get_x_topology(std::array<point<int_t>, pin_cnt> const sorted_points) const;
};

extern std::array<Hconnectivity<4>, 2> const topologies_4;
extern std::array<Hconnectivity<5>, 6> const topologies_5;
extern std::array<Hconnectivity<6>, 23> const topologies_6;
extern std::array<Hconnectivity<7>, 111> const topologies_7;
extern std::array<Hconnectivity<8>, 642> const topologies_8;
extern std::array<Hconnectivity<9>, 4334> const topologies_9;
extern std::array<Hconnectivity<10>, 33510> const topologies_10;

}
}

#endif

