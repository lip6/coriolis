
#include "coloquinte/circuit_helper.hxx"
#include "coloquinte/circuit.hxx"

namespace coloquinte{

std::int64_t get_HPWL_length(netlist const & circuit, placement_t const & pl, index_t net_ind){
    if(circuit.get_net(net_ind).pin_cnt <= 1) return 0;

    auto pins = get_pins_1D(circuit, pl, net_ind);
    auto minmaxX = std::minmax_element(pins.x_.begin(), pins.x_.end()), minmaxY = std::minmax_element(pins.y_.begin(), pins.y_.end());
    return ((minmaxX.second->pos - minmaxX.first->pos) + (minmaxY.second->pos - minmaxY.first->pos));
}

std::int64_t get_RSMT_length(netlist const & circuit, placement_t const & pl, index_t net_ind){
    if(circuit.get_net(net_ind).pin_cnt <= 1) return 0;
    auto pins = get_pins_2D(circuit, pl, net_ind);
    std::vector<point<int_t> > points;
    for(pin_2D const p : pins){
        points.push_back(p.pos);
    }
    return RSMT_length(points, 8);
}

namespace gp{

void add_force(pin_1D const p1, pin_1D const p2, linear_system & L, float_t force){
    if(p1.movable && p2.movable){
        L.add_force(
            force,
            p1.cell_ind, p2.cell_ind,
            p1.offs,     p2.offs
        );
    }
    else if(p1.movable){
        L.add_fixed_force(
            force,
            p1.cell_ind,
            p2.pos,
            p1.offs
        );
    }
    else if(p2.movable){
        L.add_fixed_force(
            force,
            p2.cell_ind,
            p1.pos,
            p2.offs
        );
    }
}

void add_force(pin_1D const p1, pin_1D const p2, linear_system & L, float_t tol, float_t scale){
    add_force(p1, p2, L, scale/std::max(tol, static_cast<float_t>(std::abs(p2.pos-p1.pos))));
}

point<linear_system> empty_linear_systems(netlist const & circuit, placement_t const & pl){
    point<linear_system> ret = point<linear_system>(linear_system(circuit.cell_cnt()), linear_system(circuit.cell_cnt()));

    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        bool found_true_net=false;
        for(auto p : circuit.get_cell(i)){
            if(circuit.get_net(p.net_ind).pin_cnt > 1){
                found_true_net = true;
                break;
            }
        }

        if( (XMovable & circuit.get_cell(i).attributes) == 0 or not found_true_net){
            ret.x_.add_triplet(i, i, 1.0f);
            ret.x_.add_doublet(i, pl.positions_[i].x_);
        }
        if( (YMovable & circuit.get_cell(i).attributes) == 0 or not found_true_net){
            ret.y_.add_triplet(i, i, 1.0f);
            ret.y_.add_doublet(i, pl.positions_[i].y_);
        }
    }

