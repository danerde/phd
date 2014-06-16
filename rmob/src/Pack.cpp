/*
 * Pack.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#include <rmob/Pack.h>

Pack::Pack(Pose p,double size):CicleObject(p,size, 5){

}


void Pack::draw(const Pose& tf, Mat& m)const{
	V2d l = (pose.location)*tf.scale+tf.location;
	double r = pose.heading+tf.heading;
	double s = size*tf.scale;
	circle(m ,l, s, cvScalar(0, 155, 0),2);
	line(m, l, l+V2d::polar(r,s), cvScalar(0, 155, 0),2);
}
