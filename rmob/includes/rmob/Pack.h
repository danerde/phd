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
	Pack(Pose p,double size);
	virtual void draw(const Pose& tf, Mat& m)const;
};

#endif /* PACK_H_ */
