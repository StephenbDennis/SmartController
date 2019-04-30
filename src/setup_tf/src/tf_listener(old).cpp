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


//OLD?
double t_yaw[3][3];
double t_pitch[3][3];
double t_roll[3][3];
dof6 t_form(dof6);
void set_a(double);
void set_b(double);
void set_c(double);

double t_abc[4][3];
dof6 t_form_new(dof6 loc);
void set_abc(dof6);

dof6 createPos(double, double, double, double, double, double, double);
void reset(void);

double PI = atan(1) * 4;
const int tagNum = 3;
const int frameNum = 2;
int timeout=15;


string frameName[] = {"camera_1","camera_2"};
string tagName[]={"tag4","tag5","tag12"};
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
  ros::Rate rate(20.0);

  known k_tag;

  k_tag.id=2;
  k_tag.data.x=0;
  k_tag.data.y=0;
  k_tag.data.z=0;
  k_tag.data.roll =0;

//   string frame = "camera_1";
//   string tags[]={"tag0","tag1"};//,"tag2","tag3"};

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

		//set transform matrix for each axis to allign it with the first tag
		//OLD
		//set_a(raw[0][0].roll);
		//set_b(raw[0][0].yaw);	
		//set_c(raw[0][0].pitch);
		
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
				if(world_timer[k]<timeout)
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
				//set world if possible
				if(t_formed_timer[j][k_tag.id]<timeout )
				{
					offset[j].x=(t_val[j][k_tag.id].x-k_tag.data.x);
					offset[j].y=(t_val[j][k_tag.id].y-k_tag.data.y);
					offset[j].z=(t_val[j][k_tag.id].z-k_tag.data.z);
				}
				else
				{
					valid =false;		
					for (int k=0; k<tagNum; k++)
					{
						if(world_timer[k]<timeout)
						{
							offset[j].x=(t_val[j][k].x-world[k].x);
							offset[j].y=(t_val[j][k].y-world[k].y);
							offset[j].z=(t_val[j][k].z-world[k].z);
							valid =true;
							break;
						}
						
					}

				}
				if(valid)
				{
					for (int k=0; k<tagNum; k++)
					{
						if((t_formed_timer[j][k]<timeout))
						{
							world[k]=t_val[j][k];
							world[k].x -=offset[j].x;
							world[k].y -=offset[j].y;
							world[k].z -=offset[j].z;

							world_timer[k]=0;
						}
						
					}
				}
			}
			
		}
		

		//print shifted raw
		
		for (int i=0; i<tagNum; i++)
		{
			//ROS_INFO("\n %s:\n(%.3f, %.3f, %.3f)\n(%.3f, %.3f, %.3f)",tagName[i].c_str(), (t_val[i].x), (t_val[i].y), (t_val[i].z),(t_val[i].roll), (t_val[i].yaw), (t_val[i].pitch));
			//ROS_INFO("\n %s:\n(%.3f, %.3f, %.3f)\n(%.3f, %.3f, %.3f)",tagName[i].c_str(), (t_val[i].x-t_val[0].x), (t_val[i].y-t_val[0].y), (t_val[i].z-t_val[0].z),(t_val[i].roll), (t_val[i].yaw), (t_val[i].pitch));
			
			//ROS_INFO("\n %s:\n(%.3f, %.3f, %.3f)\n(%.3f, %.3f, %.3f)",tagName[i].c_str(), (world[i].x), (world[i].y), (world[i].z),(world[i].roll*(180/PI)), (world[i].yaw*(180/PI)), (world[i].pitch*(180/PI)));
			ROS_INFO("\n %s:\n(%.3f, %.3f, %.3f)\n(%.3f)",tagName[i].c_str(), (world[i].x), (world[i].y), (world[i].z),(world[i].roll*(180/PI)-world[k_tag.id].roll*(180/PI)));

		
		}

		//increment timers
		for (int j=0;j<frameNum;j++)
		{
			for (int i=0;i<tagNum;i++)
			{
				old[j][i].x=raw[j][i].x;
				raw_timer[j][i]++;	
				t_formed_timer[j][i]++;
				//cout<< raw_timer[j][i] <<" ";
			}
			//cout<<endl;
		}
        for (int i=0;i<tagNum;i++)
		{
			world_timer[i]++;
			//cout<< raw_timer[j][i] <<" ";
		}

	}
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

    //tf::TransformBroadcaster broadcaster;
    //broadcaster.sendTransform(tf::StampedTransform(tf::Transform(transform.getRotation(), transform.getOrigin()), ros::Time::now(),"world", "test"));

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