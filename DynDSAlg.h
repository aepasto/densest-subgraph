/*
 * DynDSAlg.h
 *
 *  Created on: Oct 15, 2014
 *      Author: aepasto
 */

#ifndef DYNDSALG_H_
#define DYNDSALG_H_

#include "DynGraph.h"
#include "UDynGraph.h"
#include "DSAlgs.h"
#include "DynGraphUtils.h"

// Class implementing the dynamic densest subgraph
class DynDSAdd {

public:
	const double EPS_ERR;

	explicit DynDSAdd(const double epsilon);
	virtual ~DynDSAdd();

	bool add_edge(const int u, const int v);

	double beta() const {
		return beta_;
	}

	double density_subgraph() const {
		if (densest_subgraph_set_.size() == 0) {
			return 0.0;
		}
		return static_cast<double>(edges_densest_subgraph_)
				/ static_cast<double>(densest_subgraph_set_.size());
	}



	inline int max_in_degree_upperbound() const {
		return orientation_.max_in_deg_upperbound();
	}

	inline const unordered_set<int>& densest_subgraph() const {
		return densest_subgraph_set_;
	}

	inline long num_edges() const {
		return graph_.num_edges();
	}

	inline unsigned int size_densest() const {
		return densest_subgraph_set_.size();
	}

private:

	void Construct();
	void Check(int node);

	DynGraph orientation_;
	UDynGraph graph_;

	double epsilon_;
	unordered_set<int> densest_subgraph_set_;
	double beta_;
	int edges_densest_subgraph_;
	unordered_map<int, int> level_map_;
};
#endif /* DYNDSALG_H_ */
