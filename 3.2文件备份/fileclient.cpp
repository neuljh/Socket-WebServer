#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string>
#include<iostream>
#include<ctime>
using namespace std;
 
#define PORT 13334   /*listen port*/
#define MAXSIZE 32

class user{
    string username;
    string password;
};

int main ( int argc, char *argv[]){
    time_t t;
    t=time(NULL);
    cout<<"Current Time: "<<ctime(&t)<<endl;
    cout<<"The client is connecting to the server..."<<endl;
    int sockfd,num;
    struct hostent *he;
    struct sockaddr_in server;
 
	char buf[MAXSIZE];
    string dir;
 	// char dir[50];

    if(argc!=2){
        printf("please in put the ip address! \n",argv[0]);
        return 0;
    }
    /*get ip address*/
    if((he = gethostbyname(argv[1])) == NULL){
        herror("gethostbyname error !\n");
        return 0;
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket() error !\n");
        return 0;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    // cout<<"before"<<endl;
    server.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
    // cout<<"after"<<endl;

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("connect() error! \n");
        return 0;
    }

    cout<<"connect success!"<<endl;

    char res[MAXSIZE];
    // cout<<"input response: "<<endl;
    // fgets(res,sizeof(res),stdin);
    // write(sockfd,res,strlen(res));

    cout<<"Please input file's directory: "<<endl;
    cin>>dir;
	FILE *fd= fopen(dir.c_str(),"r+");
    if(fd==NULL){
        cout<<"The refered file didn't exist!"<<endl;
        strcpy(res,"404");
        cout<<"response: "<<res<<endl;
    }else{
        strcpy(res,"200");
        cout<<"response: "<<res<<endl;
    }
    write(sockfd,res,strlen(res));
    
    if(strcmp(res,"200")==0){
        while((num=fread(buf,1,MAXSIZE,fd))>0){
            send(sockfd,buf,num,0);
        }            
        cout<<"The file specified by the user has been sent to the server!"<<endl;
        cout<<"please check in about 10 seconds, or later..."<<endl;
        fclose(fd);
    }else{
        cout<<"The refered file didn't exist.Program terminated!"<<endl;
    }
 
	
    
    close(sockfd);
	return 1;
}