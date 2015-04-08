
#ifndef COLOQUINTE_DETAILED
#define COLOQUINTE_DETAILED

#include "common.hxx"
#include "netlist.hxx"

#include <vector>
#include <limits>

namespace coloquinte{
namespace dp{

const index_t null_ind = std::numeric_limits<index_t>::max();

struct detailed_placement{
    // All position and orientation stuff
    placement_t plt_;

    std::vector<index_t> cell_rows_;

    // The placement region
    int_t min_x_, max_x_;
    int_t y_origin_;
    int_t row_height_;

    // Encode the topological state of the circuit: which cells are near each other
    // Makes extracting part of the circuit or optimizing positions at fixed topology easy
    std::vector<std::pair<index_t, index_t> > neighbours_; // The cells before and after on each row; cells spanning multiple columns use several positions
    // In order to get the neighbours in the detailed placement
    std::vector<index_t> neighbours_limits_;

    std::vector<index_t> row_first_cells_, row_last_cells_; // For each row, which cells are the on the boundaries

    // Tests the coherency between positions, widths and topological representation
    void selfcheck() const;

    detailed_placement(
            placement_t pl,
            std::vector<index_t> placement_rows,
            std::vector<index_t> cell_heights,
            std::vector<std::vector<index_t> > rows,
            int_t min_x, int_t max_x,
            int_t y_origin,
            index_t nbr_rows, int_t row_height
        );

    index_t cell_height(index_t c) const{ return neighbours_limits_[c+1] - neighbours_limits_[c]; }
    index_t cell_cnt() const{ return cell_rows_.size(); }
    index_t row_cnt()  const{ return row_first_cells_.size(); }
    index_t neighbour_index(index_t c, index_t r) const{
        assert(r - cell_rows_[c] < cell_height(c));
        return neighbours_limits_[c] + r - cell_rows_[c];
    }

    void swap_standard_cell_topologies(index_t c1, index_t c2);
    std::pair<int_t, int_t> get_limit_positions(netlist const & circuit, index_t c) const;

    index_t get_first_cell_on_row(index_t r);
    index_t get_next_cell_on_row(index_t c, index_t r);
    index_t get_prev_cell_on_row(index_t c, index_t r);

    index_t get_first_standard_cell_on_row(index_t r);
    index_t get_next_standard_cell_on_row(index_t c, index_t r);

    void reorder_standard_cells(std::vector<index_t> const old_order, std::vector<index_t> const new_order);
    void reorder_cells(std::vector<index_t> const old_order, std::vector<index_t> const new_order, index_t row);
};

void swaps_global_HPWL(netlist const & circuit, detailed_placement & pl, index_t row_extent, index_t cell_extent, bool try_flip = false);
void swaps_global_RSMT(netlist const & circuit, detailed_placement & pl, index_t row_extent, index_t cell_extent, bool try_flip = false);

void swaps_row_convex_HPWL(netlist const & circuit, detailed_placement & pl, index_t range);
void swaps_row_convex_RSMT(netlist const & circuit, detailed_placement & pl, index_t range);
void swaps_row_noncvx_HPWL(netlist const & circuit, detailed_placement & pl, index_t range);
void swaps_row_noncvx_RSMT(netlist const & circuit, detailed_placement & pl, index_t range);

void OSRP_convex_HPWL(netlist const & circuit, detailed_placement & pl);
void OSRP_convex_RSMT(netlist const & circuit, detailed_placement & pl);
void OSRP_noncvx_HPWL(netlist const & circuit, detailed_placement & pl);
void OSRP_noncvx_RSMT(netlist const & circuit, detailed_placement & pl);

void optimize_on_topology_HPWL(netlist const & circuit, detailed_placement & pl);

void row_compatible_orientation(netlist const & circuit, detailed_placement & pl, bool first_row_orient);

} // namespace dp
} // namespace coloquinte

#endif

