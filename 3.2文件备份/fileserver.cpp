#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include <math.h> 
#include<iostream>
using namespace std;
#define PORT 13334
#define BACKLOG 5
#define MAXSIZE 32

class user{
    string username;
    string password;
};

int main(){
    time_t t;
    t=time(NULL);
    cout<<"Current Time: "<<ctime(&t)<<endl;
    cout<<"Waiting for the client to connect to this server..."<<endl;
    int sockfd, connectfd,num;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t addrlen;
 
 	char buf[MAXSIZE];
    // char dir[50];
    string dir;
	FILE * fd;
 
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

    addrlen = sizeof(client);
    if((connectfd = accept(sockfd, (struct sockaddr *)&client, &addrlen )) == -1){
        perror("accept() error!");
        return 0;
    }else{
        cout<<"It's Connected!"<<endl;
    }

    int response_len;
    char response[MAXSIZE];
    response_len=read(connectfd,response,3);
    response[response_len]='\0';

    cout<<"response: "<<response<<endl;

    if(strcmp("404",response)==0){
        cout<<"The server detected that the file specified by the client does not exist! File backup failed!"<<endl;
        return 0;
    }else{
        cout<<"Please input directory of copy: "<<endl;
        cin>>dir;
        fd=fopen(dir.c_str(),"wb");
        while(num=recv(connectfd,buf,1,0)>0)
        {
            fwrite(buf,1,num,fd);
        }
        fclose(fd);
        close(connectfd);
        cout<<"The file was successfully received by the server and the backup was successful!"<<endl;
        cout<<"please check in about 10 seconds, or later..."<<endl;
    }

	return 1;
}