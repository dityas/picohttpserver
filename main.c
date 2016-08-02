#include <signal.h>

#include "init_server.h"
#include "connection.h"
#include "fmodule.h"

int skt;

void sig_exit(int sig)
{
	s_log("Shutting down..\r\n");
	exit(0);
}

void main(int argc,char* argv[])
{
	signal(SIGINT,sig_exit);
	signal(SIGTERM,sig_exit);
	skt = strt_service(argv[1],HTTP);
	conn_accept(skt,HTTP);
	close(skt);
}

