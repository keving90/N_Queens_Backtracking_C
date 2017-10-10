/*****************************************************************************************
Prologue

UCSC Extension: Advanced C Programming

Instructor: Rajainder A. Yeldandi

Author: Kevin Geiszler

Assignment Number: 7, N Queens

Topics: Backtracking

File Name: assignment7_KevinGeiszler.c

Date: 2/20/2017

Objective: This program places 8 queens on a chess board in a manner such that none of the
		   queens are capable of attacking each other.
		   
Comments:  The user is prompted to place a queen in the first row at any column they
		   desire. The remaining 7 queens are then placed in each following row
		   automatically. The user is then asked if they would like to continue. If the
		   user enters an invalid number, then an error occurs.

*****************************************************************************************/

//Preprocessor directives
#include <stdio.h>
#include <stdlib.h>

#define BOARDSIZE 8
#define DIAGONAL (2 * BOARDSIZE - 1)
#define DOWNOFFSET (DIAGONAL - BOARDSIZE)
#define FALSE 0
#define TRUE 1

/****************************************************************************************/

//Function prototypes
void writeBoard(FILE *pOut);
void clearBoard(void);
void addQueen(FILE *pOut, int colSpot);
void getUserInput(FILE *pOut, int *pColSpot);
void printErrorMessage(void);

/****************************************************************************************/

//The main function

//global variables
int queenCol[BOARDSIZE]; //queen column
int colFree[BOARDSIZE]; //is column free
int upFree[DIAGONAL]; //up diagonal free
int downFree[DIAGONAL]; //down diagonal free
int queenCount = -1; //row where queen is placed

int main(int argc, char *argv[])
{
	int colSpot;
	int *pColSpot = &colSpot;
	char response = 'y';
	FILE *pOut;
	
	//Check command line arguments
	if (argc != 2)
	{
		printf("Error! Need following file usage: %s outputFile\n", argv[0]);
		exit(1);
	}
	
	//Check if the output file has been successfully opened
	if ((pOut = fopen(argv[1], "w")) == NULL)
	{
		printf("Error opening %s.\n", argv[1]);
		exit(1);
	}
	
	while(response == 'y' || response == 'Y')
	{
	
		printf("Please enter the column number (%d to %d) for the first queen placement: ", 0, BOARDSIZE - 1);
	
		getUserInput(pOut, pColSpot);
		clearBoard();
		queenCol[++queenCount] = colSpot;
		colFree[colSpot] = FALSE;
		upFree[queenCount + colSpot] = FALSE;
		downFree[queenCount - colSpot + DOWNOFFSET] = FALSE;
		addQueen(pOut, colSpot);
		
		printf("Would you like to continue? Enter 'y' or 'Y' to continue. Any other character exits the program: ");
		
		scanf(" %c", &response);
		
		while(getchar() != '\n')
			continue;
		
		if (response == 'y' || response == 'Y')
			fprintf(pOut, "The user chose to continue using the program.\n\n");
		else
			fprintf(pOut, "The user chose to exit the program.\n\n");
		
		printf("\n");
	}
	
	fclose(pOut);
	
	return 0;
} //end main function

/****************************************************************************************/

void addQueen(FILE *pOut, int colSpot)
{
	int col; //column being tried for the queen
	queenCount++;
	
	for (col = 0; col < BOARDSIZE; col++)
	{
		if (colFree[col] && upFree[queenCount + col] &&
			downFree[queenCount - col + DOWNOFFSET])
		{
			//put the queen in position (queenCount, col)
			queenCol[queenCount] = col;
			colFree[col] = FALSE;
			upFree[queenCount + col] = FALSE;
			downFree[queenCount - col + DOWNOFFSET] = FALSE;
			
			if (queenCount == BOARDSIZE - 1)
			{
				//terminal condition
				printf("\n %d Queen solution with first queen in column: %d\n\n", BOARDSIZE, colSpot);
				fprintf(pOut, "\n %d Queen solution with first queen in column: %d\n\n", BOARDSIZE, colSpot);
				writeBoard(pOut);
			}
			else
				addQueen(pOut, colSpot); //recursive call
			
			if (queenCount - 1 < 0)
				continue;
			
			colFree[col] = TRUE; //backtrack queen
			upFree[queenCount - 1 + col] = TRUE;
			downFree[queenCount - 1 - col + DOWNOFFSET] = TRUE;
			queenCount--;
		} //end if colFree
	} //end for loop
} //end addQueen()

/****************************************************************************************/

//Print the output of N queen placement

void writeBoard(FILE *pOut)
{
	int col;
	static int qCount = 0;
	
	for (col = 0; col < BOARDSIZE; col++)
	{
		if ((queenCol[qCount] == col) && (qCount == 0))
		{
			printf("U   ");
			fprintf(pOut, "U   ");
		}
		else if ((queenCol[qCount] == col) && (qCount != 0))
		{
			printf("Q   ");
			fprintf(pOut, "Q   ");
		}
		else
		{
			printf("*   ");
			fprintf(pOut, "*   ");
		}
	} //end for loop
	printf("\n\n");
	fprintf(pOut, "\n\n");
	
	if (qCount++ < BOARDSIZE - 1)
		writeBoard(pOut);
	
	qCount = 0;
} // end writeBoard()

/****************************************************************************************/

//Clear the board for next placement

void clearBoard(void)
{
	for (int i = 0; i < BOARDSIZE; i++)
	{
		colFree[i] = TRUE;
		queenCol[i] = -1;
	} //end for loop
	
	for (int j = 0; j < DIAGONAL; j++)
	{
		upFree[j] = TRUE;
		downFree[j] = TRUE;
	} //end for loop
	
	queenCount = -1;
} //end clearBoard()

/****************************************************************************************/

//Prompt the user to choose where they would like to place the first queen in row 0

void getUserInput(FILE *pOut, int *pColSpot)
{
	while(1)
	{
		//check for integer input
		while(scanf(" %d", pColSpot) != 1)
		{
			printErrorMessage();
			fprintf(pOut, "Error, the user didn't enter an integer for their choice.\n\n");
			
			//clear buffer
			while(getchar() != '\n')
				continue;
				
			continue;
		}
		
		//check that the integer input can be placed on the board
		if (*pColSpot > BOARDSIZE-1 || *pColSpot < 0)
		{
			printErrorMessage();
			fprintf(pOut, "Error, the user's choice cannot be placed on the board: %d\n\n", *pColSpot);
			
			//clear buffer
			while(getchar() != '\n')
				continue;
				
			continue;
		}
		
		//clear buffer
		while(getchar() != '\n')
			continue;
		
		fprintf(pOut, "The user chose to place a queen at row 0 in column %d.\n", *pColSpot);
		
		break;
	}
}

/****************************************************************************************/

//A common error message that's displayed to the console during certain situations

void printErrorMessage(void)
{
	printf("Error, please enter an integer between %d and %d: ", 0, BOARDSIZE-1);
}

/****************************************************************************************/

















