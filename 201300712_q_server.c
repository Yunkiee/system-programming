// 디지털정보공학과
// 201300712 김윤기
// pthread는 구현하지 못했습니다.
// update 명령시 OK라는 문자열을 전송하는것은 되지만 key-value가 바뀌지 않습니다.

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024 // max client data length
#define PORT 30712// server port #
#define BACKLOG 5 // queue length

int main (int argc, char *argv[])
{
	int sd, nsd, pid, bytes, cliaddrsize;
	struct sockaddr_in cliaddr, servaddr;
	char data[MAX];
	char *kv[7][2]={{"name","gildong"},{"phone","123-4567"},{"color","blue"},{"city","seoul"},{"hobby","movie"},{"school","HUFS"},{"course","SP"}};
	
	if (( sd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf( stderr, "can’t open socket.\n");
		exit(1);
	}   
	// to bind the server itself to the socket
	bzero ((char*) &servaddr, sizeof( servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (PORT);

	if (bind (sd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		fprintf (stderr, "can’t bind to socket.\n");
		exit(1);
	} // bind itself to the socket

	listen (sd, BACKLOG); // declare the client-queue length

	while (1) { // a typical server waiting loop
		cliaddrsize = sizeof (cliaddr);
		if (( nsd = accept (sd, (struct sockaddr *) &cliaddr, &cliaddrsize)) < 0) {
			fprintf (stderr, "can’t accept connection.\n");
			exit(1);
		} // upon return: client addr. is known and a new socket is created
		if ((pid = fork()) < 0) { // fork error, a new thread may be used!
			fprintf (stderr, "can’t fork process.\n"); exit(1);
		}

		if (pid == 0) { // the new child server for the connected client
			close (sd); // old socket is not necessary for me
			while(1) {
				bytes = recv (nsd, data, MAX, 0); // from client
				if (bytes == 0) // client quit
				break;
				else if (bytes < 0) { // error
					fprintf (stderr, "can’t receive data.\n"); exit(1);
				}
				/*if (send (nsd, data, bytes, 0) != bytes) { // echo back
					fprintf (stderr, "can’t send data.\n"); exit(1);
				}*/
				int i;
				char *ptr = strtok(data," ");
				
				if(strcmp(ptr,"read")==0){
					ptr = strtok(NULL," \n");
					for(i=0;i<7;i++){
						if(strcmp(ptr,kv[i][0])==0){
							send(nsd,kv[i][1],MAX,0);
						}
					}
				}
				
				else if(strcmp(ptr,"update")==0){
					ptr = strtok(NULL," ");
					for(i=0;i<7;i++){
						if(strcmp(ptr,kv[i][0])==0){
							ptr = strtok(NULL," \n");
							kv[i][1]=ptr;
							send(nsd,"OK",MAX,0);
						}
					}
				}
				
				if(strcmp(ptr,"quit\n")==0)
					send(nsd,"quit_ack",MAX,0);
			
				memset(data,0,MAX);
			} // end while, client quits
			return 0; // child server exit.
		} // else : parent
		close (nsd); // parent: close the new socket
	} // end while: parent goes to the client waiting-loop again
} /* main */