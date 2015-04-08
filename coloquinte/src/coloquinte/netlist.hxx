
#ifndef COLOQUINTE_NETLIST
#define COLOQUINTE_NETLIST

#include "common.hxx"
#include <vector>
#include <cassert>


namespace coloquinte{

// Structures for construction and circuit_loader
struct temporary_pin{
    point<int_t> offset;
    index_t cell_ind, net_ind;
    temporary_pin(){}
    temporary_pin(point<int_t> offs, index_t c, index_t n) : offset(offs), cell_ind(c), net_ind(n){}
};

struct temporary_cell{
    point<int_t> size;
    capacity_t area;
    mask_t attributes;
    index_t list_index;

    temporary_cell(){}
    temporary_cell(point<int_t> s, mask_t attr, index_t ind) : size(s), attributes(attr), list_index(ind){ area = static_cast<capacity_t>(s.x_) * static_cast<capacity_t>(s.y_);}
};

struct temporary_net{
    int_t weight;
    index_t list_index;
    temporary_net(){}
    temporary_net(index_t ind, int_t wght) : weight(wght), list_index(ind){}
};


// Main class
class netlist{
    std::vector<int_t>       net_weights_;

    std::vector<capacity_t>    cell_areas_;
    std::vector<point<int_t> > cell_sizes_;
    std::vector<mask_t>        cell_attributes_;

    // Mapping of the order given at construction time to the internal representation
    std::vector<index_t>       cell_internal_mapping_;
    std::vector<index_t>       net_internal_mapping_;

    // Optimized sparse storage for nets
    std::vector<index_t>         net_limits_;
    std::vector<index_t>         cell_indexes_;
    std::vector<point<int_t> > pin_offsets_;

    // Sparse storage from cell to net appartenance
    std::vector<index_t>         cell_limits_;
    std::vector<index_t>         net_indexes_;
    std::vector<index_t>         pin_indexes_;

    public:
    netlist(std::vector<temporary_cell> cells, std::vector<temporary_net> nets, std::vector<temporary_pin> all_pins);
    netlist(){}

    void selfcheck() const;

    struct pin_t{
        point<int_t> offset;
        index_t cell_ind, net_ind;
        pin_t(point<int_t> offs, index_t c, index_t n) : offset(offs), cell_ind(c), net_ind(n){}
    };

    class net_pin_iterator{
        index_t pin_ind, net_ind;
        netlist const & N;

        public:
        pin_t operator*() const{
            return pin_t(N.pin_offsets_[pin_ind], N.cell_indexes_[pin_ind], net_ind);
        }
        net_pin_iterator & operator++(){
            pin_ind++;
            return *this;
        }
        bool operator!=(net_pin_iterator const o) const{
            return pin_ind != o.pin_ind;
        }

        net_pin_iterator(index_t net_index, index_t pin_index, netlist const & orig) : pin_ind(pin_index), net_ind(net_index), N(orig){}
    };

    class cell_pin_iterator{
        index_t pin_ind, cell_ind;
        netlist const & N;

        public:
        pin_t operator*() const{
            return pin_t(N.pin_offsets_[N.pin_indexes_[pin_ind]], cell_ind, N.net_indexes_[pin_ind]);
        }
        cell_pin_iterator & operator++(){
            pin_ind++;
            return *this;
        }
        bool operator!=(cell_pin_iterator const o) const{
            return pin_ind != o.pin_ind;
        }

        cell_pin_iterator(index_t cell_index, index_t pin_index, netlist const & orig) : pin_ind(pin_index), cell_ind(cell_index), N(orig){}
    };

    struct internal_cell{
        point<int_t> size;
        capacity_t area;
        mask_t attributes;
        netlist const & N;
        index_t index;
        index_t pin_cnt;

        internal_cell(index_t ind, netlist const & orig) :
            size(orig.cell_sizes_[ind]),
            area(orig.cell_areas_[ind]),
            attributes(orig.cell_attributes_[ind]),
            N(orig),
            index(ind),
            pin_cnt(N.cell_limits_[ind+1] - N.cell_limits_[ind])
            {}

