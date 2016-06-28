//============================================================================
// Name        : DensestSubgraph.cpp
// Author      : Alessandro Epasto
// Version     :
// Copyright   : 
// Description : Hello World in C, Ansi-style
//============================================================================

#include "DynDSAlgAddRem.h"
#include "DynGraph.h"
#include "DynDSAlg.h"
#include "UDynGraph.h"
#include "DynGraphUtils.h"
#include "DSAlgs.h"
#include "GraphScheduler.h"
#include "Stats.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

int main(int argc, char** argv) {

	if (argc != 5) {
		cerr
				<< "ERROR Requires 3 parameters. ExecAddRem epsilon(double); graph-udates.txt; stat_window_size; use_time(1 to use)"
				<< endl;
		exit(1);
	}

	double epsilon = atof(argv[1]);
	string file_name(argv[2]);
	bool use_time = atoi(argv[4])==1;
	GraphScheduler gs(file_name, use_time);

	UDynGraph g;
	DynDSAddRem dyn_alg(epsilon);

	Stats stats(atoi(argv[3]));

	bool executed;

	double threshold = 2.0 * pow(1.0 + epsilon, 6);

	while (gs.has_next()) {
		EdgeUpdate edge_up = gs.next_update();

		if (edge_up.is_add) {
			executed = dyn_alg.add_edge(edge_up.node_u, edge_up.node_v);
		} else {
			executed = dyn_alg.remove_edge(edge_up.node_u, edge_up.node_v);
		}

		double upperbound = min((double) dyn_alg.max_in_degree_upperbound(),
				2.0 * (1 + epsilon) * dyn_alg.beta());

		double density = dyn_alg.density_subgraph();

		if (executed) {
			stats.exec_op(edge_up.is_add, dyn_alg.size_densest(), density,
					upperbound, edge_up.time);
		}

		if (dyn_alg.num_edges() != 0
				&& density * threshold < upperbound - dyn_alg.EPS_ERR) {
			cerr << "[FATAL] APPROXIMATION VIOLATION Dens:"
					<< dyn_alg.density_subgraph() << "; 2*(1+e)^6*density"
					<< dyn_alg.density_subgraph() * 2.0 * pow(1.0 + epsilon, 6)
					<< " upperbound:"

					<< upperbound << endl;

			assert(false);
		}
	}

	stats.end_op();

	return 0;
}
