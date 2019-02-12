#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>

/* custom header files for the resource.c */
#include <fcntl.h>
#include "serv_req.h"
#include "exit_msg.h"

/* function to check whether resource is present or not */
int Check_Resource(struct req_info *req, char *path)
{
    /* concatanate the path from the request to the parameter path */
    strcat(path, req->resource);

    /* open the file */
    return open(path, O_RDONLY);
}

/* function to return the contents of the resource */
int Return_Resource(int clnt_sock, int resource, struct req_info *req, char *path)
{
	int send_file;  /* variable to store the outcome of the sendfile */

	/*open the file and pointer set to the start of the file*/
    FILE *fd = fopen(path, "rb");
    long filesize;			 /* variable to store the size of the file */

    /* get the file size */
    fseek(fd, 0, SEEK_END); /* set the pointer to the end of the file */
    filesize = ftell(fd);   /* get the number of bytes in the file */
    fseek(fd, 0, SEEK_SET); /* set the pointer again to the start of the file */
    fclose(fd);

    /* use sendfile to both read and send the bytes of the file */
    send_file = sendfile(clnt_sock, resource, NULL, filesize);

    return send_file;
}
