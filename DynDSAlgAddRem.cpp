/*
 * DynDSAlg.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: aepasto
 */

#include "DynGraphUtils.h"

#include <stack>
#include <cmath>
#include <iostream>
#include "DynDSAlgAddRem.h"

DynDSAddRem::DynDSAddRem(const double epsilon) :
		EPS_ERR(0.01), epsilon_original_(epsilon), epsilon_(
				2 * epsilon + epsilon * epsilon), beta_(
				0.25 / (1.0 + epsilon_)), removals_last_(0), threshold_removals_(
				1), edges_in_densest_subgraph_(0) {
}

DynDSAddRem::~DynDSAddRem() {
}

bool DynDSAddRem::remove_edge(const int u, const int v) {

	bool new_remove = graph_.remove_edge(u, v);

	if (!new_remove) {
		return false;
	}

	//++removals_last_;

	bool inside_densest_subgraph = densest_subgraph_set_.find(u)
			!= densest_subgraph_set_.end()
			&& densest_subgraph_set_.find(v) != densest_subgraph_set_.end();

	// Either or the two will be present
	orientation_.remove_edge(u, v);
	orientation_.remove_edge(v, u);
	if (graph_.degree(u) == 0) {
		level_map_.erase(u);
		densest_subgraph_set_.erase(u);
	}
	if (graph_.degree(v) == 0) {
		level_map_.erase(v);
		densest_subgraph_set_.erase(v);
	}

	// Check density subgraph
	if (inside_densest_subgraph) {
		--edges_in_densest_subgraph_;
	}

	valide_upperbound();
	assert(edges_in_densest_subgraph_ > 0 || graph_.num_edges() == 0);

	return true;
}

bool DynDSAddRem::add_edge(const int u, const int v) {
	bool new_add = graph_.add_edge(u, v);

	if (!new_add) {
		return false;
	}

	if (densest_subgraph_set_.find(u) != densest_subgraph_set_.end()
			&& densest_subgraph_set_.find(v) != densest_subgraph_set_.end()) {
		++edges_in_densest_subgraph_;
	}

	//if (removals_last_ < threshold_removals_) {
	//	graph_only_add_.add_edge(u, v);
//	}//TODO

	if (level_map_.find(u) == level_map_.end()) {
		level_map_[u] = 1;
	}
	if (level_map_.find(v) == level_map_.end()) {
		level_map_[v] = 1;
	}

	int min_node, max_node;

	if (level_map_[u] < level_map_[v]
			|| (level_map_[u] == level_map_[v]
					&& orientation_.in_degree(u) <= orientation_.in_degree(v))) {
		min_node = u;
		max_node = v;
	} else {
		min_node = v;
		max_node = u;
	}

	orientation_.add_edge(max_node, min_node);
	Check(min_node);

	valide_upperbound();
	assert(edges_in_densest_subgraph_ > 0 || graph_.num_edges() == 0);

	return true;
}

void DynDSAddRem::valide_upperbound() {
	// TODO Notice this is a trick to speed up. The check is necessary here as we
	// are using max_in_deg isntead of 2beta(1+e) for the upperbound
	if (graph_.num_edges() == 0) {
		return;
	}
	double upperbound = min(beta_ * 2.0 * (1 + epsilon_),
			(double) orientation_.max_in_deg_upperbound());

	double new_density =
			0 ? densest_subgraph_set_.empty() : static_cast<double>(edges_in_densest_subgraph_)
						/ static_cast<double>(densest_subgraph_set_.size());

	//cout << "(inside) DENS: " << new_density << endl;
	//cout << "(inside) UPP: " << upperbound << endl;

	if ((edges_in_densest_subgraph_ == 0 && graph_.num_edges() != 0)
			|| new_density * 2.0 * pow(1.0 + epsilon_, 3) < upperbound) {
		//cerr << "W: Decreased d(S)" << endl;
		Construct(false);
	}

	assert(edges_in_densest_subgraph_ != 0 || graph_.num_edges() == 0);

	new_density =
			0 ? densest_subgraph_set_.empty() : static_cast<double>(edges_in_densest_subgraph_)
						/ static_cast<double>(densest_subgraph_set_.size());
	upperbound = min(beta_ * 2.0 * (1 + epsilon_),
			(double) orientation_.max_in_deg_upperbound()); //TODO not using in_deg upperbound

	if (new_density * 2.0 * pow(1.0 + epsilon_, 3) < upperbound - EPS_ERR) {
		cerr << "(inside after construct) DENS: " << new_density << endl;
		cerr << "(inside after construct) UPP: " << upperbound << endl;
		cerr << "(inside after construct) Dens * factor: "
				<< new_density * 2.0 * pow(1.0 + epsilon_, 3) << endl;
		cerr << "(inside after construct) edges in graph: "
				<< graph_.num_edges() << endl;
		cerr.flush();
		assert(false);
	}
}

