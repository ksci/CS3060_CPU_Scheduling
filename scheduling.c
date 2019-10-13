#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"
#include <stdbool.h>

extern int* startTime;
extern int* stopTime;
extern int* responseTime;
extern int* waitTime;
extern int* turnAroundTime;
extern int* incomingTime;
extern int* burstTime;
extern int length;
extern int scheduleClock;
extern Node* head;


//FCFS is the most basic of queues, whatever comes in first, execute it and then move on
void FirstComeFirstServed()
{
	int index = 0;

	while (index < length)
	{
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->next = NULL;
		newNode->prev = NULL;
		newNode->ID = index;
		newNode->timeRemaining = burstTime[index];
		
		Enqueue(newNode);
		
		if (scheduleClock < incomingTime[index])
		{
			scheduleClock = incomingTime[index];
		}
		ExecuteForTime(burstTime[index]);
		Dequeue();

		index ++;
	}
	ExecuteRemainingQueue();
	//USE THE FOLLOWING TO SEE IF START AND STOP TIMES ARE BEING ACCURATELY CREATED
	//for(int i = 0; i < length; i++)
	//{
	//	printf("startTime: %d stopTime: %d\n",startTime[i], stopTime[i]);
	//}
}

//Round Robin executes the beginning of the queue for a fixed amount of time and places it in the back of the
//queue if that wasn't enough time to finish the job
void RoundRobin(int cpuTime)
{
	int index = 0;
	bool moveToBack = false;
	
	while (index < length)
	{
		if ((scheduleClock < incomingTime[index]) && (head==NULL))
		{
			scheduleClock = incomingTime[index];
		}
		while (incomingTime[index] <= scheduleClock && index <length)
		{
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->next = NULL;
			newNode->prev = NULL;
			newNode->ID = index;
			newNode->timeRemaining = burstTime[index];		
			Enqueue(newNode);
			index += 1;
		}
		
		if (moveToBack == false && head->timeRemaining == 0)
		{
			Dequeue();
		}
		else
		{
			Node* temp = head;
			Dequeue();
			Enqueue(temp);
		}
		moveToBack = !ExecuteForTime(cpuTime);				
	}
	ExecuteRemainingQueueRR(cpuTime);
	//USE THE FOLLOWING TO SEE IF START AND STOP TIMES ARE BEING ACCURATELY CREATED
	//for(int i = 0; i < length; i++)
	//{
	//	printf("startTime: %d stopTime: %d\n",startTime[i], stopTime[i]);
	//}
}


//Shortest Job goes to the front of the Queue. Job completes fully before moving to the next
//element in the queue
void ShortestJobFirst()
{
	int index = 0;
	
	Node* nodePtr = NULL;
	while (index < length)
	{
		if ((scheduleClock < incomingTime[index]) && (head==NULL))
		{
			scheduleClock = incomingTime[index];
		}
		while (incomingTime[index] <= scheduleClock && index < length)
		{
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->next = NULL;
			newNode->prev = NULL;
			newNode->ID = index;
			newNode->timeRemaining = burstTime[index];		
			PlaceNodeInQueue(newNode);
			
			index += 1;
		}
		
		
		ExecuteForTime(burstTime[head->ID]);
		Dequeue();
	}
	ExecuteRemainingQueue();
	
	//USE THE FOLLOWING TO SEE IF START AND STOP TIMES ARE BEING ACCURATELY CREATED
	//for(int i = 0; i < length; i++)
	//{
	//	printf("startTime: %d stopTime: %d\n",startTime[i], stopTime[i]);
	//}
}


//This is the same as Shortest Job first except it is preemptive, so instead of executing for
//the entirety of the burst time, it executes until the next job arrives
void ShortestRemainingJobFirst()
{
	int index = 0;

	Node* temp = NULL;
	while (index < length)
	{
		if ((scheduleClock < incomingTime[index]) && (head==NULL))
		{
			scheduleClock = incomingTime[index];
		}
		if (incomingTime[index] <= scheduleClock)
		{
			
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->next = NULL;
			newNode->prev = NULL;
			newNode->ID = index;
			newNode->timeRemaining = burstTime[index];
					
			PlaceNodeInQueue(newNode);
			index += 1;
		}
		
		if ((temp != NULL) && (temp->timeRemaining > 0))
		{
			PlaceNodeInQueue(temp);
		}
		
		
		if (index < length)
		{
			ExecuteForTime(incomingTime[index] - scheduleClock);
		}
		else
		{
			ExecuteRemainingQueue();
			return;
		}
		if (head->timeRemaining > 0)
		{
			temp = head;
		}
		else
		{
			temp = NULL;
		}		
		Dequeue();
	}	
	//USE THE FOLLOWING TO SEE IF START AND STOP TIMES ARE BEING ACCURATELY CREATED
	//for(int i = 0; i < length; i++)
	//{
	//	printf("startTime: %d stopTime: %d\n",startTime[i], stopTime[i]);
	//}
}
	
