//A Simple Listener for the /tf topic.
//Assumes that the fist tag in the list is at point(0,0,0), and oriientation(0,0,0)
//Output is in the 1st tags frame
//Made by Stephen Dennis 01/26/2019

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <unistd.h>

using namespace std;

struct dof6 {
	double x;
	double y;
	double z;
	double yaw;
	double pitch;
	double roll;
};
struct tagData{
	dof6 data;
	string name;
};
struct known{
	dof6 data;
	int id;
};



dof6 t_form_new(dof6 loc);
void set_abc(dof6);
dof6 createPos(double, double, double, double, double, double, double);
void reset(void);

double t_abc[4][3];

///*** Configuration Portion ***

int timeout=10; //Time that readings are valid for

const int frameNum = 1; //number of cameras in system
string frameName[] = {"camera_1"}; //namespacing of cameras

const int tagNum = 1; //number of tags in system
const int robotId = 3;// index of robot tag (not used to create map)
string tagName[]={"tag12"}; //name of all the tags in the system

double knownData[] ={2, 0, 0, 0, 0, 0, 0}; //{id,x,y,z,roll,pitch,yaw}
//index of tag where the world position is known 
//(x,y,z) Position in world
//(roll,pitch,yaw) Orentation in world (only roll will work) ***needs testing***

///*** End Configuration

double PI = atan(1) * 4;

dof6 old[frameNum][tagNum];
dof6 offset[frameNum];

dof6 raw[frameNum][tagNum];
dof6 t_val[frameNum][tagNum];
dof6 world[tagNum];

unsigned int raw_timer [frameNum][tagNum];
unsigned int t_formed_timer [frameNum][tagNum];
unsigned int world_timer [tagNum];


int main(int argc, char** argv){
  ros::init(argc, argv, "tf_listener");

  ros::NodeHandle node;
  
  tf::TransformListener listener;
  tf::TransformBroadcaster br;
  
  ros::Rate rate(100.0);
  known k_tag;
  
  k_tag.id = knownData[0];
  k_tag.data.x = knownData[1]; 
  k_tag.data.y = knownData[2];
  k_tag.data.z = knownData[3];
  k_tag.data.roll = knownData[4]; 
  k_tag.data.pitch = knownData[5]; 
  k_tag.data.yaw = knownData[6]; 


  bool clear = true;
  while (node.ok()){

	tf::StampedTransform t_forms[frameNum][tagNum];

    try{

		for(int j = 0; j<frameNum;j++)
		{
			for (int i = 0; i < tagNum; i++)
			{
				stringstream ss;
				ss<<(j+1);
				listener.lookupTransform(frameName[j], (tagName[i]+"_"+ss.str()), ros::Time(0), t_forms[j][i]);
			}	
		}

		if (clear)
		{
			reset();
			clear=false;
		}


		
		for (int j = 0; j < frameNum; j++)
		{
			for (int i = 0; i < tagNum; i++)
			{
				raw[j][i] = createPos(
					t_forms[j][i].getOrigin().x(), t_forms[j][i].getOrigin().y(), t_forms[j][i].getOrigin().z(),
					t_forms[j][i].getRotation().x(),t_forms[j][i].getRotation().y(), t_forms[j][i].getRotation().z(), t_forms[j][i].getRotation().w());
				
				if(raw[j][i].x!=old[j][i].x)
				{
					raw_timer[j][i]=0;
				}
				
			}
		}
		//set known points to world
		for(int i=0;i<tagNum;i++)
		{
			if(k_tag.id==i)
			{
				world[i]=k_tag.data;
				world_timer[i]=0;
			}
		}

		//transform all tags
		for (int j=0; j<frameNum; j++)
		{
			bool valid =false;
			//set transform
			for (int k=0; k<tagNum; k++)
			{
				if(world_timer[k]<timeout && k!=robotId)
				{
					if(raw_timer[j][k]<timeout)
					{
						set_abc(raw[j][k]);
						valid =true;
						break;
					}
				}
			}
			if (valid)
			{
				for (int i=0; i<tagNum; i++)
				{
					if (raw_timer[j][i]<timeout)
					{
						//do transform
						t_val[j][i] = t_form_new(raw[j][i]);	
						t_formed_timer[j][i]=0;
					}
					old[j][i].x=raw[j][i].x;
				}			
			}
		}

		//we only want one world assignment per iteration
		bool assign [tagNum];
		for (int num =0; num<tagNum;num++)
		{
			assign[num]=false;
		}

		//load all tags that are in the same frame as the the known tag.
		for (int j=0;j<frameNum;j++)
		{
			//if known is found in a frame set all tags in frame to world
			if(t_formed_timer[j][k_tag.id]<timeout)
			{
				//get offset
				offset[j].x=(t_val[j][k_tag.id].x-world[k_tag.id].x);
				offset[j].y=(t_val[j][k_tag.id].y-world[k_tag.id].y);
				offset[j].z=(t_val[j][k_tag.id].z-world[k_tag.id].z);
				
				for(int i=0;i<tagNum;i++)
				{
					if(t_formed_timer[j][i]<timeout)
					{
						if(!assign[i])
						{
							world[i] = t_val[j][i];
							world[i].x -=offset[j].x;
							world[i].y -=offset[j].y;
							world[i].z -=offset[j].z;

							world_timer[i]=0;
							assign[i]=true;
						}	
					}
				}
			} 
		}
		//handle other tags
		for (int j=0;j<frameNum;j++)
		{
			bool valid =false;
			for (int i=0; i<tagNum; i++)
			{
				//find a tag that is within the frame and assigned within this loop
				if(t_formed_timer[j][i]<timeout)
				{
					if(assign[i])
					{
						offset[j].x=(t_val[j][i].x-world[i].x);
						offset[j].y=(t_val[j][i].y-world[i].y);
						offset[j].z=(t_val[j][i].z-world[i].z);
					    valid = true;
						break;
					}
					
				}
			}
			//if a offset has been found assign all tags in this frame
			if(valid)
			{
				for (int i=0; i<tagNum; i++)
				{
					if(t_formed_timer[j][i]<timeout)
					{
						if(!assign[i])
						{
							world[i] = t_val[j][i];
							world[i].x -=offset[j].x;
							world[i].y -=offset[j].y;
							world[i].z -=offset[j].z;

							world_timer[i]=0;
							assign[i]=true;
						}	
					}
				}
			}
			
		}

		//print shifted raw
		for (int i=0; i<tagNum; i++)
		{
			if(i==robotId)
			{
				//broadcast
				tf::Transform robot_pose;
				tf::Quaternion rQ;
				robot_pose.setOrigin( tf::Vector3(world[i].x,world[i].y,world[i].roll));
				rQ.setRPY(0,0,0);
				robot_pose.setRotation(rQ);
				br.sendTransform(
				tf::StampedTransform(robot_pose, ros::Time::now(), "world", "robot"));
				//display location
				//ROS_INFO("\n %s:\n(%.3f, %.3f, %.3f)\n(%.3f)",tagName[i].c_str(), (world[i].x), (world[i].y), (world[i].z),(world[i].roll*(180/PI)-world[k_tag.id].roll*(180/PI)));

			}
		}

		//increment timers
		for (int j=0;j<tagNum;j++)
		{
			for (int i=0;i<frameNum;i++)
			{
				old[i][j].x=raw[i][j].x;
				raw_timer[i][j]++;	
				t_formed_timer[i][j]++;
			}
			world_timer[j]++;
		}
	}
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

    rate.sleep();
  }
  return 0;
};

