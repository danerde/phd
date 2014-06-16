/*
 * V2d.h
 *
 *  Created on: Jun 16, 2014
 *      Author: dan
 */

#ifndef V2D_H_
#define V2D_H_

#include "rmob_core.h"

using namespace std;
using namespace cv;

static const double d2r=0.0174532925;
static const double r2d=57.2957795;

class V2d{
public:
	double x,y;
	V2d(double x=0, double y=0);
	virtual ~V2d();

	double ang()const{return atan2(y,x);}
	double len()const{return hypot(x,y);}
	V2d operator+(const V2d& v)const{return V2d(x+v.x,y+v.y);}
	V2d operator-(const V2d& v)const{return V2d(x-v.x,y-v.y);}
	V2d operator*(const V2d& v)const{return V2d(x*v.x,y*v.y);}
	V2d operator*(const double& v)const{return V2d(x*v,y*v);}
	V2d operator/(const double& v)const{return V2d(x/v,y/v);}
	V2d scale(double fx, double fy)const{return V2d(x*fx,y*fy);}
	string str()const{ stringstream s;s<<"("<<x<<","<<y<<")"; return s.str(); }
	string str_polar()const{ stringstream s;s<<"("<<ang()<<","<<len()<<")"; return s.str(); }
	string str_polar_deg()const{ stringstream s;s<<"("<<ang()*r2d<<","<<len()*r2d<<")"; return s.str(); }
	static V2d polar(double ang, double len){ return V2d( len*cos(ang), len*sin(ang) ); }
	V2d rotated(double a)const{ return polar(ang()+a,len()); }

	double w()const{ return x; }
	double h()const{ return y; }
	int x_int()const{ return int(round(x)); }
	int y_int()const{ return int(round(y)); }
	int w_int()const{ return int(round(x)); }
	int h_int()const{ return int(round(y)); }

	operator cv::Vec2d()const{ return cv::Vec2d(x,y); }
	V2d(const cv::Vec2d& v):x(v[0]),y(v[1]){}
	operator cv::Point2d()const{ return cv::Point2d(x,y); }
	V2d(const cv::Point2d& v):x(v.x),y(v.y){}

	operator cv::Vec2i()const{ return cv::Vec2i(x_int(),y_int()); }
	V2d(const cv::Vec2i& v):x(v[0]),y(v[1]){}
	operator cv::Point2i()const{ return cv::Point2i(x_int(),y_int()); }
	V2d(const cv::Point2i& v):x(v.x),y(v.y){}

	bool in_range(double x1, double y1, double x2, double y2)const{
		return x1<=x and x<=x2 and y1<=y and y<=y2;
	}
};
typedef V2d Dimension;
inline
ostream& operator<<(ostream& o, const V2d& v){ return o<<v.str(); }

inline
double angle(double a){ return V2d(a, 1).ang(); }

inline
double angle_2PI(double _a){ double a = angle(_a); if(a>=0)return a; return (M_PI+a)+M_PI; }


#endif /* V2D_H_ */