        cell_pin_iterator begin(){ return cell_pin_iterator(index, N.cell_limits_[index], N); }
        cell_pin_iterator end(){ return cell_pin_iterator(index, N.cell_limits_[index+1], N); }
    };

    struct internal_net{
        int_t weight;
        netlist const & N;
        index_t index;
        index_t pin_cnt;

        internal_net(index_t ind, netlist const & orig) :
            weight(orig.net_weights_[ind]),
            N(orig),
            index(ind),
            pin_cnt(N.net_limits_[ind+1] - N.net_limits_[ind])
            {}

        net_pin_iterator begin(){ return net_pin_iterator(index, N.net_limits_[index], N); }
        net_pin_iterator end(){ return net_pin_iterator(index, N.net_limits_[index+1], N); }
    };

    internal_cell get_cell(index_t ind) const{
        return internal_cell(ind, *this);
    }
    internal_net  get_net(index_t ind) const{
        return internal_net(ind, *this);
    }

    index_t cell_cnt() const{ return cell_internal_mapping_.size(); }
    index_t net_cnt()  const{ return net_internal_mapping_.size(); }
    index_t pin_cnt()  const{ return pin_offsets_.size(); }

    index_t get_cell_ind(index_t external_ind) const{ return cell_internal_mapping_[external_ind]; }
    index_t get_net_ind(index_t external_ind) const{ return net_internal_mapping_[external_ind]; }

};

inline netlist::netlist(std::vector<temporary_cell> cells, std::vector<temporary_net> nets, std::vector<temporary_pin> all_pins){
    struct extended_pin : public temporary_pin{
        index_t pin_index;
        extended_pin(temporary_pin const p) : temporary_pin(p){} 
    };
    std::vector<extended_pin> pins;
    for(temporary_pin const p : all_pins){
        pins.push_back(extended_pin(p));
    }

    cell_limits_.resize(cells.size()+1);
    net_limits_.resize(nets.size()+1);

    net_weights_.resize(nets.size());

    cell_areas_.resize(cells.size());
    cell_sizes_.resize(cells.size());
    cell_attributes_.resize(cells.size());

    cell_internal_mapping_.resize(cells.size());
    net_internal_mapping_.resize(nets.size());

    cell_indexes_.resize(pins.size());
    pin_offsets_.resize(pins.size());
    net_indexes_.resize(pins.size());
    pin_indexes_.resize(pins.size());

    for(index_t i=0; i<nets.size(); ++i){
        net_internal_mapping_[i] = i;
    }
    for(index_t i=0; i<cells.size(); ++i){
        cell_internal_mapping_[i] = i;
    }

    std::sort(pins.begin(), pins.end(), [](extended_pin const a, extended_pin const b){ return a.net_ind < b.net_ind; });
    for(index_t n=0, p=0; n<nets.size(); ++n){
        net_weights_[n] = nets[n].weight;

        net_limits_[n] = p;
        while(p<pins.size() && pins[p].net_ind == n){
            cell_indexes_[p] = pins[p].cell_ind;
            pin_offsets_[p]  = pins[p].offset;
            pins[p].pin_index = p;
            ++p;
        }
    }
    net_limits_.back() = pins.size();

    std::sort(pins.begin(), pins.end(), [](extended_pin const a, extended_pin const b){ return a.cell_ind < b.cell_ind; });

    for(index_t c=0, p=0; c<cells.size(); ++c){
        cell_areas_[c] = cells[c].area;
        cell_attributes_[c] = cells[c].attributes;
        cell_sizes_[c] = cells[c].size;

        cell_limits_[c] = p;
        while(p<pins.size() && pins[p].cell_ind == c){
            net_indexes_[p] = pins[p].net_ind;
            pin_indexes_[p] = pins[p].pin_index;
            ++p;
        }
    }
    cell_limits_.back() = pins.size();
}

struct placement_t{
    std::vector<point<int_t> > positions_;
    std::vector<point<bool> > orientations_;

    index_t cell_cnt() const{
        assert(positions_.size() == orientations_.size());
        return positions_.size();
    }

    void selfcheck() const;
};

} // namespace coloquinte

#endif