dof6 t_form_new(dof6 loc)
{
	dof6 tempLoc;
	//a transform
	tempLoc.x = (loc.x*t_abc[0][0] + loc.y*t_abc[1][0] + loc.z*t_abc[2][0]);
	tempLoc.y = (loc.x*t_abc[0][1] + loc.y*t_abc[1][1] + loc.z*t_abc[2][1]);
	tempLoc.z = (loc.x*t_abc[0][2] + loc.y*t_abc[1][2] + loc.z*t_abc[2][2]);

	tempLoc.x -= t_abc[3][0];
	tempLoc.y -= t_abc[3][1];
	tempLoc.z -= t_abc[3][2];
	
	tempLoc.roll=(loc.roll);
	tempLoc.yaw=loc.yaw;
	tempLoc.pitch=loc.pitch;
	return tempLoc;
}

void set_abc(dof6 data)
{
	t_abc[0][0] = cos(data.roll)*cos(data.yaw);
	t_abc[0][1] = (cos(data.roll)*sin(data.yaw)*sin(data.pitch) - sin(data.roll)*cos(data.pitch));
	t_abc[0][2] = (cos(data.roll)*sin(data.yaw)*cos(data.pitch) + sin(data.roll)*sin(data.pitch));

	t_abc[1][0] = sin(data.roll)*cos(data.yaw);
	t_abc[1][1] = (sin(data.roll)*sin(data.yaw)*sin(data.pitch) + cos(data.roll)*cos(data.pitch));
	t_abc[1][2] = (sin(data.roll)*sin(data.yaw)*cos(data.pitch) - cos(data.roll)*sin(data.pitch));;

	t_abc[2][0] = -sin(data.yaw);
	t_abc[2][1] = cos(data.yaw) * sin(data.pitch);
	t_abc[2][2] = cos(data.yaw) * cos(data.pitch);

	t_abc[3][0] = data.x;
	t_abc[3][1] = data.y;
	t_abc[3][2] = data.z;
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

void reset()
{
	for(int i =0; i<5; i++)
	{
		std::cout<<"Starting in "<<5-i<<std::endl;
		sleep(1);

	}
	for(int i = 0; i<frameNum;i++)
	{
		for(int j = 0; j<tagNum;j++)
		{
			raw_timer [i][j] = 1000;
			t_formed_timer [i][j] = 1000;
			world_timer [j] = 1000;
		}
	}
}