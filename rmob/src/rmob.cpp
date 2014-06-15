//============================================================================
// Name        : rmob.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

#define foreach BOOST_FOREACH

using namespace std;
using namespace cv;

const double d2r=0.0174532925;
const double r2d=57.2957795;
class V2d{
public:
	double x,y;
	V2d(double x=0, double y=0):x(x),y(y){}
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
};
typedef V2d Dimension;
ostream& operator<<(ostream& o, const V2d& v){ return o<<v.str(); }

double angle(double a){ return V2d(a, 1).ang(); }
double angle_2PI(double _a){ double a = angle(_a); if(a>=0)return a; return (M_PI+a)+M_PI; }

class Pose{
public:
	V2d location;
	double heading;
	double scale;
	Pose(V2d l=V2d(), double h=0, double s=1):location(l),heading(h),scale(s){}
};
class Object{
public:
	typedef boost::shared_ptr<Object> Ptr;
	int object_type;
	int phisical_type;
	Pose pose;
	int temporal_pt;
	V2d speed;
	Object(Pose p, int ot, int pt):object_type(ot),phisical_type(pt),pose(p),temporal_pt(pt){}
	virtual ~Object(){}
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
	CicleObject(Pose p, double s, int pt):Object(p, 2, pt),size(s){}
	virtual double distance(const Object& p)const{
		if(p.object_type == object_type){
			const CicleObject& o = (const CicleObject&)p;
			return (pose.location-o.pose.location).len()-size-o.size;
		}
		return 0;
	}
};

class Robot: public CicleObject{
public:
	Robot(Pose p,double size):CicleObject(p,size, 10){

	}

	virtual void draw(const Pose& tf, Mat& m)const{
		V2d l = (pose.location)*tf.scale+tf.location;
		double r = pose.heading+tf.heading;
		double s = size*tf.scale;
		circle(m ,l, s, cvScalar(155, 155, 0),3);
		line(m, l, l+V2d::polar(r,s), cvScalar(155, 155, 0),3);
	}
};

class Pack: public CicleObject{
public:
	Pack(Pose p,double size):CicleObject(p,size, 5){

	}
	virtual void draw(const Pose& tf, Mat& m)const{
		V2d l = (pose.location)*tf.scale+tf.location;
		double r = pose.heading+tf.heading;
		double s = size*tf.scale;
		circle(m ,l, s, cvScalar(0, 155, 0),2);
		line(m, l, l+V2d::polar(r,s), cvScalar(0, 155, 0),2);
	}
};

class Time{
public:
	double t;
	Time(double t=0):t(t){}
	void tick(double msec){ t+=msec; }
	double now()const{ return t/1000.0; }
};

class TH{
public:
	typedef boost::shared_ptr<TH> Ptr;
	bool stop;
	boost::mutex m;
	boost::condition_variable v;
	vector<Object::Ptr> obj;
	Pose* tf;
	Mat* page;
	boost::mutex* gm;
	int* gv;
	boost::condition_variable* gs;
	TH():stop(false),tf(0),page(0),gm(0),gv(0),gs(0){}
//	TH(const TH& t):stop(t.stop),tf(t.tf),page(t.page),gm(t.gm),gv(t.gv){}
	virtual ~TH(){}
};

void thread_draw(TH* th){
	boost::mutex::scoped_lock l(th->m);
	while(!th->stop){
		th->v.wait(l);
		if(th->stop) break;
		foreach(Object::Ptr obj, th->obj){
			obj->draw(*(th->tf), *(th->page));
		}
		{ boost::mutex::scoped_lock ll(*(th->gm)); (*(th->gv))--; if((*(th->gv))==0)th->gs->notify_one(); }
	}
}

#define THREAD_NUMBER 1
class World{
public:
	vector<Object::Ptr> objects;
	Pose tf;
#	if(THREAD_NUMBER==1)
#	else
	private:
		boost::thread_group threads;
		boost::mutex gm;
		boost::condition_variable gs;
		int gv;
		std::vector<TH::Ptr> ths;
#	endif
public:
	World(){
#		if(THREAD_NUMBER==1)
#		else
			for(size_t i=0;i<THREAD_NUMBER;i++){
				ths.push_back(TH::Ptr(new TH()));
				ths[i]->gm = &gm;
				ths[i]->gv = &gv;
				ths[i]->tf = &tf;
				ths[i]->gs = &gs;
				threads.add_thread(new boost::thread(boost::bind(thread_draw, ths[i].get())));
			}
#		endif
	}
	~World(){
#		if(THREAD_NUMBER==1)
#		else
			foreach(TH::Ptr t, ths){
				t->stop = false;
				t->v.notify_one();
			}
			threads.join_all();
#		endif
	}
	void update(const Time& t){
		foreach(Object::Ptr p, objects){
			V2d speed = (p->speed*t.now()).rotated(p->pose.heading);
			p->pose.location = p->pose.location+speed;
			p->pose.heading = p->pose.heading + p->speed.ang();
		}

	}
	void draw(Mat& page){
#		if(THREAD_NUMBER==1)
		foreach(Object::Ptr p, objects){
			p->draw(tf, page);
		}
#		else
		{
			boost::mutex::scoped_lock l(gm);
			int n = threads.size()-1;
			int c = objects.size()/n;
			int r = objects.size() - c*n;
			int o = 0;
			for(int i=0;i<n;i++){
				ths[i]->obj.clear();
				for(int j=0;j<c;j++){
					ths[i]->obj.push_back(objects[o++]);
					ths[i]->page = &page;
				}
			}
			for(int j=0;j<r;j++){
				ths[n]->obj.push_back(objects[o++]);
				ths[n]->page = &page;
			}
			gv=threads.size();
			foreach(TH::Ptr t, ths){
				t->v.notify_one();
			}
			gs.wait(l);
		}
#		endif
	}
};

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void my_handler(int s){
   printf("Caught signal %d\n",s);
   exit(1);
}

double frand(double s, double e){
	return (rand()%1000/1000.0)*(e-s)+s;
}


int main() {

	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);


	const int ImageW=1000,ImageH=1000;
	cv::Mat m(ImageH,ImageW, CV_8UC3, cvScalar(255,255,255));
	cv::Mat mr(ImageH,ImageW, CV_8UC3, cvScalar(255,255,255));
	line(m,Point(0,0),Point(0,500),cvScalar(0,0,0));
	line(m,Point(0,0),Point(500,0),cvScalar(0,0,0));
	World w;
	w.tf = Pose(V2d(ImageW/2,ImageH/2),0,2);

	for(int i=0;i<1000;i++){
		Object::Ptr robot = Object::Ptr(new Robot(Pose(V2d(0,0),0*d2r),10));
		robot->speed = V2d::polar(1.0*d2r,15);
		w.objects.push_back(robot);
	}

	int k=0;
	Time time;
	int duration = 30;
	while(k!=1310819 and k!=1048603){

		if(rand()%5==0){
			foreach(Robot::Ptr robot, w.objects)
				robot->speed = V2d::polar(frand(-5,5)*d2r,(rand()%20));
		}

		time = Time(duration);
		w.update(time);
		m.setTo(cv::Scalar(255,255,255));
		w.draw(m);
		cv::flip(m,mr,0);
		cv::imshow("OK",mr);
		k = cv::waitKey(duration);
		if(k>0) cout<<"pressed: "<<k<<endl;
	}

	return 0;
}
