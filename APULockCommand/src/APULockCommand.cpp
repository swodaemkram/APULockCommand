//============================================================================
// Name        : APULockCommand.cpp
// Author      : Mark Meadows
// Version     : 00.00.30
// Copyright   : Copyright (c) 2019 Mark MEadows
// Description : APULockCommand in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netinet/in.h>

using namespace std;

int timeout = 0;
int res;
int sock = socket(AF_UNIX, SOCK_STREAM, 0);

struct sockaddr_un address;

int main(int argc, char *argv[])
{
/*
==================================================================================================================
Lets process the startup commands
==================================================================================================================
*/
int procnumber = 0;
char domain_socket[250] = "";
char command[250] = "";


if (argv[1] == NULL)
	{
		printf("Must have Instance Number ! \n\n");
		exit(1);
	}
procnumber = atoi(argv[1]);

sprintf(domain_socket,"/var/run/APU_LockService%d.socket",procnumber);

if (argv[2] == NULL)
	{
		printf("Must have a command to send service\n\n");
	}

strncpy(command,argv[2],strlen(argv[2]));
/*
===================================================================================================================
Finished processing the startup commands
===================================================================================================================
connect and Send The Command
===================================================================================================================
*/

//----------------------Make connection or time out----------------------------------------------------------------
do
{
	memset(&address, 0x00, sizeof(address));
	address.sun_family = AF_UNIX;
	strncpy(address.sun_path, domain_socket, strlen(domain_socket));
	res = connect(sock, (struct sockaddr *)&address, sizeof(address));
	usleep(100000);
	if (timeout > 50)
	{
		printf("Time out check server ....!\n");
		return(1);
	}
timeout++;
}
while(res < 0);
//---------------------------End of make connection ---------------------------------------------------------------
//----------------------------Send pending command-----------------------------------------------------------------
printf("Command %s sent\n",command);
	//printf("connect = %d\n",res);//DEBUG
usleep(100000);
res = write(sock, command, strlen(command));
	//printf("write = %d\n",res);//DEBUG
	//close(sock);
	//return res;
//-----------------------------End of sending pending command------------------------------------------------------
//-----------------------------Get response from service-----------------------------------------------------------
int rval = 0;
char buf[1024] = {};

do
{
	bzero(buf, sizeof(buf));      //Zero out buffer
	    rval = read(sock, buf, 1024); //Read from the socket

	    if(rval > 0 )
		{
		printf("Response from service was %s\n",buf);
		}

}
while(rval < 0);
//-------------------------------End of Get Response-----------------------------------------------------------------
close(sock);
return(0);
}

















