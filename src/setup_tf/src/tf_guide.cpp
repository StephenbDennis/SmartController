#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <unistd.h>
//#include <studio.h>
#include <curl/curl.h>

using namespace std;

struct dof6 {
	double x;
	double y;
	double z;
	double yaw;
	double pitch;
	double roll;
};

string IP="hi";
int wayNum =5;

double PI = atan(1) * 4;

///***Conf***
double dist_buffer = .08; //size of waypoint size
double ang_buffer = .2; //dictates how tight turning is controlled
string s_for ="http://192.168.1.126/_for"; //forward command
string s_CCW ="http://192.168.1.126/_l";   //left turn command
string s_CW ="http://192.168.1.126/_r";    //right turn command
///***END Conf***

bool done = false;


dof6 createPos(double, double, double, double, double, double, double);

int main(int argc, char** argv){
  ros::init(argc, argv, "r_guide");
  ros::NodeHandle node;
  
  tf::TransformListener listener;
  
  ros::Rate rate(5);
 
  dof6 dest[wayNum];

  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();

    int curTag = 0;
  dof6 r_pos_old;
  r_pos_old.x=0;
  r_pos_old.y=0;
  while (node.ok()){


	tf::StampedTransform robot;
    tf::StampedTransform way[wayNum];
    dof6 r_pos;
    

    try{

		listener.lookupTransform("world", "robot", ros::Time(0),robot);
		
		r_pos = createPos(
					robot.getOrigin().x(), robot.getOrigin().y(), robot.getOrigin().z(),
					robot.getRotation().x(),robot.getRotation().y(), robot.getRotation().z(), robot.getRotation().w());

        for(int i = 0; i < wayNum;i++)
        {
            std::stringstream ss;
            ss<<i;
            listener.lookupTransform("world", "way_"+ss.str(), ros::Time(0),way[i]);
		
		    dest[i] = createPos(
					way[i].getOrigin().x(), way[i].getOrigin().y(), way[i].getOrigin().z(),
					way[i].getRotation().x(),way[i].getRotation().y(), way[i].getRotation().z(), way[i].getRotation().w());

        }
        double d_x;
        double d_y;		
        double dist;
        double angle;
        bool x_neg;
        bool y_neg;

        if(dest[curTag].x ==-100 && dest[curTag].y ==-100)
        {
            done =true;
        }
        bool lost = (r_pos.x == r_pos_old.x);//IIf tag data gets stale stop sending commands
        r_pos_old=r_pos;
        d_x = (dest[curTag].x-r_pos.x);
        d_y = (dest[curTag].y-r_pos.y);
        double ang_real = (PI/4)+(PI/4-r_pos.z);
        if(ang_real>PI)
        {
            ang_real= -PI +(ang_real-PI);
        }
        else if(ang_real<-PI)
        {
            ang_real= PI +(ang_real+PI);
        }

        angle= atan2(d_y,d_x);
        std::cout<<"angle: "<<angle<<"\n";

        //d_x = (r_pos.x - dest[curTag].x);
        //d_y = (r_pos.y - dest[curTag].y);

        // ((d_x < 0) ? (x_neg = false) : (x_neg = true));
        // ((d_y < 0) ? (y_neg = false) : (y_neg = true));

        // d_x = fabs(d_x);
        // d_y = fabs(d_y);

        //calc angle
        // if(x_neg)
        // {
        //     if(y_neg)
        //     {
        //         angle = (PI/2) - tan(d_y/d_x);
        //     }
        //     else
        //     {
        //         angle = (PI/2) + tan(d_y/d_x);
        //     }
            
        // }
        // else
        // {
        //     if(y_neg)
        //     {
        //         angle = -(PI/2) + tan(d_y/d_x);
        //     }
        //     else
        //     {
        //         angle = -(PI/2) - tan(d_y/d_x);
        //     }
            
        // }
        //calc distance
        dist = sqrt(pow((r_pos.x - dest[curTag].x),2)+pow((r_pos.y-dest[curTag].y),2));    
        
        if((dist < dist_buffer) || (done))
        {
            cout<<"Point reached"<<endl;

            //GOTO next tag
            if(curTag < (wayNum-1))//if(curTag < (1))//stop at second point
            {
                curTag++;
                for (int disp = 0; disp<3;disp++)
                {
                    cout<<"Point "<< curTag << " reached"<<endl;
                    //sleep(1);
                }
                
            }
            else
            {
                cout<<"Final destination reached"<<endl;
                done = true;
                break;
            }
            

        }
        else
        {
            bool c_ang = false;
            double dif = ang_real-angle;
            cout <<"dif: "<<dif<<"\n";
            if((dif < -PI) || (dif>0 && dif<PI))
            {
                //CW
                if(!c_ang)
                {
                    if(fabs(dif)>ang_buffer)
                    {
                        if(lost)
                        {
                            cout<<"Not Found"<<endl;
                        }
                        else
                        {
                            //do move 
                            c_ang = true;
                            cout<<"CW"<<endl;
                            curl_easy_setopt(curl, CURLOPT_URL, s_CW.c_str());
                            curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
                            res = curl_easy_perform(curl);   
                        }
                    }
                }
            }
            if((dif > PI) || (dif<0 && dif>-PI))
            {
                //CCW
                if(!c_ang)
                {
                    if(fabs(dif)>ang_buffer)
                    {
                        if(lost)
                        {
                            cout<<"Not Found"<<endl;
                        }
                        else
                        {
                            //do move 
                            cout<<"CCW"<<endl;
                            c_ang = true;
                            curl_easy_setopt(curl, CURLOPT_URL, s_CCW.c_str());
                            curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
                            res = curl_easy_perform(curl); 
                        }
                        
                    }
                }
            }
            if(!c_ang)
            {
                if(lost)
                {
                    cout<<"Not Found"<<endl;
                }
                else
                {
                    //do move 
                    cout<<"Drive forward"<<endl;
                    curl_easy_setopt(curl, CURLOPT_URL, s_for.c_str());
                    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
                    res = curl_easy_perform(curl);
                }
                
            }

        //    double op_angle;
        //    bool c_ang = false;

        //    if(fabs(angle-r_pos.z)>ang_buffer)
        //    {
        //         if(angle>0)
        //         {
        //             op_angle=angle - PI;
                    
        //             if(r_pos.z<op_angle || r_pos.z > angle)
        //             {
                        
        //                 //CCW
        //                 cout<<"Rotate CCW"<<endl;
        //                 c_ang = true;
        //                 curl_easy_setopt(curl, CURLOPT_URL, s_CCW.c_str());
        //                 curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //                 res = curl_easy_perform(curl);
        //             }
        //             else if((r_pos.z<angle && r_pos.z > 0) || (r_pos.z>op_angle && r_pos.z < 0))
        //             {
        //                 //CW
        //                 cout<<"Rotate CW"<<endl;
        //                 c_ang = true;
        //                 curl_easy_setopt(curl, CURLOPT_URL, s_CW.c_str());
        //                 curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //                 res = curl_easy_perform(curl);
        //             }
        //         } 
        //         else
        //         {
        //             op_angle=angle + PI;
        //             if(r_pos.z<angle || r_pos.z > op_angle)
        //             {
        //                 //CW
        //                 cout<<"Rotate CW"<<endl;
        //                 c_ang = true;
        //                 curl_easy_setopt(curl, CURLOPT_URL, s_CW.c_str());
        //                 curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //                 res = curl_easy_perform(curl);
        //             }
        //             else if((r_pos.z>angle && r_pos.z < 0) || (r_pos.z<op_angle && r_pos.z > 0))
        //             {
        //                 //CCW
        //                 cout<<"Rotate CCW"<<endl;
        //                 c_ang = true;
        //                 curl_easy_setopt(curl, CURLOPT_URL, s_CCW.c_str());
        //                 curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //                 res = curl_easy_perform(curl);
        //             }
        //         }
        //    }
        //    if(!c_ang)
        //    {
        //         cout<<"Drive forward"<<endl;
        //         curl_easy_setopt(curl, CURLOPT_URL, s_for.c_str());
        //         curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //         res = curl_easy_perform(curl);
        //    }
            
        }
        





		//display location
		//ROS_INFO("\n %s:\n(%.3f, %.3f)\n(%.3f)","robot", (r_pos.x), (r_pos.y),(r_pos.z));

	}
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

    rate.sleep();
  }
  return 0;
};

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
