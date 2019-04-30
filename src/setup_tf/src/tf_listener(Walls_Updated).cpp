//A Simple Listener for the /tf topic.
//Assumes that the fist tag in the list is at point(0,0,0), and oriientation(0,0,0)
//Output is in the 1st tags frame
//Made by Stephen Dennis 01/26/2019

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <math.h>

using namespace std;

struct dof6 {
	double x;
	double y;
	double z;
	double yaw;
	double pitch;
	double roll;
};

double PI = atan(1) * 4;

double t_yaw[3][3];
double t_pitch[3][3];
double t_roll[3][3];

dof6 t_form(dof6);
void set_a(double);
void set_b(double);
void set_c(double);
dof6 createPos(double, double, double, double, double, double, double);


int main(int argc, char** argv){
  ros::init(argc, argv, "tf_listener");

  ros::NodeHandle node;

  
  tf::TransformListener listener;
  tf::TransformBroadcaster map_br;
  ros::Rate rate(30.0);

  int tagNum =1;
  string frame = "camera_1";
  string tags[]={"tag12_1"};//,"tag3"};
  string fixed_tags[]={"Tag12"};
  long unsigned int timer[tagNum];
  dof6 old[1];
  dof6 pos[1];
 

 for (int i=0;i<tagNum;i++)
 {
	timer[i]=0;
 }

  while (node.ok()){

	tf::StampedTransform t_forms[tagNum];
	tf::StampedTransform fixed[tagNum];

    try{

		//have listener get the tags value
		for (int i = 0; i < tagNum; i++)
		{
			listener.lookupTransform("map", fixed_tags[i], ros::Time(0), fixed[i]);
			listener.lookupTransform(frame, tags[i], ros::Time(0), t_forms[i]);

		}

		dof6 values[tagNum];
		dof6 t_values[tagNum];
		
		for (int i = 0; i < tagNum; i++)
		{
			values[i] = createPos(
        		t_forms[i].getOrigin().x(), t_forms[i].getOrigin().y(), t_forms[i].getOrigin().z(),
        		t_forms[i].getRotation().x(),t_forms[i].getRotation().y(), t_forms[i].getRotation().z(), t_forms[i].getRotation().w());
			pos[i] = createPos(
        		fixed[i].getOrigin().x(), fixed[i].getOrigin().y(), fixed[i].getOrigin().z(),
        		fixed[i].getRotation().x(),fixed[i].getRotation().y(), fixed[i].getRotation().z(), fixed[i].getRotation().w());
		}

		for (int i = 0; i < tagNum; i++)
		{
			
			if(values[i].x!=old[i].x)
			{
				timer[i]=0;
			}

		}

		bool cont =false;
		int val =-1;
		double min =100;

		for(int i=0;i<tagNum;i++)
		{
			if(timer[i]<30)
			{
				// double dist=sqrt(pow(values[i].x,2)+pow(values[i].y,2)+pow(values[i].z,2));
				// if(dist<min)
				// {
				// 	min=dist;
				// 	val=i;
				// 	cont=true;
				// }
				val=i;
				cont=true;
			}
		}

		if(cont)
		{
			//set transform matrix for each axis to allign it with the first tag
			set_a(values[val].roll + pos[val].roll);
			set_b(values[val].yaw + pos[val].yaw);	
			set_c(values[val].pitch+pos[val].pitch);

			//transform all tags
			for (int i=0; i<tagNum; i++)
			{
				t_values[i] = t_form(values[i]);
				old[i].x=values[i].x;
			}
			ROS_INFO("\n %s:\n(%.3f, %.3f, %.3f)\n(%.3f, %.3f, %.3f)",tags[val].c_str(), (pos[val].z - t_values[val].z), (pos[val].y - t_values[val].y), (pos[val].x + t_values[val].x),
			(t_values[val].roll), (t_values[val].yaw), (t_values[val].pitch ));
			//broadcast
			tf::Transform turtle1;
			tf::Quaternion myQ0;
			turtle1.setOrigin( tf::Vector3((pos[val].z - t_values[val].z), (pos[val].y - t_values[val].y), (pos[val].x + t_values[val].x)));
			myQ0.setRPY((t_values[val].roll), (t_values[val].yaw), (t_values[val].pitch ));
			turtle1.setRotation(myQ0);
			map_br.sendTransform(
			tf::StampedTransform(turtle1, ros::Time::now(), "map", "turtle1"));

		}

		for (int i=0;i<tagNum;i++)
		{
		    old[i].x=values[i].x;
			timer[i]++;	
		}
	}
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

	// tf::TransformBroadcaster broadcaster;
  	// tf::Transform tran;
  	// tran.setOrigin(tf::Vector3(pos[1].x, pos[1].y, pos[1].z));
  	// tf::Quaternion myQ;
  	// myQ.setRPY(pos[1].yaw,pos[1].pitch,pos[1].roll);
  	// tran.setRotation(myQ);
	// broadcaster.sendTransform(tf::StampedTransform(tran,ros::Time::now(),"world", "Tag1"));

    rate.sleep();
  }
  return 0;
};

