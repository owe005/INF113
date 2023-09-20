/*

Author: Ole Westby

A Covid-19 Testing Station simulation using Pthreads

/*/


#include <pthread.h>    // pthread
#include <time.h>		// time
#include <unistd.h>     // sleep
#include <semaphore.h>  // semaphore
#include <stdlib.h>     // random
#include <stdio.h>      // prinftf

pthread_t *Cars;		//N threads for Cars
pthread_t Doctor;		//Separate Thread for Doctor

int WaitingLine = 0;
int CurrentIndex = 0;

//Declaration of Semaphores and Mutex Lock
sem_t Doctor_Sleep;
sem_t Car_Sem;
sem_t QueueSem[3];
pthread_mutex_t WaitingLineAccess;

//Define the amount of cars driving
#define NUM_CARS 5

//Declared Functions
void *doctor_actions();
void *car_actions(void *threadID);

int main(int argc, char* argv[])
{
	int number_of_Cars = NUM_CARS;
	int id;

	//Init. Mutex Lock and Semaphores
	sem_init(&Doctor_Sleep, 0, 0);
	sem_init(&Car_Sem, 0, 0);
	for(id = 0; id < 3; ++id);
		sem_init(&QueueSem[id], 0, 0);

	pthread_mutex_init(&WaitingLineAccess, NULL);
		
	//Memrory allocation for cars
	Cars = (pthread_t*) malloc(sizeof(pthread_t)*number_of_Cars);

	//Creating Doctor thread and N Car threads
	pthread_create(&Doctor, NULL, doctor_actions, NULL);	
	for(id = 0; id < number_of_Cars; id++)
		pthread_create(&Cars[id], NULL, *car_actions,(void*) (long)id);

	//Waiting for Doctor thread and N Car threads
	pthread_join(Doctor, NULL);
	for(id = 0; id < number_of_Cars; id++)
		pthread_join(Cars[id], NULL);

	//Free memory
	free(Cars); 
	return 0;
}

void *doctor_actions()
{
	while(1)
	{
		sem_wait(&Doctor_Sleep); //Doctor is currently sleeping

        sleep(5);

		printf("Doctor returns from break.\n******************************\n");

		while(1)
		{
			pthread_mutex_lock(&WaitingLineAccess);
			if(WaitingLine == 0) 
			{
				pthread_mutex_unlock(&WaitingLineAccess);
                printf("Nobody in queue..\n******************************\n");
				break;
			}

			//Next car gets tested.
			sem_post(&QueueSem[CurrentIndex]);
			CurrentIndex = (CurrentIndex + 1) % 3;
			pthread_mutex_unlock(&WaitingLineAccess);
            WaitingLine--;

            sleep(5); 

            printf("Next car drives forward to get tested. Remaining cars in queue is %d\n******************************\n", WaitingLine);
			sem_post(&Car_Sem);
			usleep(1000);
		}
	}
}

void *car_actions(void *threadID) 
{
	int DrivingTime;

	while(1)
	{
		printf("Car %ld is driving..\n******************************\n", (long)threadID);

		DrivingTime = rand() % 25;
		sleep(DrivingTime);		//Sleep for a random time period

		printf("Car %ld wishes to get tested for Covid-19\n******************************\n", (long)threadID);

        sleep(5);
		
		pthread_mutex_lock(&WaitingLineAccess);
		int Queue = WaitingLine;
		pthread_mutex_unlock(&WaitingLineAccess);

		if(Queue < 3)		//Car tries to enter queue, max line in queue is 3
		{
			if(Queue == 0)		//If the car entering the queue is the only one in queue, wake up the doctor
				sem_post(&Doctor_Sleep);

			// lock
			pthread_mutex_lock(&WaitingLineAccess);
			int index = (CurrentIndex + WaitingLine) % 3;
			WaitingLine++;

            printf("Car %ld gets in line.\n******************************\n", (long)threadID);
            printf("Queue is now: %d (+1)\n******************************\n", WaitingLine);
			pthread_mutex_unlock(&WaitingLineAccess);
			// unlock

			sem_wait(&QueueSem[index]);		//Car leaves queue

			printf("Car %ld is getting tested.\n******************************\n", (long)threadID);

            sleep(rand() % 10); //We'll assume the Doctor is not a robot and the testing time varies.

			sem_wait(&Car_Sem);		//Car waits to go next
			printf("Car %ld finished testing.\n******************************\n",(long)threadID);
		}
		else //If queue is full, don't let car into queue

			printf("Car %ld will try again later.\n******************************\n", (long)threadID);
	}
}