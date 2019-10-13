#include <stdlib.h>
#include "scheduling.h"
#include <stdio.h>
#include <stdbool.h>

extern Node* head;
extern Node* tail;
extern int scheduleClock;
extern int* incomingTime;
extern int* burstTime;
extern int* startTime;
extern int* stopTime;
extern int length;
extern int* responseTime;
extern int* waitTime;
extern int* turnAroundTime;

void Enqueue(Node* node)
{
	if (head == NULL)
	{
		//printf("Added head to List with ID:%d and Time:%d\n", node->ID, node->timeRemaining);
		head = node;
	}

	else
	{
		//printf("Added item to tail of list with ID:%d and Time:%d\n",node->ID, node->timeRemaining);
		Node* temp = head;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = node;
		node->prev = temp;
		node->next = NULL;
	}
}
void Dequeue()
{
	if (head == NULL)
	{
		//printf("Trying to Dequeue when already NULL\n");
	}
	else if ( head->next == NULL)
	{
		//printf("Dequeue ID:%d and head is now NULL\n", head->ID);
		head = NULL;
	}
	else
	{
		//printf("Dequeued ID:%d and head is now ID:%d Time:%d\n",head->ID, head->next->ID, head->next->timeRemaining);
		head = head->next;
	}
	
}
void PlaceNodeInQueue(Node* node)
{
	if (head == NULL)
	{
		Enqueue(node);
	}
	else
	{
		if (node->timeRemaining < head->timeRemaining)
		{
			//printf("Placed Node ID:%d at the front of the list because it is the shortest\n",node->ID);
			Node* temp = head;
			head = node;
			temp->prev = head;
			head->next = temp;
		}
		else
		{
			Node* temp = head;
			//find place in list to put value
			while ((temp->timeRemaining <= node->timeRemaining) && (temp->next != NULL))
			{
				temp = temp->next;
			}
			if (temp->timeRemaining > node->timeRemaining)
			{
				//printf("Placed Node ID:%d in the middle of the Queue\n",node->ID);
				temp->prev->next = node;
				node->next = temp;
				temp->prev = node;
			}
			else
			{
				//printf("Placed Node ID:%d at the end of the Queue\n",node->ID);
				temp->next = node;
				node->prev = temp;
				node->next = NULL;
			}

		}
		//printf("Placed node with ID:%d in the list",node->ID);
		PrintQueue();
	}
}

//Print the contents of the queue for debugging purposes	
void PrintQueue()
{
	if (head != NULL)
	{
		//printf("Contents of Queue:\n");
		Node* node = head;
		while (node != NULL)
		{
			//printf("ID: %d, timeRemaining: %d\n",node->ID, node->timeRemaining);
			
			if (node == node->next)
			{
				//printf("error detected in Node ID:%d circular reference found\n",node->ID);
				return;	
			}
			node = node->next;
		}	
	}
	else
	{
		//printf("Queue is empty\n");
	}
}	





//Execute the top of the queue for a specified time
bool ExecuteForTime(int time)
{
	if (head != NULL)
	{
		if (startTime[head->ID] < 0)
		{
			startTime[head->ID] = scheduleClock;
			//printf("First time running this process, start time recorded for ID%d as %d\n",head->ID,startTime[head->ID]);
		}
		
		int timeDifference = time - head->timeRemaining;
		
		//Uses entire cycle and finishes
		if (timeDifference == 0)
		{
			scheduleClock += time;
			head->timeRemaining = 0;
			stopTime[head->ID] = scheduleClock;
			//printf("Executed for %d seconds and finished ID%d, time clock now at %d and stop time recorded as:%d\n",time, head->ID, scheduleClock, stopTime[head->ID]);
			return true;
			Dequeue();
			
		}
		//Uses portion of cycle and finishes
		else if (timeDifference > 0)
		{
			scheduleClock += head->timeRemaining;
			stopTime[head->ID] = scheduleClock;	
			//printf("Executed for %d seconds and finished ID%d, time clock now at %d and stop time recorded as:%d\n",head->timeRemaining, head->ID, scheduleClock, stopTime[head->ID]);
			head->timeRemaining = 0;		
			return true;
		}
		//Uses entire cycle and doesn't finish
		else
		{
			scheduleClock += time;
			head->timeRemaining -= time;
			//printf("Executed for %d seconds and did not finish, Time Remaining:%d System Clock:%d\n",time, head->timeRemaining, scheduleClock);
			return false;
		}		
	}	
}

//Execute the remainder of the queue in order
void ExecuteRemainingQueue()
{
	while (head)
	{
		ExecuteForTime(head->timeRemaining);
		Dequeue();
		PrintQueue();
	}
}

//Execute whatever is remaining in the queue with a time default
void ExecuteRemainingQueueRR(int cycleTime)
{
	while (head)
	{
		if (head->timeRemaining == 0)
		{
			Dequeue();
		}
		if (head)
		{			
			ExecuteForTime(cycleTime);
			Node* temp = head;
			Dequeue();
			if (temp->timeRemaining != 0)
			{
				Enqueue(temp);
			}
		}	
	}		
}

//clear all arrays and prepare them to be used again
void ClearArrays()
{
	for (int i = 0; i < length; i++)
	{
		startTime[i] = -1;
		stopTime[i] = -1;
		responseTime[i] = -1;
		waitTime[i] = -1;
		turnAroundTime[i] = -1;		
	}
}

//clear the queue
void ClearList()
{
	head = NULL;
	tail = NULL;
}
//Create a statistics struct with the averages for this run
Result RunStatistics(int* startTime, int* stopTime)
{
	for (int i = 0; i < length; i++)
	{
		responseTime[i] = startTime[i] - incomingTime[i];
		waitTime[i] = stopTime[i] - incomingTime[i] - burstTime[i];
		turnAroundTime[i] = burstTime[i] + waitTime[i];
	}
	
	double sumResponse = 0.0;
	double sumWait = 0.0;
	double sumTurnAround = 0.0;
	
	for (int i = 0; i < length; i++)
	{
		sumResponse += responseTime[i];
		sumWait += waitTime[i];
		sumTurnAround += turnAroundTime[i];
	}
	Result result = {.averageResponse = sumResponse/length, .averageTurnAround = sumTurnAround/length, .averageWait = sumWait/length};
	
	return result;
}

//Print Results of scheduling run
void PrintResults(Result result)
{
	printf("Avg. Resp.:%.2f, Avg. T.A.:%.2f, Avg Wait:%.2f\n", result.averageResponse, result.averageTurnAround, result.averageWait);
}

	
	
	
