
#include "coloquinte/detailed.hxx"
#include "coloquinte/circuit_helper.hxx"

namespace coloquinte{
namespace dp{

namespace{

// Tries to swap two cells; 
inline bool try_swap(netlist const & circuit, detailed_placement & pl, index_t c1, index_t c2, bool try_flip,
std::function<std::int64_t(netlist const &, detailed_placement const &, std::vector<index_t> const &)> get_nets_cost){
    assert(pl.cell_height(c1) == 1 and pl.cell_height(c2) == 1);
    assert( (circuit.get_cell(c1).attributes & XMovable) != 0 and (circuit.get_cell(c1).attributes & YMovable) != 0);
    assert( (circuit.get_cell(c2).attributes & XMovable) != 0 and (circuit.get_cell(c2).attributes & YMovable) != 0);

    auto c1_bnds = pl.get_limit_positions(circuit, c1),
         c2_bnds = pl.get_limit_positions(circuit, c2);

    // Get the possible positions for a swap
    int_t swp_min_c1 = c2_bnds.first,
          swp_min_c2 = c1_bnds.first,
          swp_max_c1 = c2_bnds.second - circuit.get_cell(c1).size.x_,
          swp_max_c2 = c1_bnds.second - circuit.get_cell(c2).size.x_;

    if(swp_max_c1 >= swp_min_c1 and swp_max_c2 >= swp_min_c2){
        // Check both orientations of the cell

        // Get all the nets involved and uniquify them (nets with more than one pin on the cells)
        std::vector<index_t> involved_nets;
        for(netlist::pin_t p : circuit.get_cell(c1)){
            involved_nets.push_back(p.net_ind);
        }
        for(netlist::pin_t p : circuit.get_cell(c2)){
            involved_nets.push_back(p.net_ind);
        }
        std::sort(involved_nets.begin(), involved_nets.end());
        involved_nets.resize(std::distance(involved_nets.begin(), std::unique(involved_nets.begin(), involved_nets.end())));

        // Test the cost for the old position and the cost swapping the cells
        std::int64_t old_cost = get_nets_cost(circuit, pl, involved_nets);

        // Save the old values
        point<int_t> p1 = pl.plt_.positions_[c1];
        point<int_t> p2 = pl.plt_.positions_[c2];
        point<bool> o1 = pl.plt_.orientations_[c1];
        point<bool> o2 = pl.plt_.orientations_[c2];

        // Warning: won't work if the two cells don't have the same height
        pl.plt_.positions_[c1].x_ = (swp_min_c1 + swp_max_c1) / 2;
        pl.plt_.positions_[c2].x_ = (swp_min_c2 + swp_max_c2) / 2;
        pl.plt_.positions_[c1].y_ = p2.y_;
        pl.plt_.positions_[c2].y_ = p1.y_;

        // For standard cell placement, we want all the rows to be aligned in the same way
        if( (circuit.get_cell(c1).attributes & YFlippable) != 0 and (circuit.get_cell(c2).attributes & YFlippable) != 0)
            std::swap(pl.plt_.orientations_[c1].y_, pl.plt_.orientations_[c2].y_);

        if(try_flip and (circuit.get_cell(c1).attributes & XFlippable) != 0 and (circuit.get_cell(c2).attributes & XFlippable) != 0){
            index_t bst_ind = 4;
            for(index_t i=0; i<4; ++i){
                pl.plt_.orientations_[c1].x_ = i % 2;
                pl.plt_.orientations_[c2].x_ = i / 2;
                std::int64_t new_cost  = get_nets_cost(circuit, pl, involved_nets);
                if(new_cost < old_cost){
                    old_cost = new_cost;
                    bst_ind = i;
                }
            }

            // One of the orientations with the new positions was better
            if(bst_ind < 4){
                pl.swap_standard_cell_topologies(c1, c2);
                pl.plt_.orientations_[c1].x_ = bst_ind % 2;
                pl.plt_.orientations_[c2].x_ = bst_ind / 2;
                // We kept the swap
                return true;
            }
            else{
                pl.plt_.positions_[c1] = p1;
                pl.plt_.positions_[c2] = p2;
                pl.plt_.orientations_[c1] = o1;
                pl.plt_.orientations_[c2] = o2;
                return false;
            }
        }
        else if(get_nets_cost(circuit, pl, involved_nets) < old_cost){
            pl.swap_standard_cell_topologies(c1, c2);
            return true;
        }
        else{
            // Reset the old values since we didn't swap anything
            pl.plt_.positions_[c1] = p1;
            pl.plt_.positions_[c2] = p2;
            pl.plt_.orientations_[c1] = o1;
            pl.plt_.orientations_[c2] = o2;
            return false;
        }

        // A better solution would be
        // Check the cost on y depending on the position (extremely simple: two positions for each cell)
        // Check the cost on x depending on the position: piecewise linear and relatively complex
        //      * Get all external pins
        //      * Get all nets involving only one of the cells: piecewise linear cost for each of them
        //      * For nets involving the two cells, we have an additional cost

    }
    else{ // We just cannot swap those two cells without pushing anything
        return false;
    }
}

inline void generic_swaps_global(netlist const & circuit, detailed_placement & pl, index_t row_extent, index_t cell_extent, bool try_flip,
std::function<std::int64_t(netlist const &, detailed_placement const &, std::vector<index_t> const &)> get_nets_cost){
    for(index_t main_row = 0; main_row < pl.row_cnt(); ++main_row){

        for(index_t other_row = main_row+1; other_row <= std::min(pl.row_cnt()-1, main_row+row_extent) ; ++other_row){

            index_t first_oc = pl.get_first_standard_cell_on_row(other_row); // The first candidate cell to be examined
            for(index_t c = pl.get_first_standard_cell_on_row(main_row); c != null_ind; c = pl.get_next_standard_cell_on_row(c, main_row)){
                assert(pl.cell_rows_[c] == main_row);
                if( (circuit.get_cell(c).attributes & XMovable) == 0) continue; // Don't touch fixed cells

                // Number of cells after/before the end of the cell
                index_t nb_after  = 0;
                index_t nb_before = 0;
                int_t pos_low = pl.plt_.positions_[c].x_ -   circuit.get_cell(c).size.x_,
                      pos_hgh = pl.plt_.positions_[c].x_ + 2*circuit.get_cell(c).size.x_;
                for(index_t oc=first_oc; oc != null_ind and nb_after <= row_extent; oc = pl.get_next_standard_cell_on_row(oc, other_row)){
                    assert(pl.cell_rows_[oc] == other_row);
                    if( (circuit.get_cell(oc).attributes & XMovable) == 0) continue; // Don't touche fixed cells

                    // Count the cells which should trigger stop or shouldn't be used at the next iteration
                    if(pl.plt_.positions_[oc].x_ >= pos_hgh) ++nb_after;
                    if(pl.plt_.positions_[oc].x_ + circuit.get_cell(oc).size.x_ <= pos_low) ++ nb_before;

                    if(try_swap(circuit, pl, c, oc, try_flip, get_nets_cost)){
                        std::swap(c, oc);
                        if(c == first_oc) first_oc = oc;
                    }
                }
                while(nb_before > cell_extent){
                    nb_before--;
                    first_oc = pl.get_next_standard_cell_on_row(first_oc, other_row);
                }
            }
        }
    }
    pl.selfcheck();
}

} // End anonymous namespace

void swaps_global_HPWL(netlist const & circuit, detailed_placement & pl, index_t row_extent, index_t cell_extent, bool try_flip){
    generic_swaps_global(circuit, pl, row_extent, cell_extent, try_flip,
        [](netlist const & circuit, detailed_placement const & pl, std::vector<index_t> const & involved_nets) -> std::int64_t{
        std::int64_t sum = 0;
        for(index_t n : involved_nets){
            if(circuit.get_net(n).pin_cnt <= 1) continue;
            sum += get_HPWL_length(circuit, pl.plt_, n);
        }
        return sum;
    });
}

void swaps_global_RSMT(netlist const & circuit, detailed_placement & pl, index_t row_extent, index_t cell_extent, bool try_flip){
    generic_swaps_global(circuit, pl, row_extent, cell_extent, try_flip,
        [](netlist const & circuit, detailed_placement const & pl, std::vector<index_t> const & involved_nets) -> std::int64_t{
        std::int64_t sum = 0;
        for(index_t n : involved_nets){
            if(circuit.get_net(n).pin_cnt <= 1) continue;
            sum += get_RSMT_length(circuit, pl.plt_, n);
        }
        return sum;
    });
}

} // namespace dp
} // namespace coloquinte




