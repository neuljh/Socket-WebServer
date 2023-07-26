#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<ctime>
#include<iostream>
using namespace std;
 
#define PORT 13333   /*listen port*/
 
int main ( int argc, char *argv[])
{
 	int sockfd;
	struct hostent *host;
 	struct sockaddr_in server;
 
	time_t t;
 	if(argc!=2){
  		printf("please input the ip address while computing the program!\n",argv[0]);
 		return 0;
	}
	 /*get ip address*/
 	if((host= gethostbyname(argv[1])) == NULL){
        herror("gethostbyname() error!\n");
        return 0;
 	}
 	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket() error! \n");
		return 0;
 	}
  	bzero(&server, sizeof(server));
 	server.sin_family = AF_INET;
 	server.sin_port = htons(PORT);
 	server.sin_addr = *((struct in_addr *)host->h_addr);
 
   	if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("connect() error !\n");
		return 0;
 	}
    if(recv(sockfd,(void *)&t,sizeof(time_t),0)==-1){
        perror("recv() error !\n");
		return 0;
    }
    cout<<"According to the TimeServer:Time is ---- "<<ctime(&t)<<endl;
    close(sockfd);
}