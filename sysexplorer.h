
#ifndef _SYSEXPLORER_H
#define _SYSEXPLORER_H


/* Declarations */

//volatile __sig_atomic_t ext = 0; /* controls program termination */

typedef struct{
	char *method;
	char *directory;
	char *pattern;
}Request;


void termination_handler(int signum);

void* conn_thread(void* fd);

Request read_request(int fd);

#endif
