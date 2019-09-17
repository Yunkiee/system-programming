#include <stdio.h>
#include <string.h>

#define N 1000
int fault_count=0;

void FIFO (int VA[], int frame[], int frame_num){
	int j, flag=0;
	for(j=0;j<=frame_num-1;j++){
		frame[j]=-1;
	}
	j=0;	//frame 초기화
	
	for(int i=0;i<N;i++){
		for(int k=0;k<=frame_num-1;k++){
			if(VA[i]==frame[k])
				flag=-1;
		}
		
		if(flag==-1){
			flag=0;
			continue;
		}	//frame에 같은 data가 있으면 hit
		
		frame[j]=VA[i];
		j++;
		fault_count++;
		if(j==frame_num)
			j=0;
	}

}

int findLRU(int time[], int n){
    int i, minimum, index=0;
	minimum=time[0];
	
    for(i=1;i<n;i++){
        if(time[i]<minimum){
            minimum=time[i];
            index=i;
        }
    }
    
    return index;
}	//LRU(least recent used) frame index를 찾는 함수
    //used time이 가장 작은 index를 리턴한다.

void LRU(int VA[], int frame[], int frame_num){
	int flag1=0, flag2=0;
	int time[frame_num];
	int time_count=0;
	int index;
	
	for(int j=0;j<=frame_num-1;j++){
		frame[j]=-1;
	}	//frame 초기화
	
	for(int i=0;i<N;i++){
		for(int k=0;k<=frame_num-1;k++){
			if(VA[i]==frame[k]){
				flag1=-1;
				time_count++;
				time[k]=time_count;
				break;
			}
		}
		
		if(flag1==-1){
			flag1=0;
			continue;
		}	//frame에 같은 data가 있으면 hit
		
		for(int k=0;k<frame_num;k++){
			if(frame[k]==-1){
				time_count++;
				fault_count++;
				frame[k]=VA[i];
				time[k]=time_count;
				flag2=-1;
				break;
			}
        }
		
		if(flag2==-1){
			flag2=0;
			continue;
		}
		
		index=findLRU(time,frame_num);
		time_count++;
		fault_count++;
		frame[index]=VA[i];
		time[index]=time_count;
		// findLRU 함수를 이용하여 index를 찾고
		// frame[index]에 VA값을 넣는다.
	}
		
}

int main(int argc, char* argv[]) {
	int page_size;
	scanf("%d",&page_size);
	int PMS;
	scanf("%d",&PMS);
	char method[10];
	scanf("%s",method);
	int VA[N];
	int hex_VA;
	for(int i=0;i<N;i++){
		scanf("%x",&hex_VA);
		VA[i]=hex_VA/(16*16*16);
	}
	
	int frame_num=PMS/page_size;	// frame 개수
	
	int frame[frame_num];
	
	if(!(strcmp(method,"FIFO")))
		FIFO(VA,frame,frame_num);
	
	else if(!(strcmp(method,"LRU")))
		LRU(VA,frame,frame_num);
	
	printf("Number of faults:%d\n",fault_count);
	printf("Frame contents:\n");
	for(int i=0;i<frame_num;i++){
		printf("%x:%x\n",i,frame[i]);
	}
	
	return 0;
}

