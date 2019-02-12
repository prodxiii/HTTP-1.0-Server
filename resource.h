#ifndef RESOURCE_H
#define RESOURCE_H

int Check_Resource(struct req_info *req, char *path);
int Return_Resource(int clnt_sock, int resource, struct req_info *req, char *path);

#endif /* RESOURCE_H */