dof6 t_form(dof6 loc)
{
	dof6 newLoc;
	dof6 tempLoc;
	//a transform
	tempLoc.x = (loc.x*t_yaw[0][0] + loc.y*t_yaw[1][0] + loc.z*t_yaw[2][0]);
	tempLoc.y = (loc.x*t_yaw[0][1] + loc.y*t_yaw[1][1] + loc.z*t_yaw[2][1]);
	tempLoc.z = (loc.x*t_yaw[0][2] + loc.y*t_yaw[1][2] + loc.z*t_yaw[2][2]);


	newLoc = tempLoc;
	//b transform
	tempLoc.x = (newLoc.x*t_pitch[0][0] + newLoc.y*t_pitch[1][0] + newLoc.z*t_pitch[2][0]);
	tempLoc.y = (newLoc.x*t_pitch[0][1] + newLoc.y*t_pitch[1][1] + newLoc.z*t_pitch[2][1]);
	tempLoc.z = (newLoc.x*t_pitch[0][2] + newLoc.y*t_pitch[1][2] + newLoc.z*t_pitch[2][2]);


	newLoc = tempLoc;
	//c transform
	tempLoc.x = (newLoc.x*t_roll[0][0] + newLoc.y*t_roll[1][0] + newLoc.z*t_roll[2][0]);
	tempLoc.y = (newLoc.x*t_roll[0][1] + newLoc.y*t_roll[1][1] + newLoc.z*t_roll[2][1]);
	tempLoc.z = (newLoc.x*t_roll[0][2] + newLoc.y*t_roll[1][2] + newLoc.z*t_roll[2][2]);

	tempLoc.yaw=loc.yaw;
	tempLoc.pitch=loc.pitch;
	tempLoc.roll=loc.roll;


	return tempLoc;
}
void set_a(double ang)
{
	t_yaw[0][0] = cos(ang);
	t_yaw[0][1] = -sin(ang);
	t_yaw[0][2] = 0;

	t_yaw[1][0] = sin(ang);
	t_yaw[1][1] = cos(ang);
	t_yaw[1][2] = 0;

	t_yaw[2][0] = 0;
	t_yaw[2][1] = 0;
	t_yaw[2][2] = 1;
}
void set_b(double ang)
{
	t_pitch[0][0] = cos(ang);
	t_pitch[0][1] = 0;
	t_pitch[0][2] = sin(ang);

	t_pitch[1][0] = 0;
	t_pitch[1][1] = 1;
	t_pitch[1][2] = 0;

	t_pitch[2][0] = -sin(ang);
	t_pitch[2][1] = 0;
	t_pitch[2][2] = cos(ang);
}
void set_c(double ang)
{
	t_roll[0][0] = 1;
	t_roll[0][1] = 0;
	t_roll[0][2] = 0;

	t_roll[1][0] = 0;
	t_roll[1][1] = cos(ang);
	t_roll[1][2] = -sin(ang);

	t_roll[2][0] = 0;
	t_roll[2][1] = sin(ang);
	t_roll[2][2] = cos(ang);
}

dof6 createPos(double p_x, double p_y, double p_z, double a_x, double a_y, double a_z, double a_w)
{
	double x_rad;
	double y_rad;
	double z_rad;

	x_rad = (double)atan2l(2.0 * (a_y * a_z + a_x * a_w), (-(a_x * a_x) - (a_y * a_y) + (a_z * a_z) + (a_w * a_w)));
	y_rad = (double)asinl(-2.0 * (a_x * a_z - a_y * a_w));
	z_rad = (double)atan2l(2.0 * (a_x * a_y + a_z * a_w), ((a_x * a_x) - (a_y * a_y) - (a_z * a_z) + (a_w * a_w)));

	dof6 obj;
	obj.x = p_x;
	obj.y = p_y;
	obj.z = p_z;

	obj.yaw =   y_rad; 
	obj.pitch = x_rad;
	obj.roll =  z_rad;

	return obj;
}