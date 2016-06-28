/*
 * DynGraph.h
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#ifndef DYNGRAPH_H_
#define DYNGRAPH_H_

#include<map>
#include<vector>

#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <climits>

using namespace std;

#define MAX_NUM_NODES 50000000

/** Handles efficiently dynamic graphs */
class DynGraph {
    friend class UDynGraph;

public:
	DynGraph();
	virtual ~DynGraph();
	// Returns true if the edge is added.
	bool add_edge(const int source, const int destination);
	void remove_node(const int node);

	// Returns true if the edge is removed.
	bool remove_edge(const int source, const int destination);
	void out_neighbors(const int source, vector<int>* vec) const;
	void in_neighbors(const int destination, vector<int>* vec) const;
	int out_degree(const int source) const;
	int in_degree(const int destination) const;
	void edges(vector<pair<int, int> >* vec) const;
	void nodes(vector<int>* vec) const;
	void clear();
	int non_zero_out_deg() const;
	double density_graph() const;
	int num_edges() const;
	int num_nodes() const;
	int max_in_deg_upperbound() const;
	void print() const;

private:
	//*******************************************************
	unordered_map<int, vector<int> > node_map_out_;
	unordered_map<unsigned long long, int> edge_map_out_;
	unordered_map<int, vector<int> > node_map_in_;
	unordered_map<unsigned long long, int> edge_map_in_;
	unordered_set<int> nodes_;
	int num_nodes_;
	int num_edges_;
	int max_in_deg_upperbound_;
	// ATTENTION: If new fields are added they need to be copied in UDynGraph assign() function
	//*******************************************************

	inline unsigned long long edge_to_id(const int src,
			const int des) const {
		assert(MAX_NUM_NODES > src);
		assert(MAX_NUM_NODES > des);

		unsigned long long ret = static_cast<unsigned long long>(MAX_NUM_NODES)
				* static_cast<unsigned long long>(src)
				+ static_cast<unsigned long long>(des);
		assert(ret);
		return ret;
	}
};

#endif /* DYNGRAPH_H_ */
