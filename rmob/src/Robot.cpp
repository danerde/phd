/*
 * Robot.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/Robot.h>
#include <rmob/World.h>
#include <rmob/Pack.h>

namespace {
const Robot::COLOR RED= cvScalar(0, 0, 255);
const Robot::COLOR REG= cvScalar(155, 155, 0);
const Robot::COLOR GREEN= cvScalar(0, 255, 0);

}

Robot::Robot(Pose p,double size):CicleObject(p,size, 10){
	c = REG;
}

Robot::~Robot() {
}

void Robot::save_state(){
	Object::save_state();
	_c=c;
}

void Robot::draw(const Pose& tf, Mat& m)const{
	V2d l = (_pose.location)*tf.scale+tf.location;
	double r = _pose.heading+tf.heading;
	double rr = _speed.ang()+tf.heading;
	double s = _size*tf.scale;
	double ss = _speed.len()*tf.scale;
	circle(m ,l, s, _c,3);
	line(m, l, l+V2d::polar(r,s), _c,3);
	line(m, l, l+V2d::polar(rr+r,ss), GREEN,1);
}


void Robot::think(const World& wm){
	speed=speed.normal()*115;
	int rand_t = rand()%100;
	if( rand_t == 0 ){
		if( rand()%10 <5 ){
			speed = speed.rotated(1*d2r);
		}else{
			speed = speed.rotated(-1*d2r);
		}
		if(fabs(speed.ang())>10*d2r) speed = speed.rotated(-speed.ang());
	}

	V2d s = V2d::polar(pose.heading, size);
	V2d nloc = pose.location + s;
	if(not nloc.in_range(wm.borderL+size,wm.borderB+size,wm.borderR-size,wm.borderT-size) ){
		speed = speed.rotated(15*d2r);
	}else if(rand_t>70){
		speed = speed.setAng(0);
	}

	foreach(Object::Ptr obj, wm.objects){
		if(obj->phisical_type != 5)continue;
		if(obj->_isPickedup)continue;
		if(Pack::getPtr(obj)->_used)continue;
		V2d to_obj = (obj->_pose.location - pose.location).rotated(-pose.heading);
		if(to_obj.len() < (size+obj->size)*1.5)
			if(fabs(to_obj.ang())<20*d2r){
				picked.push_back(obj);
				obj->_isPickedup=true;
				Pack::getPtr(obj)->used=true;//access
				obj->speed = V2d();//access
		}
	}

	if( pose.location.len() < 30 ){
		foreach(Object::Ptr obj, picked){
			obj->isPickedup=false;//access
		}
		picked.clear();
	}

}

void Robot::action(const World& wm){
	foreach(Object::Ptr obj, picked){
		obj->pose.location = pose.location + V2d::polar(pose.heading, size+obj->size);
	}

	think(wm);

	const vector<Object::Ptr>& objects = wm.objects;
	c = REG;
	V2d heading = V2d::polar(this->pose.heading,1);
	foreach(Object::Ptr obj, objects){
		if(obj.get()==this) continue;
		if(obj->phisical_type<phisical_type)continue;
		V2d dir = pose.location - obj->pose.location;
		if( dir.len() < 5+(size+obj->size) ){
			double a = dir.ang()-heading.ang();
			a = angle(a);
			if( fabs(a) < M_PI_2 ) continue;
//			dir = dir*-1;
//			dir = dir.rotated(a);
//			pose.heading = dir.ang();
			c = RED;
			//speed=speed.normal()*0.1;
		}
	}
	double borderL = wm.borderL;
	double borderR = wm.borderR;
	double borderT = wm.borderT;
	double borderB = wm.borderB;
	if(pose.location.x<borderL+size or pose.location.x>borderR-size or pose.location.y<borderB+size or pose.location.y>borderT-size){
		double dl = (pose.location.x-borderL);
		double dr = (borderR-pose.location.x);
		double dt = (borderT-pose.location.y);
		double db = (pose.location.y-borderB);
		if(dl < dr and dl < dt and dl < db and (M_PI_2<fabs(pose.heading))){
//			double ref = M_PI - pose.heading;
//			pose.heading = angle(ref);
			speed = speed.normal()*0.1;
		}else
		if(dr < dl and dr < dt and dr < db and (M_PI_2>fabs(pose.heading))){
//			double ref = M_PI - pose.heading;
//			pose.heading = angle(ref);
			speed = speed.normal()*0.1;
		}else
		if(dt < dr and dt < dl and dt < db and pose.heading>0){
//			double ref = ((M_PI - (pose.heading-M_PI_2))+M_PI_2);
//			pose.heading = angle(ref);
			speed = speed.normal()*0.1;
		}else
		if(db < dr and db < dt and db < dl and pose.heading<0){
//			double ref = ((M_PI - (pose.heading-M_PI_2))+M_PI_2);
//			pose.heading = angle(ref);
			speed = speed.normal()*0.1;
		}
	}

}

