/*
 * @Author: robox-xx 1399786770@qq.com
 * @Date: 2023-03-22 20:56:06
 * @LastEditors: robox-xx 1399786770@qq.com
 * @LastEditTime: 2023-04-03 14:25:20
 * @FilePath: /rc_ws/src/tf_change/src/poseToOdom.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <std_msgs/Float64MultiArray.h>

std_msgs::Float64MultiArray the_best_way;

float tf_x;
float tf_y;
float the_send_x;
float the_send_y;
ros::Publisher RealWhereToshoot;

void LadarCallback(const std_msgs::Float64MultiArray msg)
{  
   std_msgs::Float64MultiArray the_best_goal;
   the_best_way = msg;
   // ROS_INFO("I reeeee x:%lf ,y:%lf, dis:%lf. ",the_best_way.data[0], the_best_way.data[1], the_best_way.data[2]);   
   the_send_x = tf_x+the_best_way.data[0];
   the_send_y = tf_y+the_best_way.data[1];
   the_best_goal.data.push_back(the_send_x);
   the_best_goal.data.push_back(the_send_y);
   //Release the location point where the car is going
   RealWhereToshoot.publish(the_best_goal);


   // ROS_INFO("tf x:%lf,y:%lf", tf_x,tf_y);
   // ROS_INFO("goal x:%lf,y:%lf", the_send_x,the_send_y);
   ROS_INFO("I send x:%lf,y:%lf",the_best_goal.data[0],the_best_goal.data[1]);
}

int main(int argc, char** argv)
{

   ros::init(argc, argv, "tf_listener");
   ros::NodeHandle  nh;
   ros::Subscriber radar_data;
   radar_data = nh.subscribe("laser_the_best_way_to_shoot", 5, LadarCallback);

   RealWhereToshoot = nh.advertise<std_msgs::Float64MultiArray>("/map/shootPosition", 5);
//Create a tf listener
   tf::TransformListener listener;
 
  ros::Rate rate(10.0);
  while (nh.ok()&&ros::ok()){
    ros::spinOnce();
    tf::StampedTransform transform;
    try{
       listener.lookupTransform("map", "radar_link",
                               ros::Time(0), transform);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
//Obtain relative information between coordinate systems
    tf_x = transform.getOrigin().x();
    tf_y = transform.getOrigin().y();
    rate.sleep();
  }
  return 0;
};