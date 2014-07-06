/*
 * Pack.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef PACK_H_
#define PACK_H_

#include "Object.h"

class Pack: public CicleObject{
public:
	typedef cv::Scalar COLOR;
private:
	Pose dr_pose;
	V2d dr_speed;
	double dr_size;
	COLOR dr_c;

public:
	COLOR c;
	bool used;

public:
	Pack(Pose p,double size);
	virtual void draw_init();
	virtual void draw(const Pose& tf, Mat& m)const;
	virtual void action(const World& wm);

	static boost::shared_ptr<Pack> getPtr(Ptr p){ return boost::shared_static_cast<Pack>(p); }
};

#endif /* PACK_H_ */
