
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "search.h"  
#include "urldecode.h"
#include "sysexplorer.h"
#include "threadpool.h"
#include "search.h"
#include "cache.h"


#define HOSTNAME "localhost"
#define PORT 11880
#define MAX_IDLE_THREADS 10
#define BACKLOG 10				/*maximum number of pending connections queue will hold*/
#define Y_BIG	121
#define Y_SMALL	89

#define CHECK_ERRNO(x) if((x) == -1){								\
	fprintf(stderr, "In %s(%d) [function %s]: %s\n", 		\
	__FILE__, __LINE__, __FUNCTION__ , strerror(errno));	\
	abort();												\
}
 

int main(int argc, char** argv)
{

		int fd;					//the file descriptor of server socket(listening socket)
    struct sockaddr_in address; 		/* the network address of the
					  				 listening socket */
  	int sopt;

  	thpool_t pool;


  	/* create the listening socket, as an INET (internet) socket with
  	TCP (stream) data transfer */
  	CHECK_ERRNO(fd=socket(AF_INET, SOCK_STREAM, 0));

		/* we set the 3rd parameter to "SO_REUSEADDR",to allow other
			sockets to bind to this port,unless an other active 
			listening socket bound to the port already*/
  	sopt = 1;
  	CHECK_ERRNO(setsockopt(fd,SOL_SOCKET,
			 SO_REUSEADDR,&sopt, sizeof(sopt)));

  	/* Prepare address for a call to bind.
   	The specified address will be the INADDR_ANY address (meaning "every
   	address on this machine" and the port.
  	*/
  	//memset(&address, 0, sizeof(address));
  	address.sin_family      = AF_INET;
  	address.sin_addr.s_addr = htonl(INADDR_ANY);
  	address.sin_port        = htons(PORT);

  	/* Bind listening socket to the listening address */
  	CHECK_ERRNO(bind(fd, 
		   (struct sockaddr*) &address, sizeof(address)));

  	/* Set server socket to listen  */
  	CHECK_ERRNO(listen(fd, BACKLOG));
  	if(init_ThreadPool(&pool,MAX_IDLE_THREADS) != 0){
  		printf("Unable to create TheadPool\n");
  		abort();
  	}
  	printf("%s\n","Server running...waiting for connections.");

  	//signal(SIGINT, termination_handler);

  	init_cache(100);

  	while(1){
  		int new_conn;
			pthread_t t_id;

			//socklen_t peer_addrlen=sizeof(address);
			if((new_conn = accept(fd, NULL, NULL)) == -1){ //*/(struct sockaddr*) &address, &peer_addrlen)) == -1){
				fprintf(stderr,"Unable to Create new Connection" "%s\n", strerror(errno));
				sched_yield();
				free(new_conn);
				continue;
			} 
				
			#ifndef DEBUG
			printf("ThreadPool_spawn: \n");
			#endif
			//t_id = ThreadPool_spawn(&pool, &conn_thread, new_conn);
			pthread_create(&t_id, 0, &conn_thread, (void*) new_conn);
			if(errno == EAGAIN) printf("Maximum number of connections reached!!!");
			
			//CHECK_ERRNO(pthread_detach(t_id)); 
			// printf("Thread detached successfully!\n");
			/*Free resources when thread terminates */

  	}

	return 0;
}

/*
void termination_handler(int signum)
{
	ext = 1;
	printf("\nControl-c pressed (SIGINT), do you really");
	printf(" want to exit? [Y/n]");
	char choise;
	scanf("%c",&choise);
	printf("\n");
	if ((choise == Y_SMALL) || (choise == Y_BIG)){ 
		printf("Sysexplorer terminated.\n");
	}
}
*/

