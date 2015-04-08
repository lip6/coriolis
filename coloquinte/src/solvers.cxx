
#include "coloquinte/solvers.hxx"

#include <cassert>
#include <stdexcept>

namespace coloquinte{
namespace gp{

linear_system linear_system::operator+(linear_system const & o) const{
    if(o.internal_size() != internal_size()){ throw std::runtime_error("Mismatched system sizes"); }
    linear_system ret(target_.size() + o.target_.size() - internal_size(), internal_size());

    ret.matrix_ = matrix_;
    std::vector<matrix_triplet> omatrix = o.matrix_;
    for(matrix_triplet & t : omatrix){
        if(t.c_ >= internal_size()){
            t.c_ += (target_.size() - internal_size());
        }
        if(t.r_ >= internal_size()){
            t.r_ += (target_.size() - internal_size());
        }
    }
    ret.matrix_.insert(ret.matrix_.end(), omatrix.begin(), omatrix.end());

    // ret.target_.resize(target_.size() + o.target_.size() - internal_size);
    for(index_t i=0; i<internal_size(); ++i){
        ret.target_[i] = target_[i] + o.target_[i];
    }
    for(index_t i=internal_size(); i<target_.size(); ++i){
        ret.target_[i] = target_[i];
    }
    for(index_t i=internal_size(); i<o.target_.size(); ++i){
        ret.target_[i + target_.size() - internal_size()] = o.target_[i];
    }

    return ret;
}


// The classical compressed sparse row storage
struct csr_matrix{
    std::vector<std::uint32_t> row_limits, col_indexes;
    std::vector<float> values, diag;

    std::vector<float> mul(std::vector<float> const & x) const;
    std::vector<float> solve_CG(std::vector<float> const & goal, std::vector<float> guess, std::uint32_t min_iter, std::uint32_t max_iter, float tol) const;
    csr_matrix(std::vector<std::uint32_t> const & row_l, std::vector<std::uint32_t> const & col_i, std::vector<float> const & vals, std::vector<float> const D) : row_limits(row_l), col_indexes(col_i), values(vals), diag(D){
        assert(values.size() == col_indexes.size());
        assert(diag.size()+1 == row_limits.size());
    }
};

// A matrix with successive rows padded to the same length and accessed column-major; hopefully a little better
template<std::uint32_t unroll_len>
struct ellpack_matrix{
    std::vector<std::uint32_t> row_limits, col_indexes;
    std::vector<float> values, diag;

    std::vector<float> mul(std::vector<float> const & x) const;
    std::vector<float> solve_CG(std::vector<float> goal, std::vector<float> guess, std::uint32_t min_iter, std::uint32_t max_iter, float tol) const;

    ellpack_matrix(std::vector<std::uint32_t> const & row_l, std::vector<std::uint32_t> const & col_i, std::vector<float> const & vals, std::vector<float> const D) : row_limits(row_l), col_indexes(col_i), values(vals), diag(D){
        assert(values.size() == col_indexes.size());
        assert(diag.size() % unroll_len == 0);
        assert((row_limits.size()-1) * unroll_len == diag.size() );
        assert(row_limits.back() * unroll_len == values.size());
        assert(values.size() % unroll_len == 0);
        assert(col_indexes.size() % unroll_len == 0);
    }
};

// The proxy matrix for compressed sparse storage
class doublet_matrix{
    std::vector<std::uint32_t> row_limits;
    std::vector<matrix_doublet> doublets;
    std::uint32_t size;

