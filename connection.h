#ifndef CONNECTION_H
#define CONNECTION_H

#define THRD_LMT	10

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "fmodule.h"
#include "http.h"


void *service_thread(void* ptr_sd)
{
	int* sd = (int*)ptr_sd;
	httpd(*sd);
	shutdown(*sd,SHUT_RDWR);
	pthread_exit(NULL);
}


void conn_accept(int svr,int service)
{
//	daemon(1,0);
	int c_sd,i,status;
	pthread_t svc_thread[THRD_LMT];
	struct sockaddr_in client;
	socklen_t sin_size;

	while(1)
	{
		char log_buff[500];
		sin_size = sizeof(struct sockaddr_in);
		for (i=0;i<THRD_LMT;i++)
		{
			memset(log_buff,'\0',500);
			c_sd = accept(svr,(struct sockaddr*)&client,&sin_size);
			sprintf(log_buff,"Connected: %s \r\nThreading the connection\r\n",inet_ntoa(client.sin_addr));
			s_log(log_buff);
			status = pthread_create(&svc_thread[i],NULL,service_thread,(void*)&c_sd);
			if(status)
			{
				sprintf(log_buff,"Cannot thread the connection\r\n");
				s_log(log_buff);
			}
			else
			{
				sprintf(log_buff,"Threading successful!\r\n");
				s_log(log_buff);
			}
		}
		sprintf(log_buff,"Reached thread limit, don't kno what will happen to running processes, let's see :P\r\n");
		s_log(log_buff);
	}
}

#endif
