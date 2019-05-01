# Smart Controller
This page contains the setup information for the smart controller system. This project was created as a capstone project for the USU Computer Engineering program 2018-2019. 


![alt text](https://github.com/StephenbDennis/SmartController/blob/master/Overview_updated.PNG)


## Physical Setup
This setup used the following hardware

**Master Computer:** Laptop with quad-core processor running Ubuntu 16.04 in virtualbox

**Camera 1:** Raspberry Pi 3 Model B with Raspberry Pi Camera Module V2

**Camera 2:** Raspberry Pi 3 Model B with Raspberry Pi Camera Module V2

**Robot:** A nodeMCU board

Both cameras and the master computer were conected together with a network router and ethernet cables. The master computer connected to the robot through the router with wifi. 

Apriltags must also be placed in the cameras field of view. At least one known tag must be in the system, and at least one tag needs to be visible from both cameras.  

## Software Setup

**Master Computer**

**Setting up the OS**
http://emanual.robotis.com/docs/en/platform/turtlebot3/pc_setup/#install-ros-on-remote-pc

Follow this tutorial from 6.1 to the end

For my system I used the following IP addresses

**Laptop:** 192.168.1.130

**Camera 1:** 192.168.1.110

**Camera 2:** 192.168.1.111

```cd ~/catkin_ws/src```

```git clone https://github.com/dmalyuta/apriltags2_ros.git```

```Replace ~/catkin_ws/src/apriltags2_ros/apriltags2_ros/launch/continuous_detection.launch with continuous_detection.launch```

There are two files that need to be configured:

**config/settings.yaml**

This file by default did not need to be changed if detecting tags from the 36h11 family.

**config/tags.yaml**

This file needs to have a list of the Id's of the tags that will be detected. By default no tags are included.

If multiple cameras are being used I had issues with both instances of the apriltag code using the tags.yaml I resolved this by using two files, one for each camera. These files are found in the repo.

The family of tag you will be searching for is set up in the config/settings.yaml file. By default it is set to search for tags in the 
36h11 family.

Examples of how these files need to be set up can be found at:
[wiki.ros.org/apriltags2_ros/Tutorials](http://wiki.ros.org/apriltags2_ros/Tutorials) 


then run
```cd ~/catkin_ws```
```catkin_make```

**Cameras**

**Setting up the OS**

Follow this tutorial: [http://emanual.robotis.com/docs/en/platform/turtlebot3/raspberry_pi_3_setup/#raspberry-pi-3-setup](http://emanual.robotis.com/docs/en/platform/turtlebot3/raspberry_pi_3_setup/#raspberry-pi-3-setup) 


The IP addresses that I used for this tuorial were:

**Laptop:** 192.168.1.130

**Camera 1:** 192.168.1.110

**Camera 2:** 192.168.1.111

After the tutorial some nodes need to be set up. To do this follow the instructions below. 

```cd ~/catkin_ws/src```

```git clone https://github.com/UbiquityRobotics/raspicam_node.git```

Create this file:

```sudo nano /etc/ros/rosdep/sources.list.d/30-ubiquity.list```

Paste the line below into the file above

```yaml https://raw.githubusercontent.com/UbiquityRobotics/rosdep/master/raspberry-pi.yaml```

```run rosdep update```

Install the ros dependencies

```cd ~/catkin_ws```

```rosdep install --from-paths src --ignore-src --rosdistro=kinetic -y```

The above command may fail with a "no definition for  for OS". If this happens, just continue.

run

```catkin_make```

```source ~/catkin_ws/devel/setup.bash```

```rospack profile.```



copy camera_info folder from /home/pi/catkin_ws/src/raspicam_node/camera_info to /home/pi/.ros/camera_info

copy [camerav2_640x480_10fps.launch](https://github.com/StephenbDennis/SmartController/blob/master/Camera_Files/camerav2_640x480_10fps.launch) into /home/pi/catkin_ws/src/raspicam_node/launch

copy [camerav2_640x480.yaml](https://github.com/StephenbDennis/SmartController/blob/master/Camera_Files/camerav2_640x480.yaml)  into /home/pi/.ros/camera_info and ~/catkin_ws/src/raspicam_node/camera_info

This file contains a discription of the distortion of the camera. Using this file might not give the best results, and I recommend generating one specific to your camera by following the tutorial at * [http://wiki.ros.org/camera_calibration/Tutorials/MonocularCalibration](http://wiki.ros.org/camera_calibration/Tutorials/MonocularCalibration) 

When I ran this code I had synchronization issues on the apriltag side. The modified file below fixed some of those issues.
I was also able to further reduce this problem by connected both of the cameras and the laptop to a router with ethernet cables. It appears that this might be a bandwidth issue.

Replace raspicam_node.cpp in /home/pi/catkin_ws/src/raspicam_node/src with
* [raspicam_node.cpp](https://github.com/StephenbDennis/SmartController/blob/master/Camera_Files/raspicam_node.cpp)  

After replacing the file the node must be rebuilt.

```cd ~/catkin_ws```

```catkin_make```

The * [setup](https://github.com/StephenbDennis/SmartController/blob/master/Camera_Files/setup) script also needs to be added to the ~/ directory to help simplify launching the software.

**Robot: **
The software for the robot is very simple. The nodeMCU board creates a simple webserver with a web page that allows the system to be controlled by any device on the network. This software is loaded onto the board using the arduino IDE. The network settings must be modified on this file before it is loaded. 

## Configuring system
This repo contains some additional nodes that were created to calculate the nessesary transforms, calculate paths, and control the robot.

Each of these files has settings that can be modified to adjust the performance of the system. The configrable portions are towards the top of each file and are marked with comments. 

The tf_listener.cpp file contains settings to adjust which tags are detected, and how each tag is interpreted. This file also allows the user to specify how many cameras are in the system. These adjustment are located on lines 50-64.  XXX


The pathfinder.cpp file contains the code to calculate the path that the robot will take. This file allows the user to adjust the size of the obstacles, the size of the waypoints, and the max area that the robot is allowed to travel in. These settings can be modified on lines XXX of this file.


The tf_guide.cpp file contains the code that will send commands to the robot. This file contains settings that allow the user to adjust how strinct the controls are, and the address to where the commands are sent. These settings are found on lines XXX of this file. With the system I noticed that the robot often changed ip addresses. This file is where what address would be modified.

Because these settings are in the .cpp files after any changes the nodes must be rebuilt. This is done by running the catkin_make command from the catkin directory.


## Running the Code 

Once the software has been set up on the raspberry pi's, the master computer, and the nodeMCU, then the system is ready to start.

To start the software run the following code:
 
1). Configure the ethernet port for each camera (This is usully done over wifi)

      **Camera 1:** ```ifconfig eth0 192.168.1.110```
      
      **Camera 2:** ```ifconfig eth0 192.168.1.111```

2). From the master computer verify that the nodes in catkin have been configured and run catkin_make

3). Run script to bring up detection nodes ```cd ~/catkin && ./setup``` (This script will launch several terminals)

4). Two of the terminals that launched are ssh-ing into the cameras. Log into each one and run the launch script ```(cd ~/ && 
./setup)```

5). Open 3 terminals to run the transform node, the pathfinding node, and the guide node.

6). The transform node must be started first with: ```rosrun setup_tf tf_listener```

7). The pathfinding node is then launched, this is done with:  ```rosrun setup_tf pathfinder```

8). This node accepts a point from the user and if a path is possible it then starts broadcasting waypoints.

9). The last node to start is the guide node. This is done with ```rosrun setup_tf tf_guide```

10). The robot will start driving to the selected point.
      


