
#include "coloquinte/rough_legalizers.hxx"
#include "coloquinte/optimization_subproblems.hxx"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <map>

namespace coloquinte{
namespace gp{

namespace{
    const capacity_t default_density_mul = 256;
}

void region_distribution::just_uniquify(std::vector<cell_ref> & cell_references){
    if(cell_references.size() >= 1){
        index_t j=0;
        cell_ref prev_cell = cell_references[0];
        for(auto it = cell_references.begin()+1; it != cell_references.end(); ++it){
            if(it->index_in_list_ == prev_cell.index_in_list_){
                prev_cell.allocated_capacity_ += it->allocated_capacity_;
            }
            else{
                cell_references[j] = prev_cell;
                ++j;
                prev_cell = *it;
            }
        }
        cell_references[j]=prev_cell;
        cell_references.resize(j+1);
    }
}

void region_distribution::sort_uniquify(std::vector<cell_ref> & cell_references){
    std::sort(cell_references.begin(), cell_references.end(), [](cell_ref a, cell_ref b){ return a.index_in_list_ < b.index_in_list_; });
    just_uniquify(cell_references);
}

void region_distribution::region::uniquify_references(){
    sort_uniquify(cell_references_);
}

void region_distribution::fractions_minimization(){
    for(region & R : placement_regions_){
        R.uniquify_references();
    }

    // Find cycles of cut cells, then find a spanning tree to reallocate the cells
    // TODO
}

void region_distribution::region::selfcheck() const{
    capacity_t total_allocated = 0;
    for(cell_ref const c : cell_references_){
        total_allocated += c.allocated_capacity_;
        assert(c.allocated_capacity_ > 0);
    }
    assert(total_allocated <= capacity_);
}

void region_distribution::selfcheck() const{
    for(region const & R : placement_regions_){
        R.selfcheck();
    }
    std::vector<capacity_t> capacities(cell_list_.size(), 0);
    for(region const & R : placement_regions_){
        for(cell_ref const C : R.cell_references_){
            capacities[C.index_in_list_] += C.allocated_capacity_;
        }
    }
    for(index_t i=0; i < cell_list_.size(); ++i){
        assert(capacities[i] == cell_list_[i].demand_ * cell_density_mul);
    }
}

region_distribution::region::region(capacity_t cap, point<float_t> pos, std::vector<cell_ref> cells) : capacity_(cap), pos_(pos), cell_references_(cells){}

box<int_t> region_distribution::get_box(index_t x, index_t y, index_t x_cnt, index_t y_cnt) const{
    auto ret = box<int_t>(
        placement_area_.x_min_ + ( ((std::int64_t) (placement_area_.x_max_ - placement_area_.x_min_)) * x )     / x_cnt,
        placement_area_.x_min_ + ( ((std::int64_t) (placement_area_.x_max_ - placement_area_.x_min_)) * (x+1) ) / x_cnt,
        placement_area_.y_min_ + ( ((std::int64_t) (placement_area_.y_max_ - placement_area_.y_min_)) * y )     / y_cnt,
        placement_area_.y_min_ + ( ((std::int64_t) (placement_area_.y_max_ - placement_area_.y_min_)) * (y+1) ) / y_cnt
    );
    assert(not ret.empty());
    return ret;
}

std::vector<region_distribution::region> region_distribution::prepare_regions(index_t x_cnt, index_t y_cnt) const{
    assert(placement_area_.x_max_ > placement_area_.x_min_);
    assert(placement_area_.y_max_ > placement_area_.y_min_);

    // Uses a sweepline algorithm to initialize all regions' capacities at a time, taking macros and density maps into account

    // The events in the priority queue: basically a density_limit object, but the y_min_ may be different from the original one
    struct event{
        box<int_t> box_;
        capacity_t multiplicator_;
        bool operator<(event const o) const{
            return box_.y_min_ > o.box_.y_min_ // Priority queue = highest first
            or (box_.y_min_ == o.box_.y_min_ and multiplicator_ > o.multiplicator_); // Smallest density first, just because
        }
        event(box<int_t> surface, capacity_t den) : box_(surface), multiplicator_(den) {}
        event(density_limit D, capacity_t density_mul) : box_(D.box_) {
            multiplicator_ = static_cast<capacity_t>(std::ceil(D.density_ * static_cast<float_t>(density_mul)));
        }
    };

    struct line_y{
        int_t y_min_, y_max_;
        capacity_t multiplicator_;
        line_y(int_t mn, int_t mx, capacity_t cap) : y_min_(mn), y_max_(mx), multiplicator_(cap) {}
    };

    // The regions' capacities
    std::vector<capacity_t> region_caps(x_cnt * y_cnt, 0);

    std::vector<int_t> x_reg_lims(x_cnt+1), y_reg_lims(y_cnt+1);
    for(index_t i=0; i<=x_cnt; ++i){
        x_reg_lims[i] = placement_area_.x_min_ + ( ((std::int64_t) (placement_area_.x_max_ - placement_area_.x_min_)) * i ) / x_cnt;
    }
    for(index_t i=0; i<=y_cnt; ++i){
        y_reg_lims[i] = placement_area_.y_min_ + ( ((std::int64_t) (placement_area_.y_max_ - placement_area_.y_min_)) * i ) / y_cnt;
    }

    //std::vector<box<int_t> > added;

    auto add_region = [&](box<int_t> bx, capacity_t d){
        /*
        // Failed attempt at calculating the coordinates directly
        point<int_t> dims = placement_area_.dimensions();
        auto mins = point<int_t>(placement_area_.x_min_, placement_area_.y_min_);

        index_t x_mn = (static_cast<std::int64_t>(bx.x_min_ - mins.x_ + 1) * x_cnt) / dims.x_,
                x_mx = (static_cast<std::int64_t>(bx.x_max_ - mins.x_ - 1) * x_cnt) / dims.x_ + 1,
                y_mn = (static_cast<std::int64_t>(bx.y_min_ - mins.y_ + 1) * y_cnt) / dims.y_,
                y_mx = (static_cast<std::int64_t>(bx.y_max_ - mins.y_ - 1) * y_cnt) / dims.y_ + 1;
        */

        /*
        // Additional debugging
        for(index_t k=0; k<added.size(); ++k){
            assert(not bx.intersects(added[k]));
        }
        added.push_back(bx);
        */
    
        assert(bx.x_min_ >= placement_area_.x_min_);
        assert(bx.y_min_ >= placement_area_.y_min_);
        assert(bx.x_max_ <= placement_area_.x_max_);
        assert(bx.y_max_ <= placement_area_.y_max_);

        index_t x_mn = std::upper_bound(x_reg_lims.begin(), x_reg_lims.end(), bx.x_min_) - x_reg_lims.begin() -1,
                y_mn = std::upper_bound(y_reg_lims.begin(), y_reg_lims.end(), bx.y_min_) - y_reg_lims.begin() -1,
                x_mx = std::lower_bound(x_reg_lims.begin(), x_reg_lims.end(), bx.x_max_) - x_reg_lims.begin(),
                y_mx = std::lower_bound(y_reg_lims.begin(), y_reg_lims.end(), bx.y_max_) - y_reg_lims.begin();

        for(index_t x=x_mn; x<x_mx; ++x){
            for(index_t y=y_mn; y<y_mx; ++y){
                box<int_t> cur_box = get_box(x, y, x_cnt, y_cnt);
                assert(bx.intersects(cur_box));
                box<int_t> inter = bx.intersection(cur_box);
                point<int_t> dims = inter.dimensions();
                region_caps[y*x_cnt + x] += d * static_cast<capacity_t>(dims.x_) * static_cast<capacity_t>(dims.y_);
            }
        }
    };

    // All rectangles and new rectangles are pushed there
    std::priority_queue<event> events;

    for(density_limit D : density_map_){
        // Keep only the useful parts of the rectangles to simplify the algorithm
        if(D.box_.intersects(placement_area_)){
            density_limit pushed;
            pushed.density_ = D.density_;
            pushed.box_ = D.box_.intersection(placement_area_);
            assert(not pushed.box_.empty()); // always true with this definition of intersects
            events.push(event(pushed, full_density_mul));
        }
    }

    // The initial sweepline, with begin and end of the line
    std::map<int_t, line_y> active_obstacles;

    line_y placement_begin (placement_area_.y_min_, placement_area_.y_max_, full_density_mul),
           placement_end   (placement_area_.y_min_, placement_area_.y_max_, 0);

    active_obstacles.insert(std::pair<int_t, line_y>(placement_area_.x_min_, placement_begin)); // Full density placement area as initial object
    active_obstacles.insert(std::pair<int_t, line_y>(placement_area_.x_max_, placement_end));


    // Main loop: sweep the line on y (the line is horizontal, and moves toward bigger y)
    while(not events.empty()){
        event D = events.top();
        int_t x_b=D.box_.x_min_, x_e=D.box_.x_max_;
        int_t y_b=D.box_.y_min_, y_e=D.box_.y_max_;
        events.pop();

        assert(x_b >= placement_area_.x_min_);
        assert(y_b >= placement_area_.y_min_);
        assert(x_e <= placement_area_.x_max_);
        assert(y_e <= placement_area_.y_max_);

        // For each delimitation between the bounds of the new rectangle
        //      If the new delimitation has higher density or this delimitation ends on y there
        //          Remove it, push a new event if it ends on y after this rectangle
        //      Else:
        //          Keep it, push a new event if it ends on y before this rectangle

        // Find covered or partially covered rectangles on the line (rectangles on the line don't overlap)
        line_y const new_elt(y_b, y_e, D.multiplicator_);
        std::vector<line_y> new_delimitations;

        // First element on the line whose x_ is after our x_min (i.e. may have an intersection), and while there is an intersection
        auto first_it = active_obstacles.upper_bound(x_b);
        assert(first_it != active_obstacles.begin());
        assert(std::prev(first_it)->first <= x_b);

        bool currently_in = false;
        for(auto it = std::prev(first_it); it != active_obstacles.end() and it->first < x_e;){
            auto next_it = std::next(it);
            assert(next_it != active_obstacles.end());
            assert(it->second.y_min_ <= y_b);
            assert(it->second.y_max_ >= y_b);
            assert(it->first < x_e);
            assert(next_it->first > x_b);
            assert(next_it-> first > it->first);

            int_t x_c_min = std::max(x_b, it->first),
                  x_c_max = std::min(x_e, next_it->first);
            assert(x_c_min < x_c_max);
            // Add the area from it->second.y_min_ to D.box_.y_min_
            if(y_b > it->second.y_min_)
                add_region(box<int_t>(it->first, next_it->first, it->second.y_min_, y_b), it->second.multiplicator_);
            it->second.y_min_ = y_b; // Now the part before has been used

            auto part_b = *it, part_e = *next_it;

            if(part_b.second.multiplicator_ > D.multiplicator_ or part_b.second.y_max_ == y_b){ // The new event is visible now

                // In case parts of the line become visible again later
                if(part_b.second.y_max_ > y_e){ // Push back a new event to account for the comeback
                    events.push(event(box<int_t>(x_c_min, x_c_max, y_e, part_b.second.y_max_), part_b.second.multiplicator_));
                }

                // Depending whether this part of the line is or is not fully covered
                if(part_b.first >= x_b){ // If covered at the beginning
                    active_obstacles.erase(it);
                }
                if(part_e.first > x_e){ // If not covered at the end
                    auto inserted = active_obstacles.insert(std::pair<int_t, line_y>(x_e, part_b.second)); 
                    assert(inserted.second);
                }

                // The events becomes visible on the line
                if(not currently_in){
                    auto inserted = active_obstacles.insert(std::pair<int_t, line_y>(x_c_min, new_elt));
                    assert(inserted.second);
                }
                currently_in = true;
            }
            else{ // The new event is not visible yet
                currently_in = false;
                if(part_b.second.y_max_ < y_e){ // Push back a new event
                    events.push(event(box<int_t>(x_c_min, x_c_max, part_b.second.y_max_, y_e), D.multiplicator_));
                }
            }

            it = next_it;
        }
    }
    for(auto it=active_obstacles.begin(); std::next(it) != active_obstacles.end(); ++it){
        assert(it->second.y_max_ == placement_area_.y_max_);
        add_region(box<int_t>(it->first, std::next(it)->first, it->second.y_min_, it->second.y_max_), it->second.multiplicator_); 
    }

    std::vector<region> ret(x_cnt*y_cnt);
    for(index_t y=0; y<y_cnt; ++y){
        for(index_t x=0; x<x_cnt; ++x){
            box<int_t> bx = get_box(x, y, x_cnt, y_cnt);
            ret[y*x_cnt + x] = region(
                region_caps[y*x_cnt + x],
                point<float_t>(0.5f * bx.x_min_ + 0.5f * bx.x_max_, 0.5f * bx.y_min_ + 0.5f * bx.y_max_),
                std::vector<cell_ref>()
            );
        }
    }
    return ret;
}

void region_distribution::x_bipartition(){
    std::vector<region> old_placement_regions = prepare_regions(2*x_regions_cnt(), y_regions_cnt());
    placement_regions_.swap(old_placement_regions);

    index_t old_x_regions_cnt = x_regions_cnt();
    index_t old_y_regions_cnt = y_regions_cnt();
    x_regions_cnt_ *= 2;

    for(index_t x=0; x < old_x_regions_cnt; ++x){
        for(index_t y=0; y < old_y_regions_cnt; ++y){
            index_t i = y * old_x_regions_cnt + x;
            old_placement_regions[i].distribute_cells(get_region(2*x, y), get_region(2*x+1, y));
        }
    }
}

void region_distribution::y_bipartition(){
    std::vector<region> old_placement_regions = prepare_regions(x_regions_cnt(), 2*y_regions_cnt());
    placement_regions_.swap(old_placement_regions);

    index_t old_x_regions_cnt = x_regions_cnt();
    index_t old_y_regions_cnt = y_regions_cnt();
    y_regions_cnt_ *= 2;

    for(index_t x=0; x < old_x_regions_cnt; ++x){
        for(index_t y=0; y < old_y_regions_cnt; ++y){
            index_t i = y * old_x_regions_cnt + x;
            old_placement_regions[i].distribute_cells(get_region(x, 2*y), get_region(x, 2*y+1));
        }
    }
}

// The big awful function that handles optimal cell distribution between two regions; not meant to be called externally
void region_distribution::region::distribute_new_cells(region & region_a, region & region_b, std::vector<cell_ref> basic_cells){
    struct cost_diff_cell : cell_ref{
        float_t marginal_cost_;

        bool operator<(cost_diff_cell const o) const{ return marginal_cost_ < o.marginal_cost_; }
        cost_diff_cell(cell_ref cell, float_t cost) : cell_ref(cell), marginal_cost_(cost) {}
    };

    std::vector<cost_diff_cell> cells;
    for(cell_ref const c : basic_cells){
        cells.push_back(cost_diff_cell(c, region_a.distance(c) - region_b.distance(c)));
    }

    // Cells trending toward a first
    std::sort(cells.begin(), cells.end());//, [](cell_ref const c1, cell_ref const c2) -> bool{ return c1.marginal_cost_ < c2.marginal_cost_; });

    index_t preference_limit=0,         // First cell that would rather go to b (or cells.size())
         a_capacity_limit=0,            // After the last cell that region_a can take entirely (or 0)
         b_capacity_limit=cells.size(); // Last cell (but first in the vector) that region_b can take entirely (or cells.size())

    capacity_t remaining_capacity_a = region_a.capacity_, remaining_capacity_b = region_b.capacity_;
    for(;preference_limit < cells.size() && cells[preference_limit].marginal_cost_ <= 0.0; ++preference_limit);

    { // Block
    capacity_t remaining_cap_a = region_a.capacity_;
    index_t i=0;
    while(i<cells.size()){
        remaining_cap_a -= cells[i].allocated_capacity_;
        if(remaining_cap_a >= 0){
            a_capacity_limit = i+1;
            remaining_capacity_a = remaining_cap_a;
        }
        ++i;
    }
    } // Block

    { // Block
    capacity_t remaining_cap_b = region_b.capacity_;
    index_t i=cells.size();
    while(i>0){
        --i;
        remaining_cap_b -= cells[i].allocated_capacity_;
        if(remaining_cap_b >= 0){
            b_capacity_limit = i;
            remaining_capacity_b = remaining_cap_b;
        }
    }
    } // Block

    std::vector<cell_ref> & cells_a_side = region_a.cell_references_;
    std::vector<cell_ref> & cells_b_side = region_b.cell_references_;
    cells_a_side.clear();
    cells_b_side.clear();
    if(preference_limit >= b_capacity_limit and preference_limit <= a_capacity_limit){
        cells_a_side.insert(cells_a_side.end(), cells.begin(), cells.begin() + preference_limit);
        cells_b_side.insert(cells_b_side.end(), cells.begin() + preference_limit, cells.end());
    }
    else{
        index_t cut_position;
        capacity_t allocated_to_a_part;

        // Where do we cut?
        if(preference_limit < b_capacity_limit){ // Pack on b
            cut_position = b_capacity_limit-1; // Exists since preference_limit >= 0
            allocated_to_a_part = cells[cut_position].allocated_capacity_ - remaining_capacity_b;
        }
        else{ // Pack on a
        // if(preference_limit > a_capacity_limit)
            cut_position = a_capacity_limit; // Necessarily a correct position since preference_limits <= cells.size()
            allocated_to_a_part = remaining_capacity_a;
        }

        cells_a_side.reserve( cut_position             +1);
        cells_b_side.reserve(cells.size()-cut_position +1);

        cells_a_side.insert(cells_a_side.end(), cells.begin(), cells.begin() + cut_position);

        cell_ref cell_cut_a = cells[cut_position], cell_cut_b = cells[cut_position];
        cell_cut_a.allocated_capacity_ = allocated_to_a_part;
        cell_cut_b.allocated_capacity_ -= allocated_to_a_part;
        if(cell_cut_a.allocated_capacity_ > 0){ cells_a_side.push_back(cell_cut_a); }
        if(cell_cut_b.allocated_capacity_ > 0){ cells_b_side.push_back(cell_cut_b); }

        cells_b_side.insert(cells_b_side.end(), cells.begin() + cut_position+1, cells.end());
    }
}

void region_distribution::region::distribute_cells(region & a, region & b) const{
    distribute_new_cells(a, b, cell_references_);
    assert(a.allocated_capacity() + b.allocated_capacity() == allocated_capacity());
    assert(a.capacity() + b.capacity() == capacity());
}

void region_distribution::region::redistribute_cells(region & Ra, region & Rb){
    if(Ra.capacity() > 0 and Rb.capacity() > 0){
        std::vector<cell_ref> cells;
        cells.reserve(Ra.cell_references_.size()+Rb.cell_references_.size());
        cells.insert(cells.end(), Ra.cell_references_.begin(), Ra.cell_references_.end());
        cells.insert(cells.end(), Rb.cell_references_.begin(), Rb.cell_references_.end());

        distribute_new_cells(Ra, Rb, cells);
    }
}

void region_distribution::region::distribute_new_cells(std::vector<std::reference_wrapper<region_distribution::region> > regions, std::vector<cell_ref> all_cells){
    std::vector<capacity_t> caps;
    for(region_distribution::region & R : regions){
        caps.push_back(R.capacity_);
        R.cell_references_.clear();
    }

    sort_uniquify(all_cells);
    std::sort(all_cells.begin(), all_cells.end(), [](cell_ref const a, cell_ref const b){ return a.allocated_capacity_ > b.allocated_capacity_ or (a.allocated_capacity_ == b.allocated_capacity_ and a.pos_.x_+a.pos_.y_ < b.pos_.x_+b.pos_.y_); });

    std::vector<std::vector<float_t> > costs(regions.size());
    std::vector<capacity_t> demands;
    for(auto const C : all_cells){
        for(index_t r=0; r<regions.size(); ++r){
            costs[r].push_back(regions[r].get().distance(C));
        }
        demands.push_back(C.allocated_capacity_);
    }

    std::vector<std::vector<capacity_t> > res = transport_generic(caps, demands, costs);

    assert(res.size() == regions.size());
    for(index_t i=0; i<regions.size(); ++i){
        assert(res[i].size() == all_cells.size());
        for(index_t j=0; j<all_cells.size(); ++j){
            if(res[i][j] > 0){
                cell_ref C = all_cells[j];
                C.allocated_capacity_ = res[i][j];
                regions[i].get().cell_references_.push_back(C);
            }
        }
    }

}

void region_distribution::region::redistribute_cells(std::vector<std::reference_wrapper<region_distribution::region> > regions){
    if(regions.size() > 1){
        std::vector<cell_ref> all_cells;
        for(region_distribution::region & R : regions){
            all_cells.insert(all_cells.end(), R.cell_references_.begin(), R.cell_references_.end());
        }
        distribute_new_cells(regions, all_cells);
    }
}

void region_distribution::region::distribute_cells(std::vector<std::reference_wrapper<region_distribution::region> > regions) const{
    distribute_new_cells(regions, cell_references_);
}

void region_distribution::multipartition(index_t x_width, index_t y_width){
    assert(x_width > 0 and y_width > 0);

    std::vector<region> old_placement_regions = prepare_regions(x_width*x_regions_cnt(), y_width*y_regions_cnt());
    placement_regions_.swap(old_placement_regions);

    index_t old_x_regions_cnt = x_regions_cnt();
    index_t old_y_regions_cnt = y_regions_cnt();
    x_regions_cnt_ *= x_width;
    y_regions_cnt_ *= y_width;

    for(index_t x=0; x < old_x_regions_cnt; ++x){
        for(index_t y=0; y < old_y_regions_cnt; ++y){

            index_t i = y * old_x_regions_cnt + x;
            std::vector<std::reference_wrapper<region> > destination_regions;

            capacity_t tot_cap = 0;
            // Take the new regions
            for(index_t l_x=0; l_x<x_width; ++l_x){
                for(index_t l_y=0; l_y<y_width; ++l_y){
                    // Initialize it
                    region & cur_reg = get_region(x_width*x + l_x, y_width*y + l_y);
                    destination_regions.push_back(std::reference_wrapper<region>(cur_reg));
                    tot_cap += cur_reg.capacity();
                }
            }
            // Distribute the cells
            old_placement_regions[i].distribute_cells(destination_regions);
            assert(tot_cap == old_placement_regions[i].capacity());
        }
    }
}

void region_distribution::redo_bipartitions(){
    // This function performs optimization between neighbouring regions in various directions
    // The most important feature is diagonal optimization, since it is not done during partitioning
    redo_diagonal_bipartitions();
    redo_adjacent_bipartitions();
}

void region_distribution::redo_diagonal_bipartitions(){
    // Take four cells at a time and optimize them
    auto const optimize_quad_diag = [&](index_t x, index_t y){
        region::redistribute_cells(get_region(x, y), get_region(x+1, y+1));
        region::redistribute_cells(get_region(x+1, y), get_region(x, y+1));
    };

    // x is the fast index: the innermost loop operates on it
    auto const optimize_diag_on_y = [&](index_t y){
        for(index_t x=0; x+1 < x_regions_cnt(); x+=2){
            if(x+2 < x_regions_cnt()){
                // x odd
                optimize_quad_diag(x+1, y);
            }
            // x even
            optimize_quad_diag(x, y);
        }
    };

    // OpenMP doesn't allow y+1 < y_regions_cnt(), but anyway y_regions_cnt() >= 1
    #pragma omp parallel for
    for(index_t y=0; y < y_regions_cnt()-1; y+=2){
        // y even
        optimize_diag_on_y(y);
    }
    #pragma omp parallel for
    for(index_t y=1; y < y_regions_cnt()-1; y+=2){
        // y odd
        optimize_diag_on_y(y);
    }
}

void region_distribution::redo_adjacent_bipartitions(){
    auto const optimize_H = [&](index_t x, index_t y){
        region::redistribute_cells(get_region(x, y), get_region(x+1, y));
    };
    auto const optimize_V = [&](index_t x, index_t y){
        region::redistribute_cells(get_region(x, y), get_region(x, y+1));
    };

    // x bipartitions
    #pragma omp parallel for
    for(index_t y=0; y < y_regions_cnt(); ++y){
        for(index_t x=0; x+1 < x_regions_cnt(); x+=2){
            if(x+2 < x_regions_cnt()){
                // x odd
                optimize_H(x+1, y);
            }
            // x even
            optimize_H(x, y);
        }
    }
    // y bipartitions
    #pragma omp parallel for
    for(index_t x=0; x < x_regions_cnt(); ++x){
        for(index_t y=0; y+1 < y_regions_cnt(); y+=2){
            if(y+2 < y_regions_cnt()){
                // y odd
                optimize_V(x, y+1);
            }
            // y even
            optimize_V(x, y);
        }
    }
}

void region_distribution::redo_multipartitions(index_t x_width, index_t y_width){
    if(x_width < 2 and y_width < 2) throw std::runtime_error("Multipartitioning requires an optimization window of 2 or more\n");

    auto const reoptimize_group = [&](index_t x, index_t y){
        std::vector<std::reference_wrapper<region> > to_opt;
        for(index_t l_x=x; l_x < std::min(x+x_width, x_regions_cnt()); ++l_x){
            for(index_t l_y=y; l_y < std::min(y+y_width, y_regions_cnt()); ++l_y){
                to_opt.push_back(std::reference_wrapper<region>(get_region(l_x, l_y)));
            }
        }
        region::redistribute_cells(to_opt);
    };

    auto const optimize_on_y = [&](index_t y){
        for(index_t x=0; x < x_regions_cnt(); x+=x_width){
            if(x+x_width < x_regions_cnt()){
                reoptimize_group(x+x_width/2, y);
            }
            reoptimize_group(x, y);
        }
    };

    for(index_t y=0; y < y_regions_cnt(); y+=y_width){
        if(y+y_width < y_regions_cnt()){
            optimize_on_y(y+y_width/2);
        }
        optimize_on_y(y);
    }
}

inline void region_distribution::region::distribute_new_cells(std::vector<std::reference_wrapper<region> > regions, std::vector<cell_ref> cells, std::function<float_t (point<float_t>)> coord){
	// Gather all cells and the useful regions
	std::vector<std::reference_wrapper<region> > all_regions;

	for(region & reg_ref : regions){
		if(reg_ref.capacity() > 0){
			cells.insert(cells.end(), reg_ref.cell_references_.begin(), reg_ref.cell_references_.end());
			reg_ref.cell_references_.clear();
			all_regions.push_back(std::reference_wrapper<region>(reg_ref));
		}
		else{
			assert(reg_ref.cell_references_.empty());
		}
	}

	// Sort the regions by coordinate
	std::sort(all_regions.begin(), all_regions.end(), [&](std::reference_wrapper<region> const a, std::reference_wrapper<region> const b){ return coord(a.get().pos_) < coord(b.get().pos_); });
	// And the cells
	std::sort(cells.begin(), cells.end(), [&](cell_ref const a, cell_ref const b){ return coord(a.pos_) < coord(b.pos_); });
	just_uniquify(cells);

	std::vector<t1D_elt> sources, sinks;
	for(cell_ref const c : cells){
		sources.push_back(t1D_elt(static_cast<int_t>(coord(c.pos_)), c.allocated_capacity_));
	}
	for(region & reg_ref : all_regions){
		sinks.push_back(t1D_elt(static_cast<int_t>(coord(reg_ref.pos_)), reg_ref.capacity()));
	}

	std::vector<capacity_t> const positions = transport_1D(sources, sinks);

	std::vector<capacity_t> prev_cap(1, 0);
	for(t1D_elt e: sinks){
		assert(e.second > 0);
		prev_cap.push_back(prev_cap.back() + e.second);
	}

	for(index_t i=0; i<sources.size(); ++i){
		assert(positions[i] + sources[i].second <= prev_cap.back());
		assert(positions[i] >= 0);
		assert(sources[i].second > 0);
		if(i>0)
			assert(sources[i-1].second + positions[i-1] <= positions[i]);
	}

	for(index_t i=0, r=0; i<sources.size(); ++i){
		cell_ref const c = cells[i];

		capacity_t cur_pos = positions[i];
		capacity_t cur_cap = sources[i].second;
		while(cur_cap > 0){
			while(prev_cap[r+1] <= cur_pos){ // After the end of region r
				++r;
			}
			cell_ref new_c = c;
			capacity_t used_cap = std::min(prev_cap[r+1] - cur_pos, cur_cap);
			new_c.allocated_capacity_ = used_cap;
			assert(used_cap >= 0);
			if(used_cap > 0){
				all_regions[r].get().cell_references_.push_back(new_c);
			}
			cur_pos += used_cap;
			cur_cap -= used_cap;
		}
	}
}

inline void region_distribution::region::redistribute_cells(std::vector<std::reference_wrapper<region> > & regions, std::function<float_t (point<float_t>)> coord){
    std::vector<cell_ref> cells;
	for(region & reg_ref : regions){
		if(reg_ref.capacity() > 0){
			cells.insert(cells.end(), reg_ref.cell_references_.begin(), reg_ref.cell_references_.end());
			reg_ref.cell_references_.clear();
		}
		else{
			assert(reg_ref.cell_references_.empty());
		}
	}
    distribute_new_cells(regions, cells, coord);
}

void region_distribution::redo_line_partitions(){
    // Optimize a single line or column
    #pragma omp parallel for
    for(index_t y=0; y<y_regions_cnt(); ++y){
        std::vector<std::reference_wrapper<region> > regs;
        for(index_t x=0; x<x_regions_cnt(); ++x){
            regs.push_back(std::reference_wrapper<region>(get_region(x, y)));
        }
        region::redistribute_cells(regs, [](point<float_t> p){ return p.x_; });
    }
    #pragma omp parallel for
    for(index_t x=0; x<x_regions_cnt(); ++x){
        std::vector<std::reference_wrapper<region> > regs;
        for(index_t y=0; y<y_regions_cnt(); ++y){
            regs.push_back(std::reference_wrapper<region>(get_region(x, y)));
        }
        region::redistribute_cells(regs, [](point<float_t> p){ return p.y_; });
    }
}

void region_distribution::x_resize(index_t sz){
    assert(sz > 0);
    std::vector<region> old_placement_regions = prepare_regions(sz, y_regions_cnt());
    placement_regions_.swap(old_placement_regions);

    index_t old_x_regions_cnt = x_regions_cnt();

    x_regions_cnt_ = sz;

    #pragma omp parallel for
    for(index_t y=0; y<y_regions_cnt(); ++y){
        std::vector<cell_ref> cells;
        for(index_t x=0; x<old_x_regions_cnt; ++x){
            std::vector<cell_ref> const & cur = old_placement_regions[y*old_x_regions_cnt + x].cell_references_;
            cells.insert(cells.end(), cur.begin(), cur.end());
        }

        std::vector<std::reference_wrapper<region> > regs;
        for(index_t x=0; x<x_regions_cnt(); ++x){
            regs.push_back(std::reference_wrapper<region>(get_region(x, y)));
        }
        region::distribute_new_cells(regs, cells, [](point<float_t> p){ return p.x_; });
    }
}

void region_distribution::y_resize(index_t sz){
    assert(sz > 0);
    std::vector<region> old_placement_regions = prepare_regions(x_regions_cnt(), sz);
    placement_regions_.swap(old_placement_regions);

    index_t old_y_regions_cnt = y_regions_cnt();

    y_regions_cnt_ = sz;

    #pragma omp parallel for
    for(index_t x=0; x<x_regions_cnt(); ++x){
        std::vector<cell_ref> cells;
        for(index_t y=0; y<old_y_regions_cnt; ++y){
            std::vector<cell_ref> const & cur = old_placement_regions[y*x_regions_cnt() + x].cell_references_;
            cells.insert(cells.end(), cur.begin(), cur.end());
        }

        std::vector<std::reference_wrapper<region> > regs;
        for(index_t y=0; y<y_regions_cnt(); ++y){
            regs.push_back(std::reference_wrapper<region>(get_region(x, y)));
        }
        region::distribute_new_cells(regs, cells, [](point<float_t> p){ return p.y_; });
    }
}

void region_distribution::redo_diag_partitions(index_t len){
    if(x_regions_cnt() <= 1 or y_regions_cnt() <= 1) return;

    auto const reoptimize_rdiag = [&](index_t x_b, index_t y_b){
        index_t max_delta = std::min(x_regions_cnt()-x_b, y_regions_cnt()-y_b);
        for(index_t delta=0; delta+len/2<max_delta; delta += len/2){
            std::vector<std::reference_wrapper<region> > to_opt;
            for(index_t m=0; m < std::min(max_delta-delta, len); ++m){
                to_opt.push_back(std::reference_wrapper<region>(get_region(x_b+delta+m, y_b+delta+m)));
            }
            region::redistribute_cells(to_opt);
        }
    };
    auto const reoptimize_ldiag = [&](index_t x_b, index_t y_b){
        index_t max_delta = std::min(x_regions_cnt()-x_b, y_b+1);
        for(index_t delta=0; delta+len/2<max_delta; delta += len/2){
            std::vector<std::reference_wrapper<region> > to_opt;
            for(index_t m=0; m < std::min(max_delta-delta, len); ++m){
                to_opt.push_back(std::reference_wrapper<region>(get_region(x_b+delta+m, y_b-delta-m)));
            }
            region::redistribute_cells(to_opt);
        }
    };

    for(index_t y=y_regions_cnt()-2; y >= 1; --y)
        reoptimize_rdiag(0, y);
    for(index_t x=0; x+1<x_regions_cnt(); ++x)
        reoptimize_rdiag(x, 0);
    for(index_t y=1; y+1<y_regions_cnt(); ++y)
        reoptimize_ldiag(0, y);
    for(index_t x=0; x+1<x_regions_cnt(); ++x)
        reoptimize_ldiag(x, y_regions_cnt()-1);
}

region_distribution::region_distribution(
    box<int_t> placement_area,
    netlist const & circuit, placement_t const & pl,
    std::vector<density_limit> const & density_map, bool full_density
    ):
        x_regions_cnt_(1),
        y_regions_cnt_(1),
        placement_area_(placement_area),
        density_map_(density_map),
        full_density_mul(default_density_mul)
    {

    capacity_t tot_area = 0;
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        auto c = circuit.get_cell(i);
        if( (c.attributes & XMovable) != 0 and (c.attributes & YMovable) != 0){
            cell_list_.push_back(movable_cell(c.area, static_cast<point<float_t> >(pl.positions_[i]) + 0.5f * static_cast<point<float_t> >(c.size), i));
            tot_area += c.area;
        }
        else{ // Create an obstacle corresponding to the macro
            auto pos = pl.positions_[i];
            auto end = pos + c.size;
            density_limit macro;
            macro.box_ = box<int_t>(pos.x_, end.x_, pos.y_, end.y_);
            macro.density_ = 0.0f;
            density_map_.push_back(macro);
        }
    }