    return ret;
}

namespace{ // Anonymous namespace for helper functions

void get_HPWLF(std::vector<pin_1D> const & pins, linear_system & L, float_t tol){
    if(pins.size() >= 2){
        auto min_elt = std::min_element(pins.begin(), pins.end()), max_elt = std::max_element(pins.begin(), pins.end());

        for(auto it = pins.begin(); it != pins.end(); ++it){
            // Just comparing the iterator is poorer due to redundancies in the benchmarks!
            if(it != min_elt){
                add_force(*it, *min_elt, L, tol, 1.0f/(pins.size()-1));
                if(it != max_elt){ // Hopefully only one connexion between the min and max pins
                    add_force(*it, *max_elt, L, tol, 1.0f/(pins.size()-1));
                }
            }
        }
    }
}

void get_HPWLR(std::vector<pin_1D> const & pins, linear_system & L, float_t tol){
    std::vector<pin_1D> sorted_pins = pins;
    std::sort(sorted_pins.begin(), sorted_pins.end());
    // Pins are connected to the pin two places away
    for(index_t i=0; i+2<sorted_pins.size(); ++i){
        add_force(sorted_pins[i], sorted_pins[i+2], L, tol, 0.5f);
    }
    // The extreme pins are connected with their direct neighbour too
    if(sorted_pins.size() > 1){
        add_force(sorted_pins[0], sorted_pins[1], L, tol, 0.5f);
        add_force(sorted_pins[sorted_pins.size()-1], sorted_pins[sorted_pins.size()-2], L, tol, 0.5f);
    }
}

void get_star(std::vector<pin_1D> const & pins, linear_system & L, float_t tol, index_t star_index){
    // The net is empty, but we still populate the diagonal to avoid divide by zeros
    if(pins.size() < 2){
        L.add_triplet(star_index, star_index, 1.0f);
        return;
    }

    for(pin_1D p : pins){
        pin_1D star_pin = pin_1D(star_index, 0, 0, true);
        add_force(p, star_pin, L, 1.0/pins.size());
    }
}

void get_clique(std::vector<pin_1D> const & pins, linear_system & L, float_t tol){
    // Pins are connected to the pin two places away
    for(index_t i=0; i+1<pins.size(); ++i){
        for(index_t j=i+1; j<pins.size(); ++j){
            add_force(pins[i], pins[j], L, tol, 1.0f/(pins.size()-1));
        }
    }
}

} // End anonymous namespace

point<linear_system> get_HPWLF_linear_system (netlist const & circuit, placement_t const & pl, float_t tol, index_t min_s, index_t max_s){
    point<linear_system> L = empty_linear_systems(circuit, pl);
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        // Has the net the right pin count?
        index_t pin_cnt = circuit.get_net(i).pin_cnt;
        if(pin_cnt < min_s or pin_cnt >= max_s) continue;

        auto pins = get_pins_1D(circuit, pl, i);
        get_HPWLF(pins.x_, L.x_, tol);
        get_HPWLF(pins.y_, L.y_, tol);
    }
    return L;
}

point<linear_system> get_HPWLR_linear_system (netlist const & circuit, placement_t const & pl, float_t tol, index_t min_s, index_t max_s){
    point<linear_system> L = empty_linear_systems(circuit, pl);
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        // Has the net the right pin count?
        index_t pin_cnt = circuit.get_net(i).pin_cnt;
        if(pin_cnt < min_s or pin_cnt >= max_s) continue;

        auto pins = get_pins_1D(circuit, pl, i);
        get_HPWLR(pins.x_, L.x_, tol);
        get_HPWLR(pins.y_, L.y_, tol);
    }
    return L;
}

point<linear_system> get_star_linear_system  (netlist const & circuit, placement_t const & pl, float_t tol, index_t min_s, index_t max_s){
    point<linear_system> L = empty_linear_systems(circuit, pl);
    L.x_.add_variables(circuit.net_cnt());
    L.y_.add_variables(circuit.net_cnt());
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        // Has the net the right pin count?
        index_t pin_cnt = circuit.get_net(i).pin_cnt;
        if(pin_cnt < min_s or pin_cnt >= max_s){
            // Put a one in the intermediate variable in order to avoid non-invertible matrices
            L.x_.add_triplet(i+circuit.cell_cnt(), i+circuit.cell_cnt(), 1.0f);
            L.y_.add_triplet(i+circuit.cell_cnt(), i+circuit.cell_cnt(), 1.0f);
            continue;
        }

        auto pins = get_pins_1D(circuit, pl, i);
        // Provide the index of the star's central pin in the linear system
        get_star(pins.x_, L.x_, tol, i+circuit.cell_cnt());
        get_star(pins.y_, L.y_, tol, i+circuit.cell_cnt());
    }
    return L;
}

point<linear_system> get_clique_linear_system (netlist const & circuit, placement_t const & pl, float_t tol, index_t min_s, index_t max_s){
    point<linear_system> L = empty_linear_systems(circuit, pl);
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        // Has the net the right pin count?
        index_t pin_cnt = circuit.get_net(i).pin_cnt;
        if(pin_cnt < min_s or pin_cnt >= max_s) continue;

        auto pins = get_pins_1D(circuit, pl, i);
        get_clique(pins.x_, L.x_, tol);
        get_clique(pins.y_, L.y_, tol);
    }
    return L;
}

