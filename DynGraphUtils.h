/*
 * DynGraphUtils.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#ifndef UDYNGRAPHUTILS_H_
#define UDYNGRAPHUTILS_H_

#include "DynGraph.h"
#include "UDynGraph.h"
#include <vector>

using namespace std;

// TODO use template
double density(const UDynGraph& graph, vector<int>::iterator subset_begin,
		vector<int>::iterator subset_end);
double density(const UDynGraph& graph,
		unordered_set<int>::iterator subset_begin,
		unordered_set<int>::iterator subset_end);

int max_iter(const double num_nodes, const double epsilon);

// TODO use template
pair<unsigned int, unsigned long long> num_nodes_and_edges_induced(
		const UDynGraph& graph, unordered_set<int>::iterator subset_begin,
		unordered_set<int>::iterator subset_end);
pair<unsigned int, unsigned long long> num_nodes_and_edges_induced(
		const UDynGraph& graph, vector<int>::iterator subset_begin,
		vector<int>::iterator subset_end);

#endif /* UDYNGRAPHUTILS_H_ */
