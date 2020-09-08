#include "acoplannerRecUdp.hpp"

namespace bigrobot1_acoplannerudp {
    bigrobot1_acoplannerudp::bigrobot1_acoplannerudp(const ros::NodeHandle &nh, const ros::NodeHandle &nh_private, std::string node_name){

        initialize();
    }

    void bigrobot1_acoplannerudp::initialize() {
        initSocket();   
        loop_timer_ = nh_.createTimer(ros::Duration(0.02), boost::bind(&bigrobot1_acoplannerudp::timerCb, this));
    }

    bool bigrobot1_acoplannerudp::initSocket() {
        int sockListen;
        if((sockListen = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            printf("socket fail\n");
            return -1;
            }
        int set = 1;
        setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
        struct sockaddr_in recvAddr;
        memset(&recvAddr, 0, sizeof(struct sockaddr_in));
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(4001);
        recvAddr.sin_addr.s_addr = INADDR_ANY;
        // 必须绑定，否则无法监听
        if(bind(sockListen, (struct sockaddr *)&recvAddr, sizeof(struct sockaddr)) == -1){
            printf("bind fail\n");
            return -1;
            }     

        std::cout << "Listen the broadcast haved finished. " <<std::endl;
        //close(sockListen);

        return true;
    }

   
    void bigrobot1_acoplannerudp::timerCb() {
        ROS_INFO_ONCE("udp commucation start");
        //sendmsgs();
        recvfrom();
    }
    
    void bigrobot1_acoplannerudp::recvfromudp(){
	ROS_INFO("recvive messages");

    int recvbytes;
	char recvbuf[128];
	int addrLen = sizeof(struct sockaddr_in);
	if((recvbytes = recvfrom(sockListen, recvbuf, 128, 0,(struct sockaddr *)&recvAddr, &addrLen)) != -1)
        {
		    recvbuf[recvbytes] = '\0';
		    printf("receive a broadCast messgse:%s\n", recvbuf);
	    }else{
		    printf("recvfrom fail\n");
	    }
	close(sockListen);
    }
//namespace carnet
}
    int main(int argc, char **argv) {
        std::string node_name = "bigrobot1_acoplannerudp";
        ros::init(argc, argv, node_name);
        ros::NodeHandle nh("");
        ros::NodeHandle nh_private("~");
        bigrobot1_acoplannerudp::bigrobot1_acoplannerudp reciever(nh, nh_private, node_name);
        ROS_INFO("initially recieved.");
        ros::spin();
    }
