/*
 * Robot.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "Object.h"

class Robot: public CicleObject{

public:
	typedef cv::Scalar COLOR;
private:
	Pose dr_pose;
	V2d dr_speed;
	double dr_size;
	COLOR dr_c;

public:
	COLOR c;
	vector<Object::Ptr> picked;

public:
	Robot(Pose p,double size);
	virtual ~Robot();

	virtual void draw_init();
	virtual void draw(const Pose& tf, Mat& m)const;
	virtual void action(const World& wm);
	virtual void think(const World& wm);

	static boost::shared_ptr<Robot> getPtr(Ptr p){ return boost::shared_static_cast<Robot>(p); }
};

#endif /* ROBOT_H_ */
