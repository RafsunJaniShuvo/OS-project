#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
sem_t A;
sem_t B;
sem_t C;
sem_t D;






void *Theater_function(void *);
void *student_ID_function(void *studentId);
int Flag = 0;
int main()
{
    pthread_t Movie_Thread;
    pthread_t *st_id;

    int i,numStudents,numGlass;


    printf("Number of students:\n");

    scanf("%d",&numStudents); /// taking number of students

    printf("3D Glasses:\n");

    scanf("%d",&numGlass);///taking number of 3d glasses

    int Number[numStudents];
    st_id= (pthread_t*) malloc(sizeof(pthread_t)*numStudents); /// Dynamic memory allocation

    for (i = 0; i < numStudents; i++)
    {
        Number[i] = i;
    }


    sem_init(&A, 0, numGlass);
    sem_init(&B, 0, 1);
    sem_init(&C, 0, 0);
    sem_init(&D, 0, 0);


    pthread_create(&Movie_Thread, NULL, Theater_function, NULL); /// parameter as NUll


    for (i = 0; i < numStudents; i++)
    {
        pthread_create(&st_id[i], NULL,student_ID_function, (void *)&Number[i]); ///Sending number parameter as id
    }

    for (i = 0; i < numStudents; i++)
    {
        pthread_join(st_id[i],NULL);///Sending parameter as NUll
    }


    Flag = 1;///when all studentss finished watching movie

    sem_post(&C); /// function unlocks the specified semaphore by performing a semaphore.
    pthread_join(Movie_Thread,NULL); ///shall suspend execution of the calling thread.
    return 0;
}

void *Theater_function(void *userInput)
{

    while (!Flag)
    {
         printf("flag value: %d\n",Flag);
         sem_wait(&C);
        if (!Flag)
        {  printf("flag value: %d\n",Flag);
            printf("The student has finished watching movie.\n");
            sem_post(&D);
        }
        else
        {

             printf("flag value: %d\n",Flag);
            printf("All of the students finished watching movie.\n");
        }
    }
}

void *student_ID_function(void *studentId)
{
    int num = *(int *)studentId; /// typecasting ptr to point to an int

    printf(" %d entering  to watch movie\n", num);
    Sleep(100);///sleeping for 100 mili seconds

    printf(" %d have to wait  for a 3D glass.\n", num);

    sem_wait(&A); /// process will wait in waiting room

    printf("Student %d have a 3D glass and waiting for entering theater.\n", num);

    sem_wait(&B); ///waiting in theater


    sem_post(&A); ///unlocking  waiting room

    printf("%d entering the theater hall.\n", num);

    sem_post(&C);///unlocking


    sem_wait(&D); ///waiting

    sem_post(&B);///unlocking

    printf("Student %d has finished watching movie.\n", num);
}
