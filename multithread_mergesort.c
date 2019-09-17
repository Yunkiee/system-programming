#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


#define MAX 1000000

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int done=0;

typedef struct tTHREAD
{
	int first;
	int last;
	int size;
	int A[MAX];
}THREAD;

void merge(int A[], int first, int last){
	int temp[MAX], i, j, k, a, b, mid;
	mid=(first+last)/2;
	i=first;
	j=mid+1;
	k=0;
	while(i<=mid&&j<=last){
		if(A[i]<=A[j])
			temp[k++]=A[i++];
		else
			temp[k++]=A[j++];
	}
	for(;i<=mid;i++)
		temp[k++]=A[i];
	for(;j<=last;j++)
		temp[k++]=A[j];
	
	b=first;
	for(a=0;a<k;a++)
		A[b++]=temp[a];
}

void mergesort(int A[], int first, int last){
	if(first<last){
		int mid=(first+last)/2;
		mergesort(A,first,mid);
		mergesort(A,mid+1,last);
		merge(A,first,last);
	}
}

void thr_exit(){
	pthread_mutex_lock(&mutex);
	done=1;
	pthread_cond_signal(&condition);
	pthread_mutex_unlock(&mutex);
}	
//	thread_merge쓰레드가 실행될 때마다 done=1로 설정, condition신호를 보냄. 
//	CS에 한 쓰레드만 접근하기 위해 mutex lock 사용

void *thread_merge(void *p){
	THREAD* pp = (THREAD *)p;
	pthread_mutex_lock(&mutex);
	mergesort(pp->A,pp->first,pp->last);
	pp->first+=pp->size;
	pp->last+=pp->size;		
	pthread_mutex_unlock(&mutex);
	thr_exit();
}
//	mergesort 할때는 CS에 접근하기 위하여 mutex lock 사용

void *thread_print(void *p){
	THREAD* pp = (THREAD *)p;
	int s=pp->size;
	for(int i=0;i<s;i++){
		int k=pp->A[i];
		printf("%d\n",k);
	}
}

int main(int argc, char* argv[]) {
	int n1, n2;
	scanf("%d", &n1);
	pthread_t threads[n1];
	scanf("%d", &n2);
	THREAD t;
	int size;
	int k=0;
	
	for(int i=0;i<n2;i++){
		scanf("%d",&t.A[i]);	
	}
	
	while(n1>=1){
		size=n2/n1;
		t.size=size;
		t.first=0;
		t.last=size-1;
		
		for(k=0;k<n1;k++){
			pthread_create(&threads[k],NULL,thread_merge,&t);
			pthread_join(threads[k],NULL);
		}
		
		pthread_mutex_lock(&mutex);	
		while(done==0)
			pthread_cond_wait(&condition,&mutex);
		pthread_mutex_unlock(&mutex);
		//	thread_merge에서 실행되는 thr_exit에서 condition 신호가 올때까지 wait 
		//	각 단계에서 만나야하는 쓰레드들의 결합을 의미함
		//	conditional variable을 이용한 구현 (CV를 사용할땐 mutex와 함께 사용)
		//	pthread는 mesa semantic이므로 while문을 사용하여 recheck
		
		n1=n1/2;
	}

	pthread_create(&threads[0],NULL,thread_print,&t);
	pthread_join(threads[0],NULL);
	
	return 0;
}