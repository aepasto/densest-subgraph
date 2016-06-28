/*
 * DynDSAlg.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: aepasto
 */

#include "DynDSAlg.h"
#include "DynGraphUtils.h"

#include <stack>
#include <cmath>

DynDSAdd::DynDSAdd(const double epsilon) :
		EPS_ERR(0.01), epsilon_(epsilon), beta_(0.25 / (1.0 + epsilon)), edges_densest_subgraph_(
				0) {

}

DynDSAdd::~DynDSAdd() {
}

bool DynDSAdd::add_edge(const int u, const int v) {
	bool new_add = graph_.add_edge(u, v);

	if (!new_add) {
		return false;
	}

	if (densest_subgraph_set_.find(u) != densest_subgraph_set_.end()
			&& densest_subgraph_set_.find(v) != densest_subgraph_set_.end()) {
		++edges_densest_subgraph_;
	}

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
	return true;
}

void DynDSAdd::Construct() {
	DSResult result;
	Incremental(graph_, beta_, epsilon_, densest_subgraph_set_.begin(),
			densest_subgraph_set_.end(), &result, &level_map_, &orientation_);

	densest_subgraph_set_.clear();
	densest_subgraph_set_.insert(result.subgraph.begin(),
			result.subgraph.end());

	pair<int, unsigned long long> nodes_edges_pair =
			num_nodes_and_edges_induced(graph_, densest_subgraph_set_.begin(),
					densest_subgraph_set_.end());
	edges_densest_subgraph_ = nodes_edges_pair.second;


	beta_ = result.density * (1 + epsilon_);
}

void DynDSAdd::Check(int first_node) {
	stack<int> to_check;
	to_check.push(first_node);

	double threshold = 2.0 * beta_ * (1.0 + epsilon_);
	int max_iter_num = max_iter(graph_.num_nodes(), epsilon_);

	while (!to_check.empty()) {
		int node = to_check.top();
		to_check.pop();

		int curr_deg = orientation_.in_degree(node);
		if (curr_deg < threshold) {
			continue;
		}

		if (level_map_[node] >= max_iter_num) {
			Construct();
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
