
#include "circuit.hxx"
#include "detailed.hxx"

namespace coloquinte{
namespace dp{

detailed_placement legalize(netlist const & circuit, placement_t const & pl, box<int_t> surface, int_t row_height);
void get_result(netlist const & circuit, detailed_placement const & dpl, placement_t & pl);

} // namespace dp
} // namespace coloquinte
