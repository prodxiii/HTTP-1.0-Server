#include <stdio.h>
#include <stdlib.h>
#include "exit_msg.h"

/* exit due to user error */
void Exit_User(const char *msg, const char *detail)
{
	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}

/* exit due to system error */
void Exit_System(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
