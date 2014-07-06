/*
 * Object.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Pose.h"
//#include "World.h"

class World;

class Object{
public:
	typedef boost::shared_ptr<Object> Ptr;
	static int cID(){ static int c=0;return c++; }


	int id;
	int object_type;
	int phisical_type;
	Pose pose;
	int temporal_pt;
	V2d speed;
	V2d speed_impuls;
	bool isPickedup;
	double size;


	Pose _pose;
	V2d _speed;
	double _size;
	bool _isPickedup;

	Object(Pose p, int ot, int pt);
	virtual ~Object();


	virtual double distance(const Object& p)const=0;
	virtual void save_state();
	virtual void draw(const Pose& tf, Mat& m)const=0;
	virtual void action(const World& wm)=0;

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
	CicleObject(Pose p, double s, int pt);


	virtual double distance(const Object& p)const;
};

#endif /* OBJECT_H_ */
