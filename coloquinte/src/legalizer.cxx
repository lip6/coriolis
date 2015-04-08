
#include "coloquinte/legalizer.hxx"
#include "coloquinte/optimization_subproblems.hxx"

#include <algorithm>
#include <cmath>
#include <queue>

namespace coloquinte{
namespace dp{

void get_result(netlist const & circuit, detailed_placement const & dpl, placement_t & gpl){
    for(index_t c=0; c<circuit.cell_cnt(); ++c){
        if( (circuit.get_cell(c).attributes & XMovable) != 0)
            gpl.positions_[c].x_ = dpl.plt_.positions_[c].x_;
        if( (circuit.get_cell(c).attributes & YMovable) != 0)
            gpl.positions_[c].y_ = dpl.plt_.positions_[c].y_;

        if( (circuit.get_cell(c).attributes & XFlippable) != 0)
            gpl.orientations_[c].x_ = dpl.plt_.orientations_[c].x_;
        if( (circuit.get_cell(c).attributes & YFlippable) != 0)
            gpl.orientations_[c].y_ = dpl.plt_.orientations_[c].y_;
    }
}

struct cell_to_leg{
    int_t x_pos, y_pos;
    index_t original_cell;
    int_t width;
    index_t nbr_rows;

    bool operator<(cell_to_leg const o) const{ return x_pos < o.x_pos; }

    cell_to_leg(int_t x, int_t y, index_t ind, int_t w, index_t rows)
    : x_pos(x), y_pos(y),
    original_cell(ind),
    width(w),
    nbr_rows(rows)
    {}

    legalizable_task<int_t> task() const{ return legalizable_task<int_t>(width, x_pos, original_cell); }
};

struct fixed_cell_interval{
    int_t min_x, max_x;
    index_t cell_ind;

    bool operator<(fixed_cell_interval const o) const{ return min_x > o.min_x; }
    fixed_cell_interval(int_t mn, int_t mx, index_t ind) : min_x(mn), max_x(mx), cell_ind(ind){}
};

struct cell_leg_properties{
    int_t   x_pos;
    index_t row_pos;
    index_t ind;

