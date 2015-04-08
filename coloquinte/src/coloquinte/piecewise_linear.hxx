

#include "common.hxx"

#include <vector>

namespace coloquinte{

typedef std::pair<int_t, int_t> p_v;

struct piecewise_linear_function{
    std::vector<p_v> point_values;

    static piecewise_linear_function minimum(piecewise_linear_function const & a, piecewise_linear_function const & b);
    piecewise_linear_function previous_min_of_sum(piecewise_linear_function const & o, int_t added_cell_width) const;
    piecewise_linear_function previous_min() const;

    int_t value_at(int_t pos) const;
    int_t last_before(int_t pos) const;

    void add_monotone(int_t slope, int_t offset);
    void add_bislope(int_t s_l, int_t s_r, int_t pos);

    piecewise_linear_function(){}
    piecewise_linear_function(int_t min_def, int_t max_def);
};

} // End namespace coloquinte

