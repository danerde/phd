/*
 * World.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef WORLD_H_
#define WORLD_H_

#include "Time.h"
#include "Object.h"
#include "Pose.h"
#include "boost/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "Barier.h"

class World{
public:
	vector<Object::Ptr> objects;
	Pose tf;

	int borderL;
	int borderR;
	int borderT;
	int borderB;

public:
	World();
	~World();

	void save_state();

	void update(const Time& t);

	double getBorderL()const{ return tf.location.x+borderL; }
	double getBorderR()const{ return tf.location.x+borderR; }
	double getBorderT()const{ return tf.location.y+borderT; }
	double getBorderB()const{ return tf.location.y+borderB; }

	void draw(Mat& page);


private:
	boost::thread_group threads;
	boost::mutex mtx;
	boost::condition_variable update_run;
	Time update_time;
	Barier barier;

	void update_thread(int n, int c);
	void update_proccess(size_t from, size_t to);
};

#endif /* WORLD_H_ */
