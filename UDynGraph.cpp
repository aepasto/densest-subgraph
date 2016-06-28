/*
 * UDynGraph.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#include "UDynGraph.h"
#include<iostream>

double UDynGraph::density_graph() const {
	assert(this->graph.num_edges() % 2 == 0);
	return this->graph.density_graph() / 2.0; // The underlying graph has two edges
											  //per each undirected edge.
}

void UDynGraph::assign(const UDynGraph& other) {
	this->clear();
	/*vector<pair<int, int> > edges;
	 other.edges(&edges);

	 vector<pair<int, int> >::iterator it = edges.begin();

	 for (;it != edges.end(); ++it) {
	 pair<int, int>  edge = *it;
	 this->add_edge(edge.first, edge.second);
	 }*/

	graph.edge_map_in_ =
			static_cast<const unordered_map<unsigned long long, int>&>(other.graph.edge_map_in_);
	graph.edge_map_out_ = static_cast<const unordered_map<unsigned long long,
			int>&>(other.graph.edge_map_out_);
	graph.node_map_out_ =
			static_cast<const unordered_map<int, vector<int> >&>(other.graph.node_map_out_);
	graph.node_map_in_ =
			static_cast<const unordered_map<int, vector<int> >&>(other.graph.node_map_in_);
	graph.nodes_ = other.graph.nodes_;
	graph.num_nodes_ = other.graph.num_nodes_;
	graph.num_edges_ = other.graph.num_edges_;
	graph.max_in_deg_upperbound_ = other.graph.max_in_deg_upperbound_;
}

void UDynGraph::clear() {
	this->graph.clear();
}

bool UDynGraph::add_edge(const int u, const int v) {
	assert(u != v);
	bool add_1 = this->graph.add_edge(u, v);
	bool add_2 = this->graph.add_edge(v, u);
	if (add_1 ^ add_2) {
		cerr << "ERROR: " << add_1 << " " << add_2 << endl;
		cerr << "ADDING: " << u << " " << v << endl;
		cerr.flush();
	}
	assert(!add_1 ^ add_2);
	return add_1;

}

bool UDynGraph::remove_edge(const int u, const int v) {
	assert(u != v);
	bool rem_1 = this->graph.remove_edge(u, v);
	bool rem_2 = this->graph.remove_edge(v, u);
	assert(!rem_1 ^ rem_2);
	return rem_1;
}

void UDynGraph::remove_node(const int node) {
	this->graph.remove_node(node);
}

void UDynGraph::neighbors(const int u, vector<int>* vec) const {
	return this->graph.out_neighbors(u, vec);
}

unsigned int UDynGraph::degree(const int u) const {
	return this->graph.out_degree(u);
}

void UDynGraph::edges(vector<pair<int, int> >* vec) const {
	this->graph.edges(vec);
}

void UDynGraph::nodes(vector<int>* vec) const {
	this->graph.nodes(vec);
}

unsigned int UDynGraph::num_nodes() const {
	return this->graph.num_nodes();
}

unsigned int UDynGraph::num_edges() const {
	assert(this->graph.num_edges() % 2 == 0);
	return this->graph.num_edges() / 2;
}
