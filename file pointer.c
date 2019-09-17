// 학번 : 201300712
// 학과 : 디지털정보공학과
// 성명 : 김윤기

#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 256
int main(int argc, char **argv)
{
	char line[BUFFER_SIZE];	// line이라는 buffer를 받는다.
	FILE *fpin1, *fpin2, *fpout;
	int line_count=1;
	
	if(argc != 4) {
		perror(argv[0]);
		return 1;	
	}	// 인자가 4개가 아니면 error
	if ((fpin1 = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		return 3;
	}	// argv[1] 이 없다면 error
	if ((fpin2 = fopen(argv[2], "r")) == NULL) {
		perror(argv[2]);
		return 3;
	}	// argv[2] 이 없다면 error
	if((fpout=fopen(argv[3],"w"))==NULL){
		perror(argv[3]);
		return 4;
	}	// argv[3] 이 없다면 error
	if (setvbuf (fpin1, line, _IOLBF, BUFFER_SIZE) != 0) { // line buffering
		perror("setvbuf(fpin1)");
		return 6;
	}	// fpin1 의 line 버퍼링 설정, full beffering이 되면 error
	if (setvbuf (fpin2, line, _IOLBF, BUFFER_SIZE) != 0) {
		perror("setvbuf(fpin2)");
		return 7;
	}	// fpin2 의 line 버퍼링 설정, full beffering이 되면 error
	fpin1=fopen(argv[1],"r");
	fpin2=fopen(argv[2],"r");
	fpout=freopen(argv[3],"w+",stdout);
	// argv[1][2][3] 에 전달받은 파일 경로를 file pointer 변수에 담는다.
	// argv[3]은 출력형식이므로 stdout의 freopen함수를 이용한다.

	while (1){
		fgets(line,BUFFER_SIZE,fpin1);	// fpin1이 가리키는 포인터의 string을
														//	fgets를 이용하여 line이라는 버퍼에 담는다.
		if(feof(fpin1)) break;		// argv[1]의 파일이 끝나게 되면 break, while을 빠져나간다.
		line[strlen(line)-1]='\0';	// fgets를 사용하였을 때 개행문자가 buffer에 담기는 것을 삭제한다.
		printf("%d : ",line_count);	// 줄번호를 line_count라는 변수를 이용, 출력
		puts(line);		//line 버퍼에 담긴 string을 argv[3]에 입력
		line_count++;	//line_count 증가
		
		fgets(line,BUFFER_SIZE,fpin2);
		if(feof(fpin2)) break;
		line[strlen(line)-1]='\0';
		printf("%d : ",line_count);
		puts(line);
		line_count++;	// fp1과 마찬가지의 알고리즘의 형태로 fp2 의 copy 진행
	}

	if(!feof(fpin1)){
		while(1){
			fgets(line,BUFFER_SIZE,fpin1);
			if(feof(fpin1)) break;
			line[strlen(line)-1]='\0';
			printf("%d : ",line_count);
			puts(line);
			line_count++;
		}
	}	// 만약 fpin2가 0에 도달했을때 (argv[2] 파일이 끝났을 때)
		// 같은 알고리즘의 형태로 fp1의 남은 string copy 진행
	
	if(!feof(fpin2)){
		while(1){
			fgets(line,BUFFER_SIZE,fpin2);
			if(feof(fpin2)) break;
			line[strlen(line)-1]='\0';
			printf("%d : ",line_count);
			puts(line);
			line_count++;
		}
	}	// 만약 fpin1가 0에 도달했을때 (argv[1] 파일이 끝났을 때)
		// 같은 알고리즘의 형태로 fp2의 남은 string copy 진행

	fclose(fpin1);
	fclose(fpin2);
	fclose(stdout);
	return 0;
}

