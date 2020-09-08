#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
int main(){
	char msg[128] = "I am broadCast message from server!";
	int brdcFd;
	if((brdcFd = socket(PF_INET, SOCK_DGRAM, 0)) == -1){
		printf("socket fail\n");
		return -1;
	}
	int optval = 1;//这个值一定要设置，否则可能导致sendto()失败
	setsockopt(brdcFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int));
	struct sockaddr_in theirAddr;
	memset(&theirAddr, 0, sizeof(struct sockaddr_in));
	theirAddr.sin_family = AF_INET;
	theirAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
	theirAddr.sin_port = htons(4001);
	int sendBytes;
	if((sendBytes = sendto(brdcFd, msg, strlen(msg), 0,
			(struct sockaddr *)&theirAddr, sizeof(struct sockaddr))) == -1){
		printf("sendto fail, errno=%d\n", errno);
		return -1;
	}
	printf("msg=%s, msgLen=%d, sendBytes=%d\n", msg, strlen(msg), sendBytes);
	close(brdcFd);
	return 0;
}