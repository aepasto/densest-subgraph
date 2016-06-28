/*
 * GraphScheduler.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: aepasto
 */

#include "GraphScheduler.h"
#include <cstring>
#include <cassert>
#include <iostream>
#include <functional>
using namespace std;

GraphScheduler::~GraphScheduler() {
	this->file_stream_.close();
}

GraphScheduler::GraphScheduler(const string& file_name, bool store_time) {

	store_time_ = store_time;
	file_stream_.open(file_name.c_str(), ios_base::in);
	retrieve_all_edges();
	position_queue_ = 0;
	add_count = 0;
	remove_count = 0;
}

void GraphScheduler::retrieve_all_edges() {

	std::string delimiter = " ";
	string line;
	vector<string> tokens;

	EdgeUpdate next_edge;
	EdgeUpdateNoTime next_edge_no_time;

	while (getline(file_stream_, line)) {
		tokens.clear();

		size_t pos = 0;
		std::string token;
		while (pos != line.npos) {
			pos = line.find(delimiter);
			token = line.substr(0, pos);
			tokens.push_back(token);
			line.erase(0, pos + delimiter.length());
		}

		assert(tokens.size() == 4);
		if (tokens[0][0] == '+') {
			next_edge.is_add = true;
		} else if (tokens[0][0] == '-') {
			next_edge.is_add = false;
		} else {
			assert(false);
		}

		next_edge.node_u = atoi(tokens[1].c_str());
		next_edge.node_v = atoi(tokens[2].c_str());
		next_edge.time = atoi(tokens[3].c_str());
		assert(next_edge.node_u >= 0);
		assert(next_edge.node_v >= 0);
		assert(next_edge.time >= 0);

		if(next_edge.node_u == next_edge.node_v){
			continue;
		}

		if (store_time_){
			edge_queue_.push_back(next_edge);
		} else {
			next_edge_no_time.is_add = next_edge.is_add;
			next_edge_no_time.node_u = next_edge.node_u;
			next_edge_no_time.node_v = next_edge.node_v;
			edge_queue_no_time_.push_back(next_edge_no_time);
		}
		//if (edge_queue_.size() % 100000000 == 99999999) {
		//	cerr << "READ #: " << edge_queue_.size() + 1 << endl;
		//}
	}

	// Only in C++11
	//edge_queue_.shrink_to_fit();
	//edge_queue_no_time_.shrink_to_fit();
}

EdgeUpdate GraphScheduler::next_update() {

	EdgeUpdate edge_queue;
	if (store_time_){
		edge_queue = edge_queue_[position_queue_];
	} else {
		EdgeUpdateNoTime no_time = edge_queue_no_time_[position_queue_];
		edge_queue.is_add = no_time.is_add;
		edge_queue.node_u = no_time.node_u;
		edge_queue.node_v = no_time.node_v;
		edge_queue.time = 0;
	}
	++position_queue_;

	if (edge_queue.is_add) {
		++add_count;
	} else {
		++remove_count;
	}
	/*if (edge_queue.is_add && add_count % 100000 == 99999) {
		cerr << "ADD #: " << add_count + 1 << endl;
		cerr.flush();
	} else if (!edge_queue.is_add && remove_count % 100000 == 99999) {
		cerr << "REM #: " << remove_count + 1 << endl;
		cerr.flush();
	}*/
	return edge_queue;
}