    placement_regions_ = prepare_regions(1, 1);

    if(full_density){
        cell_density_mul = default_density_mul;
    }
    else{
        cell_density_mul = placement_regions_[0].capacity() / tot_area;
        if(cell_density_mul < full_density_mul){
            throw std::runtime_error("The density required by the obstacles is above 1");
        }
    }

    for(index_t i=0; i<cell_list_.size(); ++i){
        movable_cell const & c = cell_list_[i];
        if(c.demand_ == 0){
            throw std::runtime_error("A cell has been found with demand 0");
        }
        placement_regions_[0].cell_references_.push_back( cell_ref(c.demand_ * cell_density_mul, c.pos_, i) );
    }
}

region_distribution region_distribution::full_density_distribution(box<int_t> placement_area, netlist const & circuit, placement_t const & pl, std::vector<density_limit> const & density_map){
    return region_distribution(placement_area, circuit, pl, density_map, true);
}
region_distribution region_distribution::uniform_density_distribution(box<int_t> placement_area, netlist const & circuit, placement_t const & pl, std::vector<density_limit> const & density_map){
    return region_distribution(placement_area, circuit, pl, density_map, false);
}

void region_distribution::update(netlist const & circuit, placement_t const & pl){
    for(movable_cell & c : cell_list_){
        index_t i = c.index_in_placement_;
        c.pos_ = static_cast<point<float_t> >(pl.positions_[i]) + 0.5f * static_cast<point<float_t> >(circuit.get_cell(i).size);
    }
    for(region & R : placement_regions_){
        for(cell_ref & c : R.cell_references_){
            c.pos_ = cell_list_[c.index_in_list_].pos_;
        }
    }
}

std::vector<region_distribution::movable_cell> region_distribution::export_positions() const{
    std::vector<point<float_t> > weighted_pos(cell_list_.size(), point<float_t>(0.0, 0.0));

    for(region const & R : placement_regions_){
        for(cell_ref C : R.cell_references_){
            weighted_pos[C.index_in_list_] = weighted_pos[C.index_in_list_] + static_cast<float_t>(C.allocated_capacity_) * R.pos_;
        }
    }

    std::vector<movable_cell> ret;
    for(index_t i=0; i<cell_list_.size(); ++i){
        movable_cell C = cell_list_[i];
        C.pos_ = ( static_cast<float_t>(1.0) / static_cast<float_t>(C.demand_ * cell_density_mul) ) * weighted_pos[i];
        ret.push_back(C);
    }
    return ret;
}

struct OSRP_task{
    float_t size;
    float_t goal_pos;
    float_t weight;
    index_t orig;
    OSRP_task(float_t p, float_t s, float_t w, index_t i) : size(s), goal_pos(p), weight(w), orig(i) {}
    OSRP_task(){}
};

std::vector<float_t> get_optimal_quadratic_pos(std::vector<OSRP_task> cells, float_t pos_begin, float_t pos_end){

    if(cells.empty()){ return std::vector<float_t>(); }

    struct OSRP_cluster{
        index_t begin, end;
        float_t pos, size;
        float_t weight;
        OSRP_cluster(index_t b, index_t e, float_t p, float_t s, float_t w) : begin(b), end(e), pos(p), size(s), weight(w) {}
        void merge(OSRP_cluster const o){
            begin = o.begin;
            pos = (weight * pos + o.weight * o.pos) / (weight + o.weight);
            size += o.size;
            weight += o.weight;
        }
    };

    std::sort(cells.begin(), cells.end(), [](OSRP_task a, OSRP_task b){ return a.goal_pos < b.goal_pos; });

    // Modify the goal pos to get absolute goal positions between pos_begin and pos_end - sum_sizes
    float_t sum_sizes = 0.0;
    for(auto & c : cells){
        c.goal_pos -= sum_sizes;
        sum_sizes += c.size;
    }
    float_t abs_begin = pos_begin + 0.5 * cells[0].size; // First cell must be far enough from the beginning
    float_t abs_end = pos_end - sum_sizes + 0.5 * cells[0].size; // Last cell must be far enough from the end
    for(auto & c : cells){
        c.goal_pos = std::max(c.goal_pos, abs_begin);
        c.goal_pos = std::min(c.goal_pos, abs_end);
    }

    std::vector<OSRP_cluster> clusters;
    for(index_t i=0; i<cells.size(); ++i){
        OSRP_cluster to_add(
            i, i,
            cells[i].goal_pos,
            cells[i].size, cells[i].weight
        );
        while(not clusters.empty() and (clusters.back().pos >= to_add.pos)){
            to_add.merge(clusters.back());
            clusters.pop_back();
        }

        clusters.push_back(to_add);
    }

    std::vector<float_t> ret(cells.size(), 0.0);
    // For every cell, recover its true position from the absolute position of a cluster
    float_t sum_prev_sizes = 0.0;
    for(OSRP_cluster cur : clusters){
        for(index_t i=cur.begin; i <= cur.end; ++i){
            ret[cells[i].orig] = cur.pos + sum_prev_sizes;
            sum_prev_sizes += cells[i].size;
        }
    }
    return ret;
}

std::vector<region_distribution::movable_cell> region_distribution::export_spread_positions_quadratic() const{
    std::vector<point<float_t> > weighted_pos(cell_list_.size(), point<float_t>(0.0, 0.0));

    for(index_t y=0; y<y_regions_cnt(); ++y){
        for(index_t x=0; x<x_regions_cnt(); ++x){
            region const & R = get_region(x, y);
            index_t n = R.cell_references_.size();
            float_t total_capacity = static_cast<float_t>(R.capacity());
            box<float_t> surface = static_cast<box<float_t> >(get_box(x, y, x_regions_cnt(), y_regions_cnt()));
    
            std::vector<OSRP_task> x_cells(n), y_cells(n);
            for(index_t i=0; i<n; ++i){
                point<float_t> pt = R.cell_references_[i].pos_;
                float_t cap = static_cast<float_t>(R.cell_references_[i].allocated_capacity_);
                x_cells[i] = OSRP_task(pt.x_, cap/total_capacity * (surface.x_max_ - surface.x_min_), 1.0, i);
                y_cells[i] = OSRP_task(pt.y_, cap/total_capacity * (surface.y_max_ - surface.y_min_), 1.0, i);
            }
            std::vector<float_t> x_ret = get_optimal_quadratic_pos(x_cells, surface.x_min_, surface.x_max_);
            std::vector<float_t> y_ret = get_optimal_quadratic_pos(y_cells, surface.y_min_, surface.y_max_);
    
            for(index_t i=0; i<n; ++i){
                weighted_pos[R.cell_references_[i].index_in_list_] +=
                      static_cast<float_t>(R.cell_references_[i].allocated_capacity_)
                    * point<float_t>(x_ret[i], y_ret[i]);
            }
        }
    }

    std::vector<movable_cell> ret;
    for(index_t i=0; i<cell_list_.size(); ++i){
        movable_cell C = cell_list_[i];
        assert(C.demand_ > 0);
        C.pos_ = ( static_cast<float_t>(1.0) / static_cast<float_t>(C.demand_) ) * weighted_pos[i];
        assert(std::isfinite(C.pos_.x_) and std::isfinite(C.pos_.y_));
        ret.push_back(C);
    }
    return ret;
}

std::vector<region_distribution::movable_cell> region_distribution::export_spread_positions_linear() const{
    std::vector<point<float_t> > weighted_pos(cell_list_.size(), point<float_t>(0.0, 0.0));

    for(index_t y=0; y<y_regions_cnt(); ++y){
        for(index_t x=0; x<x_regions_cnt(); ++x){
            region const & R = get_region(x, y);
            index_t n = R.cell_references_.size();
            float_t total_capacity = static_cast<float_t>(R.capacity());
            box<float_t> surface = static_cast<box<float_t> >(get_box(x, y, x_regions_cnt(), y_regions_cnt()));
            assert(surface.x_max_ > surface.x_min_ and surface.y_max_ > surface.y_min_);
    
            std::vector<legalizable_task<float_t> > x_cells, y_cells;
    
            for(auto const C : R.cell_references_){
                float_t cap = static_cast<float_t>(C.allocated_capacity_);
                float_t x_cap_prop = cap/total_capacity * (surface.x_max_ - surface.x_min_),
                        y_cap_prop = cap/total_capacity * (surface.y_max_ - surface.y_min_);
                x_cells.push_back(legalizable_task<float_t>(x_cap_prop, C.pos_.x_, C.index_in_list_));
                y_cells.push_back(legalizable_task<float_t>(y_cap_prop, C.pos_.y_, C.index_in_list_));
            }
    
            OSRP_leg<float_t> x_leg(surface.x_min_, surface.x_max_), y_leg(surface.y_min_, surface.y_max_);
    
            std::sort(x_cells.begin(), x_cells.end());
            for(legalizable_task<float_t> & C : x_cells)
                C.target_pos -= 0.5 * C.width;
            for(legalizable_task<float_t> & C : x_cells)
                x_leg.push(C);
            auto x_pl = x_leg.get_placement();
            for(index_t i=0; i<n; ++i){
                assert(std::isfinite(x_pl[i].second));
                weighted_pos[x_pl[i].first].x_ += (x_pl[i].second + 0.5f * x_cells[i].width) * static_cast<float_t>(x_cells[i].width * total_capacity / (surface.x_max_ - surface.x_min_));
            }

            std::sort(y_cells.begin(), y_cells.end());
            for(legalizable_task<float_t> & C : y_cells)
                C.target_pos -= 0.5 * C.width;
            for(legalizable_task<float_t> & C : y_cells)
                y_leg.push(C);
            auto y_pl = y_leg.get_placement();
            for(index_t i=0; i<n; ++i){
                assert(std::isfinite(y_pl[i].second));
                weighted_pos[y_pl[i].first].y_ += (y_pl[i].second + 0.5f * y_cells[i].width) * static_cast<float_t>(y_cells[i].width * total_capacity / (surface.y_max_ - surface.y_min_));
            }
        }
    }

    std::vector<movable_cell> ret;
    for(index_t i=0; i<cell_list_.size(); ++i){
        movable_cell C = cell_list_[i];
        assert(C.demand_ > 0);
        C.pos_ = ( 1.0f / static_cast<float_t>(C.demand_ * cell_density_mul) ) * weighted_pos[i];
        assert(std::isfinite(C.pos_.x_) and std::isfinite(C.pos_.y_));
        ret.push_back(C);
    }
    return ret;
}

float_t region_distribution::region::cost() const{
    float_t res = 0.0;
    for(cell_ref const C : cell_references_){
        res += (std::abs(C.pos_.x_-pos_.x_) + std::abs(C.pos_.y_-pos_.y_)) * static_cast<float_t>(C.allocated_capacity_);
    }
    return res;
}

float_t region_distribution::cost() const{
    float_t res = 0.0;
    capacity_t tot_cap = 0;
    for(region const & R : placement_regions_){
        res += R.cost();
        tot_cap += R.allocated_capacity();
    }
    // Average over the cells' areas
    return res / static_cast<float_t>(tot_cap);
}

} // Namespace gp
} // Namespace coloquinte

