#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

struct thread_data
{
	int id;
	int turn_for_each;
};
double list[1000];
//int turn=100000;
void* calcu(void *arg)
{
	struct thread_data *my_data;
	my_data=(struct thread_data*)arg;
	int id=my_data->id;
	double x, y, pi;
	int n, k;
	srand((unsigned)time(NULL)); //播种随机数
	for (n = 1, k = 0; n <= my_data->turn_for_each; n++) 
	{
		x = rand() / (double)RAND_MAX;
		y = rand() / (double)RAND_MAX;
		if (sqrt((x - 0.5)*(x - 0.5) + (y - 0.5)*(y - 0.5)) < 0.5) 
		{
			k++;
		}
	}
	pi = k / (0.25 * my_data->turn_for_each);
	list[id]=pi;
	//printf("pi for %d : %lf\n",my_data->id,pi);
	return NULL;
}
int main(int argc,char *argv[])
{

	int thread_n = atoi(argv[2]);
	//printf("thread n %d\n",thread_n);
	int rc=0;
	pthread_t threads[1000];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	struct thread_data data[1000];
	for(int i=0;i<thread_n;++i)
	{
		data[i].id = i;
		data[i].turn_for_each=100000/thread_n;
		//printf("i %d\n",data[i].id);
		rc = pthread_create(&threads[i],NULL,calcu,(void*)(&(data[i])));
		if(rc)
		{
			printf("error:unable to create thread %d \n",rc);
			exit(-1);
		}
	}
	void* status;
	for(int i=0;i<thread_n;++i)
	{
		rc = pthread_join(threads[i],&status);
		if(rc)
		{
			printf("error: unable to join,%d\n",rc);
			exit(-1);
		}
	}
	double pires=0;
	for(int i=0;i<thread_n;++i)
	{
		pires+=list[i];
		//printf("%d pi %lf\n",i,list[i]);
	}
		
	printf("pi: %lf\n",pires/thread_n);
	return 0;
}