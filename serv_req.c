#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>

/* custom header files for the serv_req.c */
#include "serv_req.h"
#include "exit_msg.h"
#include "resource.h"

/* function to abstract request, respond headers and resource */
int Service_Request(int clnt_sock, char *path)
{
    /* struct to store the information from the request */
    struct req_info *req;
    int resource = 0; /* whether resource is present or not */

    /* create memory to store the struct information */
    req = malloc(sizeof(struct req_info));

    /* initialize the struct */
    Init_ReqInfo(req);

    /* get HTTP request */
    Get_Request(clnt_sock, req);

    /* if status is 200 OK check the correct resource */
    if (req->code == 200)
    {

        if ((resource = Check_Resource(req, path)) < 0)
        {
            req->code = 404;
        }
    }

    /* print out the http response headers */
    Output_HTTP_Headers(clnt_sock, req, path);

    /* if status is 200 OK return the correct resource */
    if (req->code == 200)
    {
        if (Return_Resource(clnt_sock, resource, req, path) < 1)
        {
            Exit_System("UNABLE TO RETURN RESOURCE (Return_Resource() failed`)");
        }
    }

    /* free the memory for the struct req */
    free(req);


    if (resource > 0)
    {
        if (close(resource) < 0)
        {
            Exit_System("UNABLE TO CLOSE RESOURCE (close() failed)");
        }
    }

    return 0;
}

/*  Initialises a request information structure  */
void Init_ReqInfo(struct req_info *req)
{
    req->resource = malloc(sizeof(char) * 256);
    req->code = 200;
}

/* Get the request from the client */
void Get_Request(int clnt_sock, struct req_info *req)
{
    char buffer[REQUEST_SIZE] = {0};
    int read_req;
    char *req_type;

    /* read the request and store it in the buffer */ 
    read_req = read(clnt_sock, buffer, REQUEST_SIZE - 1);
    if (read_req < 0)
        Exit_System("CAN NOT READ FROM CLIENT (read() failed)");

    /* get request type and file name */
    req_type = strtok(buffer, " ");
    strcpy(req->resource, strtok(NULL, " "));
}
