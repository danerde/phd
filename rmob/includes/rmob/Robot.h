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
	Robot(Pose p,double size);
	virtual ~Robot();

	virtual void draw(const Pose& tf, Mat& m)const;
	virtual void think(const World& wm);
};

#endif /* ROBOT_H_ */
