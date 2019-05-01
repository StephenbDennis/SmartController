#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>

struct point
{
    double x;
    double y;
};

double getDist_Line(point, point, point);
double getDist(point, point);
int main(int argc, char** argv){
  ros::init(argc, argv, "way_br");
  ros::NodeHandle n;

  tf::TransformBroadcaster way_br;
  tf::TransformListener listener;

  ros::Rate r(30);

  double obs_rad[2];
  
  int wayNum = 5;
  
    //***Config****
    obs_rad[0]=.25; //radius of the first obstacle
    obs_rad[1]=.25; //radius of the second obstacle
    double buf =.1; //radius of robot
    double yMax =2; //max boundry Y
    double xMax =10; //max bountry X
    //***END***
    
    point pos[5];
    pos[0].x = -10; pos[0].y = -10;
    pos[1].x = -10; pos[1].y = -10;
    pos[2].x = -10; pos[2].y = -10;
    pos[3].x = -10; pos[3].y = -10;
    pos[4].x = -10; pos[4].y = -10;
  //Define value of PI
    double PI = atan(1) * 4;

    tf::StampedTransform t_robot;
    tf::StampedTransform t_obs_1;
    tf::StampedTransform t_obs_2;
    for (int time=0;time<60;time++)
    {
      try{
        listener.lookupTransform("world", "robot", ros::Time(0), t_robot);
        listener.lookupTransform("world", "obs_1", ros::Time(0), t_obs_1);
        listener.lookupTransform("world", "obs_2", ros::Time(0), t_obs_2);
      }
      catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
      }
      r.sleep();
    }
    

    double xVal, yVal;
    std::cout<<"Robot position is X: "<<t_robot.getOrigin().x()<< " Y: "<<t_robot.getOrigin().y()<<"\n";
    std::cout<<"obs1 position is X: "<<t_obs_1.getOrigin().x()<< " Y: "<<t_obs_1.getOrigin().y()<<"\n";
    std::cout<<"obs2 position is X: "<<t_obs_2.getOrigin().x()<< " Y: "<<t_obs_2.getOrigin().y()<<"\n";
    std::cout<<"Enter desired position";
    std::cout<<"\nX:";
    std::cin>>xVal;
    std::cout<<"\nY:";
    std::cin>>yVal;

    std::cout<<"\nCalculating Waypoints\n";
    pos[0].x=t_robot.getOrigin().x();
    pos[0].y=t_robot.getOrigin().y();

    point a,b,c1,c2;
    double dist1 = sqrt(pow((t_obs_1.getOrigin().x()-pos[0].x),2)+pow((t_obs_1.getOrigin().y()-pos[0].y),2));
    double dist2 = sqrt(pow((t_obs_2.getOrigin().x()-pos[0].x),2)+pow((t_obs_2.getOrigin().y()-pos[0].y),2));

    point way[2][2];
    if(dist1<dist2)
    {
      way[0][0].x =t_obs_1.getOrigin().x();
      way[0][0].y =(yMax - t_obs_1.getOrigin().y())/2 + t_obs_1.getOrigin().y();
      way[0][1].x =t_obs_1.getOrigin().x();
      way[0][1].y =t_obs_1.getOrigin().y()/2;
      way[1][0].x =t_obs_2.getOrigin().x();
      way[1][0].y =(yMax - t_obs_2.getOrigin().y())/2 + t_obs_2.getOrigin().y();
      way[1][1].x =t_obs_2.getOrigin().x();
      way[1][1].y =t_obs_2.getOrigin().y()/2;
    }
    else
    {
      way[1][0].x =t_obs_1.getOrigin().x();
      way[1][0].y =(yMax - t_obs_1.getOrigin().y())/2 + t_obs_1.getOrigin().y();
      way[1][1].x =t_obs_1.getOrigin().x();
      way[1][1].y =t_obs_1.getOrigin().y()/2;
      way[0][0].x =t_obs_2.getOrigin().x();
      way[0][0].y =(yMax - t_obs_2.getOrigin().y())/2 + t_obs_2.getOrigin().y();
      way[0][1].x =t_obs_2.getOrigin().x();
      way[0][1].y =t_obs_2.getOrigin().y()/2;
    }
    for(int i=0;i<2;i++)
    {
      for(int j=0;j<2;j++)
      {
      //std::cout<<"POS waypoints: "<<i<<" "<<j<<" x:"<<way[i][j].x<<" y:"<<way[i][j].y<<"\n";
      }
    }
    
    
    int posCount =0;
    int wayVal =0;
    bool done =false;
    for (int i=1; i<wayNum;i++)
    {
      //start loop
      //robot position
      a.x=pos[posCount].x;
      a.y=pos[posCount].y;

      //desired point
      b.x=xVal;
      b.y=yVal;

      //obs location
      c1.x=t_obs_1.getOrigin().x();
      c1.y=t_obs_1.getOrigin().y();

      c2.x=t_obs_2.getOrigin().x();
      c2.y=t_obs_2.getOrigin().y();

      if(!done)
      {
        //std::cout<<"obs1: "<<getDist_Line(a, b, c1)<<" min: "<<(obs_rad[0]+buf)<<"\n";
        //std::cout<<"obs2: "<<getDist_Line(a, b, c2)<<" min: "<<(obs_rad[1]+buf)<<"\n";
        //check obs
        if((getDist_Line(a, b, c1)>(obs_rad[0]+buf)) && (getDist_Line(a, b, c2)>(obs_rad[1]+buf)))
        {
          //clear path, Go
          done=true;
          std::cout<<"PATH FOUND!\n";
          posCount++;
          pos[posCount].x=xVal;
          pos[posCount].y=yVal;
        }
        else
        {
          if(way[wayVal][0].y<yMax-buf)
          {
            if(way[wayVal][1].y>buf)
            {
              //both valid check with distance
              if(getDist(a,way[wayVal][1])>getDist(a,way[wayVal][0]))
              {
                posCount++;
                pos[posCount]=way[wayVal][0];

              }
              else
              {
                posCount++;
                pos[posCount]=way[wayVal][1];
              }
              
              //std::cout<<"Both are valid\n";
            }
            else
            {
              //top valid bottom not
              posCount++;
              pos[posCount]=way[wayVal][0];
            }   
            wayVal++;   
          }
          else if (way[wayVal][1].y>buf)
          {
            //top false botom valid
            posCount++;
            pos[posCount]=way[wayVal][1];
            wayVal++;
          }  
          else
          {
            //both false
            std::cout<<"ERROR\n";
          }
          
        }
      }
      else
      {
        posCount++;
        pos[posCount].x=-100;
        pos[posCount].y=-100;
      }
      
      
    }

    if(!done)
    {
      std::cout<<"No Path Found\n";
    }
    
    bool disp=true;
    while(n.ok() && done){

    for(int i=0; i < wayNum; i++)
    {
        //TF of wayX location w/ respect to map
        tf::Transform way;
        tf::Quaternion Q;
        way.setOrigin( tf::Vector3(pos[i].x,pos[i].y,0));
        Q.setRPY(0,0,0);
        way.setRotation(Q);
        std::string temp;
        std::stringstream ss;
        ss<<i;
        temp=ss.str(); 
        way_br.sendTransform(
        tf::StampedTransform(way, ros::Time::now(), "world", "way_"+temp));
        if(disp)
        {
          std::cout<<"Waypoint:"<<i<<" x:"<<pos[i].x<<" y:"<<pos[i].y<<"\n";
        }
        
    }
    disp=false;
    //break;
    



    r.sleep();
  }
}

double getDist_Line(point a,point b, point c)
{
  double A = (b.y-a.y)/(b.x-a.x);
  //std::cout<<"A: "<<A;
  double B = -1;
  //std::cout<<"B: "<<B;
  double C = (-(b.y-a.y)/(b.x-a.x))*a.x+a.y;
  //std::cout<<"C: "<<C;
  double val =(A*c.x+B*c.y+C)/sqrt(A*A+B*B);
  //std::cout<<"val: "<<val;
  if(val<0)
  {
    val *= -1;
  }
  return val;
}
double getDist(point a,point b)
{
  double A = (b.y-a.y);
  double B = (b.x-a.x);
  return sqrt(A*A+B*B);
}

