/*
  =========
  SERVER.C
  =========
  author        : Prodipta Guha
  student id    : 793023
  email         : pguha@student.unimelb.edu.au

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include <sys/socket.h> /* socket definations */
#include <sys/types.h>  /* socket types */
#include <arpa/inet.h>  /* inet functions */
#include <unistd.h>		/* misc. UNIX functions */

/* custom header files for the server.c */
#include "exit_msg.h"
#include "serv_req.h"

int main(int argc, char *argv[])
{
	/* socket descriptor for server and client */
	int serv_sock, clnt_sock;
	/* serv_addr -> local address & clnt_addr -> remote address*/
	struct sockaddr_in serv_addr, clnt_addr;
	/* to enable multithreading */
	pid_t process_id;
	/* store the path to web root */
	char *path;
	/* local Port to bind */
	in_port_t serv_port;

	/* check for correct number of arguments */
	if (argc <= 2)
		Exit_User("Parameter(s)", "[Server Port] [Path to web root]");

	/* get parameters from the command line interface */
	serv_port = atoi(argv[1]);
	path = argv[2];

	/* create socket for incoming connections */
	if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		Exit_System("socket() failed");

	/* populate socket address structure */
	memset(&serv_addr, 0, sizeof(serv_addr));	  /* zero out structure */
	serv_addr.sin_family = AF_INET;				   /* IPv4 address family */
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	serv_addr.sin_port = htons(serv_port);		   /* Local Port */

	/* bind socket ato the local address */
	if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		Exit_System("bind() failed");

	/* mark the socket so it will listen for incoming connections */
	if (listen(serv_sock, 5) < 0)
		Exit_System("listen() failed");

	/* loop infinitely to accept and service connections */
	while (1)
	{

		/* set length of client address structure */
		socklen_t clnt_addrLen = sizeof(clnt_addr);

		/* wait for client to connect */
		if ((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addrLen)) < 0)
			Exit_System("accept() failed");

		/* clnt_sock is connected to a client */

		/* report connected client */
		char clnt_name[INET_ADDRSTRLEN]; /* string to contain client address */

		/* convert binary representation of the client's address to dotted-quad string */
		if (inet_ntop(AF_INET, &clnt_addr.sin_addr.s_addr, clnt_name, sizeof(clnt_name)) != NULL)
			printf("Handling client %s/%d\n", clnt_name, ntohs(clnt_addr.sin_port));
		else
			puts("Unable to get client address");

		/* fork process id to create clone of the current process */
		if ((process_id = fork()) == 0)
		{
			/*  this is now the forked child process, so
			close listening socket and service request */

			if (close(serv_sock) < 0)
				Exit_System("SERVER SOCKET FAILED IN CHILD PROCESS (close() failed)");

			/* this is the function that abstracts */
			Service_Request(clnt_sock, path);

			/*  close connected socket and exit  */

			if (close(clnt_sock) < 0)
				Exit_System("CLIENT SOCKET FAILED IN SERVER PROCESS (close() failed)");

			exit(EXIT_SUCCESS);
		}

		/*  if we get here, we are still in the parent process,
	    so close the connected socket, clean up child processes,
	    and go back to accept a new connection. */
		if (close(clnt_sock) < 0)
			Exit_System("SERVER SOCKET FAILED IN PARENT PROCESS (close() failed)");

		/* wait for process id to change state */
		waitpid(-1, NULL, WNOHANG);
	}

	return 0;
}
/***************************PROGRAMMING IS FUN***************************/
