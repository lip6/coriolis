
#include "coloquinte/topologies.hxx"
#include "coloquinte/circuit_helper.hxx"
#include "coloquinte/union_find.hxx"

#include <array>
#include <algorithm>
#include <cassert>
#include <set>

namespace coloquinte{
using edge_t = std::pair<index_t, index_t>;

namespace{
struct minmax_t{
    int_t min, max;

    minmax_t(int_t mn, int_t mx) : min(mn), max(mx) {}
    minmax_t() {}
    void merge(minmax_t const o){
        min = std::min(o.max, min);
        max = std::max(o.min, max);
    }
    void merge(int_t const p){
        min = std::min(p, min);
        max = std::max(p, max);
    }
};
}

namespace steiner_lookup{

template<int pin_cnt>
int_t Hconnectivity<pin_cnt>::get_wirelength(std::array<point<int_t>, pin_cnt> const sorted_points) const{
    std::array<minmax_t, pin_cnt-2> minmaxs;
    for(index_t i=0; i<pin_cnt-2; ++i){
        minmaxs[i] = minmax_t(sorted_points[i+1].y_, sorted_points[i+1].y_);
    }
    std::uint8_t b_con = extremes & 15u, e_con = extremes >> 4;
    minmaxs[b_con].merge(sorted_points.front() .y_);
    minmaxs[e_con].merge(sorted_points.back()  .y_);
    for(std::uint8_t const E : connexions){
        minmaxs[(E >> 4)].merge(minmaxs[(E & 15u)]);
    }
    int_t cost = sorted_points.back().x_ - sorted_points.front().x_ + sorted_points[b_con+1].x_ - sorted_points[e_con+1].x_;
    for(std::uint8_t const E : connexions){
        cost += std::abs(sorted_points[(E >> 4) +1].x_ - sorted_points[(E & 15u) +1].x_);
    }
    for(index_t i=0; i<pin_cnt-2; ++i){
        cost += (minmaxs[i].max - minmaxs[i].min);
    }
    return cost;
}

template<int pin_cnt>
std::array<edge_t, pin_cnt-1> Hconnectivity<pin_cnt>::get_x_topology(std::array<point<int_t>, pin_cnt> const sorted_points) const{
    std::array<edge_t, pin_cnt-1> ret;
    std::uint8_t b_con = extremes & 15u, e_con = extremes >> 4;
    ret[0] = edge_t(0, b_con+1);
    ret[1] = edge_t(pin_cnt-1, e_con+1);
    for(index_t i=0; i<pin_cnt-3; ++i){
        std::uint8_t E = connexions[i];
        ret[i+2] = edge_t((E & 15u) +1, (E >> 4) +1);
    }

    return ret;
}
} // End namespace steiner_lookup

namespace {

template<int n, int array_size>
int_t get_wirelength_from_sorted(std::vector<point<int_t> > const & pins, std::array<steiner_lookup::Hconnectivity<n>, array_size> const & lookups){
    std::array<point<int_t>, n> points;
    std::copy_n(pins.begin(), n, points.begin());

    int_t cost = std::numeric_limits<int_t>::max();
    for(auto const L : lookups){
        cost = std::min(cost, L.get_wirelength(points));
    }
    return cost;
}

std::int64_t get_wirelength_from_topo(std::vector<point<int_t> > const & points, std::vector<std::pair<index_t, index_t> > Htopo){
    std::vector<minmax_t> minmaxs(points.size());
    for(index_t i=0; i<points.size(); ++i){
        minmaxs[i] = minmax_t(points[i].y_, points[i].y_);
    }
    for(auto const E : Htopo){
        minmaxs[E.second].merge(minmaxs[E.first]);
    }
    std::int64_t cost = 0;
    for(edge_t const E : Htopo){
        cost += std::abs(points[E.first].x_ - points[E.second].x_);
    }
    for(index_t i=0; i<points.size(); ++i){
        cost += (minmaxs[i].max - minmaxs[i].min);
    }
    return cost;
}

struct indexed_pt : point<int_t>{
    index_t index;
    indexed_pt(point<int_t> pt, index_t pos) : point<int_t>(pt), index(pos) {}
    indexed_pt(){}
};

template<int n, int array_size>
std::vector<std::pair<index_t, index_t> > get_topology_from_sorted(std::vector<point<int_t> > const & pins, std::array<steiner_lookup::Hconnectivity<n>, array_size> const & lookups){
    std::array<point<int_t>, n> points;
    std::copy_n(pins.begin(), n, points.begin());

    // Find the horizontal topology with the smallest cost
    int_t cost = std::numeric_limits<int_t>::max();
    index_t ind  = std::numeric_limits<index_t>::max();
    for(index_t i=0; i<array_size; ++i){
        int_t this_cost = lookups[i].get_wirelength(points);
        if(this_cost < cost){
            cost = this_cost;
            ind = i;
        }
    }
    assert(ind != std::numeric_limits<index_t>::max());
    auto ret = lookups[ind].get_x_topology(points);
    return std::vector<std::pair<index_t, index_t> >(ret.begin(), ret.end());
}

std::vector<edge_t> get_vertical_topology(std::vector<point<int_t> > pins, std::vector<edge_t> const & Htopo){
    index_t const null_ind = std::numeric_limits<index_t>::max();

    std::vector<indexed_pt> ipoints(pins.size());
    for(index_t i=0; i<pins.size(); ++i){
        ipoints[i] = indexed_pt(pins[i], i);
    }

    std::sort(ipoints.begin(), ipoints.end(), [](indexed_pt a , indexed_pt b){return a.y_ < b.y_; });

    // First pin with y ordering
    std::vector<index_t> min_y_pin(pins.size());
    for(index_t i=0; i<ipoints.size(); ++i){
        min_y_pin[ipoints[i].index] = i;
    }
    std::vector<index_t> max_y_pin = min_y_pin;


    std::vector<index_t> nxt_y_pin(pins.size(), null_ind);
    std::vector<edge_t> ret;
    for(auto const E : Htopo){
        // Assuming a correctly ordered horizontal topology where the first node of the edge is never visited again
        index_t f=E.first, s=E.second;
        index_t first_yf=min_y_pin[f], first_ys=min_y_pin[s];

        // Push the edges from the first and insert one of its elements in the second's linked structure
        if(max_y_pin[f] < min_y_pin[s] or max_y_pin[s] < min_y_pin[f]){
            for(index_t yf=first_yf; nxt_y_pin[yf] != null_ind; yf = nxt_y_pin[yf]){
                ret.push_back(edge_t(yf, nxt_y_pin[yf]));
            }

            if(max_y_pin[f] < min_y_pin[s]){
                nxt_y_pin[max_y_pin[f]] = min_y_pin[s];
                min_y_pin[s] = max_y_pin[f];
            }
            else if(max_y_pin[s] < min_y_pin[f]){
                nxt_y_pin[max_y_pin[s]] = min_y_pin[f];
                max_y_pin[s] = min_y_pin[f];
                nxt_y_pin[min_y_pin[f]] = null_ind;
            }
            else{
                abort();
            }
        }
        else{ // Need to chose a pin with two connexions because there will be no L route
            // One pin from the second is in the middle of the first
            if(max_y_pin[f] > max_y_pin[s]){
                index_t middle_pin = max_y_pin[s];
                index_t yf=first_yf;
                // Make the first connexions
                for(; nxt_y_pin[yf] < middle_pin; yf = nxt_y_pin[yf]){
                    ret.push_back(edge_t(yf, nxt_y_pin[yf]));
                }
                // Make the two connexions with the new pin
                ret.push_back(edge_t(yf, middle_pin));
                yf = nxt_y_pin[yf];
                ret.push_back(edge_t(yf, middle_pin));
                // Finish the connexions
                for(; nxt_y_pin[yf] != null_ind; yf = nxt_y_pin[yf]){
                    ret.push_back(edge_t(yf, nxt_y_pin[yf]));
                }
            }
            // One pin from the first is in the middle of the second
            else{
                for(index_t yf=first_yf; nxt_y_pin[yf] != null_ind; yf = nxt_y_pin[yf]){
                    ret.push_back(edge_t(yf, nxt_y_pin[yf]));
                }
                index_t middle_pin = max_y_pin[f];
                // Find the place where we can insert this pin
                index_t ys=first_ys;
                for(; nxt_y_pin[ys] < middle_pin; ys = nxt_y_pin[ys]);
                nxt_y_pin[middle_pin] = nxt_y_pin[ys];
                nxt_y_pin[ys] = middle_pin;
            }
        }
    }
    // The last visited gives the remaining connexions to push
    for(index_t yf=min_y_pin[Htopo.back().second]; nxt_y_pin[yf] != null_ind; yf = nxt_y_pin[yf]){
        ret.push_back(edge_t(yf, nxt_y_pin[yf]));
    }
    
    // Back to the original ordering
    for(auto & E : ret){
        E.first = ipoints[E.first].index;
        E.second = ipoints[E.second].index;
    }
    return ret;
}

inline void northeast_octant_neighbours(std::vector<point<int_t> > pins, std::vector<std::pair<index_t, index_t> > & edges){

    std::vector<indexed_pt> point_list;
    for(index_t i=0; i<pins.size(); ++i){
        point_list.push_back(indexed_pt(pins[i], i));
    }

    std::sort(point_list.begin(), point_list.end(),
                [](indexed_pt const a, indexed_pt const b){ return a.x_ + a.y_ < b.x_ + b.y_; }
              );

    // Decreasing order of x and y; multiset not necessary because no two elements have same coordinate
    std::set<indexed_pt, std::function<bool (indexed_pt const, indexed_pt const)> >
                      active_upper_octant([](indexed_pt const a, indexed_pt const b)->bool{return a.x_ > b.x_;}),
                      active_lower_octant([](indexed_pt const a, indexed_pt const b)->bool{return a.y_ > b.y_;});

    for(indexed_pt const current : point_list){
        { // North to north-east region
            auto first_it = active_upper_octant.lower_bound(current); // Largest x with x <= current.x
            auto it = first_it;
            for(; it != active_upper_octant.end() && it->x_ - it->y_ >= current.x_ - current.y_; ++it){
                edges.push_back(std::pair<index_t, index_t>(current.index, it->index));
            }
            if(first_it != active_upper_octant.end()){ active_upper_octant.erase(first_it, it); }
            active_upper_octant.insert(it, current); // Hint to insert the element since it is the correct position
        } // End region
        { // North-east to east region
            auto first_it = active_lower_octant.lower_bound(current); // Largest y with y <= current.y
            auto it = first_it;
            for(; it != active_lower_octant.end() && it->y_ - it->x_ >= current.y_ - current.x_; ++it){
                edges.push_back(std::pair<index_t, index_t>(current.index, it->index));
            }
            if(first_it != active_lower_octant.end()){ active_lower_octant.erase(first_it, it); }
            active_lower_octant.insert(it, current); // Hint to insert the element since it is the correct position
        } // End region
    }
}

// Gets the nearest octant neighbour for each point in the south-east quadrant
inline void southeast_octant_neighbours(std::vector<point<int_t> > pins, std::vector<std::pair<index_t, index_t> > & edges){
    for(auto & pin : pins){
        pin.y_ = - pin.y_;
    }
    northeast_octant_neighbours(pins, edges);
}

std::vector<std::pair<index_t, index_t> > get_small_horizontal_topology_from_sorted(std::vector<point<int_t> > const & pins){
    assert(pins.size() <= 10);

    switch(pins.size()){
        case 2:
            return std::vector<edge_t>(1, edge_t(0, 1));
        case 3:
            return std::vector<edge_t>{{0, 1}, {1, 2}};
        case 4:
            return get_topology_from_sorted<4, 2>(pins, steiner_lookup::topologies_4);
        case 5:
            return get_topology_from_sorted<5, 6>(pins, steiner_lookup::topologies_5);
        case 6:
            return get_topology_from_sorted<6, 23>(pins, steiner_lookup::topologies_6);
        case 7:
            return get_topology_from_sorted<7, 111>(pins, steiner_lookup::topologies_7);
        case 8:
            return get_topology_from_sorted<8, 642>(pins, steiner_lookup::topologies_8);
        case 9:
            return get_topology_from_sorted<9, 4334>(pins, steiner_lookup::topologies_9);
        case 10:
            return get_topology_from_sorted<10, 33510>(pins, steiner_lookup::topologies_10);
        default: // Only 1 and 0 left (11 and more are protected by an assertion)
            return std::vector<edge_t>();
    }
}

// Get an ordering of the edges that is compatible with the processing functions
std::vector<edge_t> get_tree_topo_sort(std::vector<edge_t> const & topo){
    std::vector<edge_t> sorted_topo;
    std::vector<std::vector<index_t> > neighbours(topo.size()+1);
    for(edge_t const E : topo){
        neighbours[E.first].push_back(E.second);
        neighbours[E.second].push_back(E.first);
    }
    std::vector<index_t> to_visit;
    std::vector<int_t> nbr_unvisited(topo.size()+1);
    for(index_t i=0; i<=topo.size(); ++i){
        nbr_unvisited[i] = neighbours[i].size();
        assert(topo.size() == 0 or nbr_unvisited[i] >= 1);
        if(nbr_unvisited[i] == 1)
            to_visit.push_back(i);
    }
    std::vector<int> visited(topo.size()+1, 0);
    while(not to_visit.empty()){
        index_t f = to_visit.back();
        assert(visited[f] == 0);
        visited[f] = 1;
        to_visit.pop_back();
        for(index_t s : neighbours[f]){
            --nbr_unvisited[s];
            if(visited[s] == 0){ // It is not a node we already visited
                sorted_topo.push_back(edge_t(f, s));
            }
            if(nbr_unvisited[s] == 1){
                to_visit.push_back(s);
            }
        }
    }
    assert(sorted_topo.size() == topo.size());
    return sorted_topo;
}

std::vector<edge_t> get_big_horizontal_topology_from_sorted(std::vector<point<int_t> > const & pins, index_t exactitude_limit){
    auto spanning = get_MST_topology(pins);

    // TODO: perform local optimizations on the topology using exact Steiner tree algorithms

    // Remove horizontal suboptimalities i.e. when the connexions to the left and right are unbalanced
    // Reuse existing code by translation to vertical topology
    auto first_Htopo = get_tree_topo_sort(spanning);
    auto Vtopo = get_vertical_topology(pins, first_Htopo);
    Vtopo = get_tree_topo_sort(Vtopo);

    std::vector<point<int_t> > inverted_coords = pins;
    for(point<int_t> & pt : inverted_coords){
        std::swap(pt.x_, pt.y_);
    }
    auto Htopo = get_vertical_topology(inverted_coords, Vtopo);

    // Sort the tree so that it is usable when building an RSMT    
    return get_tree_topo_sort(Htopo);
}

} // End anonymous namespace

std::vector<std::pair<index_t, index_t> > get_MST_topology(std::vector<point<int_t> > const & pins){

	std::vector<edge_t> edges;
    
    if(pins.size() <= 2){
        if(pins.size() == 2){
            edges.push_back(edge_t(0, 1));
        }
        if(pins.size() == 3){
            auto D = [](point<int_t> a, point<int_t> b){ return std::abs(a.x_ - b.x_) + std::abs(a.y_ - b.y_); };
            auto dists = std::array<int_t, 3>({D(pins[1], pins[2]), D(pins[1], pins[2]), D(pins[0], pins[1])});
            index_t mx = std::max_element(dists.begin(), dists.end()) - dists.begin();
            for(index_t i=0; i<3; ++i){
                if(i != mx)
                    edges.push_back(edge_t((i+1) % 3, (i+2) % 3));
            }
        }
        return edges;
    }
    
    northeast_octant_neighbours(pins, edges);
    southeast_octant_neighbours(pins, edges);

	std::vector<edge_t> returned_edges;

    auto edge_length = [&](edge_t E){
        point<int_t> p1 = pins[E.first],
                     p2 = pins[E.second];
        return std::abs(p1.x_ - p2.x_) + std::abs(p1.y_ - p2.y_);
    };
	// Perform Kruskal to get the tree
	std::sort(edges.begin(), edges.end(), [&](edge_t a, edge_t b){ return edge_length(a) < edge_length(b); });

	union_find merger(pins.size());

	for(index_t i=0; i<edges.size() && returned_edges.size()+1 < pins.size(); ++i){
		edge_t E = edges[i];
		if(merger.find(E.first) != merger.find(E.second)){
			merger.merge(E.first, E.second);
            assert(merger.find(E.first) == merger.find(E.second));
			returned_edges.push_back(E);
		}
	}
	assert(returned_edges.size() + 1 == pins.size());
    assert(merger.is_connex());
	return returned_edges;
}

std::int64_t MST_length(std::vector<point<int_t> > const & pins){
    auto edges = get_MST_topology(pins);
    std::int64_t sum = 0;
    for(auto E : edges){
        sum += std::abs(pins[E.first].x_ - pins[E.second].x_);
        sum += std::abs(pins[E.first].y_ - pins[E.second].y_);
    }
    return sum;
}

std::int64_t RSMT_length(std::vector<point<int_t> > const & pins, index_t exactitude_limit){
    assert(exactitude_limit <= 10 and exactitude_limit >= 3);
    if(pins.size() <= 3){
        if(pins.size() == 2){
            return std::abs(pins[0].x_ - pins[1].x_) + std::abs(pins[0].y_ - pins[1].y_);
        }
        else if(pins.size() == 3){
            auto minmaxX = std::minmax_element(pins.begin(), pins.end(), [](point<int_t> a, point<int_t> b){ return a.x_ < b.x_; }), 
                 minmaxY = std::minmax_element(pins.begin(), pins.end(), [](point<int_t> a, point<int_t> b){ return a.y_ < b.y_; });
            return (minmaxX.second->x_ - minmaxX.first->x_) + (minmaxY.second->y_ - minmaxY.first->y_);
        }
        else{
            return 0;
        }
    }
    else{
        std::vector<point<int_t> > points = pins;
        std::sort(points.begin(), points.end(), [](point<int_t> a , point<int_t> b){return a.x_ < b.x_; });

        if(points.size() <= exactitude_limit){
            switch(points.size()){
                case 4:
                    return get_wirelength_from_sorted<4, 2>(points, steiner_lookup::topologies_4);
                case 5:
                    return get_wirelength_from_sorted<5, 6>(points, steiner_lookup::topologies_5);
                case 6:
                    return get_wirelength_from_sorted<6, 23>(points, steiner_lookup::topologies_6);
                case 7:
                    return get_wirelength_from_sorted<7, 111>(points, steiner_lookup::topologies_7);
                case 8:
                    return get_wirelength_from_sorted<8, 642>(points, steiner_lookup::topologies_8);
                case 9:
                    return get_wirelength_from_sorted<9, 4334>(points, steiner_lookup::topologies_9);
                case 10:
                    return get_wirelength_from_sorted<10, 33510>(points, steiner_lookup::topologies_10);
                default:
                    abort();
            }
        }
        else{ // Need to create the full topology, then calculate the length back
            //return MST_length(points);
            auto horizontal_topology = get_big_horizontal_topology_from_sorted(points, exactitude_limit);
            return get_wirelength_from_topo(points, horizontal_topology);
        }
    }
}

point<std::vector<std::pair<index_t, index_t> > > get_RSMT_topology(std::vector<point<int_t> > const & pins, index_t exactitude_limit){

    assert(exactitude_limit <= 10 and exactitude_limit >= 3);

    // For 3 pin and fewer, the topology is very simple
    if(pins.size() <= 2){
        if(pins.size() == 2){
            auto ret = std::vector<edge_t>(1, edge_t(0, 1));
            return point<std::vector<edge_t> >(ret, ret);
        }
        else{
            return point<std::vector<edge_t> >();
        }
    }
    else if(pins.size() == 3){
        std::vector<indexed_pt> ipoints(pins.size());
        for(index_t i=0; i<pins.size(); ++i){
            ipoints[i] = indexed_pt(pins[i], i);
        }
        auto xpoints=ipoints;
        std::sort(xpoints.begin(), xpoints.end(), [](indexed_pt a , indexed_pt b){return a.x_ < b.x_; });
        auto ypoints=ipoints;
        std::sort(ypoints.begin(), ypoints.end(), [](indexed_pt a , indexed_pt b){return a.y_ < b.y_; });
        
        return point<std::vector<edge_t> >{{{xpoints[0].index, xpoints[1].index}, {xpoints[1].index, xpoints[2].index}}, {{ypoints[0].index, ypoints[1].index}, {ypoints[1].index, ypoints[2].index}}};
    }
    else{
        std::vector<edge_t> horizontal_topology;

        // Sort the pins by x coordinate
        std::vector<indexed_pt> ipoints(pins.size());
        for(index_t i=0; i<pins.size(); ++i){
            ipoints[i] = indexed_pt(pins[i], i);
        }
        std::sort(ipoints.begin(), ipoints.end(), [](indexed_pt a , indexed_pt b){return a.x_ < b.x_; });
        std::vector<point<int_t> > sorted_pins(pins.size());
        for(index_t i=0; i<pins.size(); ++i){
            sorted_pins[i] = ipoints[i];
        }

        // Get the topology for this ordering
        if(pins.size() <= exactitude_limit){
            horizontal_topology = get_small_horizontal_topology_from_sorted(sorted_pins);
        }
        else{
            horizontal_topology = get_big_horizontal_topology_from_sorted(sorted_pins, exactitude_limit);
        }

        // Back to the original ordering
        for(auto & E : horizontal_topology){
            E.first = ipoints[E.first].index;
            E.second = ipoints[E.second].index;
        }

        return point<std::vector<edge_t> >(horizontal_topology, get_vertical_topology(sorted_pins, horizontal_topology));
    }
}

} // Namespace coloquinte

