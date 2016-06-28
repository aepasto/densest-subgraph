/*
 * DSAlgs.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#include "DSAlgs.h"
#include "DynGraphUtils.h"
#include "UDynGraph.h"

#include<iostream>
#include <cstdlib>

using namespace std;

void Find(const UDynGraph& graph, const double beta, const double epsilon,
		DSResult * result, unordered_map<int, int>* level_map,
		DynGraph* orientation, bool* valid_orientation,
		unordered_map<int, int>* level_degree_map) {

	if (orientation != NULL) {
		orientation->clear();
	}
	if (level_degree_map != NULL) {
		level_degree_map->clear();
	}

	vector<int> nodes;
	graph.nodes(&nodes);
	unordered_set<int> present(nodes.begin(), nodes.end());

	unordered_set<int> to_delete;
	unordered_set<int> to_delete_next;

	UDynGraph m_graph;
	m_graph.assign(graph);

	result->density = m_graph.density_graph();
	m_graph.nodes(&result->subgraph);
	level_map->clear();

	//cout << " [IN FIND] begin dens " << result->density << endl;

	const int max_iter_num = max_iter(nodes.size(), epsilon);
	const double threshold = 2.0 * beta * (1.0 + epsilon);

	assert(threshold > 0.0);

	vector<int> v_neighbors;

	for (vector<int>::iterator nodes_it = nodes.begin();
			nodes_it != nodes.end(); ++nodes_it) {
		int v = *nodes_it;
		if ((double) m_graph.degree(v) < threshold) {
			to_delete.insert(v);

		}
	}

	for (int iter = 1; iter <= max_iter_num; ++iter) {
		if (to_delete.empty()) {
			break;
		}

		to_delete_next.clear();

		if (level_degree_map != NULL) {
			for (unordered_set<int>::iterator to_delete_it = to_delete.begin();
					to_delete_it != to_delete.end(); ++to_delete_it) {
				int v = *to_delete_it;
				(*level_degree_map)[v] = m_graph.degree(v);
			}
		}

		for (unordered_set<int>::iterator to_delete_it = to_delete.begin();
				to_delete_it != to_delete.end(); ++to_delete_it) {
			int v = *to_delete_it;

			if (present.find(v) == present.end()) {
				continue;
			}

			present.erase(v);
			v_neighbors.clear();
			m_graph.neighbors(v, &v_neighbors);
			(*level_map)[v] = iter;
			m_graph.remove_node(v);

			for (vector<int>::iterator neighbor_v_it = v_neighbors.begin();
					neighbor_v_it != v_neighbors.end(); ++neighbor_v_it) {
				int neighbor_v = *neighbor_v_it;

				if (present.find(neighbor_v) != present.end()) {
					if (orientation != NULL) {

						orientation->add_edge(neighbor_v, v);
					}
					if ((double) m_graph.degree(neighbor_v) < threshold
							&& to_delete.find(neighbor_v) == to_delete.end()) {
						to_delete_next.insert(neighbor_v);
					}
				}
			}
		}

		double new_density = m_graph.density_graph();
		if (new_density > result->density) {
			result->density = new_density;
			result->subgraph.clear();
			m_graph.nodes(&result->subgraph);
			//cout << " [IN FIND] improved density " << result->density << endl;
			//cout << " [IN FIND] SIZE S" << result->subgraph.size() << endl;
			//cout << " [IN FIND] EDGES in curr subgraph" << m_graph.num_edges()
			//		<< endl;
			//cout << " [IN FIND] NODES in curr subgraph" << m_graph.num_nodes()
			//		<< endl;

		}

		to_delete.clear();
		to_delete.insert(to_delete_next.begin(), to_delete_next.end());
	}

	*valid_orientation = present.empty();
}

// Starting from beta_zero, runs Find until it fails.
// If provided it will use the current_solution graph as starting point to set beta.
template<typename Iter>
void Incremental_tmpl(const UDynGraph& graph, const double beta_zero,
		const double epsilon, const Iter& current_subgraph_begin,
		const Iter& current_subgraph_end, DSResult * result,
		unordered_map<int, int>* level_map, DynGraph* orientation,
		unordered_map<int, int>* level_degree_map) {

	DSResult optimal;
	double beta = beta_zero;

	double curr_dens = density(graph, current_subgraph_begin,
			current_subgraph_end);

	assert(beta_zero > 0 || curr_dens > 0);

	if (curr_dens > 0) {
		optimal.subgraph.clear();
		optimal.subgraph.assign(current_subgraph_begin, current_subgraph_end);
		optimal.density = curr_dens;
		if (orientation != NULL) {
			orientation->clear();
		}
	} else {
		optimal.subgraph.clear();
		optimal.density = -1;
		if (orientation != NULL) {
			orientation->clear();
		}
	}

	beta = max(beta_zero, curr_dens * (1.0 + epsilon));

	DSResult new_res;

	while (true) {
		bool valid_orientation = false;

		//cerr << "Find: " << beta << endl;
		//cerr << "Curr dens: " << optimal.density << endl;
		//cerr << "SIZE ds: " << optimal.subgraph.size() << endl;
		//cerr << "Edges graphs: " << graph.num_edges() << endl;

		Find(graph, beta, epsilon, &new_res, level_map, orientation,
				&valid_orientation, level_degree_map);
		if (new_res.density >= beta && new_res.density > optimal.density) {
			optimal.density = new_res.density;
			//cout << " [IN ITERATIVE] " << new_res.density << endl;
			optimal.subgraph.clear();
			optimal.subgraph.assign(new_res.subgraph.begin(),
					new_res.subgraph.end());
			beta = new_res.density * (1.0 + epsilon);
		} else {
			assert(valid_orientation); //this must be true for the theory.
			if (orientation != NULL) {
				assert(orientation->num_edges() == graph.num_edges());
			}
			if (level_degree_map != NULL) {
				assert(level_degree_map->size() == graph.num_nodes());
			}
			break;
		}
	}

	assert(optimal.density >= 0);
	assert(optimal.density * (1 + epsilon) >= beta);
	*result = optimal;

	assert(graph.num_edges() == 0 || optimal.density > 0);

	//cout << "END INCREMENTAL: " << endl;
	//cout << "Curr dens: " << result->density << endl;
	//cout << "SIZE ds: " << result->subgraph.size() << endl;
}

void Incremental(const UDynGraph& graph, const double beta_zero,
		const double epsilon, vector<int>::iterator current_subgraph_begin,
		vector<int>::iterator current_subgraph_end, DSResult * result,
		unordered_map<int, int>* level_map, DynGraph* orientation,
		unordered_map<int, int>* level_degree_map) {
	Incremental_tmpl(graph, beta_zero, epsilon, current_subgraph_begin,
			current_subgraph_end, result, level_map, orientation,
			level_degree_map);
}

void Incremental(const UDynGraph& graph, const double beta_zero,
		const double epsilon,
		unordered_set<int>::iterator current_subgraph_begin,
		unordered_set<int>::iterator current_subgraph_end, DSResult * result,
		unordered_map<int, int>* level_map, DynGraph* orientation,
		unordered_map<int, int>* level_degree_map) {
	Incremental_tmpl(graph, beta_zero, epsilon, current_subgraph_begin,
			current_subgraph_end, result, level_map, orientation,
			level_degree_map);
}

void Bahmani(const UDynGraph& graph, const double epsilon, DSResult * result) {

	vector<int> curr_subgraph;
	vector<int> new_subgraph;
	graph.nodes(&curr_subgraph);
	unordered_set<int> present(curr_subgraph.begin(), curr_subgraph.end());

	UDynGraph m_graph;
	m_graph.assign(graph);

	result->density = m_graph.density_graph();
	result->subgraph.assign(curr_subgraph.begin(), curr_subgraph.end());

	double last_density = result->density;

	while (!curr_subgraph.empty()) {
		double threshold = 2.0 * last_density * (1.0 + epsilon);
		new_subgraph.clear();

		for (vector<int>::iterator curr_subgraph_it = curr_subgraph.begin();
				curr_subgraph_it != curr_subgraph.end(); ++curr_subgraph_it) {
			int v = *curr_subgraph_it;

			if ((double) m_graph.degree(v) >= threshold) {
				new_subgraph.push_back(v);
			} else {
				present.erase(v);
			}
		}

		for (vector<int>::iterator curr_subgraph_it = curr_subgraph.begin();
				curr_subgraph_it != curr_subgraph.end(); ++curr_subgraph_it) {
			int v = *curr_subgraph_it;
			if (present.find(v) == present.end()) {
				m_graph.remove_node(v);
			}
		}

		double new_density = m_graph.density_graph();
		if (new_density > result->density) {
			result->density = new_density;
			result->subgraph.assign(new_subgraph.begin(), new_subgraph.end());
			result->edges_in_subgraph = m_graph.num_edges();
		}
		curr_subgraph.assign(new_subgraph.begin(), new_subgraph.end());
		last_density = new_density;

		if (last_density == 0) { // no edge in the graph, we can stop.
			break;
		}
	}
}