void* conn_thread(void* fd){
	int *FD=(int*)fd;  /*cast fd to int and store it to FD */
	char msg[40000];

	Request request;
  	request = read_request(FD);

  	if(request.method == "GET"){
  		if(!find_Page(request.directory, fd)){
			DIR *dir = opendir(request.directory);
			if(dir == NULL){
				if(errno == EACCES){
					sprintf(msg,"HTTP/1.1 403 Forbidden CRLF\n\n <html><head><title> Error 403 </title></head> \
					<body> <center><h1>Error 403. You are not allowed to access .</h1></center></body>\n");
					free(request.directory);
					write((int)FD, msg, strlen(msg));			
					//free(msg);
				}else{
					free(request.directory);
					sprintf(msg,"HTTP/1.1 500 Internal Server Error CRLF\n\n <html><head><title> Something went wrong </title></head> \
							<body><center> <h1> Internal Server Error. Something went wrong.</h1></center></body>");
					write((int)FD, msg, strlen(msg));							
					//free(msg);
				}

			}else{
	            printf("-> Directory: %s\n", request.directory);
				sprintf(msg,"HTTP/1.1 200 OK CRLF\n\n <html> <head><title> %s </title><style> h1.dirlist {background-color: gray; }</style> </head> \
					<body> <h1 class=""dirlist"">Directory listing: %s </h1><tbody><h2>Find Pattern </h2> \
					<form method=""POST""> <input type=""text"" name=""pattern"""" /> <input type=""submit"" name=""submit"" \
	                                value=""Search"" /></form><table border=""1"" width=""350"" cellpadding=""0"" cellspacing=""0""><h3> <th>Name</th> <th>Type</th> <th>Size</th> </h3>",request.directory,request.directory);
				
				//write((int)FD, msg, strlen(msg));							
				//free(msg);

				struct dirent *dirp; 
				struct dirent Predirp;
				struct  stat size;
				int filesize;
				
				char temp[512];
	            char temp1[1024];
				
				while(readdir_r(dir, &Predirp, &dirp) == NULL){		/* show sub-directories and file names which contains into this directory */ 
					if(dirp == NULL) break;
					sprintf(temp,"%s%s",request.directory,dirp->d_name);
					if(dirp->d_type == 0x4 ){			/* directory shows to folder */
						sprintf(temp1,"<tr><td><a href=""%s/"">%s/</a></td><td>Directory</td><td> %s </td></tr>",temp,dirp->d_name," --- ");
					}else if(dirp->d_type == 0x8 ){						/* directory shows to file */
						if( stat(temp, &size) != 1) filesize = size.st_size;
						sprintf(temp1,"<tr><td> %s </td><td>File</td><td> %d </td></tr>",dirp->d_name,filesize);
					}else if(dirp->d_type == 0xa ){
						sprintf(temp1,"<tr><td> %s </td><td>Sympolic Link</td><td> %s </td></tr>",dirp->d_name," --- ");
					}else if(dirp->d_type == 0xc ){
						sprintf(temp1,"<tr><td> %s </td><td>Socket</td><td> %s </td></tr>",dirp->d_name," --- ");
					}
					//write((int)FD, msg, strlen(msg));
					//write((int)FD, "</br>", 5);
					strcat(msg, temp1); 
					strcat(msg,"</br>");

				} 
				//write((int)FD, "</tbody></table></body></html>", 30);
				strcat(msg,"</tbody></table></body></html>");
				write((int)FD, msg, strlen(msg));
				closedir(dir);
				close(FD);

				add_Page(request.directory, msg);
				delete_LRU_page();
			}
		}

  	}else if(request.method == "POST"){
  		char* results;
		char temp[1024];
		char temp1[512];
  		int i,k,j=0,l=0;
  		struct stat statbuf;

                printf("\n ---------pattern prin ti find = %s\n",request.pattern);
		sprintf(temp,"HTTP/1.1 200 OK CRLF\n\n <html> <head><title>Find results for: %s </title><style> h1.dirlist {background-color: gray; } </style> </head> <body> <h1 class=""dirlist""> \
		Find results for: %s </h1> <table><table width=""450""><thead> <tr> <th>Name</th>      <th>Type</th>      <th>Size</th> </tr></thead><tbody>",request.pattern,request.pattern);
		write((int)FD, temp, strlen(temp));
		results = malloc(4096*sizeof(char));
  		results=find(request.pattern,request.directory);
		printf("Find results:\n%s\n", results);

		for(i=0;i<strlen(results);i++){
  			if(results[i] == '\n'){
  				for(k=j;k<i;k++){
					temp[l] = results[k];
					l++;
				}
				temp[l]='\0';
  				j = i;
				l = 0;
				
				stat(temp, &statbuf);
				/*if(S_IFLNK(statbuf.st_mode))
					sprintf(temp1,"<tr><td> %s </td><td>Sympolic Link</td><td> %s </td></tr>",temp," --- ");
				if(S_IFSOCK(statbuf.st_mode))
					sprintf(temp1,"<tr><td> %s </td><td>Socket</td><td> %s </td></tr>",temp," --- ");
				*/if(S_ISDIR(statbuf.st_mode))  
    				sprintf(temp1,"<tr><td><a href=""%s/"">%s/</a></td><td>Directory</td><td> %s </td></tr>",temp,temp," --- ");
				else 
    				sprintf(temp1,"<tr><td> %s </td><td>File</td><td> %d </td></tr>",temp,statbuf.st_size);
					
				write((int)FD, temp1, strlen(temp1));
				write((int)FD, "</br>", 5);
			}
		}

		write((int)FD, "</tbody></table></body></html>", 30);
		close(FD);
  		
  	}

}
 
Request read_request(int fd){

	Request temp;
	char buffer[2048];
	int rd;

	printf("read request\n");
	
	rd = read((int)fd, buffer, 2048);
	//puts(buffer);

	if(rd < 0){
		fprintf(stderr, "\nUnable read from socket: %s", strerror(errno));
		if(((errno == EWOULDBLOCK) || errno == EAGAIN) || (errno == EINTR)){/*An asynchronous 
								signal occurred and prevented completion of the call or Resource 
								temporarily unavailable. Try agian!  */
			printf("Try Again!!!\n");
		}else{
			if(buffer != NULL) free(buffer);
			temp.method = NULL;
			temp.directory = NULL;
			temp.pattern = NULL;
			return(temp);
		}
	}

	if((buffer[0] == 'G') && (buffer[1] == 'E') && (buffer[2] == 'T') && (buffer[3] == ' ')){
		*strchr(buffer + 4, ' ') = '\0';	/* store to buffer the word after 'GET' */ 
		temp.method = "GET";
		temp.directory = www_urldecode(buffer + 4);
		temp.pattern = NULL;
	}else if((buffer[0] == 'P') && (buffer[1] == 'O') && (buffer[2] == 'S') && (buffer[3] == 'T') && (buffer[4] == ' ')){
		char *result;
		int i,j,k;
		//i=strcspn(buffer,"pattern=");
		for(k=0;k<=strlen(buffer);k++){
			if((buffer[k] == 't') && (buffer[k+1] == 't') && (buffer[k+2] == 'e') && (buffer[k+3] == 'r') && (buffer[k+4] == 'n') && (buffer[k+5] == '='))
				i=k+6;
		}
		for(k=0;k<=strlen(buffer);k++){
			if((buffer[k] == '&') && (buffer[k+1] == 's') && (buffer[k+2] == 'u') && (buffer[k+3] == 'b') && (buffer[k+4] == 'm'))
				j=k-1;
		}
		//j=strcspn(buffer,"&submit=");
		result=malloc((j-i+1)*sizeof(char));
		*strncpy(result,buffer + i, j-i+1);
		result[j-i+1] = '\0';
		for(k=0;k<=j-i+1;k++){
			if(result[k] == '+')			
				result[k]=' ';	
		}
		temp.method = "POST";
		*strchr(buffer + 5, ' ') = '\0';
		temp.directory = www_urldecode(buffer +5);
		temp.pattern = result;
                printf("\n----------post meta to temp patern = %s\n", temp.pattern);
		printf("\n----------directory meta to temp = %s\n", temp.directory);
                //free(result);
                //return(temp);
	}else{
		temp.method = NULL;
		temp.directory = NULL;
		temp.pattern = NULL;
	}

	//free(buffer);
	return(temp);
}
