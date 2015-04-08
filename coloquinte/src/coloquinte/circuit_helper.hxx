
#ifndef COLOQUINTE_GP_HELPERCIRCUIT
#define COLOQUINTE_GP_HELPERCIRCUIT

#include "common.hxx"
#include "netlist.hxx"

namespace coloquinte{

struct pin_1D{
    index_t cell_ind;
    int_t   pos;
    int_t   offs;
    bool    movable;

    bool operator<(pin_1D const o) const { return pos < o.pos; }

    pin_1D(index_t c, int_t p, int_t o, bool m) : cell_ind(c), pos(p), offs(o), movable(m){}
};
struct pin_2D{
    index_t      cell_ind;
    point<int_t> pos;
    point<int_t> offs;
    bool         movable;

    pin_2D(index_t c, point<int_t> p, point<int_t> o, bool m) : cell_ind(c), pos(p), offs(o), movable(m){}
    pin_1D x() const{ return pin_1D(cell_ind, pos.x_, offs.x_, movable); }
    pin_1D y() const{ return pin_1D(cell_ind, pos.y_, offs.y_, movable); }
};

inline int_t dist(pin_2D const a, pin_2D const b){
    point<int_t> diff = a.pos - b.pos;
    return std::abs(diff.x_) + std::abs(diff.y_);
}

inline std::vector<pin_2D>         get_pins_2D(netlist const & circuit, placement_t const & pl, index_t net_ind){
    std::vector<pin_2D> ret;
    for(auto p : circuit.get_net(net_ind)){
        assert(std::isfinite(pl.positions_[p.cell_ind].x_) and std::isfinite(pl.positions_[p.cell_ind].y_));
        assert(std::isfinite(pl.orientations_[p.cell_ind].x_) and std::isfinite(pl.orientations_[p.cell_ind].y_));

        point<int_t> offs;
            offs.x_ = pl.orientations_[p.cell_ind].x_ ? p.offset.x_ : circuit.get_cell(p.cell_ind).size.x_ - p.offset.x_;
            offs.y_ = pl.orientations_[p.cell_ind].y_ ? p.offset.y_ : circuit.get_cell(p.cell_ind).size.y_ - p.offset.y_;
        point<int_t> pos  = offs + pl.positions_[p.cell_ind];

        assert(std::isfinite(offs.x_) and std::isfinite(offs.y_));
        assert(std::isfinite(pos.x_) and std::isfinite(pos.y_));

        bool movable = (circuit.get_cell(p.cell_ind).attributes & XMovable) != 0 and (circuit.get_cell(p.cell_ind).attributes & YMovable) != 0;
        ret.push_back(pin_2D(p.cell_ind, pos, offs, movable));
    }
    return ret;
}

inline point<std::vector<pin_1D> > get_pins_1D(netlist const & circuit, placement_t const & pl, index_t net_ind){
    point<std::vector<pin_1D> > ret;
    for(auto p : circuit.get_net(net_ind)){
        assert(std::isfinite(pl.positions_[p.cell_ind].x_) and std::isfinite(pl.positions_[p.cell_ind].y_));
        assert(std::isfinite(pl.orientations_[p.cell_ind].x_) and std::isfinite(pl.orientations_[p.cell_ind].y_));

        point<int_t> offs;
            offs.x_ = pl.orientations_[p.cell_ind].x_ ? p.offset.x_ : circuit.get_cell(p.cell_ind).size.x_ - p.offset.x_;
            offs.y_ = pl.orientations_[p.cell_ind].y_ ? p.offset.y_ : circuit.get_cell(p.cell_ind).size.y_ - p.offset.y_;
        point<int_t> pos  = offs + pl.positions_[p.cell_ind];

        assert(std::isfinite(offs.x_) and std::isfinite(offs.y_));
        assert(std::isfinite(pos.x_) and std::isfinite(pos.y_));

        bool x_movable = (circuit.get_cell(p.cell_ind).attributes & XMovable) != 0;
        bool y_movable = (circuit.get_cell(p.cell_ind).attributes & YMovable) != 0;

        ret.x_.push_back(pin_1D(p.cell_ind, pos.x_, offs.x_, x_movable));
        ret.y_.push_back(pin_1D(p.cell_ind, pos.y_, offs.y_, y_movable));
    }
    return ret;
}

std::int64_t MST_length(std::vector<point<int_t> > const & pins);
std::int64_t RSMT_length(std::vector<point<int_t> > const & pins, index_t exactitude_limit);
std::int64_t get_HPWL_length(netlist const & circuit, placement_t const & pl, index_t net_ind);
std::int64_t get_RSMT_length(netlist const & circuit, placement_t const & pl, index_t net_ind);

std::vector<std::pair<index_t, index_t> > get_MST_topology(std::vector<point<int_t> > const & pins);
point<std::vector<std::pair<index_t, index_t> > > get_RSMT_topology(std::vector<point<int_t> > const & pins, index_t exactitude_limit);

} // namespace coloquinte

#endif

