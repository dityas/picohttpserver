#ifndef FMODULE_H
#define FMODULE_H

#define WEBROOT	"/home/aditya/Network/docs"
#define LOGDIR "/home/aditya/Network/docs/server.log"

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "../netfunct.h"
#include <time.h>

unsigned char ok_header[] = "HTTP/1.1 200 OK\r\n";
unsigned char nf_header[] = "HTTP/1.1 404 NOT FOUND\r\n";
unsigned char info_header[] = "Server: d@Rk m0dul3\r\n";

unsigned char* mimes[] = {".html\0",".css\0",".jpg\0"};
unsigned char* h_mime[] = {"Content-Type: text/html\r\n","Content-Type: text/css\r\n","Content-Type: image/jpg\r\n"};

void timestp(int fd)
{
	unsigned char timestamp[40];
	memset(timestamp,'\0',strlen(timestamp));
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(timestamp,"%d-%d-%d %d:%d:%d\r\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	write(fd,timestamp,strlen(timestamp));
}

int get_mime(unsigned char* mime)
{
	int i;
	for(i=0;i<3;i++)
	{
		if((strcmp(mime,mimes[i])) == 0)
			return i;
	}
	return -1;
}

void s_log(unsigned char* wlog)
{
	int logfd = open(LOGDIR,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
	timestp(logfd);
	write(logfd,wlog,strlen(wlog));
	close(logfd);
}

void send_nfhead(int sd)
{
	net_send(sd,nf_header);
	net_send(sd,info_header);
	net_send(sd,"\r\n");
	net_send(sd,"<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1></body></html>");
}

void send_okhead(int sd)
{
	net_send(sd,ok_header);
	net_send(sd,info_header);
}

void send_head(int sd,unsigned char* mime,int fsize)
{
	int s_no;
	unsigned char len[30];
	sprintf(len,"Content-Length: %d\r\n",fsize);
	printf("%s\r\n",len);
	s_no = get_mime(mime);
	if(s_no == -1)
		send_nfhead(sd);
	send_okhead(sd);
	net_send(sd,h_mime[s_no]);
	net_send(sd,len);
	net_send(sd,"\r\n");
}

int get_size(int fd)
{
	struct stat size;
	if ((fstat(fd,&size)) == -1)
		return -1;
	return (int)size.st_size;
}

int fserve(int skt,unsigned char* path,unsigned char* mime)
{
	if (mime == NULL)
	{
		send_nfhead(skt);
		return -1;
	}
	unsigned char* fstream;
	int fd,fsize;
	unsigned char dir[50] = "";

	strcat(dir,WEBROOT);
	printf("%s\r\n",dir);
	strcat(dir,path);
	printf("%s\r\n",dir);

	if((fd = open(dir,O_RDONLY,0)) == -1)
		send_nfhead(skt);
	if((fsize = get_size(fd)) == -1)
	{
		printf("File may be curropted\r\n");
		return -1;
	}
	if((fstream = malloc(sizeof(unsigned char)*fsize)) == NULL)
	{
		printf("Out of memory\r\n");
		return -1;
	}
	read(fd,fstream,fsize);
	close(fd);
	printf("done reading file\r\n");
	send_head(skt,mime,fsize);
	net_send(skt,fstream);
	printf("done sending\r\n");
	free(fstream);
	return 0;
}

#endif


