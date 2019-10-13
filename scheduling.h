#include <stdbool.h>

//Struct for Doubly Linked List which I will use for the process queue
typedef struct Nodes{
	struct Nodes* next;
	struct Nodes* prev;
	int timeRemaining;
	int ID;
}Node;

typedef struct Results{
	double averageResponse;
	double averageWait;
	double averageTurnAround;
}Result;

void Enqueue(Node* node);
void Dequeue();
void ClearList();
void PlaceNodeInQueue(Node* node);
bool ExecuteForTime(int time);
void ExecuteRemainingQueue();
void ExecuteRemainingQueueRR(int cycleTime);
Result RunStatistics(int* startTime, int* stopTime);
void PrintResults(Result result);
void ClearArrays();
void PrintQueue();

void FirstComeFirstServed();
void RoundRobin(int cpuTime);
void ShortestJobFirst();
void ShortestRemainingJobFirst();
