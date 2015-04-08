
#include "coloquinte/detailed.hxx"
#include "coloquinte/circuit_helper.hxx"

#include <cassert>

namespace coloquinte{
namespace dp{

detailed_placement::detailed_placement(
        placement_t pl,
        std::vector<index_t> placement_rows,
        std::vector<index_t> cell_heights,
        std::vector<std::vector<index_t> > rows,
        int_t min_x, int_t max_x,
        int_t y_origin,
        index_t nbr_rows, int_t row_height
    )
    :
        plt_(pl),
        cell_rows_(placement_rows),
        min_x_(min_x), max_x_(max_x),
        y_origin_(y_origin)
    {

    assert(row_height > 0);
    assert(min_x < max_x);
    assert(rows.size() == nbr_rows);

    neighbours_limits_.push_back(0); 
    for(index_t h : cell_heights){
        neighbours_limits_.push_back(neighbours_limits_.back() + h);
    }

    neighbours_ .resize(neighbours_limits_.back(), std::pair<index_t, index_t>(null_ind, null_ind) );

    row_first_cells_ .resize(nbr_rows, null_ind);
    row_last_cells_  .resize(nbr_rows, null_ind);

    std::vector<bool> explored(neighbours_limits_.back(), false);
    // Now we extract the dependencies
    for(index_t r=0; r<rows.size(); ++r){

        if(not rows[r].empty()){
            row_first_cells_[r] = rows[r].front();
            row_last_cells_[r]  = rows[r].back();
        }

        for(index_t c : rows[r]){
            // Has this row of the cell already been visited?
            assert(not explored[neighbour_index(c, r)]);
            explored[neighbour_index(c, r)] = true;
        }

        for(index_t i=0; i+1<rows[r].size(); ++i){
            index_t c1 = rows[r][i], c2 = rows[r][i+1];

            // Save in the internal format
            neighbours_[neighbour_index(c1, r)].second = c2;
            neighbours_[neighbour_index(c2, r)].first  = c1;

            // The positions are correct
        }
    }

    // Every level of every cell must have been visited
    for(bool o : explored)
        assert(o);

    // Verify that we haven't made any obvious mistake
    selfcheck();
}

void detailed_placement::selfcheck() const{
    assert(row_first_cells_.size() == row_last_cells_.size());

    for(index_t i=0; i<cell_cnt(); ++i){
        for(index_t l=0; l<cell_height(i); ++l){
            // not verified now since we don't modify the position for the obstacles
            // : assert(c.position.x_ >= min_x_ and c.position.x_ + c.width <= max_x_);

            index_t n_ind = l + neighbours_limits_[i];
            assert(cell_rows_[i] + cell_height(i) <= row_cnt());

            if(neighbours_[n_ind].first != null_ind){
                index_t oi = neighbours_[n_ind].first;
                // Correct neighbour position
                assert(neighbours_[neighbour_index(oi, cell_rows_[i]+l)].second == i);
            }
            else{
                // Beginning of a row
                assert(row_first_cells_[cell_rows_[i] + l] == i);
            }
            if(neighbours_[n_ind].second != null_ind){
                index_t oi = neighbours_[n_ind].second;
                // Correct neighbour position
                assert(neighbours_[neighbour_index(oi, cell_rows_[i]+l)].first == i);
            }
            else{
                // End of a row
                assert(row_last_cells_[cell_rows_[i] + l] == i);
            }
        }
    }
}

void detailed_placement::swap_standard_cell_topologies(index_t c1, index_t c2){
    assert(cell_height(c1) == cell_height(c2));
    assert(cell_height(c1) == 1 and cell_height(c2) == 1);

    index_t row_c1 = cell_rows_[c1],
            row_c2 = cell_rows_[c2];

    index_t b_c1 = neighbours_[neighbours_limits_[c1]].first;
    index_t b_c2 = neighbours_[neighbours_limits_[c2]].first;
    index_t a_c1 = neighbours_[neighbours_limits_[c1]].second;
    index_t a_c2 = neighbours_[neighbours_limits_[c2]].second;

    // Two cases: they were adjacent or they were not
    // Luckily updating in the neighbours first then swapping the recorded neighbours works in both cases for standard cells

    // Update the pointers in the cells' neighbours
    if(b_c1 != null_ind) neighbours_[neighbour_index(b_c1, row_c1)].second = c2;
    else row_first_cells_[row_c1] = c2;
    if(b_c2 != null_ind) neighbours_[neighbour_index(b_c2, row_c2)].second = c1;
    else row_first_cells_[row_c2] = c1;

    if(a_c1 != null_ind) neighbours_[neighbour_index(a_c1, row_c1)].first  = c2;
    else row_last_cells_[row_c1] = c2;
    if(a_c2 != null_ind) neighbours_[neighbour_index(a_c2, row_c2)].first  = c1;
    else row_last_cells_[row_c2] = c1;

    // Swap the properties in both cells
    std::swap(neighbours_[neighbours_limits_[c1]], neighbours_[neighbours_limits_[c2]]);
    std::swap(cell_rows_[c1], cell_rows_[c2]);
}

std::pair<int_t, int_t> detailed_placement::get_limit_positions(netlist const & circuit, index_t c) const{
    auto ret = std::pair<int_t, int_t>(min_x_, max_x_);
    for(index_t l=neighbours_limits_[c]; l<neighbours_limits_[c+1]; ++l){
        index_t b_i = neighbours_[l].first,
                a_i = neighbours_[l].second;

        if(b_i != null_ind){
            ret.first  = std::max(ret.first,  plt_.positions_[b_i].x_ + circuit.get_cell(b_i).size.x_);
        }
        if(a_i != null_ind){
            ret.second = std::min(ret.second, plt_.positions_[a_i].x_);
        }
    }
    return ret;
}

index_t detailed_placement::get_first_cell_on_row(index_t r){
    return row_first_cells_[r];
}

index_t detailed_placement::get_first_standard_cell_on_row(index_t r){
    index_t c = get_first_cell_on_row(r);
    while(c != null_ind and cell_height(c) != 1){
        index_t next_c = get_next_cell_on_row(c, r);
        assert(c != next_c);
        c = next_c;
    }
    assert(c == null_ind or cell_rows_[c] == r);
    return c;
}

index_t detailed_placement::get_next_cell_on_row(index_t c, index_t r){
    return neighbours_[neighbour_index(c, r)].second;
}
index_t detailed_placement::get_prev_cell_on_row(index_t c, index_t r){
    return neighbours_[neighbour_index(c, r)].first;
}

index_t detailed_placement::get_next_standard_cell_on_row(index_t c, index_t r){
    do{
        index_t next_c = get_next_cell_on_row(c, r);
        assert(c != next_c);
        c = next_c;
    }while(c != null_ind and cell_height(c) != 1);
    assert(c == null_ind or cell_rows_[c] == r);
    return c;
}

void detailed_placement::reorder_cells(std::vector<index_t> const old_order, std::vector<index_t> const new_order, index_t r){
    assert(old_order.size() == new_order.size());
    assert(not old_order.empty());

    index_t before_row = get_prev_cell_on_row(old_order.front(), r);
    index_t after_row  = get_next_cell_on_row(old_order.back(),  r);

    for(index_t i=0; i<new_order.size(); ++i){
        auto & nghs = neighbours_[neighbour_index(new_order[i], r)];
        if(i > 0){
            nghs.first = new_order[i-1];
        }
        else{
            nghs.first = before_row;
        }
        if(i+1 < new_order.size()){
            nghs.second = new_order[i+1];
        }
        else{
            nghs.second = after_row;
        }
    }

    if(before_row != null_ind) neighbours_[neighbour_index(before_row, r)].second = new_order.front();
    else row_first_cells_[r] = new_order.front();
    if(after_row != null_ind) neighbours_[neighbour_index(after_row, r)].first = new_order.back();
    else row_last_cells_[r] = new_order.back(); 
}

void detailed_placement::reorder_standard_cells(std::vector<index_t> const old_order, std::vector<index_t> const new_order){
    assert(old_order.size() == new_order.size());
    assert(not old_order.empty());

    index_t before_row = neighbours_[neighbours_limits_[old_order.front()]].first;
    index_t after_row  = neighbours_[neighbours_limits_[old_order.back() ]].second;

    index_t r = cell_rows_[new_order.front()];

    for(index_t i=0; i<new_order.size(); ++i){
        assert(cell_height(new_order[i]) == 1);
        assert(cell_rows_[new_order[i]] == r);

        auto & nghs = neighbours_[neighbours_limits_[new_order[i]]];
        if(i > 0){
            nghs.first = new_order[i-1];
        }
        else{
            nghs.first = before_row;
        }
        if(i+1 < new_order.size()){
            nghs.second = new_order[i+1];
        }
        else{
            nghs.second = after_row;
        }
    }

    if(before_row != null_ind) neighbours_[neighbour_index(before_row, r)].second = new_order.front();
    else row_first_cells_[r] = new_order.front();
    if(after_row != null_ind) neighbours_[neighbour_index(after_row, r)].first = new_order.back();
    else row_last_cells_[r] = new_order.back(); 
}

void row_compatible_orientation(netlist const & circuit, detailed_placement & pl, bool first_row_orient){
    for(index_t c=0; c<circuit.cell_cnt(); ++c){
        if( (circuit.get_cell(c).attributes & YFlippable) != 0 and pl.cell_height(c) == 1){
            pl.plt_.orientations_[c].y_ = (pl.cell_rows_[c] % 2 != 0) ^ first_row_orient;
        }
    }
}

} // namespace dp
} // namespace coloquinte


