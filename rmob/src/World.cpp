/*
 * World.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/World.h>
#include <rmob/Object.h>

World::World() {
	borderL=-1000;
	borderR=1000;
	borderT=1000;
	borderB=-1000;

	threads.add_thread(new boost::thread(boost::bind(&World::update_thread,this)));
}

World::~World() {
	threads.interrupt_all();
	update_run.notify_all();
}




void World::update(const Time& t){
	boost::mutex::scoped_lock l(mtx);
	update_time = t;
	//update_proccess();
	update_run.notify_one();
}
void World::update_thread(){
	boost::mutex::scoped_lock l(mtx);
	while(not boost::this_thread::interruption_requested()){
		update_run.wait(l);
		if(boost::this_thread::interruption_requested()) break;
		update_proccess();
	}
}
void World::update_proccess(){
	const Time& t = update_time;
	const World& self = *this;
	foreach(Object::Ptr obj, objects){
		if(obj->isPickedup) continue;
		obj->action(self);
	}

	for(int i=0; i<(int)objects.size(); i++){
		Object::Ptr p = objects[i];
		if(p->isPickedup) continue;
		V2d speed = (p->speed*t.now()).rotated(p->pose.heading) + (p->speed_impuls*t.now());
		p->speed_impuls = p->speed_impuls*0.8;
		p->pose.location = p->pose.location+speed;
		p->pose.heading = angle(p->pose.heading + p->speed.ang());
	}

}


void World::draw(Mat& page){
	{boost::mutex::scoped_lock l(mtx);
		foreach(Object::Ptr p, objects){
			p->draw_init();
		}
	}

	foreach(Object::Ptr p, objects){
		p->draw(tf, page);
	}
}