void DynDSAddRem::Construct(bool use_add_only_graph) {
	assert(!use_add_only_graph); // Not used

	DSResult result;
	if (graph_.num_edges() == 0) { // Nothing to do
		return;
	}

	/*if (use_add_only_graph) { // When there is a node of > max_iter level and few removals
	 Incremental(graph_only_add_, beta_, epsilon_,
	 densest_subgraph_set_.begin(), densest_subgraph_set_.end(),
	 &result, &level_map_, &orientation_);

	 densest_subgraph_set_.clear();
	 densest_subgraph_set_.insert(result.subgraph.begin(),
	 result.subgraph.end());

	 if (density(graph_, result.subgraph.begin(), result.subgraph.end())
	 < (beta_ / (1.0 + epsilon_original_))) { // This should not happen
	 cerr << "WARNING: Low density after > MAX_ITER and few removal"
	 << std::endl;

	 }
	 }*/

	Incremental(graph_, 0.25 / (2.0 * (1 + epsilon_)), epsilon_,
			densest_subgraph_set_.begin(), densest_subgraph_set_.end(), &result,
			&level_map_, &orientation_);

	densest_subgraph_set_.clear();
	densest_subgraph_set_.insert(result.subgraph.begin(),
			result.subgraph.end());

	pair<unsigned int, unsigned long long> pair_num =
			num_nodes_and_edges_induced(graph_, densest_subgraph_set_.begin(),
					densest_subgraph_set_.end());

	edges_in_densest_subgraph_ = pair_num.second;
	assert(
			(unsigned int ) pair_num.first
					== (unsigned int ) densest_subgraph_set_.size());
	assert(result.density >= 0);

	beta_ = result.density * (1 + epsilon_);
	removals_last_ = 0;
	//graph_only_add_.clear();
	//graph_only_add_.assign(graph_);//TODO
	//threshold_removals_ = ceil(
	//		(epsilon_original_ * graph_.num_edges() * log1p(epsilon_original_))
	//				/ (6 * log(graph_.num_nodes()) * pow((1.0 + epsilon_), 3)));

	//cout << "[IN CONSTR] Dens:" << result.density << endl;
	//cout << "[IN CONSTR] BETA:" << beta_ << endl;
	//cout << "[IN CONSTR] |S|:" << densest_subgraph_set_.size() << endl;
	//cout << "[IN CONSTR] |E[S]|:" << edges_in_densest_subgraph_ << endl;

	double upperbound = min((double) orientation_.max_in_deg_upperbound(),
			beta_ * 2.0 * (1 + epsilon_));

	assert(result.density * 2.0 * pow(1.0 + epsilon_, 3) >= upperbound);

	assert(edges_in_densest_subgraph_ > 0 || graph_.num_edges() == 0);

}

void DynDSAddRem::Check(int first_node) {
	stack<int> to_check;
	to_check.push(first_node);

	double threshold = 2.0 * beta_ * (1.0 + epsilon_);
	int max_iter_num = max_iter(graph_.num_nodes(), epsilon_);

	while (!to_check.empty()) {
		int node = to_check.top();
		to_check.pop();

		int curr_deg = orientation_.in_degree(node);
		if (static_cast<double>(curr_deg) < threshold) {
			continue;
		}

		if (level_map_[node] >= max_iter_num) {
			// TODO (true) is necesasry in theory to bound complexity but in practive false is faster.
			//Construct(true /* use add-only graph*/);
			Construct(false);
			return;
		}

		int edges_to_reverse = ceil(curr_deg - threshold);
		vector<int> in_neighbors;
		orientation_.in_neighbors(node, &in_neighbors);

		for (vector<int>::iterator it = in_neighbors.begin();
				it != in_neighbors.end(); ++it) {
			int in_neighbor = *it;
			if (level_map_[in_neighbor] == level_map_[node]) {
				orientation_.remove_edge(in_neighbor, node);
				orientation_.add_edge(node, in_neighbor);
				--edges_to_reverse;

				to_check.push(in_neighbor);
				if (edges_to_reverse == 0) {
					break;
				}
			}
		}
		++level_map_[node];

		to_check.push(node);
	}
}
