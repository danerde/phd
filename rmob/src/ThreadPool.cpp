/*
 * ThreadPool.cpp
 *
 *  Created on: Jun 15, 2014
 *      Author: dan
 */

#include <rmob/ThreadPool.h>

ThreadPool::ThreadPool(int n) {
	for( int i=0; i<n; i++){
		create_thread();
	}
}

ThreadPool::~ThreadPool() {
	stop_all();
}

