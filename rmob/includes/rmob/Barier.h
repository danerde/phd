#ifndef ____Barier___H_
#define ____Barier___H_

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

class Barier{

public:

	boost::mutex m;
	boost::condition_variable c;
	int var;

	Barier(int v=0):var(v){}
	void set(int v){
		boost::mutex::scoped_lock l(m);
		var=v;
	}
	void wait(boost::mutex::scoped_lock& ll){
		ll.unlock();
		boost::mutex::scoped_lock l(m);
		while(boost::this_thread::interruption_requested()==false and var>0){
			c.wait(l);
		}
		ll.lock();
	}
	void notify(){
		boost::mutex::scoped_lock l(m);
		var--;
		c.notify_one();
	}
};









#endif


