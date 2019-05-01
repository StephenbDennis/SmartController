# Smart Controller
This page contains information to set up the the smart controller system. This project was created as a capstone project for the USU Computer Engineering program. This project was done Stephen Dennis.  

## Physical Setup
This setup used the following hardware

**Master Computer:** Laptop with quad-core processor running Ubuntu 16.04 in virtualbox

**Camera 1:** Raspberry Pi 3 Model B with Raspberry Pi Camera Module V2

**Camera 2:** Raspberry Pi 3 Model B with Raspberry Pi Camera Module V2

* [Name](http://www.google.com) - hyperlink


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
[wiki.ros.org/apriltags2_ros/Tutorials](wiki.ros.org/apriltags2_ros/Tutorials) 


then run
```cd ~/catkin_ws```
```catkin_make```

**Camera**

**Setting up the OS**
http://emanual.robotis.com/docs/en/platform/turtlebot3/raspberry_pi_3_setup/#raspberry-pi-3-setup

The IP addresses that I used for this tuorial were:

**Laptop:** 192.168.1.130

**Camera 1:** 192.168.1.110

**Camera 2:** 192.168.1.111

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

copy [camerav2_640x480_7fps.launch](http://www.google.com)  and [camerav2_640x480_20fps.launch](http://www.google.com) 

into /home/pi/catkin_ws/src/raspicam_node/launch

copy [camerav2_640x480.yaml](http://www.google.com)  into /home/pi/.ros/camera_info and ~/catkin_ws/src/raspicam_node/camera_info

When I ran this code I had synchronization issues on the apriltag side. The modified file below fixed some of those issues.
I was also able to further reduce this problem by connected both of the cameras and the laptop to a router with ethernet cables. It appears that this might be a bandwidth issue.

Replace raspicam_node.cpp in /home/pi/catkin_ws/src/raspicam_node/src with
* [raspicam_node.cpp](http://www.google.com)  

After replacing the file the node must be rebuilt.

```cd ~/catkin_ws```

```catkin_make```


## Cofiguring system

## Running the Code 
