
#ifndef COLOQUINTE_GP_ROUGH_LEGALIZER
#define COLOQUINTE_GP_ROUGH_LEGALIZER

#include "common.hxx"
#include "netlist.hxx"

#include <vector>
#include <cassert>
#include <cmath>
#include <functional>

/*
 * A simple class to perform approximate legalization with extreme efficiency
 * 
 * To be called during global placement or before an exact legalization
 *
 */

namespace coloquinte{
namespace gp{

class region_distribution{
    /*
     * Coordinates are mostly float but obstacles and areas are integers for correctness
     */

    public:
    struct movable_cell{
        capacity_t demand_; // == area; No FP!!!
        point<float_t> pos_;  // Target position, determining the cost to allocate it
        // int_t x_size, y_size; // May split cells
        index_t index_in_placement_;

        movable_cell();
        movable_cell(capacity_t demand, point<float_t> p, index_t ind);
    };

    // Specifies a maximum density of movable cells per usable area
    // Representing either a macroblock or a routing congestion
    struct density_limit{
        box<int_t> box_;
        float_t density_; // from 0.0 for a macro to 1.0 if it does nothing
    };

    private:

    struct region;
    
    struct cell_ref{
        capacity_t allocated_capacity_;
        point<float_t> pos_;
        index_t index_in_list_;

        cell_ref(){}
        cell_ref(capacity_t demand, point<float_t> p, index_t ind) : allocated_capacity_(demand), pos_(p), index_in_list_(ind){}
        friend region;
    };
    
    struct region{
        public:
        // Data members
        capacity_t capacity_; // ==area; No floating point!!! 
        point<float_t> pos_;
    
        std::vector<cell_ref> cell_references_;

        // Constructors
        region(){} // Necessary if we want to resize vectors 
        region(capacity_t cap, point<float_t> pos, std::vector<cell_ref> cells);

        // Helper functions for bipartitioning
        private:
        static void distribute_new_cells(region & a, region & b, std::vector<cell_ref> cells); // Called by the other two to do the dirty work
        public:
        void distribute_cells(region & a, region & b) const;    // Distribute the cells from one region to two
        static void redistribute_cells(region & a, region & b); // Optimizes the distribution between two regions

        // Helper functions for multipartitioning
        private:
        static void distribute_new_cells(std::vector<std::reference_wrapper<region_distribution::region> > regions, std::vector<cell_ref> cells);
        public:
        void distribute_cells(std::vector<std::reference_wrapper<region_distribution::region> > regions) const;
        static void redistribute_cells(std::vector<std::reference_wrapper<region_distribution::region> > regions);

        // Helper functions for 1D transportation
        public:
        static void distribute_new_cells(std::vector<std::reference_wrapper<region_distribution::region> > regions, std::vector<cell_ref> cells, std::function<float_t (point<float_t>)> coord);
        static void redistribute_cells(std::vector<std::reference_wrapper<region_distribution::region> > & regions, std::function<float_t (point<float_t>)> coord);

        public:
        void uniquify_references();
        void selfcheck() const;

        // Accessors
        capacity_t capacity() const;
        capacity_t allocated_capacity() const;
        capacity_t unused_capacity() const;
        index_t cell_cnt() const;

        float_t distance(cell_ref const & C) const;
        float_t cost() const;
    };

    private:
    // Members
    index_t x_regions_cnt_, y_regions_cnt_;
    
    std::vector<movable_cell> cell_list_;
    std::vector<region> placement_regions_;

    box<int_t> placement_area_;
    std::vector<density_limit> density_map_;
    const capacity_t full_density_mul; // Multiplicator giving the grain for fractional areas for the surface
          capacity_t cell_density_mul; // ANd for the cells
    float_t density_scaling_factor_;
    
    private:
    // Helper functions
    region & get_region(index_t x_coord, index_t y_coord);
    region const & get_region(index_t x_coord, index_t y_coord) const;
    box<int_t> get_box(index_t x, index_t y, index_t x_cnt, index_t y_cnt) const;

    static void sort_uniquify(std::vector<cell_ref> & cell_references);
    static void just_uniquify(std::vector<cell_ref> & cell_references);

    // Prepare regions with the right positions and capacities; different levels of nesting are compatible
    std::vector<region> prepare_regions(index_t x_cnt, index_t y_cnt) const;

    public:
    
    inline index_t x_regions_cnt() const;
    inline index_t y_regions_cnt() const;
    inline index_t regions_cnt()   const;
    
    inline index_t cell_cnt() const;
    inline index_t fractional_cell_cnt() const;
    
