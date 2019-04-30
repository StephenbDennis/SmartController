#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
  ros::init(argc, argv, "world");
  ros::NodeHandle n;

  srand (time(NULL)); 

  ros::Rate r(30);

  tf::TransformBroadcaster br;

  //Define value of PI
  double PI = atan(1) * 4;
  int count = 0;
  double elevation  = 0;
  while(n.ok()){


    
    tf::Transform pose;
    tf::Quaternion myQ0;
    pose.setOrigin( tf::Vector3(0,0,elevation));
    myQ0.setRPY(0,0,0);
    pose.setRotation(myQ0);
    br.sendTransform(
    tf::StampedTransform(pose, ros::Time::now(), "robot", "elevation"));

  if(count > 30)
  {
    count = 0;
    elevation +=(double)(rand() %100 +1)/100;

  }

  cout<<"elevation: "<<elevation<<endl;
  count++;
   r.sleep();
  }
}