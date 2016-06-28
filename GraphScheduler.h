/*
 * GraphScheduler.h
 *
 *  Created on: Oct 22, 2014
 *      Author: aepasto
 */

#ifndef GRAPHSCHEDULER_H_
#define GRAPHSCHEDULER_H_

#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

enum Update {
	ADD, REM
};

typedef struct EdgeUpdate {
	int node_u;
	int node_v;
	int time;
	bool is_add;
} EdgeUpdate;

typedef struct EdgeUpdateNoTime {
	int node_u;
	int node_v;
	bool is_add;
} EdgeUpdateNoTime;

class GraphScheduler {
public:
	GraphScheduler(const string& file_name, bool store_time_);
	virtual ~GraphScheduler();

	EdgeUpdate next_update();
	inline bool has_next() {
		if (store_time_) {
			return (unsigned int) position_queue_
					< (unsigned int) edge_queue_.size();
		} else {
			return (unsigned int) position_queue_
					< (unsigned int) edge_queue_no_time_.size();
		}
	}

private:
	bool store_time_;
	int add_count;
	int remove_count;
	void retrieve_all_edges();
	ifstream file_stream_;
	vector<EdgeUpdate> edge_queue_;
	vector<EdgeUpdateNoTime> edge_queue_no_time_;

	unsigned int position_queue_;
};

#endif /* GRAPHSCHEDULER_H_ */
