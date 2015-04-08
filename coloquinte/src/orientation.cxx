#include "coloquinte/circuit_helper.hxx"

#include <stack>
#include <functional>
#include <algorithm>

namespace coloquinte{
namespace gp{

namespace{
index_t const null_ind = std::numeric_limits<index_t>::max();

inline void opt_orient(netlist const & circuit, placement_t & pl, std::function<int_t (point<int_t>)> i_coor, std::function<bool & (point<bool> &)> b_coor,mask_t FLIPPABLE){
    std::stack<index_t> opt_cells;
    for(index_t cell_ind = 0; cell_ind < circuit.cell_cnt(); ++cell_ind){
        if( (circuit.get_cell(cell_ind).attributes & FLIPPABLE) != 0)
            opt_cells.push(cell_ind);
    }
    while(not opt_cells.empty()){
        index_t cell_ind = opt_cells.top(); opt_cells.pop();
        assert((circuit.get_cell(cell_ind).attributes & FLIPPABLE) != 0);

        // What is the current orientation?
        bool old_orientation = b_coor(pl.orientations_[cell_ind]);
        int_t pos = i_coor(pl.positions_[cell_ind]);
        int_t size = i_coor(circuit.get_cell(cell_ind).size);

        // Check both orientations of the cell
        std::vector<index_t> involved_nets;
        for(netlist::pin_t p : circuit.get_cell(cell_ind)){
            involved_nets.push_back(p.net_ind);
        }
        // Deal with cells with multiple pins in one net (uniquify)
        std::sort(involved_nets.begin(), involved_nets.end());
        involved_nets.resize(std::distance(involved_nets.begin(), std::unique(involved_nets.begin(), involved_nets.end())));

        std::int64_t p_cost = 0, n_cost = 0;
        std::vector<index_t> extreme_elements;
        for(index_t n : involved_nets){
            std::vector<pin_1D> other_pins;
            std::vector<int_t> offsets;
            for(auto p : circuit.get_net(n)){
                if(p.cell_ind != cell_ind){
                    other_pins.push_back(pin_1D(
                        p.cell_ind,
                        i_coor(pl.positions_[p.cell_ind])
                + (b_coor(pl.orientations_[p.cell_ind]) ? i_coor(p.offset) : i_coor(circuit.get_cell(p.cell_ind).size) - i_coor(p.offset)),
                        0, // Don't care about the offset
                        (circuit.get_cell(p.cell_ind).attributes & FLIPPABLE) != 0)
                    );
                }
                else{
                    offsets.push_back(i_coor(p.offset));
                }
            }
            assert(offsets.size() > 0);
            if(other_pins.size() > 0){ // Else the orientation of the cell doesn't change anything
                auto minmaxC = std::minmax_element(other_pins.begin(), other_pins.end());
                auto minmaxO = std::minmax_element(offsets.begin(), offsets.end());
                p_cost += std::max(pos + *minmaxO.second, minmaxC.second->pos) - std::min(pos + *minmaxO.first, minmaxC.first->pos);
                n_cost += std::max(pos + size - *minmaxO.first, minmaxC.second->pos) - std::min(pos + size - *minmaxO.second, minmaxC.first->pos);

                int_t min_pin_pos = std::min(pos + *minmaxO.second, pos + size - *minmaxO.first),
                      max_pin_pos = std::max(pos + *minmaxO.second, pos + size - *minmaxO.first);

                // Do the extreme elements change between the two positions?
                if(minmaxC.second->movable
              and (minmaxC.second->pos < max_pin_pos)
              and (minmaxC.second->pos > min_pin_pos) ){
                    extreme_elements.push_back(minmaxC.second->cell_ind);
                }
                if(minmaxC.first->movable
              and (minmaxC.first->pos < max_pin_pos)
              and (minmaxC.first->pos > min_pin_pos) ){ 
                    extreme_elements.push_back(minmaxC.first->cell_ind);
                }
            }
        }

        if(p_cost < n_cost)
            b_coor(pl.orientations_[cell_ind]) = true;
        if(p_cost > n_cost)
            b_coor(pl.orientations_[cell_ind]) = false;

        // If we changed the orientation, check the extreme pins which changed and try their cells again
        if(b_coor(pl.orientations_[cell_ind]) != old_orientation){
            std::sort(extreme_elements.begin(), extreme_elements.end());
            extreme_elements.resize(std::distance(extreme_elements.begin(), std::unique(extreme_elements.begin(), extreme_elements.end())));
            for(index_t extreme_cell : extreme_elements){
                if( (circuit.get_cell(extreme_cell).attributes & FLIPPABLE) != 0)
                    opt_cells.push(extreme_cell);
            }
        }
    }
}
/*
inline void spread_orient(netlist const & circuit, placement_t & pl, std::function<float_t & (point<float_t> &)> coor, mask_t FLIPPABLE){
    std::vector<float_t> weights(circuit.cell_cnt(), 0.0);
    for(index_t n=0; n<circuit.net_cnt(); ++n){
        float_t min_pos=INF, max_pos=-INF;
        float_t min_offs=INF, max_offs=-INF;
        index_t min_ind=null_ind, max_ind=null_ind;
        for(netlist::pin_t p : circuit.get_net(n)){
            if( (circuit.get_cell(p.cell_ind).attributes & FLIPPABLE) != 0){
                float_t pos = coor(pl.positions_[p.cell_ind]);
                if(pos < min_pos){
                    min_pos = pos;
                    min_ind = p.cell_ind;
                    min_offs = coor(p.offset);
                }
                if(pos > max_pos){
                    max_pos = pos;
                    max_ind = p.cell_ind;
                    max_offs = coor(p.offset);
                }
            }
            else{
                float_t pos = coor(pl.positions_[p.cell_ind]) + coor(pl.orientations_[p.cell_ind]) * coor(p.offset);
                if(pos < min_pos){
                    min_pos = pos;
                    min_ind = null_ind;
                }
                if(pos > max_pos){
                    max_pos = pos;
                    max_ind = null_ind;
                }
            }
        }

        float_t net_weight = circuit.get_net(n).weight;

        if(min_ind != null_ind) weights[min_ind] += net_weight * min_offs;
        if(max_ind != null_ind) weights[max_ind] -= net_weight * max_offs;
    }

    for(index_t c=0; c<circuit.cell_cnt(); ++c){
        coor(pl.orientations_[c]) = (weights[c] >= 0.0) ? 1.0 : -1.0;
    }
}
*/
} // End anonymous namespace

void optimize_x_orientations(netlist const & circuit, placement_t & pl){
    opt_orient(circuit, pl, [](point<int_t> p) -> int_t { return p.x_; }, [](point<bool> & p) -> bool & { return p.x_; }, XFlippable);
}
void optimize_y_orientations(netlist const & circuit, placement_t & pl){
    opt_orient(circuit, pl, [](point<int_t> p) -> int_t { return p.y_; }, [](point<bool> & p) -> bool & { return p.y_; }, YFlippable);
}

// Iteratively optimize feasible orientations; performs only one pass
void optimize_exact_orientations(netlist const & circuit, placement_t & pl){
    optimize_x_orientations(circuit, pl);
    optimize_y_orientations(circuit, pl);
}

/*
void spread_orientations(netlist const & circuit, placement_t & pl){
    spread_orient(circuit, pl, [](point<float_t> & p) -> float_t & { return p.x_; }, XFlippable);
    spread_orient(circuit, pl, [](point<float_t> & p) -> float_t & { return p.y_; }, YFlippable);
}
*/

} // namespace gp
} // namespace coloquinte


