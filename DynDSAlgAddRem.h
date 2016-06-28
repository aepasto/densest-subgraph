/*
 * DynDSAlgAddRem.h
 *
 *  Created on: Oct 21, 2014
 *      Author: aepasto
 */

#ifndef DYNDSALGADDREM_H_
#define DYNDSALGADDREM_H_

#include "DynGraph.h"
#include "UDynGraph.h"
#include "DSAlgs.h"
#include "DynGraphUtils.h"

#include <unordered_set>
#include <iostream>

using namespace std;

// Class implementing the dynamic densest subgraph
class DynDSAddRem {

public:
	const double EPS_ERR;

	explicit DynDSAddRem(const double epsilon);
	virtual ~DynDSAddRem();

	bool add_edge(const int u, const int v);
	bool remove_edge(const int u, const int v);

	inline double beta() const {
		return beta_;
	}

	inline double density_subgraph() const {
		double dens =
				0.0 ? densest_subgraph_set_.empty() : static_cast<double>(edges_in_densest_subgraph_)
								/ static_cast<double>(densest_subgraph_set_.size());
		if (!(edges_in_densest_subgraph_ > 0 || graph_.num_edges() == 0)) {
			cerr << "|E(S)| =" << edges_in_densest_subgraph_ << endl;
			cerr << "|S| =" << densest_subgraph_set_.size() << endl;
			cerr << "|E| =" << graph_.num_edges() << endl;
			cerr.flush();

			assert(false);
		}
		assert(dens > 0 || graph_.num_edges() == 0);

		return dens;
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

	void Construct(bool use_add_only_graph);
	void Check(int node);

	void valide_upperbound();

	DynGraph orientation_;
	UDynGraph graph_;
	//UDynGraph graph_only_add_; //TODO NOT USED

	double epsilon_original_;
	double epsilon_;

	unordered_set<int> densest_subgraph_set_;
	double beta_;
	unordered_map<int, int> level_map_;
	int removals_last_;
	int threshold_removals_;
	unsigned long long edges_in_densest_subgraph_;
};

#endif /* DYNDSALGADDREM_H_ */
