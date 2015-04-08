
#ifndef COLOQUINE_GP_SOLVERS
#define COLOQUINE_GP_SOLVERS

#include "common.hxx"

#include <vector>

namespace coloquinte{
namespace gp{

struct matrix_doublet{
    index_t c_;
    float val_;
    bool operator<(matrix_doublet const o) const{ return c_ < o.c_; }
    matrix_doublet(){}
    matrix_doublet(index_t c, float v) : c_(c), val_(v){}
};

struct matrix_triplet{
    index_t r_, c_;
    float_t val_;
    matrix_triplet(index_t ri, index_t ci, float_t v) : r_(ri), c_(ci), val_(v){}
    bool operator<(matrix_triplet const o){ return r_ < o.r_ || (r_ == o.r_ && c_ < o.c_); }
};

class linear_system{
    std::vector<matrix_triplet> matrix_;
    std::vector<float_t> target_;
    index_t internal_size_;
    
    public:
    void add_triplet(index_t row, index_t col, float_t val){ matrix_.push_back(matrix_triplet(row, col, val)); }

    linear_system operator+(linear_system const & o) const;

    void add_doublet(index_t row, float_t val){
        target_[row] += val;
    }

    void add_force(
        float_t force,
        index_t c1,    index_t c2,
        float_t offs1, float_t offs2
    ){
        add_triplet(c1, c1, force);
        add_triplet(c2, c2, force);
        add_triplet(c1, c2, -force);
        add_triplet(c2, c1, -force);
        add_doublet(c1, force * (offs2-offs1));
        add_doublet(c2, force * (offs1-offs2));
    }

    void add_fixed_force(
        float_t force,
        index_t c,
        float_t fixed_pos,
        float_t offs
    ){
        add_triplet(c, c, force);
        add_doublet(c, force * (fixed_pos-offs));
    }

    void add_anchor(
        float_t scale,
        index_t c,
        float_t pos
    ){
        add_triplet(c, c, scale);
        add_doublet(c, scale*pos);
    }

    linear_system(index_t s) : target_(s, 0.0), internal_size_(s){}
    linear_system(index_t s, index_t i) : target_(s, 0.0), internal_size_(i){}

    index_t size() const{ return target_.size(); }
    index_t internal_size() const{ return internal_size_; }
    void add_variables(index_t cnt){ target_.resize(target_.size() + cnt, 0.0); }

    std::vector<float_t> solve_CG(std::vector<float_t> guess, index_t nbr_iter);
};

} // namespace gp
} // namespace coloquinte

#endif


