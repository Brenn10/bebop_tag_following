/************************************************************************
* Controller header file.
* The Controller class controls the movement of the AR.Drone through tum_ardrone
* Publishes to the rostopic /tum_ardrone/com
*
* Authors: Shannon Hood, Patrick Hamod, Kelly Benson
* Last Modified: 09/09/16
*/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Image.h"
#include "std_srvs/Empty.h"
#include "std_msgs/Bool.h"
#include "std_msgs/UInt8.h"
#include "std_msgs/Empty.h"
#include "nav_msgs/Odometry.h"
#include "nav_msgs/Path.h"
#include "ar_track_alvar_msgs/AlvarMarkers.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"
#include <bebop_tag_following/Dispatch.h>

#include "State.h"

#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream>
#include <math.h>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include "PIDController.h"

#define NO_TAG_FOUND 0
#define FOUND_TAG 1
#define LOST_TAG 2
#define DRONE_DISPATCHED 3

class bebopTagFollowing {
  public:
    // Constructor to interact with the rosnode
    bebopTagFollowing(ros::NodeHandle& nh);

    // Callback functions
    void positionCallback(const ar_track_alvar_msgs::AlvarMarkers::ConstPtr& msg);
    void cameraCallback(const std_msgs::Bool::ConstPtr& isBottomCam);
    void turtlebotCallback(const nav_msgs::Odometry::ConstPtr& msg);
    void orbSlamCallback(const nav_msgs::Path::ConstPtr& msg);
    void clCallback(const nav_msgs::Path::ConstPtr& msg);
    void dispatchDrone( const bebop_tag_following::Dispatch::ConstPtr& msg);
    void retrieveDrone( const bebop_tag_following::Dispatch::ConstPtr& msg);

    // Misc. functions
    void shutDown();
    void initialize();
    void hover();

  private:
    static const double HEIGHT_OVER_TAG = 1;
    // Publishers
    ros::Publisher drone_comms;
    ros::Publisher cmdVel;
    ros::Publisher land;
    ros::Publisher takeOff;
    ros::ServiceClient flattrim;

    // Subscribers
    ros::Subscriber tagPose;
    ros::Subscriber orbSlamSub;
    ros::Subscriber clSub;
    ros::Subscriber dispatch;
    ros::Subscriber retrieve;


    PIDController *z_pid,*yaw_pid,*disp_pid;
    bool printedWarn;
    bool tagDetected;
    bool seenOnce;
    enum Dispatched {following,turning,leaving,dispatched,searching,returning};
    Dispatched dispatch_state;
    double disp_angle,disp_vel;
    double lastSeen;
    double startTime;
    double currentTime;
    double timeDelta;

    State state; // Pose of the tag from ar_track_alvar
    State uavOffset; // Pose of turtlebot from GVG

    geometry_msgs::Pose lastOrbPose;
    geometry_msgs::Pose lastUGVPose;
};
