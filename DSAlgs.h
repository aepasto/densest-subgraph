/*
 * DSAlgs.h
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#ifndef DSALGS_H_
#define DSALGS_H_

#include "DynGraph.h"
#include <vector>
#include <unordered_map>
#include <cstdlib>

class UDynGraph;

typedef struct DSResult {
	double density;
	std::vector<int> subgraph;
	unsigned long edges_in_subgraph;

	DSResult() {
		density = -1;
		edges_in_subgraph = 0;
	}

	void clear() {
		density = -1;
		edges_in_subgraph = 0;
		subgraph.clear();
	}
} DSResult;

using namespace std;

void Find(const UDynGraph& graph, const double beta, const double epsilon,
		DSResult * result, unordered_map<int, int>* level_map,
		DynGraph* orientation, bool* valid_orientation,
		unordered_map<int, int>* level_degree_map = NULL);
//TODO use template
void Incremental(const UDynGraph& graph, const double beta_zero,
		const double epsilon, vector<int>::iterator current_subgraph_begin,
		vector<int>::iterator current_subgraph_end, DSResult * result,
		unordered_map<int, int>* level_map, DynGraph* orientation,
		unordered_map<int, int>* level_degree_map = NULL);
void Incremental(const UDynGraph& graph, const double beta_zero,
		const double epsilon,
		unordered_set<int>::iterator current_subgraph_begin,
		unordered_set<int>::iterator current_subgraph_end, DSResult * result,
		unordered_map<int, int>* level_map, DynGraph* orientation,
		unordered_map<int, int>* level_degree_map = NULL);

void Bahmani(const UDynGraph& graph, const double epsilon, DSResult * result);

#endif /* DSALGS_H_ */
