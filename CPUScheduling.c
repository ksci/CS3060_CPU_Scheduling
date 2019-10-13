/*Author:		Kyle Miller
 * Project:		Project 5 Scheduling CS 3060
 * Declaration:	I wrote all of this code myself
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include "scheduling.h"
#include "FileReadIn.h"



int scheduleClock = 0;				//global clock variable
int* startTime = NULL;				//array for start times for each process
int* stopTime = NULL;				//array for stop times for each process
int* responseTime = NULL;			//array to store response time for each process
int* waitTime = NULL;				//array to store wait time for each process
int* turnAroundTime = NULL; 		//array to store turnAroundTime for each process
int* incomingTime = NULL;			//array to store incomingTime for each process (from file)
int* burstTime = NULL;				//array to store burst time for each process (from file)
int length = 0;						//length of the array (number of processes)

//Initialize beginning and end of list
Node* head = NULL;
Node* tail = NULL;

FILE* fptr;

int main(int argc, char* argv[])
{

	length = GetNumberOfLines(fptr, argv[1]);		//get the length of the input file
	
	SetArrayLength(length);							//set the length of our arrays
	
	ReadFileIntoArray(fptr, argv[1]);				//put the contents of the file into the arrays

	scheduleClock = incomingTime[0];
	ClearArrays();
	FirstComeFirstServed();									//Run FCFS algorithm   
	Result result = RunStatistics(startTime,stopTime);		//Get statistics
	printf("First Come, First Served\n");					//Print statistics
	PrintResults(result);
	
	ClearArrays();
	ClearList();
	scheduleClock = 0;
	
	ShortestJobFirst();
	printf("Shortest Job First\n");
	result = RunStatistics(startTime, stopTime);
	PrintResults(result);
	
	ClearArrays();
	ClearList();
	scheduleClock = 0;
	
	ShortestRemainingJobFirst();
	printf("Shortest Remaining Time First\n");
	result = RunStatistics(startTime, stopTime);
	PrintResults(result);

	ClearArrays();
	ClearList();
	scheduleClock = 0;
	
	RoundRobin(100);
	printf("Round Robin with Time Quantum of 100\n");
	result = RunStatistics(startTime, stopTime);
	PrintResults(result);



	return 0;
}

