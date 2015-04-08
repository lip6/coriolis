
#include "coloquinte/detailed.hxx"
#include "coloquinte/circuit_helper.hxx"

#include <lemon/smart_graph.h>
#include <lemon/network_simplex.h>

#include <cassert>

namespace coloquinte{
namespace dp{

void optimize_on_topology_HPWL(netlist const & circuit, detailed_placement & pl){
    // Solves a minimum cost flow problem to optimize the placement at fixed topology
    // Concretely, it means aligning the pins to minimize the wirelength
    // It uses the Lemon network simplex solver from the Coin-OR initiative, which should scale well up to hundred of thousands of cells

    using namespace lemon;
    DIGRAPH_TYPEDEFS(SmartDigraph);
    // Create a graph with the cells and bounds of the nets as node
    SmartDigraph g;

    std::vector<Node> cell_nodes(circuit.cell_cnt());
    for(index_t i=0; i<circuit.cell_cnt(); ++i){
        if((circuit.get_cell(i).attributes & XMovable) != 0)
            cell_nodes[i] = g.addNode();
    }
    std::vector<Node> Lnet_nodes(circuit.net_cnt()), Unet_nodes(circuit.net_cnt());
    for(index_t i=0; i<circuit.net_cnt(); ++i){
        if(circuit.get_net(i).pin_cnt > 0){
            Lnet_nodes[i] = g.addNode();
            Unet_nodes[i] = g.addNode();
        }
    }

    // Two nodes for position constraints
    Node fixed = g.addNode();

    typedef std::pair<SmartDigraph::Arc, int_t> arc_pair;
    typedef std::pair<SmartDigraph::Node, int_t> node_pair;
    // The arcs corresponding to constraints of the original problem
    std::vector<arc_pair> constraint_arcs;

    // Now we add every positional constraint, which becomes an arc in the min-cost flow problem
    for(index_t i=0; i<circuit.cell_cnt(); ++i){ // The cells
        for(index_t l = pl.neighbours_limits_[i]; l < pl.neighbours_limits_[i+1]; ++l){
            index_t oi = pl.neighbours_[l].second;
            if(oi == null_ind) continue;
            
            if((circuit.get_cell(i).attributes & XMovable) != 0 and (circuit.get_cell(oi).attributes & XMovable) != 0){
                // Two movable cells: OK
                auto A = g.addArc(cell_nodes[oi], cell_nodes[i]);
                constraint_arcs.push_back(arc_pair(A, -circuit.get_cell(i).size.x_));
            }
            else if((circuit.get_cell( i).attributes & XMovable) != 0){
                // The cell c is movable and constrained on the right
                auto A = g.addArc(fixed, cell_nodes[i]);
                constraint_arcs.push_back(arc_pair(A, pl.plt_.positions_[oi].x_ - circuit.get_cell(i).size.x_));
            }
            else if((circuit.get_cell(oi).attributes & XMovable) != 0){
                // The cell oc is movable and constrained on the left
                auto A = g.addArc(cell_nodes[oi], fixed);
                constraint_arcs.push_back(arc_pair(A, -pl.plt_.positions_[i].x_ - circuit.get_cell(i).size.x_));
            }
        }
    }

    
    for(index_t r=0; r<pl.row_cnt(); ++r){ // And the boundaries of each row
        index_t lc = pl.row_first_cells_[r];
        if(lc != null_ind and (circuit.get_cell(lc).attributes & XMovable) != 0){
            auto Al = g.addArc(cell_nodes[lc], fixed);
            constraint_arcs.push_back(arc_pair(Al, -pl.min_x_));
        }
    }
    for(index_t r=0; r<pl.row_cnt(); ++r){ // And the boundaries of each row
        index_t rc = pl.row_last_cells_[r];
        if(rc != null_ind and (circuit.get_cell(rc).attributes & XMovable) != 0){
            auto Ar = g.addArc(fixed, cell_nodes[rc]);
            constraint_arcs.push_back(arc_pair(Ar, pl.max_x_ - circuit.get_cell(rc).size.x_));
        }
    }
    

    // And every pin of every net: arcs too
    for(index_t n=0; n<circuit.net_cnt(); ++n){
        for(auto p : circuit.get_net(n)){
            index_t c = p.cell_ind;
            int_t pin_offs = (pl.plt_.orientations_[c].x_ ? p.offset.x_ : circuit.get_cell(c).size.x_ - p.offset.x_); // Offset to the beginning of the cell
            if((circuit.get_cell(c).attributes & XMovable) != 0){
                Arc Al = g.addArc(cell_nodes[c], Lnet_nodes[n]);
                constraint_arcs.push_back(arc_pair(Al, pin_offs));
                Arc Ar = g.addArc(Unet_nodes[n], cell_nodes[c]);
                constraint_arcs.push_back(arc_pair(Ar, -pin_offs));
            }
            else{ // Fixed offset
                auto Al = g.addArc(fixed, Lnet_nodes[n]);
                constraint_arcs.push_back(arc_pair(Al, pl.plt_.positions_[c].x_ + pin_offs));
                auto Ar = g.addArc(Unet_nodes[n], fixed);
                constraint_arcs.push_back(arc_pair(Ar, - pl.plt_.positions_[c].x_ - pin_offs));
            }
        }
    }

    // Then the only capacitated arcs: the ones for the nets
    std::vector<node_pair> net_supplies;
    for(index_t n=0; n<circuit.net_cnt(); ++n){
        if(circuit.get_net(n).pin_cnt > 0){
            net_supplies.push_back(node_pair(Unet_nodes[n],  circuit.get_net(n).weight));
            net_supplies.push_back(node_pair(Lnet_nodes[n], -circuit.get_net(n).weight));
        }
    }

    // Create the maps to have cost and capacity for the arcs
    IntArcMap cost(g, 0);
    IntArcMap capacity(g, circuit.net_cnt());
    IntNodeMap supply(g, 0);

    for(arc_pair A : constraint_arcs){
        cost[A.first] = A.second;
    }

    for(node_pair N : net_supplies){
        supply[N.first] = N.second;
    }

    // Then we (hope the solver can) solve it
    NetworkSimplex<SmartDigraph> ns(g);
    ns.supplyMap(supply).costMap(cost);
    auto res = ns.run();
    if(res != ns.OPTIMAL){
        abort();
    }
    
    // And we get the new positions as the dual values of the current solution (compared to the fixed pin) 
    for(index_t c=0; c<circuit.cell_cnt(); ++c){ // The cells
        if((circuit.get_cell(c).attributes & XMovable) != 0){
            pl.plt_.positions_[c].x_ = ns.potential(cell_nodes[c]) - ns.potential(fixed);
        }
    }
    pl.selfcheck();
}

} // namespace dp
} // namespace coloquinte


