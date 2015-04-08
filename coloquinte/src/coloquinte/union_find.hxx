
#ifndef COLOQUINTE_UNION_FIND
#define COLOQUINTE_UNION_FIND

#include "common.hxx"

#include <vector>

namespace coloquinte{

class union_find{
	std::vector<index_t> connex_representants;

	public:
	index_t size() const { return connex_representants.size(); }

	void merge(index_t a, index_t b){
		connex_representants[find(a)] = b;
	}

	index_t find(index_t ind){
		if(connex_representants[ind] != ind){
			connex_representants[ind] = find(connex_representants[ind]);
		}
		return connex_representants[ind];
	}

	union_find(index_t s) : connex_representants(s){
		for(index_t i=0; i<size(); ++i){
			connex_representants[i] = i;
		}
	}
	
    bool is_connex(){
        bool connex = true;
        for(index_t i=0; i+1<size(); ++i){
            connex = connex && (find(i) == find(i+1));
        }
        return connex;
    }
	
};

} // End namespace coloquinte

#endif

