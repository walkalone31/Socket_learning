#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc , char *argv[])

{
    //socket的建立
    char inputBuffer[256] = {};
    char message[] = {"Hi,this is server.\n"};
    int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    socklen_t addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    //serverInfo.sin_family = AF_INET;
    
    //serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_addr.s_addr = inet_addr("192.168.91.212");
    serverInfo.sin_port = htons(8080);


    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    //std::cout << "INADDR_ANY = " << INADDR_ANY <<std::endl;
    //std::cout << "INADDR_ANY htonl = " << htonl(INADDR_ANY) <<std::endl;
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverInfo.sin_addr.s_addr), str, INET_ADDRSTRLEN);
    std::cout << "Server IP = " << str << std::endl;
    std::cout << "Server Port = " << ntohs(serverInfo.sin_port) << std::endl;

    while(1){
        std::cout << "accept" << std::endl;
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        std::cout << "accepting..." << std::endl;
        char str2[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientInfo.sin_addr.s_addr), str2, INET_ADDRSTRLEN);
        std::cout << "client IP = " << str2 << std::endl;

        send(forClientSockfd,message,sizeof(message),0);

        while(true)
        {
            recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
            //printf("Get:%s\n",inputBuffer);
            std::cout << "feedback = " << inputBuffer << std::endl;
        }
    }
    return 0;
}
