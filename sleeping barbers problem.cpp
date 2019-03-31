#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/ipc.h>
#include<semaphore.h>

#define N 5

time_t endTime;
sem_t mutex,customer,barber;
int i=0;

void barber(void *arg)
{
	while(time(NULL)<endTime || i>0)
	{
		sem_wait(&customer);           
		sem_wait(&mutex);
		i--;
		printf("Barber:cut hair,i is:%d.\n",i);
		sem_post(&mutex);
		sem_post(&barber);
		sleep(3);       
	}
}

void customer(void *arg)
{
	while(time(NULL)<endTime)
	{
		sem_wait(&mutex);
		if(i<N)
		{
			i++;
			printf("Customer:add i,i is:%d\n",i);
			sem_post(&mutex);
			sem_post(&customer);
			sem_wait(&barber);
		}
		else
			sem_post(&mutex);
		sleep(1);
	}
}

int main(int argc,char *argv[])
{
	pthread_t id1,id2;
	int status=0;
	endTime=time(NULL)+20;/*Shop Hours is 20s*/

	sem_init(&mutex,0,1);
	sem_init(&customer,0,0);
	sem_init(&barber,0,1);

	//Barber thread
	status=pthread_create(&id1,NULL,(void *)barber,NULL);
	if(status!=0)
		perror("create barber is failure!\n");
	//Customer thread initialization
	status=pthread_create(&id2,NULL,(void *)customer,NULL);
	if(status!=0)
		perror("create customer is failure!\n");

	
	pthread_join(id2,NULL);
	pthread_join(id1,NULL);

	exit(0);
}

