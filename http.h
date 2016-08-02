#ifndef HTTP_H
#define HTTP_H

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "../netfunct.h"
#include "fmodule.h"

void srv_rq(unsigned char* req,int c_sd);

void srv_head(int skt)
{
	unsigned char rep[50] = "HTTP/1.1 ";
	strcat(rep,ok_header);
	net_send(skt,rep);
	net_send(skt,info_header);
}

void srv_get(unsigned char* req,int skt)
{
	unsigned char* mime;
	unsigned char* dir = &req[4];
	int es;
	req[strlen(req)-1] = '\0';
	if (req[strlen(req)-1] == '/')
		strcat(req,"index.html");
	mime = strstr(req,".");
	printf("%s\r\n",mime);
	fserve(skt,dir,mime);
	printf("Done servring\r\n");
}

void httpd(int c_sd)
{
	int version,len;
	unsigned char buff[1024];
	len = recv(c_sd,buff,1024,0);
	if((version = check_req(buff)) == -1)
		printf("Invalid request\r\n");
	srv_rq(buff,c_sd);
}

int check_req(unsigned char* buff)
{
	unsigned char* ver;
	unsigned char log[40];
	if((ver = strstr(buff,"HTTP/")) == NULL)
	{
		return -1;
	}
	*ver  ='\0';
	sprintf(log,"%s\r\n",buff);
	s_log(log);
	return 0;
}

void srv_rq(unsigned char* req,int c_sd)
{
	unsigned char* rq = req;
	if((strncmp(rq,"GET ",4)) == 0)
		srv_get(rq,c_sd);
	else if((strncmp(rq,"HEAD ",5)) == 0)
		srv_head(c_sd);
}

#endif
