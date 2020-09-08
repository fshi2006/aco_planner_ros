//
// Created by lizhiwei on 20-06-22.
//

#ifndef ACOPLANNERUDP_HPP
#define ACOPLANNERUDP_HPP

#define NUM_ROBOT 3
#define BUF_SIZE  4096

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

//added for ros
#include <ros/ros.h>
#include "nav_msgs/Path.h"

#pragma pack(1)

typedef struct _Node{
    double x;
    double y;
}Node;

typedef struct Vline_{

    int ID;

    int size;

    std::vector<Node> SingleTaskNode_;

    int pack(char* buf)
    {
        //size = SingleTaskNode_.size();
        int index = 0;
        // memcpy(buf + index,this,8);index += 8;
        for (int i = 0 ; i< size ; i++)
        {
            Node node;
            memcpy(buf + index,&SingleTaskNode_[i],sizeof(node));
            index += sizeof(node);
            //std::cout<< "vline pack for. " <<std::endl;
        }
        return index;
    }

    int unpack(char* buf)
    {
        SingleTaskNode_.clear();

        int index = 0;
        // memcpy(buf + index,this,8);index += 8;
        for (int i = 0 ; i< size ; i++)
        {
            Node node;
            memcpy(&node,buf + index,sizeof(node));index += sizeof(node);
            SingleTaskNode_.push_back(node);
        }
        return index;
    }


}Vline;

struct Data{

    enum
    {
       FunNum =0xaabb,
    };

    int NumOfTotal;

    std::vector<Vline> TotalTaskNode_;

    bool unpack(char* buf)
    {
        TotalTaskNode_.clear();

        int index = 0;
        memcpy(this,buf + index,4);index += 4;

        for (int i = 0 ; i< NumOfTotal ; i++)
        {
            Vline_ line ;
            index += line.unpack(buf + index);
            TotalTaskNode_.push_back(line);
        }
        return true;
    }

    int pack(char* buf)
    {
        //std::cout<< "data pack start. " <<std::endl;
        //NumOfTotal = TotalTaskNode_.size();
        int index = 0;
        memcpy(buf + index,this,4);index += 4;
        for (int i = 0 ; i< NumOfTotal ; i++)
        {
            index += TotalTaskNode_[i].pack(buf + index);
            //std::cout<< "data pack for. " <<std::endl;
        }

        //std::cout<< "data pack end. " <<std::endl;
        return index;
    }

};

using namespace std;

namespace acoplannerudp {

    class acoplannerudp {

    public:
        acoplannerudp(const ros::NodeHandle &nh, const ros::NodeHandle &nh_private,
                 std::string node_name);

        // ros
        ros::NodeHandle nh_;
        ros::NodeHandle nh_private_;

        std::string node_name;
        ros::Timer  loop_timer_;
        ros::Subscriber path_sub;

        //UDP
        int UdpNetSocket;

        struct sockaddr_in remoteAddr;

        void initialize();

        void callbackFromWayPoints(const nav_msgs::Path msg);

        void timerCb();

        bool initSocket(void);

        void sendmsgs();

        double path_elapse_time_;

        int sendData( Data* data, char* buf );

    private:

        Data data;

        Node tmpTaskNode;

        Vline vline;

        char sendbuf[BUF_SIZE];

    };
}//namespace acoplannerudp

#pragma pack()

#endif //ACOPLANNERUDP_HPP


