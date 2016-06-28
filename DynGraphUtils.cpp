/*
 * DynGraphUtils.h
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#ifndef DYNGRAPHUTILS_H_
#define DYNGRAPHUTILS_H_

#include "UDynGraph.h"

#include <unordered_set>
#include <iostream>
#include <cmath>

template<typename Iter>
pair<unsigned int, unsigned long long> num_nodes_and_edges_induced_tmpl(
		const UDynGraph& graph, Iter begin, Iter end) {
	unsigned long long induced_edges = 0;

	unordered_set<int> nodes(begin, end);

	for (unordered_set<int>::iterator it = nodes.begin(); it != nodes.end();
			++it) {

		vector<int> neighbors;
		graph.neighbors(*it, &neighbors);
		for (vector<int>::iterator it_neighbor = neighbors.begin();
				it_neighbor != neighbors.end(); ++it_neighbor) {
			if (nodes.find(*it_neighbor) != nodes.end()) {
				++induced_edges;
			}
		}
	}
	assert(induced_edges % 2 == 0);

	return make_pair(nodes.size(), induced_edges / 2);
}

pair<unsigned int, unsigned long long> num_nodes_and_edges_induced(
		const UDynGraph& graph, vector<int>::iterator begin,
		vector<int>::iterator end) {
	return num_nodes_and_edges_induced_tmpl(graph, begin, end);
}

pair<unsigned int, unsigned long long> num_nodes_and_edges_induced(
		const UDynGraph& graph, unordered_set<int>::iterator begin,
		unordered_set<int>::iterator end) {
	return num_nodes_and_edges_induced_tmpl(graph, begin, end);
}

template<typename Iter>
double density_tmpl(const UDynGraph& graph, Iter begin, Iter end) {

	pair<int, unsigned long long> induced = num_nodes_and_edges_induced(graph,
			begin, end);

	if (induced.first == 0) {
		return 0;
	}

	return static_cast<double>(induced.second)
			/ static_cast<double>(induced.first);
}

double density(const UDynGraph& graph, vector<int>::iterator begin,
		vector<int>::iterator end) {
	return density_tmpl(graph, begin, end);
}
double density(const UDynGraph& graph, unordered_set<int>::iterator begin,
		unordered_set<int>::iterator end) {
	return density_tmpl(graph, begin, end);
}

int max_iter(const double num_nodes, const double epsilon) {
	return ceil(log(num_nodes) / log1p(epsilon)) + 5; // this is to be sure we have room for rounding error when node count increases
}

#endif /* DYNGRAPHUTILS_H_ */