    /*
     * Two types of export
     *    Region center             : upper bound of legalization cost
     *    1D quadratic optimization : lower bound of legalization cost
     */

    std::vector<movable_cell> export_positions() const;
    std::vector<movable_cell> export_spread_positions_quadratic() const;
    std::vector<movable_cell> export_spread_positions_linear() const;

    // The cost as seen by the partitioning algorithms (but not the export)
    float_t cost() const;

    /*
     * Further partitions
     */
    
    void x_bipartition();
    void y_bipartition();
    void x_resize(index_t sz);
    void y_resize(index_t sz);
    void multipartition(index_t x_width, index_t y_width);
    void multipartition(index_t width){ multipartition(width, width); }
    
    /*
     * Optimization functions
     */

    // Bipartitioning: only two regions are considered at a time
    void redo_adjacent_bipartitions();
    void redo_diagonal_bipartitions();
    void redo_bipartitions();

    // Line partitioning: optimal on coordinate axis with Manhattan distance (Euclidean distance could use it in any direction)
    void redo_line_partitions();

    // Multipartitioning: several regions considered, slow runtimes
    void redo_diag_partitions(index_t len);
    void redo_multipartitions(index_t x_width, index_t y_width);
    void redo_multipartitions(index_t width){ redo_multipartitions(width, width); }

    // Try to remove duplicate fractional cells    
    void fractions_minimization();

    // Verify
    void selfcheck() const;

    private:
    region_distribution(box<int_t> placement_area, netlist const & circuit, placement_t const & pl, std::vector<density_limit> const & density_map, bool full_density);

    public:
    /*
     * Obtain a region_distribution from a placement
     *
     *     Full density: the object tries to pack the cells as much as possible while still respecting the density limits
     *     Uniform density: not only are the density limits respected, the allocated capacities are proportional to the allowed densities
     *
     */

    static region_distribution full_density_distribution(box<int_t> placement_area, netlist const & circuit, placement_t const & pl, std::vector<density_limit> const & density_map = std::vector<density_limit>());
    static region_distribution uniform_density_distribution(box<int_t> placement_area, netlist const & circuit, placement_t const & pl, std::vector<density_limit> const & density_map = std::vector<density_limit>());

    void update(netlist const & circuit, placement_t const & pl);
};

inline region_distribution::movable_cell::movable_cell(){}
inline region_distribution::movable_cell::movable_cell(capacity_t demand, point<float_t> p, index_t ind) : demand_(demand), pos_(p), index_in_placement_(ind){}

inline index_t region_distribution::x_regions_cnt() const { return x_regions_cnt_; }
inline index_t region_distribution::y_regions_cnt() const { return y_regions_cnt_; }
inline index_t region_distribution::regions_cnt()   const { index_t ret = x_regions_cnt() * y_regions_cnt(); assert(placement_regions_.size() == ret); return ret; }
inline region_distribution::region & region_distribution::get_region(index_t x_coord, index_t y_coord){
    return placement_regions_[y_coord * x_regions_cnt() + x_coord];
}
inline region_distribution::region const & region_distribution::get_region(index_t x_coord, index_t y_coord) const{
    return placement_regions_[y_coord * x_regions_cnt() + x_coord];
}

inline index_t region_distribution::cell_cnt() const{ return cell_list_.size(); }
inline index_t region_distribution::fractional_cell_cnt() const{
    index_t tot_cnt = 0;
    for(auto const & R : placement_regions_){
        tot_cnt += R.cell_cnt();
    }
    return tot_cnt;
}


inline capacity_t region_distribution::region::capacity() const{ return capacity_; }
inline capacity_t region_distribution::region::unused_capacity() const{ return capacity() - allocated_capacity(); }
inline capacity_t region_distribution::region::allocated_capacity() const{
    capacity_t ret = 0;
    for(cell_ref const C : cell_references_){
       ret += C.allocated_capacity_; 
    }
    return ret;
}
inline index_t region_distribution::region::cell_cnt() const{ return cell_references_.size(); }

inline float_t region_distribution::region::distance(region_distribution::cell_ref const & C) const{
    return std::abs(pos_.x_ - C.pos_.x_) + std::abs(pos_.y_ - C.pos_.y_);
    /*
    float_t manhattan = std::max(static_cast<float_t>(0.0), std::max(C.pos_.x_ - surface_.x_max_, surface_.x_min_ - C.pos_.x_))
                      + std::max(static_cast<float_t>(0.0), std::max(C.pos_.y_ - surface_.y_max_, surface_.y_min_ - C.pos_.y_));
    return manhattan * (1.0 + manhattan * 0.0001);
    */
}


} // Namespace gp
} // Namespace coloquinte

#endif

