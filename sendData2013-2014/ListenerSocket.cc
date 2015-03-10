
#include "ListenerSocket.h"

//using c-code in c++. Define c-variables
extern "C"
{
	//variables for initializing and listening
	int sockfd, newsockfd, portno;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;
	int n;

	void error(const char);
	int initSocketServer(int); 
	char *listenPort();
};


ListenerSocket::ListenerSocket()
{
	
}

//Opens port for listening on incoming data
int ListenerSocket::initSocketServer(int port)
{

 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno=port;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	return 0;
}

//prints data from tcp-connection
char *ListenerSocket::listenPort()
{
	char *buffer = (char *)malloc(256);
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) error("ERROR on accept");
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);

	//add Data to priority Queue

	n = write(newsockfd,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");

	close(newsockfd);
	


	return buffer;
}

void ListenerSocket::error(const char *msg)
{
    perror(msg);
    exit(1);
}