    cell_leg_properties(){}
    cell_leg_properties(int_t x, int_t r, index_t i) : x_pos(x), row_pos(r), ind(i){}
};

std::vector<cell_leg_properties> simple_legalize(
        std::vector<std::vector<fixed_cell_interval> > obstacles, std::vector<cell_to_leg> cells,
        std::vector<std::vector<index_t> > & rows,
        int_t x_min, int_t x_max, int_t y_orig,
        int_t row_height, index_t nbr_rows
    ){

    std::vector<int_t> first_available_position(nbr_rows, x_min);
    rows.resize(nbr_rows);

    // Sort the cells by x position
    std::sort(cells.begin(), cells.end());

    std::vector<cell_leg_properties> ret;

    for(cell_to_leg C : cells){
        // Dumb, quick and dirty best-fit legalization
        bool found_location = false;

        // Properties of the current best solution
        int_t best_x=0;
        int_t best_cost=0;
        index_t best_row=0;

        // Helper function
        auto check_row_cost = [&](index_t r, cell_to_leg const cell, int_t additional_cost){
            // Find where to put the cell in these rows
            // Simple method: get a range where we can put the cell

            assert(r + cell.nbr_rows <= nbr_rows);
            assert(additional_cost >= 0);

            // First position where we can put it
            int_t cur_pos = *std::max_element(first_available_position.begin() + r, first_available_position.begin() + r + cell.nbr_rows);
            int_t max_lim = x_max - cell.width;
            int_t interval_lim;
            do{
                interval_lim = max_lim;
                // For each row, test if obstacles prevent us from putting a cell here
                // Until we find a correct position or are beyond the maximum position
                for(index_t i = 0; i<cell.nbr_rows; ++i){
                    // Find the first obstacle which is after this position
                    // TODO: use lower/upper bound
                    auto it=obstacles[r+i].rbegin();
                    for(; it != obstacles[r+i].rend() && it->max_x <= cur_pos; ++it){
                    }
                    if(it != obstacles[r+i].rend()){ // There is an obstacle on the right
                        assert(it->min_x < it->max_x);
                        int_t cur_lim = it->min_x - cell.width; // Where the obstacles contrains us
                        interval_lim = std::min(cur_lim, interval_lim); // Constraint
                        if(cur_lim < cur_pos){ // If this particular obstacle constrained us so that it is not possible to make it here, we increment the position
                            cur_pos = std::max(it->max_x, cur_pos);
                        }
                    }
                }
                // Do it again until we find a solution
                // TODO: continue until we can't find a better solution (currently sticks before the first obstacle if there is enough whitespace)
            }while(interval_lim < cur_pos and interval_lim < max_lim and cur_pos < max_lim); // Not admissible and we encountered an obstacle and there is still hope

            if(interval_lim >= cur_pos){ // An admissible solution is found (and if cell.x_pos is between cur_pos and interval_lim it is optimal)
                int_t row_best_x = std::min(interval_lim, std::max(cur_pos, cell.x_pos));
                int_t row_cost_x = std::abs(row_best_x - cell.x_pos);
                if(not found_location or row_cost_x + additional_cost < best_cost){
                    found_location = true;
                    best_cost = row_cost_x + additional_cost;
                    best_x = row_best_x;
                    best_row = r;
                }
            }
        };

        // The row where we would prefer the cell to go
        if(C.nbr_rows > nbr_rows) throw std::runtime_error("Impossible to legalize a cell spanning more rows than are available\n");
        index_t central_row = std::min( (index_t) std::max( (C.y_pos - y_orig) / row_height, 0), nbr_rows-C.nbr_rows);

        // Try every possible row from the best one, until we can't improve the cost
        for(index_t row_dist = 0;
            (central_row + row_dist < nbr_rows or central_row >= row_dist)
            and (not found_location or (int_t) row_dist * row_height * C.width < (int_t) row_height + best_cost);
            ++row_dist
        ){
            if(central_row + row_dist < nbr_rows - C.nbr_rows){
                int_t add_cost = C.width * std::abs(static_cast<int_t>(central_row + row_dist) * static_cast<int_t>(row_height) + y_orig - C.y_pos);
                check_row_cost(central_row + row_dist, C, add_cost);
            }
            if(central_row >= row_dist){
                int_t add_cost = C.width * std::abs(static_cast<int_t>(central_row - row_dist) * static_cast<int_t>(row_height) + y_orig - C.y_pos);
                check_row_cost(central_row - row_dist, C, add_cost);
            }
        }

        if(not found_location){ // We didn't find any whitespace to put the cell in
            throw std::runtime_error("Didn't manage to pack a cell due to dumb algorithm\n");
        }
        else{
            assert(best_x + C.width <= x_max and best_x >= x_min);
            // Update the occupied rows
            for(index_t r = best_row; r < best_row + C.nbr_rows; ++r){
                // Include the obstacles
                while(not obstacles[r].empty()
                    and obstacles[r].back().max_x <= best_x){
                    rows[r].push_back(obstacles[r].back().cell_ind);
                    obstacles[r].pop_back();
                }
                assert(obstacles[r].empty() or obstacles[r].back().min_x >= best_x + C.width);

                rows[r].push_back(C.original_cell);
                first_available_position[r] = best_x + C.width;
            }

            ret.push_back(cell_leg_properties(best_x, best_row, C.original_cell));
        }
    }

    // Finally, push the remaining fixed cells
    for(index_t r=0; r<nbr_rows; ++r){
        while(not obstacles[r].empty()){
            rows[r].push_back(obstacles[r].back().cell_ind);
            obstacles[r].pop_back();
        }
    }

    return ret;
}

// A better legalization function which is able to push already legalized cells
std::vector<cell_leg_properties> good_legalize(
        std::vector<std::vector<fixed_cell_interval> > obstacles, std::vector<cell_to_leg> cells,
        std::vector<std::vector<index_t> > & rows,
        int_t x_min, int_t x_max, int_t y_orig,
        int_t row_height, index_t nbr_rows
    ){

    // Two possibilities:
    //    * Single OSRP (group of movable cells) at the current end of the row of standard cells
    //    * Multiple OSRPs, between each pair of obstacles
    //          -> allows pushing cells past obstacles
    //          -> tricky with multiple standard cell heights
    // Therefore I chose single OSRP, which gets cleared and pushed to the final state whenever
    //    * we encounter a multiple-rows cell
    //    * a new standard cell gets past an obstacle

    // The current group of standard cells on the right of the row
    std::vector<OSRP_leg<int_t> > single_row_problems(nbr_rows);
    for(index_t r=0; r<nbr_rows; ++r){
        single_row_problems[r] = OSRP_leg<int_t>(x_min, obstacles[r].empty() ? x_max : obstacles[r].back().min_x);
    }
    rows.resize(nbr_rows);

    // Sort the cells by x position
    std::sort(cells.begin(), cells.end());

    std::vector<cell_leg_properties> ret;

    for(cell_to_leg C : cells){
        // Dumb, quick and dirty best-fit legalization
        bool found_location = false;

        // Properties of the current best solution
        int_t best_cost=0;
        index_t best_row=0;
        index_t obstacles_passed = 0;

        // Helper function
        auto check_row_cost = [&](index_t r, cell_to_leg const cell, int_t additional_cost){
            // Find where to put the cell in these rows
            // Check if we can put it in the current ranges and at what cost; if not or if the optimal position is beyond an obstacle, try after this obstacle too

            assert(cell.nbr_rows > 0);
            assert(r + cell.nbr_rows <= nbr_rows);
            assert(additional_cost >= 0);

            // Where can we put a standard cell if we allow to move the cells?
            if(cell.nbr_rows == 1){
                int_t cur_cost = 0;

                // Can we simply add it to the single row problem?
                bool found_here = single_row_problems[r].remaining_space() >= cell.width;
                int_t loc_obstacles_passed = 0;
                if(found_here){
                    // Check the cost of pushing it here with possible displacement
                    cur_cost = single_row_problems[r].get_cost(cell.task()); // Don't update the row
                }

                // Other positions where we can put it, without moving other cells this time
                if(not found_here or cur_cost > 0){
                    index_t obstacles_to_throw = 0;
                    auto it = obstacles[r].rbegin();
                    while(it != obstacles[r].rend()){
                        ++ obstacles_to_throw;
                        auto prev_it = it++;
                        int_t region_end = it != obstacles[r].rend() ? it->min_x : x_max;
                        if(region_end >= prev_it->max_x + cell.width){
                            int_t loc_x = std::min(region_end - cell.width, std::max(prev_it->max_x, cell.x_pos));
                            int_t loc_cost = cell.width * std::abs(cell.x_pos - loc_x);
                            if(not found_here or cur_cost > loc_cost){
                                found_here = true;
                                cur_cost = loc_cost;
                                loc_obstacles_passed = obstacles_to_throw;
                            }
                        }
                    }
                }
                if(found_here and (not found_location or cur_cost + additional_cost < best_cost)){
                    found_location = true;
                    //std::cout << "Found with displacement cost " << cur_cost << " and total cost " << cur_cost + additional_cost << std::endl;
                    best_cost = cur_cost + additional_cost;
                    best_row = r;
                    obstacles_passed = loc_obstacles_passed;
                    if(loc_obstacles_passed > 0) assert(not obstacles[r].empty());
                }
            }
            else{
                // If it is a fixed cell, we use fixed locations
                throw std::runtime_error("I don't handle fucking macros\n");
            }
        };

        // The row where we would prefer the cell to go
        if(C.nbr_rows > nbr_rows) throw std::runtime_error("Impossible to legalize a cell spanning more rows than are available\n");
        index_t central_row = std::min( (index_t) std::max( (C.y_pos - y_orig) / row_height, 0), nbr_rows-C.nbr_rows);

        // Try every possible row from the best one, until we can't improve the cost
        for(index_t row_dist = 0;
            (central_row + row_dist < nbr_rows or central_row >= row_dist)
            and (not found_location or (int_t) row_dist * row_height * C.width < (int_t) row_height + best_cost);
            ++row_dist
        ){
            if(central_row + row_dist < nbr_rows - C.nbr_rows){
                int_t add_cost = C.width * std::abs(static_cast<int_t>(central_row + row_dist) * static_cast<int_t>(row_height) + y_orig - C.y_pos);
                check_row_cost(central_row + row_dist, C, add_cost);
            }
            if(central_row >= row_dist){
                int_t add_cost = C.width * std::abs(static_cast<int_t>(central_row - row_dist) * static_cast<int_t>(row_height) + y_orig - C.y_pos);
                check_row_cost(central_row - row_dist, C, add_cost);
            }
        }

        if(not found_location){ // We didn't find any whitespace to put the cell in
            throw std::runtime_error("Didn't manage to pack a cell: leave more whitespace and avoid macros near the right side\n");
        }
        else{
            //std::cout << "Cell " << C.original_cell << " of width " << C.width << " targetting row " << central_row << " and position " << C.x_pos << " put at row " << best_row << " with displacement " << best_cost / C.width << " with " << obstacles_passed << " obstacles passed" << std::endl;
            // If the cell spans multiple rows, it becomes fixed
            // In this case or if the cell goes after an obstacle, push everything before the cell to the fixed state

            if(C.nbr_rows == 1){
                if(obstacles_passed == 0){ // Ok; just update the old single row problem
                    single_row_problems[best_row].push(C.task()); // Push it to the row
                }
                else{
                    assert(obstacles_passed > 0);
                    // Empty the single row problem
                    for(auto p : single_row_problems[best_row].get_placement()){
                        rows[best_row].push_back(p.first);
                        ret.push_back(cell_leg_properties(p.second, best_row, p.first));
                    }
                    // Find where to put it
                    int_t region_begin = x_min;
                    for(index_t i=0; i<obstacles_passed; ++i){
                        assert(not obstacles[best_row].empty());
                        region_begin = obstacles[best_row].back().max_x;
                        rows[best_row].push_back(obstacles[best_row].back().cell_ind);
                        obstacles[best_row].pop_back();
                    }
                    int_t region_end = obstacles[best_row].empty() ? x_max : obstacles[best_row].back().min_x;
                    single_row_problems[best_row] = OSRP_leg<int_t>(region_begin, region_end);
                    assert(region_end - region_begin >= C.width);
                    single_row_problems[best_row].push(C.task()); // Push this only cell to the single row problem
                }
            }
            else{
                throw std::runtime_error("I don't handle fucking macros\n");
            }
        }
    }

    for(index_t r=0; r<nbr_rows; ++r){
        // Finally, push the remaining standard cells in the row
        for(auto p : single_row_problems[r].get_placement()){
            rows[r].push_back(p.first);
            ret.push_back(cell_leg_properties(p.second, r, p.first));
        }
        // And the fixed cells
        while(not obstacles[r].empty()){
            rows[r].push_back(obstacles[r].back().cell_ind);
            obstacles[r].pop_back();
        }
    }

    rows.resize(nbr_rows);
    return ret;
}


detailed_placement legalize(netlist const & circuit, placement_t const & pl, box<int_t> surface, int_t row_height){
    if(row_height <= 0) throw std::runtime_error("The rows' height should be positive\n");

    index_t nbr_rows = (surface.y_max_ - surface.y_min_) / row_height;
    // The position of the ith row is surface.y_min_ + i * row_height

    std::vector<std::vector<fixed_cell_interval> > row_occupation(nbr_rows);
    std::vector<cell_to_leg> cells;

    placement_t new_placement = pl;
    std::vector<index_t> placement_rows(circuit.cell_cnt());
    std::vector<index_t> cell_heights(circuit.cell_cnt());

    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        auto cur = circuit.get_cell(i);
        // Assumes fixed if not both XMovable and YMovable
        if( (cur.attributes & XMovable) != 0 && (cur.attributes & YMovable) != 0){
            // Just truncate the position we target
            point<int_t> target_pos = pl.positions_[i];
            index_t cur_cell_rows = (cur.size.y_ + row_height -1) / row_height;
            cells.push_back(cell_to_leg(target_pos.x_, target_pos.y_, i, cur.size.x_, cur_cell_rows));
            cell_heights[i] = cur_cell_rows;
        }
        else{
            // In each row, we put the index of the fixed cell and the range that is already occupied
            int_t low_x_pos  = pl.positions_[i].x_,
                  hgh_x_pos  = pl.positions_[i].x_ + cur.size.x_,
                  low_y_pos  = pl.positions_[i].y_,
                  hgh_y_pos  = pl.positions_[i].y_ + cur.size.y_;

            new_placement.positions_[i] = point<int_t>(low_x_pos, low_y_pos);
            if(hgh_y_pos <= surface.y_min_ or low_y_pos >= surface.y_max_ or hgh_x_pos <= surface.x_min_ or low_x_pos >= surface.x_max_){
                placement_rows[i] = null_ind;
                cell_heights[i] = 0;
            }
            else{
                assert(low_x_pos < hgh_x_pos and low_y_pos < hgh_y_pos);

                int_t rnd_hgh_x_pos = std::min(surface.x_max_, hgh_x_pos);
                int_t rnd_hgh_y_pos = std::min(surface.y_max_, hgh_y_pos);
                int_t rnd_low_x_pos = std::max(surface.x_min_, low_x_pos);
                int_t rnd_low_y_pos = std::max(surface.y_min_, low_y_pos);

                index_t first_row = (rnd_low_y_pos - surface.y_min_) / row_height;
                index_t last_row = (index_t) (rnd_hgh_y_pos - surface.y_min_ + row_height - 1) / row_height; // Exclusive: if the cell spans the next row, i.e. pos % row_height >= 0, include it too
                assert(last_row <= nbr_rows);

                placement_rows[i] = first_row;
                cell_heights[i] = last_row - first_row;
                for(index_t r=first_row; r<last_row; ++r){
                    row_occupation[r].push_back(fixed_cell_interval(rnd_low_x_pos, rnd_hgh_x_pos, i));
                }
            }
        }
    }

    for(std::vector<fixed_cell_interval> & L : row_occupation){
        std::sort(L.begin(), L.end()); // Sorts from last to first, so that we may use pop_back()
        // Doesn't collapse them yet, which may make for bigger complexities
        for(index_t i=0; i+1<L.size(); ++i){
            if(L[i].min_x < L[i+1].max_x)
                throw std::runtime_error("Sorry, I don't handle overlapping fixed cells yet\n");
        }
    }

    std::vector<std::vector<index_t> > cells_by_rows;

    auto final_cells = good_legalize(row_occupation, cells, cells_by_rows,
        surface.x_min_, surface.x_max_, surface.y_min_,
        row_height, nbr_rows
    );

    for(cell_leg_properties C : final_cells){
        new_placement.positions_[C.ind] = point<int_t>(C.x_pos, static_cast<int_t>(C.row_pos) * row_height + surface.y_min_);
        placement_rows[C.ind] = C.row_pos;
    }

    return detailed_placement(
        new_placement,
        placement_rows,
        cell_heights,
        cells_by_rows,
        surface.x_min_, surface.x_max_,
        surface.y_min_,
        nbr_rows, row_height
    );
}

} // namespace dp
} // namespace coloquinte

