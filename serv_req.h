#ifndef SERV_REQ_H
#define SERV_REQ_H

#define REQUEST_SIZE 1024

/* structure to store the request information */
struct req_info
{
    char *resource;
    int code;
};

/* function prototypes */
void Init_ReqInfo(struct req_info *req);
void Get_Request(int clnt_sock, struct req_info *req);
int Service_Request(int clnt_sock, char *path);

#endif /* SERV_REQ_H */
