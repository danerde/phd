/*
 * Object.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/Object.h>
#include <rmob/World.h>

Object::Object(Pose p, int ot, int pt):
	id(cID()),
	object_type(ot),
	phisical_type(pt),pose(p),temporal_pt(pt),isPickedup(false)
{}

Object::~Object() {  }

void Object::save_state(){
	_pose= pose;
	_speed= speed;
	_size=size;
	_isPickedup=isPickedup;
}


CicleObject::CicleObject(Pose p, double s, int pt):Object(p, 2, pt){size=(s);}

double CicleObject::distance(const Object& p)const{
	if(p.object_type == object_type){
		const CicleObject& o = (const CicleObject&)p;
		return (pose.location-o.pose.location).len()-size-o.size;
	}
	return 0;
}











