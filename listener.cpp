#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img = imread("ps.jpg", 1);
  
void chatterCallback(const geometry_msgs::Point& msgs)
{
  //ROS_INFO("I heard: [%d]", msg.x);
  cout << msgs.x << " " << msgs.y << endl;
   img.at<Vec3b>((int)msgs.x, (int)msgs.y)[0] = 255;
  img.at<Vec3b>((int)msgs.x, (int)msgs.y)[1] = 0; 
  img.at<Vec3b>((int)msgs.x, (int)msgs.y)[2] = 0;
 imshow("final",img);
 waitKey(100);

}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "listener");

 
  ros::NodeHandle n;

  
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  ros::spin();
 
  waitKey(100);
  return 0;
}
