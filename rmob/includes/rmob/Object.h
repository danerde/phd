/*
 * Object.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Pose.h"

class Object{
public:
	typedef boost::shared_ptr<Object> Ptr;


	int object_type;
	int phisical_type;
	Pose pose;
	int temporal_pt;
	V2d speed;


	Object(Pose p, int ot, int pt);
	virtual ~Object();


	virtual double distance(const Object& p)const=0;
	virtual void draw(const Pose& tf, Mat& m)const=0;
};

/*** DEPRICATED ***/
//class RectanguliarObject:public Object{
//public:
//	Dimension size;
//	RectanguliarObject(Pose p, Dimension s, int pt):Object(p, 1, pt),size(s){}
//	virtual double distance(const Object& p)const;
//};

class CicleObject:public Object{
public:
	double size;
	CicleObject(Pose p, double s, int pt);


	virtual double distance(const Object& p)const;
};

#endif /* OBJECT_H_ */