point<linear_system> get_MST_linear_system(netlist const & circuit, placement_t const & pl, float_t tol, index_t min_s, index_t max_s){
    point<linear_system> L = empty_linear_systems(circuit, pl);
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        // Has the net the right pin count?
        index_t pin_cnt = circuit.get_net(i).pin_cnt;
        if(pin_cnt < min_s or pin_cnt >= max_s or pin_cnt <= 1) continue;
            
        auto pins = get_pins_2D(circuit, pl, i);
        std::vector<point<int_t> > points;
        for(pin_2D const p : pins){
            points.push_back(p.pos);
        }
        auto const edges = get_MST_topology(points);
        for(auto E : edges){
            add_force(pins[E.first].x(), pins[E.second].x(), L.x_, tol, 1.0f);
            add_force(pins[E.first].y(), pins[E.second].y(), L.y_, tol, 1.0f);
        }
    }
    return L;
}

point<linear_system> get_RSMT_linear_system(netlist const & circuit, placement_t const & pl, float_t tol, index_t min_s, index_t max_s){
    point<linear_system> L = empty_linear_systems(circuit, pl);
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        // Has the net the right pin count?
        index_t pin_cnt = circuit.get_net(i).pin_cnt;
        if(pin_cnt < min_s or pin_cnt >= max_s or pin_cnt <= 1) continue;
            
        auto pins = get_pins_2D(circuit, pl, i);
        std::vector<point<int_t> > points;
        for(pin_2D const p : pins){
            points.push_back(p.pos);
        }
        auto const edges = get_RSMT_topology(points, 8);
        for(auto E : edges.x_){
            add_force(pins[E.first].x(), pins[E.second].x(), L.x_, tol, 1.0f);
        }
        for(auto E : edges.y_){
            add_force(pins[E.first].y(), pins[E.second].y(), L.y_, tol, 1.0f);
        }
    }
    return L;
}


std::int64_t get_HPWL_wirelength(netlist const & circuit, placement_t const & pl){
    std::int64_t sum = 0;
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        sum += get_HPWL_length(circuit, pl, i);
    }
    return sum;
}

// The true wirelength with minimum spanning trees, except for very small nets (<= 3) where we have HPWL == true WL
std::int64_t get_MST_wirelength(netlist const & circuit, placement_t const & pl){
    std::int64_t sum = 0;
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        auto pins = get_pins_2D(circuit, pl, i);
        std::vector<point<int_t> > points;
        for(pin_2D const p : pins){
            points.push_back(p.pos);
        }
        sum += MST_length(points);
    }
    return sum;
}

std::int64_t get_RSMT_wirelength(netlist const & circuit, placement_t const & pl){
    std::int64_t sum = 0;
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        sum += get_RSMT_length(circuit, pl, i);
    }
    return sum;
}

void solve_linear_system(netlist const & circuit, placement_t & pl, point<linear_system> & L, index_t nbr_iter){
    std::vector<float_t> x_sol, y_sol;
    std::vector<float_t> x_guess(pl.cell_cnt()), y_guess(pl.cell_cnt());
    
    assert(L.x_.internal_size() == x_guess.size());
    assert(L.y_.internal_size() == y_guess.size());

    for(index_t i=0; i<pl.cell_cnt(); ++i){
        x_guess[i] = static_cast<float_t>(pl.positions_[i].x_);
        y_guess[i] = static_cast<float_t>(pl.positions_[i].y_);
    }
    #pragma omp parallel sections num_threads(2)
    {
    #pragma omp section
    x_sol = L.x_.solve_CG(x_guess, nbr_iter);
    #pragma omp section
    y_sol = L.y_.solve_CG(y_guess, nbr_iter);
    }
    for(index_t i=0; i<pl.cell_cnt(); ++i){
        if( (circuit.get_cell(i).attributes & XMovable) != 0){
            assert(std::isfinite(x_sol[i]));
            pl.positions_[i].x_ = static_cast<int_t>(x_sol[i]);
        }
        if( (circuit.get_cell(i).attributes & YMovable) != 0){
            assert(std::isfinite(y_sol[i]));
            pl.positions_[i].y_ = static_cast<int_t>(y_sol[i]);
        }
    }
}

