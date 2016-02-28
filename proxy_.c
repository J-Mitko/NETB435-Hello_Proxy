#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#include "proxy_cache.h"

#define	QLEN 32	/* maximum connection queue length	*/ 
#define MAXBUF 8192 /* max buffer size*/

/* Thread lock */
pthread_rwlock_t mylock;	
	
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/*  do_proxy handles the http req from client 	
	cliendfd- connection file descriptor sent to the fucntion by thread */

void* do_proxy(void* fd)
{	
	int clientFD = (int)fd;
	int serverFD, port_ = 0;

	char *buffer_request = calloc(MAXBUF,1); //reads http request from client
	char *buffer_response = calloc(MAXBUF,1); // reads response from the server
	
	char *url = calloc(MAXBUF,1); // Resource identifier
	char *port = calloc(6,1);
	char *method = calloc(32,1); //only handle GET
	char *HTTP_version = calloc(8,1);
	char *hostname = calloc(MAXBUF,1);//www.nbu.bg
	
	char *TEMP = calloc(MAXBUF,1);
	
	char *file_name = calloc(MAXBUF,1);
	
	
	bzero((char*)buffer_request,MAXBUF);
	bzero((char*)buffer_response,MAXBUF);
	
	recv(clientFD,buffer_request,MAXBUF,0);
	sscanf(buffer_request,"%s %s %s\r\n",method, url, HTTP_version);

	//printf("\nBUFFER_Request: %s \n",buffer_request);
	
	// check validity of HTTP_version made by client
	if( (strncmp(method,"GET",3) == 0)	&&	(strncmp(HTTP_version,"HTTP/1.1",8) || (strncmp(HTTP_version,"HTTP/1.0",8)))	&&	((strncmp(url,"http://",7) == 0)) )
	{
			
		if(port_ <= 0 || port_ > 49151)
			port = "80";
		else
			sprintf(port, "%d", port_);// Converts an integer value string
		
		TEMP = strstr(buffer_request,"Host:");
		sscanf(TEMP,"Host: %s[^\r\n]", hostname);

		file_name = strrchr(url,'/');
		
		serverFD = connectTCP(hostname, port);
		//printf("\n IPPROTO_TCP=%d serverFD=%d \r\n",IPPROTO_TCP,serverFD);
		//-------------------------------------------------------------	
		int n;
		n = send(serverFD,buffer_request,strlen(buffer_request),0);
		
		//printf("\r\n file: %s \r\n", file_name+1);
		if(n < 0)
			error("ERROR Writing to socked");
		else
		{
			do
			{
				n = recv(serverFD,buffer_response,MAXBUF,0);
				//printf("%s", buffer_response);
				if(!(n<=0))
				{
					send(clientFD,buffer_response,n,0);
					
					if(file_name)
					{
						if(strstr(file_name,".jpg") || strstr(file_name,".jpeg") || strstr(file_name,".png") || strstr(file_name,".gif") || strstr(file_name,".html") || strstr(file_name,".pdf") )
						{
							pthread_rwlock_wrlock(&mylock);
								//printf("pthread_id = %u\r\n",pthread_self());
								write_file(file_name+1,buffer_response);
							pthread_rwlock_unlock(&mylock);
						}
					}
				}
			}while( n > 0);
		}	
	
	}
	else
		send(clientFD,"400 : BAD REQUEST\nONLY HTTP GET REQUESTS ALLOWED",18,0);

//Close the connection
	close(serverFD);
	close(clientFD);

}

int main(int argc, char *argv[])
{
	int sockfd, new_sockfd;
	const char *port = "9001";

	sockfd = passiveTCP(port,QLEN);
	if (sockfd < 0)
		error("ERROR opening socket");
		
	while(1)
	{
		if(new_sockfd = accept(sockfd, 0, 0))
		{
			pthread_t t;
			pthread_create(&t, 0, do_proxy, (void*)new_sockfd);
		}
	}


	close(new_sockfd);
	close(sockfd);
	
	return 0;
}
