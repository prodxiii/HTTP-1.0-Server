#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

/* custom header files for the res_head.c */
#include "res_head.h"
#include "serv_req.h"
#include "exit_msg.h"

/* function to send all the reponse headers */
int Output_HTTP_Headers(int clnt_sock, struct req_info *req, char *path)
{
    /* buffer to store the response to send to the client */
    char buffer[100];

    /* all the mime types to check for content type */
    char *html = ".html", *css = ".css", *jpg = ".jpg", *js = ".js";

    /* choose status code based on whether resource is available or not */
    if (req->code == 404)
        sprintf(buffer, "HTTP/1.0 %d\r\n", req->code);
    else
        sprintf(buffer, "HTTP/1.0 %d OK\r\n", req->code);

    /* send the buffer to the client */
    Writeline(clnt_sock, buffer, strlen(buffer));

    /* choose content type */
    if (strstr(path, html) != NULL)
        Writeline(clnt_sock, "Content-Type: text/html\r\n", 25);
    if (strstr(path, jpg) != NULL)
        Writeline(clnt_sock, "Content-Type: image/jpeg\r\n", 26);
    if (strstr(path, css) != NULL)
        Writeline(clnt_sock, "Content-Type: text/css\r\n", 24);
    if (strstr(path, js) != NULL)
        Writeline(clnt_sock, "Content-Type: text/javascript\r\n", 31);

    /* final carriage return and line feed before sending out content */
    Writeline(clnt_sock, "\r\n", 2);
    return 0;
}

/* used to be system function in linux: writes till  the current line terminator to the standard output stream.
 * acknowledgement to Unix Network Programming: 1 by W. Richard Stevens*/
ssize_t Writeline(int sockfd, const void *buf, size_t n)
{
    size_t nleft;
    ssize_t num_written;
    const char *buffer;

    buffer = buf;
    nleft = n;

    while (nleft > 0)
    {
        num_written = write(sockfd, buffer, nleft);
        if ((num_written) <= 0)
        {
            if (errno == EINTR)
                num_written = 0;
            else
                Exit_System("UNABLE TO WRITE (Writeline() failed)");
        }
        nleft -= num_written;
        buffer += num_written;
    }

    return n;
}
