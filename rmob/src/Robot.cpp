/*
 * Robot.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/Robot.h>
#include <rmob/World.h>

Robot::Robot(Pose p,double size):CicleObject(p,size, 10){

}

Robot::~Robot() {
}

void Robot::draw(const Pose& tf, Mat& m)const{
	V2d l = (pose.location)*tf.scale+tf.location;
	double r = pose.heading+tf.heading;
	double s = size*tf.scale;
	circle(m ,l, s, cvScalar(155, 155, 0),3);
	line(m, l, l+V2d::polar(r,s), cvScalar(155, 155, 0),3);
}


void Robot::think(const World& wm){
	const vector<Object::Ptr>& objects = wm.objects;
}

