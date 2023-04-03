/*
 * @Author: robox-xx 1399786770@qq.com
 * @Date: 2023-03-22 20:56:06
 * @LastEditors: robox-xx 1399786770@qq.com
 * @LastEditTime: 2023-04-03 14:25:48
 * @FilePath: /rc_ws/src/tf_change/src/tf_relationship.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>

std::string turtle_name;

void poseCallback(const turtlesim::PoseConstPtr& msg)
{
    static tf::TransformBroadcaster br;

    tf::Transform transform;

    transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
    tf::Quaternion q;
    q.setRPY(0, 0, msg->theta);
 
    transform.setRotation(q);

    // Insert coordinate transformations into the TF tree and publish coordinate transformations
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "radar_link"));
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "dynamic_tf_pub");
    if (argc != 2)
    {
        ROS_ERROR("need turtle name as argument"); 
        return -1;
    };
    turtle_name = argv[1];

    ros::NodeHandle node;
    //Subscribe to the location information of the car
    ros::Subscriber sub = node.subscribe("/acml_pose", 10, &poseCallback);

    ros::spin();

    return 0;
};
