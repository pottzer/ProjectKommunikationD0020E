//c includes for socketserverlistener :P:P
#ifndef MYHEADER_H
#define MYHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif /* MYHEADER_H */

class ListenerSocket
{
	public:	
		int sockfd, newsockfd, portno;
		socklen_t clilen;
		struct sockaddr_in serv_addr, cli_addr;
		int n;

	//initilaze c functions for socketserver listener
	public:
		ListenerSocket();
		int initSocketServer(int port);
		char *listenPort();
		void error(const char *msg);

};


