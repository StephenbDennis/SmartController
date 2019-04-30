#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <iostream>
#include <math.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "world_br");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster tag_br;

  //Define value of PI
  double PI = atan(1) * 4;
  int timeout = 100;
  int count =0;
  while(n.ok()){

    if (count<timeout)
    {
        //TF of tag0 location w/ respect to map
      tf::Transform Tag0_pose;
      tf::Quaternion myQ0;
      Tag0_pose.setOrigin( tf::Vector3(0,0,0));
      myQ0.setRPY(0,PI/2,0);
      Tag0_pose.setRotation(myQ0);
      tag_br.sendTransform(
      tf::StampedTransform(Tag0_pose, ros::Time::now(), "map", "Tag12"));

  //TF of tag1 location w/ respect to map
      tf::Transform Tag1_pose;
      tf::Quaternion myQ1;
      Tag1_pose.setOrigin( tf::Vector3(0,0,2));
      myQ1.setRPY(PI/2,PI/2,0);
      Tag1_pose.setRotation(myQ1);
      tag_br.sendTransform(
        tf::StampedTransform(Tag1_pose, ros::Time::now(), "map", "Tag1"));

  //TF of tag2 location w/ respect to map
      tf::Transform Tag2_pose;
      tf::Quaternion myQ2;
      Tag2_pose.setOrigin( tf::Vector3(3.46,0.73,2));
      myQ2.setRPY(-PI/2,PI/2,0);
      Tag2_pose.setRotation(myQ2);
      tag_br.sendTransform(
        tf::StampedTransform(Tag2_pose, ros::Time::now(), "map", "Tag2"));

  //TF of tagX location w/ respect to map
      //tf::Transform TagX_pose;
      //tf::Quaternion myQX;
      //TagX_pose.setOrigin( tf::Vector3(0,0,0));
      //myQX.setRPY(0,0,0);
      //TagX_pose.setRotation(myQ0);
      //tag_br.sendTransform(
      // tf::StampedTransform(TagX_pose, ros::Time::now(), "map", "TagX"));

    }
    else
    {
      //TF of tag0 location w/ respect to map
      tf::Transform Tag0_pose;
      tf::Quaternion myQ0;
      Tag0_pose.setOrigin( tf::Vector3(1,2,2));
      myQ0.setRPY(0,PI/2,0);
      Tag0_pose.setRotation(myQ0);
      tag_br.sendTransform(
      tf::StampedTransform(Tag0_pose, ros::Time::now(), "map", "Tag0"));

    }
    
    //count++;


   r.sleep();
  }
}