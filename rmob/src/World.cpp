/*
 * World.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/World.h>
#include <rmob/Object.h>

#if THREAD_COUNT==1
#else
#	define MTHREADING
#endif

World::World() {
	borderL=-1000;
	borderR=1000;
	borderT=1000;
	borderB=-1000;

#ifdef MTHREADING
	int count=THREAD_COUNT;
	for(int i=0;i<count;i++)
		threads.add_thread(new boost::thread(boost::bind(&World::update_thread,this, i, count)));
#endif
}

World::~World() {
#ifdef MTHREADING
	threads.interrupt_all();
	update_run.notify_all();
#endif
}




void World::update(const Time& t){
#ifdef MTHREADING
	boost::mutex::scoped_lock l(mtx);
#endif
	update_time = t;
#ifndef MTHREADING
	update_proccess(0,objects.size());
#else
	barier.set(THREAD_COUNT);
	update_run.notify_all();
	barier.wait(l);
#endif
}
void World::update_thread(int n, int count){
	boost::mutex::scoped_lock l(mtx);
	while(not boost::this_thread::interruption_requested()){
		update_run.wait(l);
		if(boost::this_thread::interruption_requested()) break;
		if(n<0) update_proccess(0, objects.size());
		else{
			size_t f=(float(n)/count)*objects.size();
			size_t t=(float(n+1)/count)*objects.size();
			update_proccess(f,t);
		}
		barier.notify();
	}
}
void World::update_proccess(size_t from, size_t to){
	const Time& t = update_time;
	const World& self = *this;

	for(int i=from; i<(int)to and i<(int)objects.size(); i++){
		Object::Ptr obj = objects[i];
		if(obj->isPickedup) continue;
		obj->action(self);
	}

	for(int i=from; i<(int)to and i<(int)objects.size(); i++){
		Object::Ptr p = objects[i];
		if(p->isPickedup) continue;
		V2d speed = (p->speed*t.now()).rotated(p->pose.heading) + (p->speed_impuls*t.now());
		p->speed_impuls = p->speed_impuls*0.8;
		p->pose.location = p->pose.location+speed;
		p->pose.heading = angle(p->pose.heading + p->speed.ang());
	}

}

void World::save_state(){
#ifdef MTHREADING
	boost::mutex::scoped_lock l(mtx);
#endif
	foreach(Object::Ptr p, objects){
		p->save_state();
	}
}

void World::draw(Mat& page){
	foreach(Object::Ptr p, objects){
		p->draw(tf, page);
	}
}
