#ifndef INIT_SERVER_H
#define INIT_SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../netfunct.h"
#include "fmodule.h"
#include <stdio.h>
#include <stdlib.h>

#define HTTP	80

int strt_service(unsigned char* ip,int service)
{
	int svr_skt,opt=1;
	struct sockaddr_in server;
	socklen_t sin_size;

	if((svr_skt = socket(PF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("While creating server socket");
		exit(EXIT_FAILURE);
	}

	printf("Server socket: %d \r\n",svr_skt);

	if((setsockopt(svr_skt,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int))) == -1)
	{
		perror("While setting socket options");
		exit(EXIT_FAILURE);
	}

	printf("Options set... \r\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(HTTP);
	inet_aton(ip,&server.sin_addr);

	if((bind(svr_skt,(struct sockaddr*)&server,sizeof(struct sockaddr))) == -1)
	{
		perror("While binding to given address");
		exit(EXIT_FAILURE);
	}

	if((listen(svr_skt,20)) == -1)
	{
		perror("While listening on port");
		exit(EXIT_FAILURE);
	}

	printf("Sever listening on %s port %d... \r\n",ip,service);
	s_log("Starting up...\r\n");
	return svr_skt;
}

#endif
