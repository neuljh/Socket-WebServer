#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<string>
using namespace std;

#define PORT 13333//The server listens on the port number, because the port number is less than 10000 is the port used by the operating system
#define BACKLOG 10//Maximum number of simultaneous connection requests

void write_file(const char* filename,char* ip_address,char* time){
    FILE *fd=fopen(filename,"a+");
    fprintf(fd,"IP Address: %s | Time: %s \n",ip_address,time);
	fclose(fd);
}


int main()
{
    cout<<"Waiting for the client to connect to this server..."<<endl;
    int sockfd;
	int connectfd;
	struct sockaddr_in server;//Server, local address information
	struct sockaddr_in client;//Client address information
	socklen_t addrlen;
	
	time_t t;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket() error!");
		return 0;
	}
 
	int opt = SO_REUSEADDR;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
 
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1){
		perror("bind() error!");
	    return 0;
	}
	if(listen(sockfd, BACKLOG) == -1){
		perror("listen() error!");
		return 0;
	}
    while(true){
        addrlen = sizeof(client);
        if((connectfd = accept(sockfd, (struct sockaddr *)&client, &addrlen )) == -1){
    		perror("accept() error!");
		    continue;
   	    }
        t=time(NULL);
        cout<<"**************************************************"<<endl;
        cout<<"Received a connection from :"<<inet_ntoa(client.sin_addr)<<endl;
        cout<<"Timeclient's IP is: "<<inet_ntoa(client.sin_addr)<<endl;
        cout<<"The current time is: "<<ctime(&t)<<endl;
        cout<<"**************************************************"<<endl;
        const char* filename="/home/neu_ljh/timerecords";
        write_file(filename,inet_ntoa(client.sin_addr),ctime(&t));
        if(!fork()){
            if(send(connectfd,(time_t *)&t,sizeof(time_t),0)==-1){
                perror("send() error!");
            }
            close(connectfd);
            return 0;
        }
        close(connectfd);
    }
	return 1;
}	