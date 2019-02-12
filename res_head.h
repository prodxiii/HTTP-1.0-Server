#ifndef RES_HEAD_H
#define RES_HEAD_H

#include <sys/types.h>

#include "serv_req.h"

/* function prototypes */
int Output_HTTP_Headers(int clnt_sock, struct req_info *req, char *path);
ssize_t Writeline(int sockfd, const void *vptr, size_t n);

#endif /* RES_HEAD_H */
