/*
 * World.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/World.h>

World::World() {

}

World::~World() {
}

void World::update(const Time& t){

//	for(int i=0; i<(int)objects.size(); i++){
//		if(not objects[i]->pose.location.in_range(borderL,borderT,borderR,borderB)){
//			objects[i]->speed = V2d(0,0);
//			cout<<" stop A"<<endl;
//			V2d dir = (objects[i]->pose.location-V2d(0,0));
//			objects[i]->pose.location = objects[i]->pose.location - V2d::polar(dir.ang(),1);
//			continue;
//		}
//		for(int j=i+1; j<(int)objects.size(); j++){
//			if( objects[i]->distance(*objects[j])<1 ){
//				objects[i]->speed = V2d(0,0);
//				cout<<" stop B"<<endl;
//				objects[j]->speed = V2d(0,0);
//				cout<<" stop C"<<endl;
//				V2d dir = (objects[i]->pose.location-objects[j]->pose.location);
//				objects[i]->pose.location = objects[i]->pose.location + V2d::polar(dir.ang(),1);
//				objects[j]->pose.location = objects[j]->pose.location - V2d::polar(dir.ang(),1);
//				break;
//			}
//		}
//	}

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
	foreach(Object::Ptr p, objects){
		p->draw(tf, page);
	}
}
