/*
 * Stats.h
 *
 *  Created on: Nov 2, 2014
 *      Author: aepasto
 */

#ifndef STATS_H_
#define STATS_H_

#include <vector>
#include <chrono>

using namespace std;

typedef struct Stat {
	unsigned long operation_num;
	unsigned int add_op_in_window;
	unsigned int rem_op_in_window;
	double micros;

	double max_approx_error;
	double max_density;

	unsigned int last_size;
	double last_density;
	double micros_per_op;
	unsigned int last_timestamp;

	unsigned int op_count_total;
} Stat;

class Stats {
public:
	explicit Stats(const int stat_window_size) :
			stat_window_size_(stat_window_size), op_count_(0) {

		add_count_window = 0;
		remove_count_window = 0;
		max_density_ = 0;
		max_approx_error_ = 0;
		last_op_count_ = last_size_ = last_density_ = 0;
	}
	virtual ~Stats();

	void exec_op(bool is_add, unsigned int size_ds, double density, double upperbound, unsigned int timestamp);
	void end_op();

	const vector<Stat> stats() {
		return stats_;
	}

private:
	void reset_window();

	const unsigned int stat_window_size_;

	unsigned int add_count_window;
	unsigned int remove_count_window;

	double max_density_;
	double max_approx_error_;

	unsigned int last_size_;
	double last_density_;
	unsigned int last_op_count_;
	unsigned int last_timestamp_;

	std::chrono::system_clock::time_point last_time_;

	unsigned int op_count_;
	vector<Stat> stats_;
};

#endif /* STATS_H_ */
