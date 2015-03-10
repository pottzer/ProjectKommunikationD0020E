#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Code written by 
// Pontus Ridderström,
// Fredrik Grönlund

// This function tests the socket sending mechanism.
// The idea is that we connect to an node,
// who recieves the message and passes it along to the
// HTTP server via WebRTC. It currently does this via the 
// client.c and server.c files.
//
// Correct calling syntax:
// ./test_unit help
// ./test_unit ip-addr port no-of-tests
int main(int argc, char *argv[])
{
    //some checks to make sure the call is sensible

    //No arguments
    if(argc < 2)
    {
	printf("Error: no arguments!\n");
	printf("Try calling %s help\n", argv[0]);
	return 1;
    }
    //help
    if(strcmp(argv[1],"help") == 0)
    {
    	printf("Correct call format:\n");
	printf("%s ipaddress port amountoftests\n", argv[0]);
	return 0;
    }
    //too few arguments
    if(argc < 4)
    {
	printf("Error: too few arguments!\n");
	printf("Try calling %s help\n", argv[0]);
	return 1;
    }
    //parse as # of repetitions
    int tot = atoi(argv[3]);
    int i, status;
    char address[63];
    char buffer[63];
    
    //Parse the address string
    snprintf(address, sizeof (address), 
	    "%s %s", argv[1], argv[2]);

    //Try to send the number an amount of times equal to tot
    //via the client.c socket module included in the folder
    for(i = 0; i < tot; i++)
    {        
        sprintf(buffer, "./tcp_sender %s %d", address, i);
	//printf("[DEBUG] buffer: %s", buffer);
        status = system(buffer); 
        printf("%d\n", i);        
    }
    return 0;
}
