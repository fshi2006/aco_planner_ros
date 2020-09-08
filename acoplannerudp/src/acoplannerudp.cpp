#include "acoplannerudp.hpp"

namespace acoplannerudp {

    acoplannerudp::acoplannerudp(const ros::NodeHandle &nh, const ros::NodeHandle &nh_private, std::string node_name):nh_(nh), nh_private_(nh_private),
    node_name(node_name)
    {

        initialize();
    }

    void acoplannerudp::initialize() {

        initSocket();

        path_sub = nh_.subscribe("aco_planner_pub", 1, &acoplannerudp::callbackFromWayPoints, this);

        loop_timer_ = nh_.createTimer(ros::Duration(0.01), boost::bind(&acoplannerudp::timerCb, this));

    }

    bool acoplannerudp::initSocket() {

        this->UdpNetSocket =  socket(PF_INET, SOCK_DGRAM, 0);
        if(this->UdpNetSocket == -1){
            printf("socket fail\n");
            return false;
        } else{
            std::cout << "create UdpNetSocket succeed!" << std::endl;
        }

        int optval = 1;//这个值一定要设置，否则可能导致sendto()失败
        setsockopt(this->UdpNetSocket, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int));
        memset(&remoteAddr, 0, sizeof(struct sockaddr_in));
        this->remoteAddr.sin_family = AF_INET;
        this->remoteAddr.sin_addr.s_addr = inet_addr("255.255.255.255");//此处可以设置为全网的广播地址，例如255.255.255.255
        this->remoteAddr.sin_port = htons(4001);

        std::cout << "Listen the broadcast haved finished. " <<std::endl;

        return true;
    }

    void acoplannerudp::callbackFromWayPoints(const nav_msgs::Path msg) {

        this->data.TotalTaskNode_.clear();
        this->vline.SingleTaskNode_.clear();

        for(int j=1; j < NUM_ROBOT + 1; j++){
		
	    this->vline.ID = j;

            for (auto it : msg.poses) {

                std::string frame = std::to_string(j);

                if (it.header.frame_id == frame) {

                    this->tmpTaskNode.x = it.pose.position.x;
                    this->tmpTaskNode.y = it.pose.position.y;
		    //ROS_INFO("x= %f, y=%f", this->tmpTaskNode.x, this->tmpTaskNode.y);
                    this->vline.SingleTaskNode_.push_back(this->tmpTaskNode);


                }
                // this->vline.SingleTaskNode_.push_back(this->tmpTaskNode);

            }
            this->vline.size = this->vline.SingleTaskNode_.size();
	        std::cout<< "The data :"<< this->vline.size <<std::endl;
            //this->data.NumOfTotal =  this->vline.SingleTaskNode_.size();
        }
        //
        this->data.TotalTaskNode_.push_back(this->vline);
	    this->data.NumOfTotal = this->data.TotalTaskNode_.size();
	    std::cout<<"this->data:"<<this->data.NumOfTotal<<std::endl;
        

	sendmsgs();

    }

    void acoplannerudp::timerCb() {
        //ROS_INFO_ONCE("udp commucation start");
        //sendmsgs();

    }

    void acoplannerudp::sendmsgs() {
        ///因为循环周期从原来的0.02s改为0.01s，所以隔一次发送。
        ROS_INFO("sending messages");


        int len = sendData(&data, sendbuf);
        // for (int  i = 0; i < 63; i++)
        // {
        //     std::cout<<"buf:"<<sendbuf[i] <<std::endl;
        // }
        
	    

        std::cout<<"len:"<<len<<std::endl;

        int sendBytes = sendto(this->UdpNetSocket, this->sendbuf, len, 0,
                           (struct sockaddr *)&remoteAddr, sizeof(struct sockaddr));

        if(sendBytes == -1) {
            printf("sendto fail, errno=%d\n", errno);
        }
        //std::cout<< "The data :"<< this->data.NumOfTotal <<std::endl;
    }

    int acoplannerudp::sendData(Data* data, char *buf) {

        ROS_INFO("senddata to buf start.");

        int index = 0;

        index += data->pack( buf+ index);

        ROS_INFO("senddata to buf finished.");

        return index;
    }

}



int main(int argc, char **argv) {

        std::string node_name = "acoplannerudp_node";
        ros::init(argc, argv, node_name);
        ros::NodeHandle nh("");
        ros::NodeHandle nh_private("~");
        acoplannerudp::acoplannerudp sender(nh, nh_private, node_name);
        ROS_INFO("Initialized sender node.");
        ros::spin();
}
