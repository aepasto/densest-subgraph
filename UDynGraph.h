/*
 * UDynGraph.h
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#ifndef UDYNGRAPH_H_
#define UDYNGRAPH_H_

#include "DynGraph.h"

class UDynGraph {
public:
    // Returns true if the edge is added.
	bool add_edge(const int u, const int v);
	void remove_node(const int u);
    // Returns true if the edge is added.
	bool remove_edge(const int u, const int v);
	void neighbors(const int u, vector<int>* vec) const;
	unsigned int degree(const int u) const;
	void edges(vector<pair<int, int> >* vec) const;
	void nodes(vector<int>* vec) const;
	void clear();
	void assign(const UDynGraph& other);
	unsigned int num_nodes() const;
	unsigned int num_edges() const;
	double density_graph() const;
private:
	DynGraph graph;
};

#endif /* UDYNGRAPH_H_ */
