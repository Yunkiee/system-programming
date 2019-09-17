#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

#define SIZE 1000

int main(){
	int fd1[2], fd2[2];
	char string[SIZE];
	char q_buf[SIZE];
	char r_buf[SIZE];
	int status;
	pid_t pid;
	pid_t cpid;
	
	int a=read(STDIN_FILENO,string,SIZE);	// stdin의 내용을 string에 담는다.
	
	pipe(fd1);
	pipe(fd2);	//파이프 선언
	if (pid=fork()==0){
		
		if(cpid=fork()==0){ //	grandchild
			close(fd2[1]);
			//		close(fd1[0]);
			//		close(fd1[1]);	//	fd1 파이프 close, fd2[1](쓰기용) 파이프 close
			read(fd2[0],r_buf,SIZE);	//	fd2[0] 파이프를 통하여 r_buf로 문자열을 읽는다.
			puts(r_buf);	//	r_buf 문자열 출력
			fflush(stdout);
			if(strstr(r_buf,"quit")!=NULL){	//	문자열에 quit가 있으면	
				cpid=getppid();	//	grancchild의 parent(=child)의 pid를 cpid에 담는다.
				puts("Goodbye from R");
				fflush(stdout);
				close(fd2[0]);
				return cpid;
				exit(status);	//	grandchild 종료
			}
		}
		else{   //child
			close(fd1[1]);
			close(fd2[0]);
			read(fd1[0],q_buf,SIZE);	//	fd1[0] 파이프를 통하여 q_buf로 문자열을 읽는다.
			write(fd2[1],q_buf,SIZE);	//	r_buf에 담긴 문자열을 fd2[1] 파이프에 담는다.
			waitpid(cpid,&status,0);	//	grandchild의 종료를 기다림
			if(strstr(q_buf,"quit")!=NULL){
				pid=getppid();	//	child의 parent(=parent)의 pid를 pid에 담는다.
				puts("Goodbye from Q");
				fflush(stdout);
				close(fd1[0]);
				close(fd2[1]);
				return pid;
				exit(status);	//	child 종료
			}
		}
	}
	else{   //parent
		close(fd1[0]);
		//		close(fd2[0]);
		//		close(fd2[1]);
		write(fd1[1],string,SIZE);
		waitpid(pid,&status,0);	//	child의 종료를 기다림
		if(strstr(string,"quit")!=NULL){
			puts("Goodbye from P");
			fflush(stdout);
			close(fd1[1]);
			exit(status);	//	parent 종료
		}
	}
}