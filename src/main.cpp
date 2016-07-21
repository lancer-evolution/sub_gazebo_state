#include <iostream>
#include <fstream>
#include "ros/ros.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/ModelStates.h>

using namespace std;

bool received_gazebo_message = false;
geometry_msgs::Pose model_pose;
geometry_msgs::Point model_position;

void pos2csv(geometry_msgs::Point pos){
  ofstream outfile("kobuki_gazebo.xlsx",std::ios::app | std::ios::binary | std::ios::ate);
  if(!outfile){
	cout << "ファイルをオープンできません" << endl;
  }
  outfile << pos.x << ","
		  << pos.y << ","
		  << pos.z << "," << endl;
  outfile.close();
}


void chatterCallback(const gazebo_msgs::ModelStatesPtr& model_states)
{
    int i = 0;

	if (!received_gazebo_message){
	  std::cout << "Gazebo model info received!" << std::endl;
    }
	
    for (unsigned int i = 0; i < model_states->name.size(); ++i)
    {
	  //cout << "[" << i << "]:" <<  model_states->name[i] << endl;
        if (model_states->name[i] == "kobuki")
        {
            model_pose = model_states->pose[i];
			model_position = model_pose.position;
			cout << model_position.x << endl;
			pos2csv(model_position);
            received_gazebo_message = true;
            break;
        }
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("gazebo/model_states",1, &chatterCallback);
  ROS_INFO("IN ");
  
  ros::spin();
  
  return 0;
}
