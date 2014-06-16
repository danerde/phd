/*
 * ThreadPool.h
 *
 *  Created on: Jun 15, 2014
 *      Author: dan
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_


#include <iostream>
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
#include <boost/function.hpp>

using namespace std;

class ThreadPool {
public:
	class G{
	public:
		boost::mutex gm;
		boost::condition_variable gsig;
		int gv;
	};
	class ThreadInfo{
	public:
		typedef boost::shared_ptr<ThreadInfo> ptr;
		bool stop;
		boost::mutex m;
		boost::condition_variable sig;
		G& g;
		boost::function<void(void)> run;
		ThreadInfo(G& g):stop(false),g(g){}
	};

public:
	boost::thread_group threads;
	std::vector<ThreadInfo::ptr> infos;
	G g;

	void run(ThreadInfo::ptr info){
		boost::mutex::scoped_lock l(info->m);
		while(not info->stop and not boost::this_thread::interruption_requested()){
			cout<<"wait"<<endl;
			info->sig.wait(l);
			cout<<"react"<<endl;
			if(info->stop or boost::this_thread::interruption_requested()) return;
			info->run();
			{
				boost::mutex::scoped_lock ll(info->g.gm);
				info->g.gv--;
				if(info->g.gv<=0)info->g.gsig.notify_one();
			}
		}
	}

	void create_thread(){
		ThreadInfo::ptr t( new ThreadInfo(g) );
		infos.push_back(t);
		threads.add_thread(
				new boost::thread(
						boost::bind(
								&ThreadPool::run,this,
								t
						)
				)
		);
		//cout<<"thread created"<<endl;
	}
	void stop_thread(int i){
		boost::mutex::scoped_lock l(infos[i]->m);
		infos[i]->stop=true;
		infos[i]->sig.notify_one();
	}
	void run_thread(int i){
		boost::mutex::scoped_lock l(infos[i]->m);
		infos[i]->stop=false;
		infos[i]->sig.notify_one();
		//cout<<"run it"<<endl;
	}
	void setRunner(int i, boost::function<void(void)> runner){
		infos[i]->run = runner;
	}
	void stop_all(){
		threads.interrupt_all();
		for(int i=0;i<(int)threads.size();i++){
			stop_thread(i);
		}
		threads.join_all();
	}
	void run_all(){
		boost::mutex::scoped_lock l(g.gm);
		g.gv = (int) threads.size();
		for(int i=0;i<(int)threads.size();i++){
			run_thread(i);
		}
		g.gsig.wait(l);
	}

	size_t size()const{ return threads.size(); }

	ThreadPool(int n);
	virtual ~ThreadPool();
};

#endif /* THREADPOOL_H_ */
