#include <stdio.h>
#include <stdlib.h>

extern int* startTime;
extern int* stopTime;
extern int* responseTime;
extern int* waitTime;
extern int* turnAroundTime;
extern int* incomingTime;
extern int* burstTime;
extern int length;




int GetNumberOfLines(FILE* fptr, char* path)
{
	if ((fptr = fopen(path,"r")) == NULL)
	{
       printf("Error! opening file");
       return 0;
	}
	//printf("File opened successfully!\n");
	
	int lineCount = 0;
	char charIn = getc(fptr);
	while (charIn!= EOF)
	{
		if (charIn == '\n')
		{
		lineCount += 1;
		}
		charIn = getc(fptr);
	}
	fclose(fptr);
	return lineCount;
}
//set the lengths of our global storage arrays
void SetArrayLength(int length)
{
	startTime = malloc(sizeof(int)*length);
	stopTime = malloc(sizeof(int)*length);
	responseTime = malloc(sizeof(int)*length);
	waitTime =malloc(sizeof(int)*length);
	turnAroundTime = malloc(sizeof(int)*length);
	incomingTime = malloc(sizeof(int)*length);
	burstTime =malloc(sizeof(int)*length);
}

//Read the file into our global variables
void ReadFileIntoArray(FILE* fptr, char* path)
{
	if ((fptr = fopen(path,"r")) == NULL)
	{
       printf("Error! opening file");
	}
	//printf("File opened successfully!\n");
	int i = 0;
	int j = 0;
	int k = 0;
	fscanf(fptr,"%d %d", &i, &j);
	while (!feof(fptr))
	{
		//printf("%d %d\n",i,j);
		incomingTime[k] = i;
		burstTime[k] = j;
		k += 1;
		fscanf(fptr,"%d %d",&i,&j);
	}
	
	//USE THE FOLLOWING TO ERROR CHECK THE FILE READ IN PROCESS
	//for (int i = 0; i < length; i++)
	//{
	//	printf("Incoming Time: %d Burst Time: %d\n",incomingTime[i],burstTime[i]);
	//}
	fclose(fptr);
}