    void get_compressed(std::vector<std::uint32_t> & limits, std::vector<matrix_doublet> & elements, std::vector<float> & diag) const;
    public:
    doublet_matrix(std::vector<matrix_triplet> const & triplets, std::uint32_t size);
    csr_matrix get_compressed_matrix() const;
    template<std::uint32_t unroll_len>
    ellpack_matrix<unroll_len> get_ellpack_matrix() const;
};

doublet_matrix::doublet_matrix(std::vector<matrix_triplet> const & triplets, std::uint32_t n) : size(n){
    row_limits.resize(size+1, 0);
     // First store the row sizes in the array
    for(uint32_t i=0; i<triplets.size(); ++i){
        ++row_limits[triplets[i].r_+1];
    }

    // The total size of the uncompressed matrix
    uint32_t tot_triplets=0;
    // Get the beginning position of each row in the csr matrix
    for(uint32_t i=1; i<n+1; ++i){
        uint32_t new_tot_triplets = tot_triplets + row_limits[i];
        row_limits[i] = tot_triplets; // Stores the beginning of the row
        tot_triplets = new_tot_triplets;
    }
    assert(tot_triplets == triplets.size());

    // Now we know the size and can allocate storage for the indices and values
    doublets.resize(tot_triplets);
    
    // We store the triplets in the new storage and tranform beginning positions into end positions
    for(uint32_t i=0; i<triplets.size(); ++i){
        doublets[row_limits[triplets[i].r_+1]] = matrix_doublet(triplets[i].c_, triplets[i].val_);
        ++row_limits[triplets[i].r_+1]; // row_limits will hold the end position of the row
    }
}

void doublet_matrix::get_compressed(std::vector<std::uint32_t> & sizes, std::vector<matrix_doublet> & elements, std::vector<float> & diag) const{
    assert(size+1 == row_limits.size());
    sizes.resize(size);
    diag.resize(size, 0.0);
    std::vector<matrix_doublet> tmp_doublets = doublets;

    for(uint32_t i=0; i<size; ++i){
        // Sort the elements in the row
        std::sort(tmp_doublets.begin() + row_limits[i], tmp_doublets.begin() + row_limits[i+1]);
        // Compress them and extract the diagonal
        std::uint32_t l=0;
        matrix_doublet cur(tmp_doublets[row_limits[i]]);
        for(uint32_t j=row_limits[i]+1; j<row_limits[i+1]; ++j){
            if(tmp_doublets[j].c_ == cur.c_){
                cur.val_ += tmp_doublets[j].val_;
            }
            else{
                if(i != cur.c_){
                    elements.push_back(cur);
                    ++l;
                }
                else{
                    diag[i] = cur.val_;
                }
                cur = tmp_doublets[j];
            }
        }
        if(i != cur.c_){
            elements.push_back(cur);
            ++l;
        }
        else{
            diag[i] = cur.val_;
        }
        sizes[i] = l;
    }
}

csr_matrix doublet_matrix::get_compressed_matrix() const{
    std::vector<matrix_doublet> tmp_doublets;
    std::vector<std::uint32_t> sizes;
    std::vector<float> diag;
    get_compressed(sizes, tmp_doublets, diag);

    // Get the limits of each row
    std::vector<std::uint32_t> new_row_limits(row_limits.size());
    new_row_limits[0] = 0;
    for(std::uint32_t i=0; i<size; ++i){
        new_row_limits[i+1] = new_row_limits[i] + sizes[i];
    }

    // Store the doublets to the sparse storage
    std::vector<std::uint32_t> col_indices(tmp_doublets.size());
    std::vector<float> values(tmp_doublets.size());
    for(std::uint32_t i=0; i<tmp_doublets.size(); ++i){
        col_indices[i] = tmp_doublets[i].c_;
        values[i] = tmp_doublets[i].val_;
    }

    return csr_matrix(new_row_limits, col_indices, values, diag);
}

template<std::uint32_t unroll_len>
ellpack_matrix<unroll_len> doublet_matrix::get_ellpack_matrix() const{
    std::vector<matrix_doublet> tmp_doublets;
    std::vector<std::uint32_t> sizes;
    std::vector<float> diag;
    get_compressed(sizes, tmp_doublets, diag);

    std::uint32_t unrolled_size = (diag.size() % unroll_len == 0)? diag.size()/unroll_len : diag.size() / unroll_len + 1;
    sizes.resize(unroll_len * unrolled_size, 0);
    diag.resize(unroll_len * unrolled_size, 1.0);

    // Store the maximum size of a group of rows
    std::vector<std::uint32_t> new_row_limits(unrolled_size+1);
    new_row_limits[0] = 0;
    for(std::uint32_t i=0; i<unrolled_size; ++i){
        std::uint32_t max_sz = sizes[unroll_len*i];
        for(int j=1; j<unroll_len; ++j){
            max_sz = std::max(max_sz, sizes[unroll_len*i + j]);
        }
        new_row_limits[i+1] = new_row_limits[i] + max_sz;
    }

    std::vector<std::uint32_t> col_indices(unroll_len * new_row_limits.back());
    std::vector<float> values(unroll_len * new_row_limits.back());

    std::uint32_t d = 0;
    for(std::uint32_t i=0; i<sizes.size(); ++i){ // For every line
        std::uint32_t ui = i/unroll_len;
        std::uint32_t k = i%unroll_len;
        std::uint32_t max_sz = new_row_limits[ui+1] - new_row_limits[ui];
        std::uint32_t row_begin = new_row_limits[ui];
        for(std::uint32_t j=0; j<sizes[i]; ++j, ++d){ // For the non-zero values
            col_indices[unroll_len * (row_begin+j) + k] = tmp_doublets[d].c_;
            values[unroll_len * (row_begin+j) + k] = tmp_doublets[d].val_;
        }
        for(std::uint32_t j=sizes[i]; j<max_sz; ++j){ // For the padding zeroes
            col_indices[unroll_len * (row_begin+j) + k] = 0;
            values[unroll_len * (row_begin+j) + k] = 0;
        }
    }
    
    return ellpack_matrix<unroll_len>(new_row_limits, col_indices, values, diag);
}

std::vector<float> csr_matrix::mul(std::vector<float> const & x) const{
    std::vector<float> res(x.size());
    assert(x.size() == diag.size());
    for(std::uint32_t i=0; i<diag.size(); ++i){
        res[i] = diag[i] * x[i];
        for(std::uint32_t j=row_limits[i]; j<row_limits[i+1]; ++j){
            res[i] += values[j] * x[col_indexes[j]];
        }
    }
    return res;
}

template<std::uint32_t unroll_len>
std::vector<float> ellpack_matrix<unroll_len>::mul(std::vector<float> const & x) const{
    std::vector<float> res(x.size());
    assert(x.size() % unroll_len == 0);
    assert(x.size() == diag.size());
    for(std::uint32_t i=0; i+1<row_limits.size(); ++i){
        float cur[unroll_len];
        for(int k=0; k<unroll_len; ++k){
            cur[k] = diag[unroll_len*i+k] * x[unroll_len*i+k];
        }
        for(std::uint32_t j=row_limits[i]; j<row_limits[i+1]; ++j){
            for(int k=0; k<unroll_len; ++k){
                cur[k] += values[unroll_len*j+k] * x[col_indexes[unroll_len*j+k]];
            }
        }
        for(int k=0; k<unroll_len; ++k){
            res[unroll_len*i+k] = cur[k];
        }
    }
    return res;
}

template<std::uint32_t unroll_len>
float dot_prod(std::vector<float> const & a, std::vector<float> const & b){
    assert(a.size() == b.size());
    float vals[unroll_len];
    for(int j=0; j<unroll_len; ++j) vals[j] = 0.0;
    for(std::uint32_t i=0; i<a.size() / unroll_len; ++i){
        for(int j=0; j<unroll_len; ++j){
            vals[j] += a[unroll_len*i + j] * b[unroll_len*i + j];
        }
    }
    float res = 0.0;
    for(int j=0; j<unroll_len; ++j) res += vals[j];
    for(int i = unroll_len*(a.size() / unroll_len); i< a.size(); ++i){
        res += a[i] * b[i];
    }
    return res;
}

std::vector<float> csr_matrix::solve_CG(std::vector<float> const & goal, std::vector<float> x, std::uint32_t min_iter, std::uint32_t max_iter, float tol_ratio) const{
    std::uint32_t n = diag.size();
    assert(goal.size() == n);
    assert(x.size() == n);
    std::vector<float> r, p(n), z(n), mul_res, preconditioner(n);
    r = mul(x);
    for(uint32_t i=0; i<n; ++i){
        r[i] = goal[i] - r[i];
        preconditioner[i] = 1.0/diag[i];
        assert(std::isfinite(preconditioner[i]));
        z[i] = preconditioner[i] * r[i];
        p[i] = z[i];
    }

    float cross_norm = dot_prod<16>(r, z);
    assert(std::isfinite(cross_norm));
    float_t const epsilon = std::numeric_limits<float_t>::min();

    float start_norm = cross_norm;
    for(uint32_t k=0; k < max_iter; ++k){
        mul_res = mul(p);

        float_t pr_prod = dot_prod<16>(p, mul_res);
        float_t alpha = cross_norm / pr_prod;

        if(
            not std::isfinite(cross_norm) or not std::isfinite(alpha) or not std::isfinite(pr_prod)
            or cross_norm <= epsilon or alpha <= epsilon or pr_prod <= epsilon
            ){
            break;
        }

        // Update the result
        for(uint32_t i=0; i<n; ++i){
            x[i] = x[i] + alpha * p[i];
            r[i] = r[i] - alpha * mul_res[i];
            z[i] = preconditioner[i] * r[i];
        }
        float new_cross_norm = dot_prod<16>(r, z); 

        // Update the scaled residual and the search direction
        if(k >= min_iter && new_cross_norm <= tol_ratio * start_norm){
            break;
        }
        float beta = new_cross_norm / cross_norm;
        cross_norm = new_cross_norm;
        for(uint32_t i=0; i<n; ++i)
            p[i] = z[i] + beta * p[i];
    }

    return x;
}

template<std::uint32_t unroll_len>
std::vector<float> ellpack_matrix<unroll_len>::solve_CG(std::vector<float> goal, std::vector<float> x, std::uint32_t min_iter, std::uint32_t max_iter, float tol_ratio) const{
    std::uint32_t n = diag.size();
    std::uint32_t old_n = x.size();
    assert(goal.size() == x.size());
    x.resize(diag.size(), 0.0);
    goal.resize(diag.size(), 0.0);

    std::vector<float> r, p(n), z(n), mul_res, preconditioner(n);
    r = mul(x);
    for(uint32_t i=0; i<n; ++i){
        r[i] = goal[i] - r[i];
        preconditioner[i] = 1.0/diag[i];
        z[i] = preconditioner[i] * r[i];
        p[i] = z[i];
    }

    float cross_norm = dot_prod<unroll_len>(r, z);
    float start_norm = cross_norm;
    for(uint32_t k=0; k < max_iter; ++k){
        mul_res = mul(p);
        float alpha = cross_norm / dot_prod<unroll_len>(p, mul_res);
        // Update the result
        for(uint32_t i=0; i<n; ++i){
            x[i] = x[i] + alpha * p[i];
            r[i] = r[i] - alpha * mul_res[i];
            z[i] = preconditioner[i] * r[i];
        }
        float new_cross_norm = dot_prod<unroll_len>(r, z); 

        // Update the scaled residual and the search direction
        if(k >= min_iter && new_cross_norm <= tol_ratio * start_norm){
            break;
        }
        float beta = new_cross_norm / cross_norm;
        cross_norm = new_cross_norm;
        for(uint32_t i=0; i<n; ++i)
            p[i] = z[i] + beta * p[i];
    }
    x.resize(old_n);
    return x;
}

std::vector<float_t> linear_system::solve_CG(std::vector<float_t> guess, index_t nbr_iter){
    doublet_matrix tmp(matrix_, size());
    csr_matrix mat = tmp.get_compressed_matrix();
    //ellpack_matrix<16> mat = tmp.get_ellpack_matrix<16>();
    guess.resize(target_.size(), 0.0);
    auto ret = mat.solve_CG(target_, guess, nbr_iter, nbr_iter, 0.0);
    ret.resize(internal_size());
    return ret;
}

}
}



