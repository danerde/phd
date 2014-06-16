
#include <rmob/rmob.h>



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
	World w;
	w.tf = Pose(V2d(ImageW/2,ImageH/2),0,1);

	for(int i=0;i<5;i++){
		Object::Ptr robot = Object::Ptr(new Robot(Pose(V2d(frand(-200,200),frand(-200,200)),0*d2r),10));
		robot->speed = V2d::polar(1.0*d2r,15);
		w.objects.push_back(robot);
	}

	int k=0;
	Time time;
	int duration = 30;
	while(k!=1310819 and k!=1048603){

		if(rand()%5==0){
			foreach(Robot::Ptr robot, w.objects)
				robot->speed = V2d::polar(frand(-5,5)*d2r,(rand()%50));
		}

		time = Time(duration);
		w.update(time);
		m.setTo(cv::Scalar(255,255,255));
		rectangle(m,Point(w.tf.location.x-200,w.tf.location.y-200),Point(w.tf.location.x+200,w.tf.location.y+200),cvScalar(0,0,0));
		w.draw(m);
		cv::flip(m,mr,0);
		cv::imshow("OK",mr);
		k = cv::waitKey(duration);
		if(k>0) cout<<"pressed: "<<k<<endl;
	}

	return 0;
}
