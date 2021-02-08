#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h> // for thread

//#include <iostream>

int thread_count = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *doAccept(void *arg)
{

	pthread_mutex_lock( &mutex1 );//lock
	thread_count++;
	printf("Number of thread: %d\n", thread_count);
	pthread_mutex_unlock( &mutex1 );//unlock

	int forClientSockfd = *((int *) arg);
        char message[] = {"Hi,this is server.\n"};

	send(forClientSockfd,message,sizeof(message),0);

	while(true)
	{

		char inputBuffer[256] = {};
		recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
		printf("================================================\n");
		printf("Get:%s\n",inputBuffer);

/*
		printf("1:%d\n",(int)inputBuffer[0]);
		printf("2:%d\n",(int)inputBuffer[1]);
		printf("3:%d\n",(int)inputBuffer[2]);
		printf("4:%d\n",(int)inputBuffer[3]);
		printf("5:%d\n",(int)inputBuffer[4]);
		printf("6:%d\n",(int)inputBuffer[5]);
		printf("7:%d\n",(int)inputBuffer[6]);
		printf("8:%d\n",(int)inputBuffer[7]);
		printf("9:%d\n",(int)inputBuffer[8]);
		printf("10:%d\n",(int)inputBuffer[9]);
		printf("11:%d\n",(int)inputBuffer[10]);
*/
		char send_message[256];
		char command1[30] = {"cpu_name\r\n"};
		char command2[30] = {"apt_update\r\n"};

/*
		printf("=======================\n");
		printf("1:%d\n",(int)command1[0]);
		printf("2:%d\n",(int)command1[1]);
		printf("3:%d\n",(int)command1[2]);
		printf("4:%d\n",(int)command1[3]);
		printf("5:%d\n",(int)command1[4]);
		printf("6:%d\n",(int)command1[5]);
		printf("7:%d\n",(int)command1[6]);
		printf("8:%d\n",(int)command1[7]);
		printf("9:%d\n",(int)command1[8]);
		printf("10:%d\n",(int)command1[9]);
		printf("11:%d\n",(int)command1[10]);
*/

		memset(send_message, 0, 255);

		if((int)inputBuffer[0] == 13 && (int)inputBuffer[1] == 10) //[0] = '\r', [1] = '\n' 
		{
			//send_message = "do nothing...\n";
			strcat(send_message, "do nothing...\n");
		}
		else if( strlen(inputBuffer) == 0 )
		{
			strcat(send_message, "break...\n");
			break;
		}
		else if( (int)inputBuffer[0] == 113 &&
			 (int)inputBuffer[1] == 13  && 
			 (int)inputBuffer[2] == 10 ) //[0] = 'q', [1] = '\r', [2] = '\n'
		{
			//send_message = {"quit...\n"};
			strcat(send_message, "quit...\n");
			break;
		}
		else if( strcmp(inputBuffer,command1) == 0 ) // command1 = "cpu_name\r\n"
		{
			    system("lscpu | sed -nr '/Model name/ s/.*:\s*(.*) @ .*/\1/p'");

			    strcat(send_message, "cpu_name \n");
		}
		else if( strcmp(inputBuffer,command2) == 0 ) // command1 = "cpu_name\r\n"
		{
			    system("echo walk74123 | sudo -S apt-get update");

			    strcat(send_message, "apt_update \n");
		}
		else
		{
			//send_message = {"I got message ! \n"};
			strcat(send_message, "I got message ! \n");
		}

		send(forClientSockfd,send_message,sizeof(send_message),0);
	}

	pthread_mutex_lock( &mutex1 );//lock
	thread_count--;
	printf("number of thread: %d\n", thread_count);
	pthread_mutex_unlock( &mutex1 );//unlock

	pthread_exit(NULL);

}

//void doConnect(int sockfd)
void *doConnect(void *arg)
{
	int sockfd = *((int *) arg);
        printf("accepting..\n");

        char str2[1024];
        //char message[] = {"Hi,this is server.\n"};
        int forClientSockfd = 0;
        struct sockaddr_in clientInfo;
        socklen_t addrlen = sizeof(clientInfo);

	pthread_t thread;
	//int status;

	while(true)
	{

		forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);

		inet_ntop(AF_INET, &(clientInfo.sin_addr.s_addr), str2, 1024);
		printf("client IP: %s\n", str2);

		int *i = (int *)malloc(sizeof(*i));
		*i = forClientSockfd;

		pthread_create(&thread, 0, doAccept, (void *) i);
		//pthread_join(thread, (void*)&status);

		pthread_mutex_lock( &mutex1 );//lock
		printf("number of thread in while: %d\n", thread_count);
		pthread_mutex_unlock( &mutex1 );//unlock

		
	}

/*
        send(forClientSockfd,message,sizeof(message),0);

        while(true)
        {

                char inputBuffer[256] = {};
                recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
                printf("================================================\n");
                printf("Get:%s\n",inputBuffer);

                char send_message[256];
                memset(send_message, 0, 255);
                if((int)inputBuffer[0] == 13 && (int)inputBuffer[1] == 10) //[0] = '\0', [1] = '\n' 
                {
                        //send_message = "do nothing...\n";
                        strcat(send_message, "do nothing...\n");
                }
                else if( (int)inputBuffer[0] == 113 &&
                                (int)inputBuffer[1] == 13  && 
                                (int)inputBuffer[2] == 10 ) //[0] = 'q', [1] = '\0', [2] = '\n'
                {
                        //send_message = {"quit...\n"};
                        strcat(send_message, "quit...\n");
                        break;
                }
                else
                {
                        //send_message = {"I got message ! \n"};
                        strcat(send_message, "I got message ! \n");
                }

                send(forClientSockfd,send_message,sizeof(send_message),0);
        }

*/

        //std::cout << "end of connect ..." << std::endl;
        printf("end of connect\n");
}



int main(int argc , char *argv[])

{
    //socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in serverInfo;
    //socklen_t addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    //serverInfo.sin_family = AF_INET;
    
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    //serverInfo.sin_addr.s_addr = inet_addr("192.168.91.212");
    serverInfo.sin_port = htons(8080);


    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,3);

    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverInfo.sin_addr.s_addr), str, INET_ADDRSTRLEN);
    printf("Server IP:%s\n",str);
    printf("Server Port:%d\n",ntohs(serverInfo.sin_port));



    pthread_t thread_main;
    //int status_main;
    int *sockfd_p = (int *)malloc(sizeof(*sockfd_p));
    *sockfd_p = sockfd;

    pthread_create(&thread_main, 0, doConnect, (void *) sockfd_p);
    pthread_join(thread_main, NULL);
    //doConnect(sockfd);

    printf("End of process\n");

    return 0;
}


