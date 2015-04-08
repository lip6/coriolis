
#include "coloquinte/circuit.hxx"

#include <map>

namespace coloquinte{

void netlist::selfcheck() const{
    index_t cell_cnt = cell_areas_.size();
    assert(cell_cnt+1 == cell_limits_.size());
    assert(cell_cnt == cell_sizes_.size());
    assert(cell_cnt == cell_attributes_.size());
    assert(cell_cnt == cell_internal_mapping_.size());

    index_t net_cnt = net_weights_.size();
    assert(net_cnt+1 == net_limits_.size());
    assert(net_cnt == net_internal_mapping_.size());

    index_t pin_cnt = pin_offsets_.size();
    assert(pin_cnt == cell_indexes_.size());
    assert(pin_cnt == pin_indexes_.size());
    assert(pin_cnt == net_indexes_.size());

    for(auto const p : pin_offsets_){
        assert(std::isfinite(p.x_) and std::isfinite(p.y_));
    }
}

// For compatibility reasons
void placement_t::selfcheck() const{
}

void verify_placement_legality(netlist const & circuit, placement_t const & pl, box<int_t> surface){
    std::vector<box<int_t> > cells;
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        auto S = circuit.get_cell(i).size;
        cells.push_back(box<int_t>(pl.positions_[i], pl.positions_[i] + S));

        // Verify that they are within the placement surface; doesn't take fixed macros into account
        if( (circuit.get_cell(i).attributes & XMovable) != 0 or (circuit.get_cell(i).attributes & YMovable) != 0){
            assert(cells[i].in(surface));
        }
    }

    // Simple sweepline algorithm to verify that there is no overlap
    struct event{
        int_t x_min, x_max, y;
        index_t cell;
        bool removal;
        bool operator<(event const o) const{
            return y < o.y
            or (y == o.y and removal and not o.removal); // Remove before inserting
        }
    };

    std::vector<event> all_events;
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        event b, e;
        b.cell = i; e.cell = i;
        b.x_min = cells[i].x_min_; e.x_min = cells[i].x_min_;
        b.x_max = cells[i].x_max_; e.x_max = cells[i].x_max_;
        b.y = cells[i].y_min_; b.removal = false;
        e.y = cells[i].y_max_; e.removal = true;
        if(b.x_max > b.x_min and e.y != b.y){
            all_events.push_back(b);
            all_events.push_back(e);
        }
    }

    std::sort(all_events.begin(), all_events.end());

    // Indexed by beginning of interval, with end of interval and cell within
    std::map<int_t, std::pair<int_t, index_t> > active_rectangles;

    for(event E : all_events){
        if(E.removal){
            auto it = active_rectangles.find(E.x_min);
            assert(it != active_rectangles.end());
            active_rectangles.erase(it);
        }
        else{ // Find anything that intersects with E; if not, add it
            auto it = active_rectangles.lower_bound(E.x_min); // First interval after
            if(it != active_rectangles.end()){
                assert(it->first >= E.x_max); //Intersection between E.cell and it->second->second
            }
            if(it != active_rectangles.begin()){
                --it;
                assert(it->second.first <= E.x_min); //Intersection between E.cell and it->second->second
            }
            active_rectangles.insert(std::pair<int_t, std::pair<int_t, index_t> >(E.x_min, std::pair<int_t, index_t>(E.x_max, E.cell)));
        }
    }

}

} // namespace coloquinte
