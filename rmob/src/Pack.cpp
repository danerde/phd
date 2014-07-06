/*
 * Pack.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/Pack.h>
#include <rmob/World.h>


namespace {
const Pack::COLOR RED= cvScalar(0, 0, 255);
const Pack::COLOR REG= cvScalar(0, 155, 155);
const Pack::COLOR GREEN= cvScalar(0, 255, 0);
}

Pack::Pack(Pose p,double size):CicleObject(p,size, 5){
	c = REG;
	used = false;

	dr_size=0;
}

void Pack::draw_init(){
	dr_pose= pose;
	dr_speed= speed;
	dr_size=size;
	dr_c=c;
}
void Pack::draw(const Pose& tf, Mat& m)const{
	V2d l = (dr_pose.location)*tf.scale+tf.location;
	double r = dr_pose.heading+tf.heading;
	double rr = dr_speed.ang()+tf.heading;
	double s = dr_size*tf.scale;
	double ss = dr_speed.len()*tf.scale;
	circle(m ,l, s, dr_c,2);
	line(m, l, l+V2d::polar(r,s), dr_c,2);
	line(m, l, l+V2d::polar(rr+r,ss), GREEN,1);
}

void Pack::action(const World& wm){
	const vector<Object::Ptr>& objects = wm.objects;
	c = REG;
	V2d heading = V2d::polar(this->pose.heading,1);
	foreach(Object::Ptr obj, objects){
		if(obj.get()==this) continue;
		V2d dir = pose.location - obj->pose.location;
		if( dir.len() >= (size+obj->size)+5 ) continue;
		speed_impuls = speed_impuls + V2d::polar(dir.ang(),obj->speed.len()) / (11-obj->phisical_type);
		double a = dir.ang()-heading.ang();
		a = angle(a);
		if( fabs(a) < M_PI_2 ) continue;
		dir = dir*-1;
		dir = dir.rotated(a);
		pose.heading = dir.ang();
		c = RED;
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
			double ref = M_PI - pose.heading;
			pose.heading = angle(ref);
		}
		if(dr < dl and dr < dt and dr < db and (M_PI_2>fabs(pose.heading))){
			double ref = M_PI - pose.heading;
			pose.heading = angle(ref);
		}
		if(dt < dr and dt < dl and dt < db and pose.heading>0){
			double ref = ((M_PI - (pose.heading-M_PI_2))+M_PI_2);
			pose.heading = angle(ref);
		}
		if(db < dr and db < dt and db < dl and pose.heading<0){
			double ref = ((M_PI - (pose.heading-M_PI_2))+M_PI_2);
			pose.heading = angle(ref);
		}
	}
}



