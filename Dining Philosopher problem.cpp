#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 

#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (philosopher_number + 4) % N 
#define RIGHT (philosopher_number + 1) % N 

int state[N]; 
int phil[N] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[N]; 

void test(int philosopher_number){ 
	if (state[philosopher_number] == HUNGRY 
		&& state[LEFT] != EATING 
		&& state[RIGHT] != EATING) { 
		state[philosopher_number] = EATING; 

		sleep(2); 
		printf("Philosopher %d takes fork %d and %d\n",philosopher_number + 1, LEFT + 1, philosopher_number + 1); 
		printf("Philosopher %d is Eating\n", philosopher_number + 1); 
		sem_post(&S[philosopher_number]); 
	} 
} 

// pick the chopsticks 
void pick_fork(int philosopher_number){ 

	sem_wait(&mutex); 
	state[philosopher_number] = HUNGRY; 

	printf("Philosopher %d is Hungry\n", philosopher_number + 1); 

	test(philosopher_number); 
	sem_post(&mutex); 
	sem_wait(&S[philosopher_number]); 

	sleep(1); 
} 

// return the chopsticks 
void return_fork(int philosopher_number) 
{ 

	sem_wait(&mutex); 

	state[philosopher_number] = THINKING; 

	printf("Philosopher %d putting fork %d and %d down\n",philosopher_number + 1, LEFT + 1, philosopher_number + 1); 
	printf("Philosopher %d is thinking\n", philosopher_number + 1); 

	test(LEFT); 
	test(RIGHT); 

	sem_post(&mutex); 
} 

void* philospher(void* num){ 
	while (1) { 

		int* i = num; 
		sleep(1); 
		pick_fork(*i); 
		sleep(0); 
		return_fork(*i); 
	} 
} 

int main(){ 

	int i; 
	pthread_t thread_no[N]; 
	sem_init(&mutex, 0, 1); 

	for (i = 0; i < N; i++) {
		sem_init(&S[i], 0, 0); 
	}
	for (i = 0; i < N; i++) {  
		pthread_create(&thread_no[i], NULL,philospher, &phil[i]); 
		printf("Philosopher %d is thinking\n", i + 1); 
	} 
	for (i = 0; i < N; i++) {
		pthread_join(thread_no[i], NULL); 
	}
} 
