#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "tag_fake_br");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster tag_fake_br;

  //Define value of PI
  double PI = atan(1) * 4;
  int timeout = 3;
  int count =0;
  while(n.ok()){

    if (count<timeout)
    {
      std::cout<<"Starting in "<<timeout-count<<std::endl;
      sleep(1);
        //TF of tag0 location w/ respect to map
      tf::Transform pose0_1;
      tf::Quaternion myQ0_1;
      pose0_1.setOrigin( tf::Vector3(0 ,0,0));
      myQ0_1.setRPY(0,PI/2,0);
      pose0_1.setRotation(myQ0_1);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose0_1, ros::Time::now(), "camera_1", "tag0_1"));
        //TF of tag0 location w/ respect to map
      tf::Transform pose1_1;
      tf::Quaternion myQ1_1;
      pose1_1.setOrigin( tf::Vector3(0 ,0,0));
      myQ1_1.setRPY(0,PI/2,0);
      pose1_1.setRotation(myQ1_1);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose1_1, ros::Time::now(), "camera_1", "tag1_1"));
          //TF of tag0 location w/ respect to map
      tf::Transform pose11_1;
      tf::Quaternion myQ11_1;
      pose11_1.setOrigin( tf::Vector3(0 ,0,0));
      myQ11_1.setRPY(0,PI/2,0);
      pose11_1.setRotation(myQ11_1);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose11_1, ros::Time::now(), "camera_1", "tag11_1"));
        //TF of tag0 location w/ respect to map
      tf::Transform pose12_1;
      tf::Quaternion myQ12_1;
      pose12_1.setOrigin( tf::Vector3(0 ,0,0));
      myQ12_1.setRPY(0,PI/2,0);
      pose12_1.setRotation(myQ12_1);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose12_1, ros::Time::now(), "camera_1", "tag12_1"));

  //TF of tag1 location w/ respect to map
      tf::Transform pose4_1;
      tf::Quaternion myQ4_1;
      pose4_1.setOrigin( tf::Vector3(0,0-.07,0));
      myQ4_1.setRPY(PI/2,PI/2,0);
      pose4_1.setRotation(myQ4_1);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose4_1, ros::Time::now(), "camera_1", "tag4_1"));

  //TF of tag2 location w/ respect to map
      tf::Transform pose5_1;
      tf::Quaternion myQ5_1;
      pose5_1.setOrigin( tf::Vector3(0,0,0));
      myQ5_1.setRPY(-PI/2,PI/2,0);
      pose5_1.setRotation(myQ5_1);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose5_1, ros::Time::now(), "camera_1", "tag5_1"));
  //TF of tag2 location w/ respect to map
      tf::Transform pose6_1;
      tf::Quaternion myQ6_1;
      pose6_1.setOrigin( tf::Vector3(0,0,0));
      myQ6_1.setRPY(-PI/2,PI/2,0);
      pose6_1.setRotation(myQ6_1);
      tag_fake_br.sendTransform(
      tf::StampedTransform(pose6_1, ros::Time::now(), "camera_1", "tag6_1"));
        //TF of tag0 location w/ respect to map
      tf::Transform pose0_2;
      tf::Quaternion myQ0_2;
      pose0_2.setOrigin( tf::Vector3(0 ,0,0));
      myQ0_2.setRPY(0,PI/2,0);
      pose0_2.setRotation(myQ0_2);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose0_2, ros::Time::now(), "camera_2", "tag0_2"));
  //TF of tag0 location w/ respect to map
      tf::Transform pose1_2;
      tf::Quaternion myQ1_2;
      pose1_2.setOrigin( tf::Vector3(0 ,0,0));
      myQ1_2.setRPY(0,PI/2,0);
      pose1_2.setRotation(myQ1_2);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose1_2, ros::Time::now(), "camera_2", "tag1_2"));
  //TF of tag0 location w/ respect to map
      tf::Transform pose11_2;
      tf::Quaternion myQ11_2;
      pose11_2.setOrigin( tf::Vector3(0 ,0,0));
      myQ11_2.setRPY(0,PI/2,0);
      pose11_2.setRotation(myQ11_2);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose11_2, ros::Time::now(), "camera_2", "tag11_2")); 
  //TF of tag0 location w/ respect to map
      tf::Transform pose12_2;
      tf::Quaternion myQ12_2;
      pose12_2.setOrigin( tf::Vector3(0 ,0,0));
      myQ12_2.setRPY(0,PI/2,0);
      pose12_2.setRotation(myQ12_2);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose12_2, ros::Time::now(), "camera_2", "tag12_2"));

  //TF of tag1 location w/ respect to map
      tf::Transform pose4_2;
      tf::Quaternion myQ4_2;
      pose4_2.setOrigin( tf::Vector3(0,0-.07,0));
      myQ4_2.setRPY(PI/2,PI/2,0);
      pose4_2.setRotation(myQ4_2);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose4_2, ros::Time::now(), "camera_2", "tag4_2"));

  //TF of tag2 location w/ respect to map
      tf::Transform pose5_2;
      tf::Quaternion myQ5_2;
      pose5_2.setOrigin( tf::Vector3(0,0,0));
      myQ5_2.setRPY(-PI/2,PI/2,0);
      pose5_2.setRotation(myQ5_2);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose5_2, ros::Time::now(), "camera_2", "tag5_2"));
  //TF of tag2 location w/ respect to map
      tf::Transform pose6_2;
      tf::Quaternion myQ6_2;
      pose6_2.setOrigin( tf::Vector3(0,0,0));
      myQ6_2.setRPY(-PI/2,PI/2,0);
      pose6_2.setRotation(myQ6_2);
      tag_fake_br.sendTransform(
        tf::StampedTransform(pose6_2, ros::Time::now(), "camera_2", "tag6_2"));
  //TF of tagX location w/ respect to map
      //tf::Transform TagX_pose;
      //tf::Quaternion myQX;
      //TagX_pose.setOrigin( tf::Vector3(0,0,0));
      //myQX.setRPY(0,0,0);
      //TagX_pose.setRotation(myQ0);
      //tag_fake_br.sendTransform(
      // tf::StampedTransform(TagX_pose, ros::Time::now(), "camera_1", "tagX"));

    }
    else
    {
  //     double rVal1=((double)(rand() % 10))/100;
  //     double rVal2=((double)(rand() % 10))/100;
  //     double rVal3=((double)(rand() % 10))/100;
  //     double rVal4=((double)(rand() % 10))/100;

  //       //TF of tag0 location w/ respect to map
  //     tf::Transform pose12_1;
  //     tf::Quaternion myQ12_1;
  //     pose12_1.setOrigin( tf::Vector3(0.5+rVal1,-1,2));
  //     myQ12_1.setRPY(0,0,PI);
  //     pose12_1.setRotation(myQ12_1);
  //     tag_fake_br.sendTransform(
  //       tf::StampedTransform(pose12_1, ros::Time::now(), "camera_1", "tag12_1"));

  // //TF of tag1 location w/ respect to map
  //     tf::Transform pose4_1;
  //     tf::Quaternion myQ4_1;
  //     pose4_1.setOrigin( tf::Vector3(1.5+rVal2,1.5,2));
  //     myQ4_1.setRPY(0,PI,0);
  //     pose4_1.setRotation(myQ4_1);
  //     tag_fake_br.sendTransform(
  //       tf::StampedTransform(pose4_1, ros::Time::now(), "camera_1", "tag4_1"));
          
  // //TF of tag1 location w/ respect to map
  //     tf::Transform pose4_2;
  //     tf::Quaternion myQ4_2;
  //     pose4_2.setOrigin( tf::Vector3(-2+rVal3,-3.5,4));
  //     myQ4_2.setRPY(0,PI,0);
  //     pose4_2.setRotation(myQ4_2);
  //     tag_fake_br.sendTransform(
  //       tf::StampedTransform(pose4_2, ros::Time::now(), "camera_2", "tag4_2"));

  // //TF of tag2 location w/ respect to map
  //     tf::Transform pose5_2;
  //     tf::Quaternion myQ5_2;
  //     pose5_2.setOrigin( tf::Vector3(2.5+rVal4,-3,4));
  //     myQ5_2.setRPY(0,PI,0);
  //     pose5_2.setRotation(myQ5_2);
  //     tag_fake_br.sendTransform(
  //       tf::StampedTransform(pose5_2, ros::Time::now(), "camera_2", "tag5_2"));

    }
    
    count++;


   r.sleep();
  }
}