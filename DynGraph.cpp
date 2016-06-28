/*
 * DynGraph.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#include "DynGraph.h"
#include <iostream>

DynGraph::DynGraph() :
		num_nodes_(0), num_edges_(0), max_in_deg_upperbound_(0) {
}

DynGraph::~DynGraph() {
}

void DynGraph::clear() {
	nodes_.clear();
	edge_map_in_.clear();
	edge_map_out_.clear();
	node_map_in_.clear();
	node_map_out_.clear();
	num_nodes_ = 0;
	num_edges_ = 0;
	max_in_deg_upperbound_ = 0;
}

bool DynGraph::add_edge(const int source, const int destination) {
	assert(source != destination);
	assert(source >= 0 && source < MAX_NUM_NODES);
	assert(destination >= 0 && destination < MAX_NUM_NODES);

	unsigned long long edge_p = edge_to_id(source, destination);
	if (edge_map_in_.find(edge_p) != edge_map_in_.end()) {
		return false;
	}

	++num_edges_;

	if (nodes_.find(source) == nodes_.end()) {
		nodes_.insert(source);
		++num_nodes_;
	}
	if (nodes_.find(destination) == nodes_.end()) {
		nodes_.insert(destination);
		++num_nodes_;
	}

	int pos_out = node_map_out_[source].size();
	node_map_out_[source].push_back(destination);
	int pos_in = node_map_in_[destination].size();
	node_map_in_[destination].push_back(source);
	edge_map_out_[edge_p] = pos_out;
	edge_map_in_[edge_p] = pos_in;

	if (node_map_in_[destination].size()
			> static_cast<unsigned int>(max_in_deg_upperbound_)) {
		max_in_deg_upperbound_ = node_map_in_[destination].size();
	}

	return true;
}

int DynGraph::max_in_deg_upperbound() const {
	return max_in_deg_upperbound_;
}

int DynGraph::non_zero_out_deg() const {
	return node_map_out_.size();
}

void DynGraph::print() const {
	auto it = node_map_in_.begin();
	for (; it != node_map_in_.end(); ++it) {
		pair<int, vector<int>> v_pair = *it;
		int src = v_pair.first;
		for (vector<int>::iterator it2 = v_pair.second.begin();
				it2 != v_pair.second.end(); ++it2) {
			int out_neighbor = *it2;
			cout << src << "<-" << out_neighbor << endl;
		}
	}

}

bool DynGraph::remove_edge(const int source, const int destination) {
	unsigned long long edge_p = edge_to_id(source, destination);
	if (edge_map_in_.find(edge_p) == edge_map_in_.end()) {
		return false;
	}

	--num_edges_;

	int pos_out = edge_map_out_[edge_p];
	int pos_in = edge_map_in_[edge_p];

	if (node_map_out_[source].size() == 1) {
		node_map_out_.erase(source);
		if (node_map_in_.find(source) == node_map_in_.end()) {
			nodes_.erase(source);
			--num_nodes_;
		}
	} else {
		node_map_out_[source][pos_out] =
				node_map_out_[source][node_map_out_[source].size() - 1];
		node_map_out_[source].resize(node_map_out_[source].size() - 1);
		int new_node = node_map_out_[source][pos_out];
		edge_map_out_[edge_to_id(source, new_node)] = pos_out;
	}

	if (node_map_in_[destination].size() == 1) {
		node_map_in_.erase(destination);
		if (node_map_out_.find(destination) == node_map_out_.end()) {
			nodes_.erase(destination);
			--num_nodes_;
		}
	} else {
		node_map_in_[destination][pos_in] =
				node_map_in_[destination][node_map_in_[destination].size() - 1];
		node_map_in_[destination].resize(node_map_in_[destination].size() - 1);
		int new_node = node_map_in_[destination][pos_in];
		edge_map_in_[edge_to_id(new_node, destination)] = pos_in;
	}

	edge_map_out_.erase(edge_p);
	edge_map_in_.erase(edge_p);
	return true;
}

int DynGraph::num_edges() const {
	return num_edges_;
}

int DynGraph::num_nodes() const {
	return num_nodes_;
}

double DynGraph::density_graph() const {
	if (num_nodes_ == 0) {
		return 0.0;
	}
	return static_cast<double>(num_edges_) / static_cast<double>(num_nodes_);
}

void DynGraph::remove_node(const int node) {
	vector<int> out_edges;
	out_neighbors(node, &out_edges);
	vector<int> in_edges;
	in_neighbors(node, &in_edges);

	for (vector<int>::iterator it = out_edges.begin(); it != out_edges.end();
			++it) {
		int v = *it;
		remove_edge(node, v);
	}
	for (vector<int>::iterator it = in_edges.begin(); it != in_edges.end();
			++it) {
		int v = *it;
		remove_edge(v, node);
	}
}

void DynGraph::out_neighbors(const int source, vector<int>* vec) const {
	vec->clear();
	if (node_map_out_.find(source) != node_map_out_.end()) {
		const vector<int>& neighbors = node_map_out_.find(source)->second;
		vec->assign(neighbors.begin(), neighbors.end());
	}
}

void DynGraph::in_neighbors(const int destination, vector<int>* vec) const {
	vec->clear();
	if (node_map_in_.find(destination) != node_map_in_.end()) {
		const vector<int>& neighbors = node_map_in_.find(destination)->second;
		vec->assign(neighbors.begin(), neighbors.end());
	}
}

void DynGraph::nodes(vector<int>* vec) const {
	vec->clear();
	vec->assign(nodes_.begin(), nodes_.end());
}

int DynGraph::out_degree(const int source) const {
	if (node_map_out_.find(source) == node_map_out_.end()) {
		return 0;
	}
	return node_map_out_.find(source)->second.size();
}

int DynGraph::in_degree(const int destination) const {
	if (node_map_in_.find(destination) == node_map_in_.end()) {
		return 0;
	}
	return node_map_in_.find(destination)->second.size();
}

void DynGraph::edges(vector<pair<int, int> >* vec) const {
	vec->clear();
	auto it = node_map_out_.begin();
	for (; it != node_map_out_.end(); ++it) {
		pair<int, vector<int>> v_pair = *it;
		int src = v_pair.first;
		for (vector<int>::iterator it2 = v_pair.second.begin();
				it2 != v_pair.second.end(); ++it2) {
			int out_neighbor = *it2;
			vec->push_back(make_pair(src, out_neighbor));
		}
	}

}
