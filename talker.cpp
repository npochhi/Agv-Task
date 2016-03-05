#include "ros/ros.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "geometry_msgs/Point.h"
using namespace cv;
using namespace std;

int isPoint(int x, int y){
  if(x >= 0 && x < 200 && y >= 0 && y < 200){
    return 1;
  }
  return 0;
}
 Mat image = imread("ps.jpg", 1);
int main(int argc, char **argv)
{
 
  // imshow("DisplayImage", image);
  // waitKey(1000);
    int cxred, cxgreen, cyred, cygreen;
    int minx, maxx, miny, maxy;
    minx = 10000;
    miny = 10000;
    maxx = -1;
    maxy = -1;
    for(int i = 0; i < 200; i++){
      for(int j = 0; j < 200; j++){
	if(image.at<Vec3b>(i, j)[2] >= 170 && image.at<Vec3b>(i, j)[2] <= 255 && image.at<Vec3b>(i, j)[0] >= 0 && image.at<Vec3b>(i, j)[0] <= 40 &&image.at<Vec3b>(i, j)[1] >= 0 && image.at<Vec3b>(i, j)[1] <= 40 ){
	  if(i < minx){
	    minx = i;
	  }
          if(i > maxx){
	    maxx = i;
	  }
          if(j < miny){
	    miny = j;
	  }
          if(j > maxy){
	    maxy = j;
	  }
	}
      }
    }
    cxred = (maxx + minx) / 2;
    cyred = (maxy + miny) / 2;
    // image.at<Vec3b>(cxred, cyred) = {255, 255, 255};
    minx = 10000;
    miny = 10000;
    maxx = -1;
    maxy = -1;
    for(int i = 0; i < 200; i++){
      for(int j = 0; j < 200; j++){
	if(image.at<Vec3b>(i, j)[1] >= 170 && image.at<Vec3b>(i, j)[1] <= 255 && image.at<Vec3b>(i, j)[0] >= 0 && image.at<Vec3b>(i, j)[0] <= 40 &&image.at<Vec3b>(i, j)[2] >= 0 && image.at<Vec3b>(i, j)[2] <= 40 ){
	  if(i < minx){
	    minx = i;
	  }
          if(i > maxx){
	    maxx = i;
	  }
          if(j < miny){
	    miny = j;
	  }
          if(j > maxy){
	    maxy = j;
	  }
	}	
      }
    }
    cxgreen = (maxx + minx) / 2;
    cygreen = (maxy + miny) / 2;
    // image.at<Vec3b>(cxgreen, cygreen) = {255, 255, 255};
    cout << cxred << " " << cyred << endl;
    cout << cxgreen << " " << cygreen << endl;

    //Real Program Starts
    int vis[200][200];
    int dis[200][200];
    int prevx[200][200];
    int prevy[200][200];
    int flag = 0;
    for(int i = 0; i < 200; i++){
      for(int j = 0; j < 200; j++){
	vis[i][j] = 0;
	 if(image.at<Vec3b>(i, j)[1] >= 240 && image.at<Vec3b>(i, j)[1] <= 255 && image.at<Vec3b>(i, j)[0] >= 240 && image.at<Vec3b>(i, j)[0] <= 255 && image.at<Vec3b>(i, j)[2] >= 240 && image.at<Vec3b>(i, j)[2] <= 255 ){
	   vis[i][j] = 1;
	 }
	 dis[i][j] = 100000;
      }
    }

    dis[cxred][cyred] = 0;
    int tempx, tempy;
    int dist = 100000;
    int alt;
 
    while(flag == 0){
      dist = 100000;
      for(int i = 0; i < 200; i++){
	for(int j = 0; j < 200; j++){
	  if(dis[i][j] < dist && vis[i][j] == 0){
	    dist = dis[i][j];
	    tempx = i;
	  tempy = j;
	  }
	  
	}
      }
      vis[tempx][tempy] = 1;
      // cout << tempx << " " << tempy << endl;
       for(int i = -1; i < 2; i++){
	for(int j = -1; j < 2; j++){
	  if(isPoint(tempx + i, tempy + j) == 1 && vis[tempx + i][tempy + j] == 0){
	    alt = dis[tempx][tempy] + abs(i) + abs(j);
	    if(alt < dis[tempx + i][tempy + j]){
	      dis[tempx + i][tempy + j] = alt;
	      prevx[tempx + i][tempy + j] = tempx;
	      prevy[tempx + i][tempy + j] = tempy;
	    }
	    if(tempx + i == cxgreen && tempy + j == cygreen){
	      flag = 1;
	    }
	  }
	}
	} 
      }

  ros::init(argc, argv, "talker");

  ros::NodeHandle n;

 
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Point>("chatter", 2000);

  ros::Rate loop_rate(10);

	int xj = cxgreen, yj = cygreen;
    	int xi, yi;
  	int count = 0;
  	while (1)
  	{
   
	        geometry_msgs::Point msg;
		if(xj == cxred && yj == cyred){
			break;
		}
   		msg.x = xj;
		msg.y = yj;
		xi = xj;
		yi = yj;
		xj = prevx[xi][yi];
		yj = prevy[xi][yi];
		cout << xi << " " << yi << endl;
    		//ROS_INFO("%d %d", msg.x, msg.y);
    		chatter_pub.publish(msg);
    		ros::spinOnce();
    		loop_rate.sleep();
    		++count;
  	}


  return 0;
}
