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

class World{
public:
	vector<Object::Ptr> objects;
	Pose tf;
public:
	World();
	~World();
	void update(const Time& t){

		for(int i=0; i<(int)objects.size(); i++){
			if(not objects[i]->pose.location.in_range(-200,-200,200,200)){
				objects[i]->speed = V2d(0,0);
				continue;
			}
			for(int j=i+1; j<(int)objects.size(); j++){
				if( objects[i]->distance(*objects[j])<1 ){
					objects[i]->speed = V2d(0,0);
					objects[j]->speed = V2d(0,0);
					break;
				}
			}
		}

		for(int i=0; i<(int)objects.size(); i++){
			Object::Ptr p = objects[i];
			V2d speed = (p->speed*t.now()).rotated(p->pose.heading);
			p->pose.location = p->pose.location+speed;
			p->pose.heading = p->pose.heading + p->speed.ang();
		}

	}
	void draw(Mat& page){
		foreach(Object::Ptr p, objects){
			p->draw(tf, page);
		}
	}
};

#endif /* WORLD_H_ */
