#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include "search.h"

char* find(char *pattern, char *directory){
    
    printf("pattern = %s\n", pattern);
    printf("directory = %s\n", directory);
	int fd[2];
	pid_t pid;
	char buffer[4096];

	if(pipe(fd) != 0)			
		printf("broken pipe\n"); //return -1;
	if((pid = fork()) <0)
		printf("Unable to fork\n"); //return -1;

        //printf("pid = %d\n",pid);
	if(pid == 0){
		close(fd[0]);
		dup2(fd[1], 1); 
        execl("/usr/bin/find", "find", directory, "-name", pattern, "-print", NULL);
        printf("execl\n");
	}else{
           // printf("pateras\n");
		close(fd[1]);
		dup2(fd[0],0);
		waitpid(pid,NULL,0);


		int rd;
		rd = read(fd[0], buffer, 4096);
		if(rd < 0){
			fprintf(stderr, "\nUnable read from socket: %s", strerror(errno));
			if(((errno == EWOULDBLOCK) || errno == EAGAIN) || (errno == EINTR)){/*An asynchronous 
								signal occurred and prevented completion of the call or Resource 
								temporarily unavailable. Try agian!  */
				printf("Try Again!!!\n");
			}

		
		}
	}
	return buffer;
}	
