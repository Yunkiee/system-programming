// 디지털정보공학과
// 201300712 김윤기
// pthread는 구현하지 못했습니다.
// update 명령시 OK라는 문자열을 전송하는것은 되지만 key-value가 바뀌지 않습니다.

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 1024 // max KB input length
#define PORT 30712 // server port #
//#define HOSTADDR "192.168.16.30" // server IP
#define HOSTADDR "127.0.0.1" // local host!!
int main (int argc, char *argv[])
{
	int sd, send_bytes, n, recv_bytes;

	struct sockaddr_in servaddr;
	char snddata[MAX], rcvdata[MAX];
	bzero ((char*) &servaddr, sizeof(servaddr)); // prepare server address, port
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr (HOSTADDR);
	servaddr.sin_port = htons (PORT);
	if (( sd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf( stderr, "can’t open socket.\n");
		exit(1);
	}

	if (connect (sd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		fprintf (stderr, "can’t connect to server.\n");
		exit(1);
	} // auto binding for this client and connect request
	while (fgets (snddata, MAX, stdin) != NULL) { // get a string from KB
		send_bytes = strlen (snddata);
		if (send (sd, snddata, send_bytes, 0) != send_bytes) { // to server
			fprintf( stderr, "can’t send data.\n");
			exit(1);
		}
		recv_bytes = 0;
		while (recv_bytes < send_bytes) { // while loop for stream I/O !
			if ((n = recv (sd, rcvdata + recv_bytes, MAX, 0)) < 0) { // from server
				fprintf (stderr, "can’t receive data.\n");
				exit(1);
			}
			recv_bytes += n;
		}
		rcvdata[recv_bytes] = 0; // NULL char for string
		if(strcmp(rcvdata,"quit_ack")==0)
			exit(1);
		printf(":  ");
		fputs (rcvdata, stdout); // display
		printf("\n");
		fflush(stdout);
	}
	close (sd);
	return 0;
}


