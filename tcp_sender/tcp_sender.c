#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    // Think about the size of the string that is going to be accepted when using this application. 
    // Maybe create a check that checks so the string buffer isn't to big, 
    // or it splits the string into sendable pieces. 
    // Important to think about is that you need to put it together as well. 
    // Pretty huge issue to not have limitation on sending data.

    char *string;
    string = argv[3];
    if (strlen(string)>256) error("ERROR string to big");     
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port data\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    n = write(sockfd,string,strlen(string));
    if (n < 0) 
         error("ERROR writing to socket");

    n = read(sockfd,string,255);
    if (n < 0) 
         error("ERROR reading from socket");

    close(sockfd);
    return 0;
}