// Intended to be used by pulling forces to adapt the forces to the cell's areas
std::vector<float_t> get_area_scales(netlist const & circuit){
    std::vector<float_t> ret(circuit.cell_cnt());
    capacity_t int_tot_area = 0;
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        capacity_t A = circuit.get_cell(i).area;
        ret[i] = static_cast<float_t>(A);
        int_tot_area += A;
    }
    float_t inv_average_area = circuit.cell_cnt() / static_cast<float_t>(int_tot_area);
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        ret[i] *= inv_average_area;
    }
    return ret;
}

point<linear_system> get_pulling_forces (netlist const & circuit, placement_t const & pl, float_t typical_distance){
    point<linear_system> L = empty_linear_systems(circuit, pl);
    float_t typical_force = 1.0f / typical_distance;
    std::vector<float_t> scaling = get_area_scales(circuit);
    for(index_t i=0; i<pl.cell_cnt(); ++i){
        L.x_.add_anchor(
            typical_force * scaling[i],
            i, pl.positions_[i].x_
        );
        L.y_.add_anchor(
            typical_force * scaling[i],
            i, pl.positions_[i].y_
        );
    }
    
    return L;
}

point<linear_system> get_linear_pulling_forces (netlist const & circuit, placement_t const & UB_pl, placement_t const & LB_pl, float_t force, float_t min_distance){
    point<linear_system> L = empty_linear_systems(circuit, UB_pl);
    assert(LB_pl.cell_cnt() == UB_pl.cell_cnt());
    std::vector<float_t> scaling = get_area_scales(circuit);
    for(index_t i=0; i<LB_pl.cell_cnt(); ++i){
        L.x_.add_anchor(
            force * scaling[i] / (std::max(static_cast<float_t>(std::abs(UB_pl.positions_[i].x_ - LB_pl.positions_[i].x_)), min_distance)),
            i, UB_pl.positions_[i].x_
        );
        L.y_.add_anchor(
            force * scaling[i] / (std::max(static_cast<float_t>(std::abs(UB_pl.positions_[i].y_ - LB_pl.positions_[i].y_)), min_distance)),
            i, UB_pl.positions_[i].y_
        );
    }
    

    return L;
}

region_distribution get_rough_legalizer(netlist const & circuit, placement_t const & pl, box<int_t> surface){
    return region_distribution::uniform_density_distribution(surface, circuit, pl);
}

void get_rough_legalization(netlist const & circuit, placement_t & pl, region_distribution const & legalizer){
    auto exportation = legalizer.export_spread_positions_linear();
    for(auto const C : exportation){
        pl.positions_[C.index_in_placement_] = static_cast<point<int_t> >(C.pos_ - 0.5f * static_cast<point<float_t> >(circuit.get_cell(C.index_in_placement_).size));
    }
}

float_t get_mean_linear_disruption(netlist const & circuit, placement_t const & LB_pl, placement_t const & UB_pl){
    float_t tot_cost = 0.0;
    float_t tot_area = 0.0;
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        float_t area = static_cast<float_t>(circuit.get_cell(i).area);
        point<int_t> diff = LB_pl.positions_[i] - UB_pl.positions_[i];

        if( (circuit.get_cell(i).attributes & XMovable) == 0) assert(diff.x_ == 0);
        if( (circuit.get_cell(i).attributes & YMovable) == 0) assert(diff.y_ == 0);

        tot_cost += area * (std::abs(diff.x_) + std::abs(diff.y_));
        tot_area += area;
    }
    return tot_cost / tot_area;
}

float_t get_mean_quadratic_disruption(netlist const & circuit, placement_t const & LB_pl, placement_t const & UB_pl){
    float_t tot_cost = 0.0;
    float_t tot_area = 0.0;
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        float_t area = static_cast<float_t>(circuit.get_cell(i).area);
        point<int_t> diff = LB_pl.positions_[i] - UB_pl.positions_[i];

        if( (circuit.get_cell(i).attributes & XMovable) == 0) assert(diff.x_ == 0);
        if( (circuit.get_cell(i).attributes & YMovable) == 0) assert(diff.y_ == 0);

        float_t manhattan = (std::abs(diff.x_) + std::abs(diff.y_));
        tot_cost += area * manhattan * manhattan;
        tot_area += area;
    }
    return std::sqrt(tot_cost / tot_area);
}

} // namespace gp
} // namespace coloquinte


