/*
 * Object.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/Object.h>

Object::Object(Pose p, int ot, int pt):
	object_type(ot),
	phisical_type(pt),pose(p),temporal_pt(pt)
{}

Object::~Object() {  }

CicleObject::CicleObject(Pose p, double s, int pt):Object(p, 2, pt),size(s){}

double CicleObject::distance(const Object& p)const{
	if(p.object_type == object_type){
		const CicleObject& o = (const CicleObject&)p;
		return (pose.location-o.pose.location).len()-size-o.size;
	}
	return 0;
}











