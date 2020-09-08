//
// Created by  on 20-06-22.
//

#ifndef ACOPLANNERREC_HPP
#define ACOPLANNERREC_HPP

#define BUF_SIZE  4096

#include <ros/ros.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <queue>
#include <chrono>
#include <string>
#include <vector>

#include "std_msgs/Float32.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/PoseStamped.h"
#include "nav_msgs/Path.h"

using namespace std;

namespace bigrobot1_acoplannerudp {
    struct VehicleState {
        geometry_msgs::Point pose;
    };
    typedef struct _Node
    {
        float x;
        float y;
        _Node(float X,float Y)
        {
            x = X;
            y = Y;
            }
            _Node()
        {
            x = 0;
		    y = 0;
            }
    }Node;


    class bigrobot1_acoplannerudp {
    private:
    char recbuff[BUF_SIZE]
    public:
        bigrobot1_acoplannerudp(const ros::NodeHandle &nh, const ros::NodeHandle &nh_private,
                 std::string node_name);

        // ros
        ros::NodeHandle nh_;
        ros::NodeHandle nh_private_;
        ros::Timer loop_timer_;
        std::string node_name_{"udp_node"};

        ros::Publisher aco_pub;

        ros::WallTime last_timestamp_for_path_received;
        ros::WallTime last_timestamp_for_pose_received;


        vector<Node> TotalTaskNode;

        std::vector<plan2control_msgs::Traj_Node> global_path;
        plan2control_msgs::Trajectory trajectory;

        //// UDP Settings
        /* local address and port */
        int CarNetSocket;
        sockaddr_in addr_local;
        int local_port;

        /* remote address and port */
        sockaddr_in addr_remote, addr_car;
        socklen_t addr_remote_len; //// do not forget to init the 'addr_remote_len'
        std::string remote_ip, car_ip;
        int remote_port, car_port;

        void initialize();

        void timerCb();

        bool initSocket(void);

        void recvfromudp();

        std::string traj_topic_;

        std::string map_frame_id_normal_;

	std::string gps_topic_;

        double state_elapse_time_;
        double path_elapse_time_;
        double pose_elapse_time_;


    };
}//namespace bigrobot1_acoplannerudp

#endif //UDPCOMM_HPP


