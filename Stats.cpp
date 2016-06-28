/*
 * Stats.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: aepasto
 */

/*
 * GraphScheduler.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: aepasto
 */

#include "Stats.h"
#include <cstring>
#include <ctime>
#include <cassert>
#include <chrono>
#include <iostream>


using namespace std;

Stats::~Stats() {
}

void Stats::reset_window() {
	Stat new_stat;

	auto now = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
			now - last_time_);
	new_stat.micros = elapsed.count();
	last_time_ = now;

	new_stat.operation_num = op_count_ - last_op_count_;
	new_stat.add_op_in_window = add_count_window;
	new_stat.rem_op_in_window = remove_count_window;
	new_stat.max_approx_error = max_approx_error_;
	new_stat.last_density = last_density_;
	new_stat.last_size = last_size_;
	new_stat.max_density = max_density_;
	new_stat.op_count_total = op_count_;
	assert(new_stat.operation_num > 0);
	new_stat.micros_per_op = new_stat.micros / new_stat.operation_num;
	new_stat.last_timestamp = last_timestamp_;

	add_count_window = 0;
	remove_count_window = 0;
	max_approx_error_ = 0;
	max_density_ = 0;
	last_op_count_ = op_count_;

	stats_.push_back(new_stat);

	cout << new_stat.op_count_total << ", " << new_stat.last_timestamp << ", "
			<< new_stat.last_density << ", " << new_stat.last_size << ", "
			<< new_stat.max_approx_error << ", " << new_stat.operation_num
			<< ", " << new_stat.micros << ", " << new_stat.micros_per_op << ", "
			<< new_stat.add_op_in_window << ", " << new_stat.rem_op_in_window
			<< endl;

	now = std::chrono::system_clock::now();
	last_time_ = now;
}

void Stats::end_op() {
	if (op_count_ != last_op_count_) {
		reset_window();
	}
}

void Stats::exec_op(bool is_add, unsigned int size_ds, double density,
		double upperbound, unsigned int last_timestamp) {
	if (op_count_ == 0) {
		last_time_ = std::chrono::system_clock::now();
		cout << "op_count_total" << ", " << "last_timestamp" << ", "
				<< "last_density" << ", " << "last_size" << ", "
				<< "max_approx_error" << ", " << "operation_num" << ", "
				<< "micros_total" << ", " << "micros_per_op" << ", "
				<< "add_op_in_window" << ", " << "rem_op_in_window" << endl;
	}

	if (is_add) {
		++add_count_window;
	} else {
		++remove_count_window;
	}

	++op_count_;

	if (density > max_density_) {
		max_density_ = density;
	}

	double approx_error;
	if (density == 0.0 && upperbound > 0.0) {
		approx_error = 1000000000; // There is no approximation bound
	} else {
		approx_error = upperbound / density;
	}

	if (approx_error > max_approx_error_) {
		max_approx_error_ = approx_error;
	}

	last_size_ = size_ds;
	last_density_ = (size_ds > 0 ? density : 0);
	if (is_add){
		last_timestamp_ = last_timestamp;
	}

	if (op_count_ % stat_window_size_ == 0) {
		reset_window();
	}
}

