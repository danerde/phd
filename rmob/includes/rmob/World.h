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
	void update(const Time& t);

	void draw(Mat& page);
};

#endif /* WORLD_H_ */
