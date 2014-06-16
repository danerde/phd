/*
 * Time.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef TIME_H_
#define TIME_H_

class Time{
public:
	double t;
	Time(double t=0);
	~Time();
	void tick(double msec){ t+=msec; }
	double now()const{ return t/1000.0; }
};

#endif /* TIME_H_ */
