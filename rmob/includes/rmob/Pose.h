/*
 * Pose.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef POSE_H_
#define POSE_H_

#include "V2d.h"

class Pose{
public:
	V2d location;
	double heading;
	double scale;
	Pose(V2d l=V2d(), double h=0, double s=1);
	~Pose();
};

#endif /* POSE_H_ */
