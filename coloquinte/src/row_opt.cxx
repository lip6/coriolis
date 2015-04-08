
#include "coloquinte/detailed.hxx"
#include "coloquinte/circuit_helper.hxx"

#include "coloquinte/optimization_subproblems.hxx"
#include "coloquinte/union_find.hxx"
#include "coloquinte/piecewise_linear.hxx"

#include <cassert>

#include <iostream>

namespace coloquinte{
namespace dp{

namespace{

struct minmax{
    int_t min, max;
    minmax(){}
    minmax(int_t f, int_t s) : min(f), max(s){}
    void merge(minmax const o){
        min = std::min(min, o.min);
        max = std::max(max, o.max);
    }
    void merge(int_t const o){
        merge(minmax(o, o));
    }
};

struct order_gettr{
    index_t cell_ind, seq_order;
    bool operator<(order_gettr const o) const{ return cell_ind < o.cell_ind; }
    bool operator<(index_t const o) const{ return cell_ind < o; }
    order_gettr(index_t c, index_t i) : cell_ind(c), seq_order(i) {}
};

std::vector<order_gettr> get_sorted_ordered_cells(std::vector<index_t> const & cells){
    std::vector<order_gettr> ret;
    for(index_t i=0; i<cells.size(); ++i){
        ret.push_back(order_gettr(cells[i],i));
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

std::vector<index_t> get_unique_nets(netlist const & circuit, std::vector<index_t> const & cells){
    std::vector<index_t> involved_nets;
    for(index_t c : cells){
        for(netlist::pin_t p : circuit.get_cell(c)){
            involved_nets.push_back(p.net_ind);
        }
    }
    // Uniquify the nets
    std::sort(involved_nets.begin(), involved_nets.end());
    involved_nets.resize(std::distance(involved_nets.begin(), std::unique(involved_nets.begin(), involved_nets.end())));
    return involved_nets;
}

struct Hnet_group{
    struct Hpin{
        index_t cell_index; // Not indexes in the circuit!!! Rather in the internal algorithm
        minmax offset;
        bool operator<(Hpin const o) const{ return cell_index < o.cell_index; }
    };
    struct Hnet{
        bool has_ext_pins;
        minmax ext_pins;
        int_t weight;

        Hnet(){
            has_ext_pins = false;
            ext_pins = minmax(std::numeric_limits<int_t>::max(), 0);
            weight = 1;
        }
    };

    std::vector<index_t> net_limits;
    std::vector<Hnet> nets;

    std::vector<Hpin> pins;
    std::vector<int_t> cell_widths;

    Hnet_group(){
        net_limits.push_back(0);
    }

    void add_net(std::vector<pin_1D> const added_pins, int_t weight){
        Hnet cur_net;
        cur_net.weight = weight;
        std::vector<Hpin> new_pins;
        for(auto const p : added_pins){
            if(p.movable){
                Hpin new_pin;
                new_pin.cell_index = p.cell_ind;
                new_pin.offset = minmax(p.offs, p.offs);
                new_pins.push_back(new_pin);
            }
            else{
                cur_net.has_ext_pins = true;

                cur_net.ext_pins.merge(p.pos);
            }
        }
        std::sort(new_pins.begin(), new_pins.end());

        if(not new_pins.empty()){ // Possible when generating from a Steiner topology
            // Uniquify just in case there are several pins on the net on a single cell
            index_t j=0;
            auto prev_pin = new_pins[0];
            for(auto it = new_pins.begin()+1; it != new_pins.end(); ++it){
                if(it->cell_index == prev_pin.cell_index){
                    prev_pin.offset.merge(it->offset);
                }
                else{
                    new_pins[j] = prev_pin;
                    ++j;
                    prev_pin = *it;
                }
            }
            new_pins[j]=prev_pin;
            new_pins.resize(j+1);
            nets.push_back(cur_net);
            net_limits.push_back(net_limits.back() + new_pins.size());
            pins.insert(pins.end(), new_pins.begin(), new_pins.end());
        }
    }

    std::int64_t get_cost(std::vector<int_t> const & pos) const{
        std::int64_t cost=0;
        for(index_t n=0; n<nets.size(); ++n){
            auto cur_net = nets[n];

            minmax mm(std::numeric_limits<int_t>::max(), std::numeric_limits<int_t>::min());
            if(cur_net.has_ext_pins){
                mm = cur_net.ext_pins;
            }

            assert(net_limits[n+1] > net_limits[n]);
            for(index_t p=net_limits[n]; p<net_limits[n+1]; ++p){
                int_t cur_pos = pos[pins[p].cell_index];
                mm.merge( minmax(cur_pos + pins[p].offset.min, cur_pos + pins[p].offset.max) );
            }
            cost += static_cast<std::int64_t>(cur_net.weight) * (mm.max - mm.min);
        }
        return cost;
    }

    std::int64_t get_cost(std::vector<int_t> const & pos, std::vector<int> const & flip) const{
        std::int64_t cost=0;
        for(index_t n=0; n<nets.size(); ++n){
            auto cur_net = nets[n];

            minmax mm(std::numeric_limits<int_t>::max(), std::numeric_limits<int_t>::min());
            if(cur_net.has_ext_pins){
                mm = cur_net.ext_pins;
            }

            assert(net_limits[n+1] > net_limits[n]);
            for(index_t p=net_limits[n]; p<net_limits[n+1]; ++p){
                int_t cur_pos = pos[pins[p].cell_index];
                bool flipped  = flip[pins[p].cell_index];
                int_t wdth    = cell_widths[pins[p].cell_index];
                mm.merge( flipped ? 
                    minmax(cur_pos + wdth - pins[p].offset.max, cur_pos + wdth - pins[p].offset.min)
                  : minmax(cur_pos + pins[p].offset.min, cur_pos + pins[p].offset.max)
                );
            }
            cost += static_cast<std::int64_t>(cur_net.weight) * (mm.max - mm.min);
        }
        return cost;
    }

};

Hnet_group get_B2B_netgroup(netlist const & circuit, detailed_placement const & pl, std::vector<index_t> const & cells){

    std::vector<order_gettr> cells_in_row = get_sorted_ordered_cells(cells);
    std::vector<index_t> involved_nets = get_unique_nets(circuit, cells);

    Hnet_group ret;
    for(index_t c : cells)
        ret.cell_widths.push_back(circuit.get_cell(c).size.x_);

    for(index_t n : involved_nets){
        std::vector<pin_1D> cur_pins = get_pins_1D(circuit, pl.plt_, n).x_;
        for(pin_1D & p : cur_pins){
            auto it = std::lower_bound(cells_in_row.begin(), cells_in_row.end(), p.cell_ind);
            if(it != cells_in_row.end() and it->cell_ind == p.cell_ind){
                p.cell_ind = it->seq_order;
            }
            else{ // Found a pin which remains fixed for this round
                p.movable = false;
            }
        }
        ret.add_net(cur_pins, circuit.get_net(n).weight);
    }

    return ret;
}

Hnet_group get_RSMT_netgroup(netlist const & circuit, detailed_placement const & pl, std::vector<index_t> const & cells){

    std::vector<order_gettr> cells_in_row = get_sorted_ordered_cells(cells);
    std::vector<index_t> involved_nets = get_unique_nets(circuit, cells);

    Hnet_group ret;
    for(index_t c : cells)
        ret.cell_widths.push_back(circuit.get_cell(c).size.x_);

    for(index_t n : involved_nets){
        auto vpins = get_pins_2D(circuit, pl.plt_, n);
        for(auto & p : vpins){
            auto it = std::lower_bound(cells_in_row.begin(), cells_in_row.end(), p.cell_ind);
            if(it != cells_in_row.end() and it->cell_ind == p.cell_ind){
                p.cell_ind = it->seq_order;
            }
            else{
                p.movable = false;
            }
        }

        std::vector<point<int_t> > pin_locations;
        for(auto p : vpins)
            pin_locations.push_back(p.pos);
        auto const Htopo = get_RSMT_topology(pin_locations, 8).x_;

        // In the horizontal topology, we transform the parts of the tree that are on the row into HPWL subnets
        // Two pins sharing an edge are in the same subnet if one of them is on the row: use union-find
        union_find UF(vpins.size());
        for(auto E : Htopo){
            if( vpins[E.first].movable or vpins[E.second].movable){
                UF.merge(E.first, E.second);
            }
        }

        std::vector<std::vector<pin_1D> > connex_comps(vpins.size());
        for(index_t i=0; i<vpins.size(); ++i){
            connex_comps[UF.find(i)].push_back(vpins[i].x());;
        }

        int_t weight = circuit.get_net(n).weight;
        for(index_t i=0; i<vpins.size(); ++i){
            if(not connex_comps[i].empty()){
                ret.add_net(connex_comps[i], weight);
            }
        }
    }

    return ret;
}

// Optimizes an ordered sequence of standard cells on the same row, returns the cost and the corresponding positions
inline std::int64_t optimize_convex_sequence(Hnet_group const & nets, std::vector<index_t> const & permutation, std::vector<int_t> & positions, std::vector<std::pair<int_t, int_t> > const & cell_ranges){
    // Get the widths of the cells in row order
    std::vector<int_t> loc_widths(permutation.size());
    std::vector<std::pair<int_t, int_t> > loc_ranges(permutation.size());
    for(index_t i=0; i<permutation.size(); ++i){
         loc_widths[permutation[i]] = nets.cell_widths[i];
         loc_ranges[permutation[i]] = cell_ranges[i];
    }

    std::vector<cell_bound> bounds;
    std::vector<int_t> right_slopes(permutation.size(), 0);
    for(index_t n=0; n<nets.nets.size(); ++n){
        index_t fst_c=std::numeric_limits<index_t>::max(), lst_c=0;
        int_t fst_pin_offs=0, lst_pin_offs=0;
        assert(nets.net_limits[n+1] > nets.net_limits[n]);
        auto cur_net = nets.nets[n];
        for(index_t p=nets.net_limits[n]; p<nets.net_limits[n+1]; ++p){
            // Permutation: index in the Hnet_group to index in the row
            index_t cur_cell = permutation[nets.pins[p].cell_index];
            if(cur_cell < fst_c){
                fst_c = cur_cell;
                fst_pin_offs = nets.pins[p].offset.min;
            }
            if(cur_cell >= lst_c){
                lst_c = cur_cell;
                lst_pin_offs = nets.pins[p].offset.max;
            }
        }
        if(cur_net.has_ext_pins){
            bounds.push_back(cell_bound(fst_c, cur_net.ext_pins.min - fst_pin_offs, cur_net.weight));
            bounds.push_back(cell_bound(lst_c, cur_net.ext_pins.max - lst_pin_offs, cur_net.weight));

            right_slopes[lst_c] += cur_net.weight;
        }
        else{
            right_slopes[lst_c] += cur_net.weight;
            right_slopes[fst_c] -= cur_net.weight;
        }
    }

    bool feasible = place_convex_single_row(loc_widths, loc_ranges, bounds, right_slopes, positions);

    auto permuted_positions = positions;
    for(index_t i=0; i<permutation.size(); ++i){
        permuted_positions[i] = positions[permutation[i]];
    }
    if(feasible)
        return nets.get_cost(permuted_positions);
    else
        return std::numeric_limits<std::int64_t>::max(); // Infeasible: return a very big cost
}

// TODO: take modified order relative to the obstacles into account
inline std::int64_t optimize_noncvx_sequence(Hnet_group const & nets, std::vector<index_t> const & permutation, std::vector<int_t> & positions, std::vector<int> & flippings, std::vector<int> const & flippability, std::vector<std::pair<int_t, int_t> > const & cell_ranges){
    // Get the widths of the cells in row order
    std::vector<int_t> loc_widths(permutation.size());
    std::vector<int> loc_flipps(permutation.size());
    std::vector<std::pair<int_t, int_t> > loc_ranges(permutation.size());
    for(index_t i=0; i<permutation.size(); ++i){
         loc_widths[permutation[i]] = nets.cell_widths[i];
         loc_ranges[permutation[i]] = cell_ranges[i];
         loc_flipps[permutation[i]] = flippability[i];
    }

    int_t min_limit = std::numeric_limits<int_t>::min();
    for(index_t i=0; i<loc_ranges.size(); ++i){
        min_limit = std::max(loc_ranges[i].first, min_limit);
        loc_ranges[i].first = min_limit;
        min_limit += loc_widths[i];
    }
    int_t max_limit = std::numeric_limits<int_t>::max();
    for(index_t i=loc_ranges.size(); i>0; --i){
        max_limit = std::min(loc_ranges[i-1].second, max_limit);
        max_limit -= loc_widths[i-1];
        loc_ranges[i-1].second = max_limit;
    }

    for(index_t i=0; i<loc_ranges.size(); ++i){
        if(loc_ranges[i].first > loc_ranges[i].second){
            return std::numeric_limits<std::int64_t>::max(); // Infeasible: return a very big cost
        }
    }

    std::vector<piecewise_linear_function> unflipped_cost_functions, flipped_cost_functions;
    for(index_t i=0; i<loc_ranges.size(); ++i){
        auto cur = piecewise_linear_function(loc_ranges[i].first, loc_ranges[i].second);
        unflipped_cost_functions.push_back(cur);
        flipped_cost_functions.push_back(cur);
    }

    for(index_t n=0; n<nets.nets.size(); ++n){
        index_t fst_c=std::numeric_limits<index_t>::max(), lst_c=0;
        int_t fst_pin_offs_mn=0, lst_pin_offs_mn=0,
              fst_pin_offs_mx=0, lst_pin_offs_mx=0;

        assert(nets.net_limits[n+1] > nets.net_limits[n]);
        auto cur_net = nets.nets[n];
        for(index_t p=nets.net_limits[n]; p<nets.net_limits[n+1]; ++p){
            // Permutation: index in the Hnet_group to index in the row
            index_t cur_cell = permutation[nets.pins[p].cell_index];
            if(cur_cell < fst_c){
                fst_c = cur_cell;
                fst_pin_offs_mn = nets.pins[p].offset.min;
                fst_pin_offs_mx = nets.pins[p].offset.max;
            }
            if(cur_cell >= lst_c){
                lst_c = cur_cell;
                lst_pin_offs_mn = nets.pins[p].offset.min;
                lst_pin_offs_mx = nets.pins[p].offset.max;
            }
        }
        if(cur_net.has_ext_pins){
            unflipped_cost_functions[fst_c].add_bislope(-cur_net.weight, 0, cur_net.ext_pins.min - fst_pin_offs_mn);
            unflipped_cost_functions[lst_c].add_bislope(0,  cur_net.weight, cur_net.ext_pins.max - lst_pin_offs_mx);
            flipped_cost_functions[fst_c].add_bislope(-cur_net.weight, 0, cur_net.ext_pins.min - loc_widths[fst_c] + fst_pin_offs_mx);
            flipped_cost_functions[lst_c].add_bislope(0,  cur_net.weight, cur_net.ext_pins.max - loc_widths[lst_c] + lst_pin_offs_mn);
        }
        else{
            unflipped_cost_functions[fst_c].add_monotone(-cur_net.weight, -fst_pin_offs_mn);
            unflipped_cost_functions[lst_c].add_monotone( cur_net.weight, -lst_pin_offs_mx);
            flipped_cost_functions[fst_c].add_monotone(-cur_net.weight, fst_pin_offs_mx - loc_widths[fst_c] );
            flipped_cost_functions[lst_c].add_monotone( cur_net.weight, lst_pin_offs_mn - loc_widths[lst_c] );
        }
    }

    std::vector<piecewise_linear_function> prev_mins, merged_costs;
    for(index_t i=0; i<loc_ranges.size(); ++i){
        merged_costs.push_back(loc_flipps[i] ?
            piecewise_linear_function::minimum(unflipped_cost_functions[i], flipped_cost_functions[i])
          : unflipped_cost_functions[i]
        );

        if(i>0){
            prev_mins.push_back(prev_mins.back().previous_min_of_sum(merged_costs.back(), loc_widths[i-1]));
        }
        else{
            prev_mins.push_back(merged_costs.back().previous_min());
        }
    }

    for(auto const M : prev_mins){
        for(index_t i=0; i+1<M.point_values.size(); ++i){
            assert(M.point_values[i].second >= M.point_values[i+1].second);
        }
    }

    flippings.resize(cell_ranges.size(), 0); positions.resize(cell_ranges.size(), 0);

    int_t pos = std::numeric_limits<int_t>::max();
    for(index_t i=loc_ranges.size(); i>0; --i){
        // Find the best position and flipping for each cell
        pos = prev_mins[i-1].last_before(std::min(pos - loc_widths[i-1], loc_ranges[i-1].second) );
        positions[i-1] = pos;

        if(loc_flipps[i-1] and flipped_cost_functions[i-1].value_at(pos) < unflipped_cost_functions[i-1].value_at(pos)){
            flippings[i-1] = 1;
        }
    }

    for(index_t i=0; i<loc_ranges.size(); ++i){
        assert(positions[i] >= loc_ranges[i].first);
        assert(positions[i] <= loc_ranges[i].second);
    }
    for(index_t i=0; i+1<loc_ranges.size(); ++i){
        assert(positions[i] + loc_widths[i] <= positions[i+1]);
    }

    auto permuted_positions = positions;
    auto permuted_flippings = flippings;
    for(index_t i=0; i<permutation.size(); ++i){
        permuted_positions[i] = positions[permutation[i]];
        permuted_flippings[i] = flippings[permutation[i]];
    }

    return nets.get_cost(permuted_positions, permuted_flippings);
}

std::vector<std::pair<int_t, int_t> > get_cell_ranges(netlist const & circuit, detailed_placement const & pl, std::vector<index_t> const & cells){
    std::vector<std::pair<int_t, int_t> > lims;

    for(index_t i=0; i+1<cells.size(); ++i){
        assert(pl.plt_.positions_[cells[i]].x_ + circuit.get_cell(cells[i]).size.x_ <= pl.plt_.positions_[cells[i+1]].x_);
    }

    // Extreme limits, except macros are allowed to be beyond the limit of the placement area
    int_t lower_lim = pl.get_limit_positions(circuit, cells.front()).first;
    int_t upper_lim = pl.get_limit_positions(circuit, cells.back()).second;

    for(index_t OSRP_cell : cells){
        auto attr = circuit.get_cell(OSRP_cell).attributes;
        auto cur_lim = std::pair<int_t, int_t>(lower_lim, upper_lim);
        int_t pos = pl.plt_.positions_[OSRP_cell].x_;
        if( (attr & XMovable) == 0 or pl.cell_height(OSRP_cell) != 1){
            cur_lim = std::pair<int_t, int_t>(pos, pos + circuit.get_cell(OSRP_cell).size.x_);
        }
	    else{
            assert(pos >= lower_lim);
            assert(pos + circuit.get_cell(OSRP_cell).size.x_ <= upper_lim);
        }
        lims.push_back(cur_lim);
    }

    return lims;
}

template<bool NON_CONVEX, bool RSMT>
void OSRP_generic(netlist const & circuit, detailed_placement & pl){
    for(index_t r=0; r<pl.row_cnt(); ++r){
        // Complete optimization on a row, comprising possible obstacles

        std::vector<index_t> cells;
        std::vector<int> flippability;

        // Get the movable cells, if we can flip them, and the obstacles on the row
        for(index_t OSRP_cell = pl.get_first_cell_on_row(r); OSRP_cell != null_ind; OSRP_cell = pl.get_next_cell_on_row(OSRP_cell, r)){
            auto attr = circuit.get_cell(OSRP_cell).attributes;
            cells.push_back(OSRP_cell);
            flippability.push_back( (attr & XFlippable) != 0 ? 1 : 0);
        }

        if(not cells.empty()){
            std::vector<std::pair<int_t, int_t> > lims = get_cell_ranges(circuit, pl, cells); // Limit positions for each cell

            Hnet_group nets = RSMT ?
                get_RSMT_netgroup(circuit, pl, cells)
             :  get_B2B_netgroup(circuit, pl, cells);

            std::vector<index_t> no_permutation(cells.size());
            for(index_t i=0; i<cells.size(); ++i) no_permutation[i] = i;

            std::vector<int_t> final_positions;
            if(NON_CONVEX){
                std::vector<int> flipped;
                optimize_noncvx_sequence(nets, no_permutation, final_positions, flipped, flippability, lims);
                for(index_t i=0; i<cells.size(); ++i){
                    bool old_orient = pl.plt_.orientations_[cells[i]].x_;
                    pl.plt_.orientations_[cells[i]].x_ = flipped[i] ? not old_orient : old_orient;
                }
            }
            else{
                optimize_convex_sequence(nets, no_permutation, final_positions, lims);
            }

            // Update the positions and orientations
            for(index_t i=0; i<cells.size(); ++i){
                pl.plt_.positions_[cells[i]].x_ = final_positions[i];
            }
        }
    } // Iteration on the rows

    pl.selfcheck();
}

template<bool NON_CONVEX, bool RSMT>
void swaps_row_generic(netlist const & circuit, detailed_placement & pl, index_t range){
    assert(range >= 2);

    for(index_t r=0; r<pl.row_cnt(); ++r){
        index_t OSRP_cell = pl.get_first_cell_on_row(r);

        while(OSRP_cell != null_ind){
            std::vector<index_t> cells;
            std::vector<std::pair<int_t, int_t> > lims;
            std::vector<int> flippables;

            for(index_t nbr_cells=0;
                    OSRP_cell != null_ind
                and nbr_cells < range;
                OSRP_cell = pl.get_next_cell_on_row(OSRP_cell, r), ++nbr_cells
            ){
                cells.push_back(OSRP_cell);
                flippables.push_back( (circuit.get_cell(OSRP_cell).attributes & XFlippable) != 0);
            }

            if(not cells.empty()){
                std::vector<std::pair<int_t, int_t> > lims = get_cell_ranges(circuit, pl, cells); // Limit positions for each cell

                Hnet_group nets = RSMT ?
                    get_RSMT_netgroup(circuit, pl, cells)
                 :  get_B2B_netgroup(circuit, pl, cells);

                std::int64_t best_cost = std::numeric_limits<std::int64_t>::max();
                std::vector<int_t> positions(cells.size());
                std::vector<int>   flippings(cells.size());
                std::vector<int_t> best_positions(cells.size());
                std::vector<int>   best_flippings(cells.size());

                std::vector<index_t> permutation(cells.size());
                for(index_t i=0; i<cells.size(); ++i) permutation[i] = i;
                std::vector<index_t> best_permutation;

                // Check every possible permutation of the cells
                do{
                    std::int64_t cur_cost = NON_CONVEX ?
                        optimize_noncvx_sequence(nets, permutation, positions, flippings, flippables, lims) :
                        optimize_convex_sequence(nets, permutation, positions, lims);
                    if(cur_cost <= best_cost){
                        best_cost = cur_cost;
                        best_permutation = permutation;
                        best_flippings = flippings;
                        best_positions = positions;
                    }
                }while(std::next_permutation(permutation.begin(), permutation.end()));

                std::vector<index_t> new_cell_order(cells.size());
                // Update the positions and the topology
                for(index_t i=0; i<cells.size(); ++i){
                    index_t r_ind = best_permutation[i]; // In the row from in the Hnet_group
                    new_cell_order[r_ind] = cells[i];
                    pl.plt_.positions_[cells[i]].x_ = best_positions[r_ind];
                    if(NON_CONVEX){
                        bool old_orient = pl.plt_.orientations_[cells[i]].x_;
                        pl.plt_.orientations_[cells[i]].x_ = best_flippings[r_ind] ? not old_orient : old_orient;
                    }
                }

                pl.reorder_cells(cells, new_cell_order, r);
                cells = new_cell_order;

                assert(best_cost < std::numeric_limits<std::int64_t>::max());
            }
    
            if(OSRP_cell != null_ind){
                assert(cells.size() == range);
                OSRP_cell = cells[range/2];
            }
        } // Iteration on the entire row
    } // Iteration on the rows

    pl.selfcheck();
}
} // End anonymous namespace

void OSRP_convex_HPWL(netlist const & circuit, detailed_placement & pl){ OSRP_generic< false, false>(circuit, pl); }
void OSRP_convex_RSMT(netlist const & circuit, detailed_placement & pl){ OSRP_generic< false, true >(circuit, pl); }
void OSRP_noncvx_HPWL(netlist const & circuit, detailed_placement & pl){ OSRP_generic< true , false>(circuit, pl); }
void OSRP_noncvx_RSMT(netlist const & circuit, detailed_placement & pl){ OSRP_generic< true , true >(circuit, pl); }
void swaps_row_convex_HPWL(netlist const & circuit, detailed_placement & pl, index_t range){ swaps_row_generic< false, false>(circuit, pl, range); }
void swaps_row_convex_RSMT(netlist const & circuit, detailed_placement & pl, index_t range){ swaps_row_generic< false, true >(circuit, pl, range); }
void swaps_row_noncvx_HPWL(netlist const & circuit, detailed_placement & pl, index_t range){ swaps_row_generic< true , false>(circuit, pl, range); }
void swaps_row_noncvx_RSMT(netlist const & circuit, detailed_placement & pl, index_t range){ swaps_row_generic< true , true >(circuit, pl, range); }

} // namespace dp
} // namespace coloquinte


