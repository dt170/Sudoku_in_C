/*
Made by : dt170
*/
#define	_CRT_SECURE_NO_WARNINGS	
#include	<stdlib.h>
#include	<stdio.h>
#define SIZE_ROWS 9
#define SIZE_COLUMNS 9
#define TRUE 1
#define FALSE 0
#define ERORR -1

//struct
typedef struct _Array
{
	short *arr;
	unsigned short size;
} Array;

//declare functions
Array ***PossibleDigits(short **sudokuBoard);
void sudokoPrintBoard(short ** sudokoBoard);
void printPos(short *arr, unsigned short size);
short* possibleSolutions(short **sudokuBoard, int row, int column, Array**** possibilities);
int sameRow(short **suduko, int row, int column, short num);
int sameColumn(short **suduko, int row, int column, short num);
int sameSquare(short **suduko, int row, int column, short num);
void checkMemory(void *arr);
int FillBoard(short **board, Array ***possibilities);
void fillPlaceWithOneOption(short** suduko, Array ****possibilities);
void updateMinRowAndCol(short **suduko, int *row, int*col, Array ***possibilities);
int checkUserInput(short *arr, unsigned short size, int  usrInp);
int checkSuduko(short **board, Array ***possibilities);

void main()
{
	short board[81] = { 5,-1,4,-1,7,-1,-1,1,-1,6,-1,2,1,-1,-1,3,-1,-1,1,-1,8,-1,4,-1,-1,6,-1,-1,5,-1,-1,6,-1,-1,2,-1,-1,2,-1,8,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,5,6,-1,6,1,5,3,7,2,8,4,-1,8,7,-1,1,9,-1,3,-1,-1,-1,-1,2,8,-1,-1,-1,9 };
	short * sudoko_board = board;
	Array*** possible_solutions;

	printf("Initial board\n");
	sudokoPrintBoard(&sudoko_board);

	printf("Press enter to start playing...\n");
	getchar();

	possible_solutions = PossibleDigits(&sudoko_board);
	if (FillBoard(&sudoko_board, possible_solutions) == -1)
		printf("User's selections led to duplications\n");
	else
	{
		sudokoPrintBoard(&sudoko_board);
		printf("Board filled successfully\n");
	}
}
//This function get short arr and print it in sudoko board
void sudokoPrintBoard(short **sudoko_board) {
	printf("\n\n  | 0 1 2| 3 4 5| 6 7 8 \n");
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0)
			printf("-----------------------\n");
		printf(" %d", i);
		for (int j = 0; j < 9; j++) {
			if (j % 3 == 0)
				printf("|");
			if (*(*sudoko_board + j + 9 * i) == -1)
				printf("  ");
			else
				printf(" %d", *(*sudoko_board + j + 9 * i));
		}
		printf("\n");
	}
	printf("-----------------------\n\n\n");
}

//Function to return a matrix of possibilities to each cell in the sudoku board.
Array ***PossibleDigits(short **sudokuBoard) {
	Array ***matrix = (Array***)malloc(SIZE_COLUMNS * sizeof(Array**));

	//double loop to create the matrix.
	for (int rowPos = 0; rowPos < SIZE_ROWS; rowPos++)
	{
		matrix[rowPos] = (Array**)malloc(SIZE_ROWS * sizeof(Array*));
		for (int colPos = 0; colPos < SIZE_COLUMNS; colPos++)
		{
			matrix[rowPos][colPos] = (Array*)malloc(sizeof(Array));
			matrix[rowPos][colPos]->arr = possibleSolutions(sudokuBoard, rowPos, colPos, &matrix);
		}
	}
	return matrix;
}

//this function finds the possible solutions for the sudoku
short* possibleSolutions(short **sudokuBoard, int row, int column, Array**** possibilities) {
	unsigned short num = 1;
	int logicSize = 0;
	int maxOptionSize = 9;
	int minOptionSize = 2;
	short *tempArr;

	tempArr = (short*)malloc(maxOptionSize * sizeof(short));
	checkMemory(tempArr); //check the memory .
						  //if there is a number no need to find possible numbers
	if (*(*sudokuBoard + row * 9 + column) != -1) {
		(*possibilities)[row][column]->size = 0;
		return NULL;
	}
	//check what numbers can be assigned at that location.
	else {
		while (num < 10) {
			if (!sameRow(sudokuBoard, row, column, num) && !sameColumn(sudokuBoard, row, column, num) && !sameSquare(sudokuBoard, row, column, num)) {
				tempArr[logicSize] = num;
				logicSize++;
			}
			num++;
		}
		//making sure the array is small as can be to save space.
		if (logicSize>2)
			tempArr = (short*)realloc(tempArr, logicSize * sizeof(unsigned short));
		else
			tempArr = (short*)realloc(tempArr, minOptionSize * sizeof(unsigned short));

	}
	(*possibilities)[row][column]->size = logicSize; //setting the size of the array in the struct
	return tempArr;
}

//This function check if the number is found in another place in the row 
//return true if found another number else return false.
int sameRow(short **suduko, int row, int column, short num) {

	for (int i = 0; i < SIZE_ROWS; i++) {
		if (*(*suduko + row * 9 + i) == num)
			return TRUE;
	}
	return FALSE;
}

//This function check if the number is found in another place in the column 
//return true if found another number else return false.
int sameColumn(short **suduko, int row, int column, short num) {

	for (int i = 0; i < SIZE_COLUMNS; i++) {
		if (*(*suduko + i * 9 + column) == num)
			return TRUE;
	}
	return FALSE;
}

//This function check if the number is found in another place in the square
//return true if found another number else return false.
int sameSquare(short **suduko, int row, int column, short num) {
	if (row < 3) {
		row = 0;
	}
	else if (row < 6) {
		row = 3;
	}
	else {
		row = 6;
	}
	if (column < 3) {
		column = 0;
	}
	else if (column < 6) {
		column = 3;
	}
	else {
		column = 6;
	}
	for (int i = row; i < row + 3; i++) {
		for (int j = column; j < column + 3; j++) {
			if (*(*suduko + i * 9 + j) == num) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

//this function check the memory when creating dynamic arrays and structs
void checkMemory(void *arr) {
	if (arr == NULL) {
		puts("error: memory allocation has been detected");
		free(arr);
		exit(1);
	}
}

//this function handles the filling of the board returns -1 when error happens or 0 when filling successfully
int FillBoard(short **board, Array ***possibilities) {
	int minRowPos, minColPos;
	int userInput;
	int endSudoko = FALSE;
	int	check;
	while (!endSudoko&&endSudoko != -1) {
		fillPlaceWithOneOption(board, &possibilities); //calling function
		check = checkSuduko(board, possibilities);//calling function
		if (check == TRUE)
			return 0;
		if (check == ERORR)
			return ERORR;
		updateMinRowAndCol(board, &minRowPos, &minColPos, possibilities);//calling function
		sudokoPrintBoard(board);//calling function
		printf("Cell number [%d,%d] currently holds the minimum number of possible values, select one of the below:\n", minRowPos, minColPos);
		printPos(possibilities[minRowPos][minColPos]->arr, possibilities[minRowPos][minColPos]->size);
		scanf("%d", &userInput);
		printf("Updating cell %d with value %d\n\n", minRowPos*SIZE_COLUMNS + minColPos, userInput);
		(*board)[minRowPos * 9 + minColPos] = (short)userInput;
		//If the user input is different that the choices he had been given, it means there are duplications.
		if (!checkUserInput(possibilities[minRowPos][minColPos]->arr, possibilities[minRowPos][minColPos]->size, (short)userInput)) {
			return ERORR;
		}
		possibilities = PossibleDigits(board);
	}
	return endSudoko;
}

//this function check all the empty places that have 1 option of filling and update the board with those values 
void fillPlaceWithOneOption(short** suduko, Array ****possibilities) {
	int flagLoop = FALSE;
	int flagUpdate = FALSE;
	while (!flagLoop) {
		//checks all the places that empty
		for (int rowPos = 0; rowPos < SIZE_ROWS; rowPos++)
		{
			for (int colPos = 0; colPos < SIZE_COLUMNS; colPos++)
			{
				if (*(*suduko + rowPos * 9 + colPos) == -1) //if empty and have 1 option it will fill the board
					if ((*possibilities)[rowPos][colPos]->size == 1) {
						*(*suduko + rowPos * 9 + colPos) = (*possibilities)[rowPos][colPos]->arr[0];
						free((*possibilities)[rowPos][colPos]->arr); //free the memory
						flagUpdate = TRUE;
						(*possibilities) = PossibleDigits(suduko);
					}
			}
		}
		if (flagUpdate == FALSE)
			flagLoop = TRUE;

		flagUpdate = FALSE;
	}
}

//finds the index of the row&col that represent the minimum possibilities
void updateMinRowAndCol(short **suduko, int *row, int*col, Array ***possibilities) {
	int firstPair = TRUE;//flag to mark the first pair to be set as minimum row,col
	int minRow, minCol;
	for (int rowPos = 0; rowPos < SIZE_ROWS; rowPos++) {
		for (int colPos = 0; colPos < SIZE_COLUMNS; colPos++) {
			if (*(*suduko + rowPos * 9 + colPos) == -1)
				if (firstPair) {// if this is the first pair enter 
					minRow = rowPos;
					minCol = colPos;
					firstPair = FALSE; // change the state and continue the loop to find the lowest .
					if ((possibilities)[rowPos][colPos]->arr && (possibilities)[minRow][minCol]->size >(possibilities)[rowPos][colPos]->size) {
						minRow = rowPos;
						minCol = colPos;
					}
				}
		}
	}
	//setting the min values 
	*row = minRow;
	*col = minCol;
}

//Function to print a given cell posibilities.
void printPos(short *arr, unsigned short size) {
	for (int i = 0; i < size; i++) {
		printf("%d. %d\n", i + 1, arr[i]);
	}
}

//Function to return true if the user input is a legal input or false if the input is different from the choices he had been given. 
int checkUserInput(short *arr, unsigned short size, int  userInput) {
	for (unsigned short i = 0; i < size; i++)
		if (userInput == arr[i])
			return TRUE;
	return FALSE;
}

//this function checks if the user entered the right answer meaning his choice wont create duplication
int checkSuduko(short **board, Array ***possibilities) {
	int boardComplete = TRUE;
	//checks all the places that still empty also check for duplication or finish
	for (int rowPos = 0; rowPos < SIZE_ROWS; rowPos++)
	{
		for (int colPos = 0; colPos < SIZE_COLUMNS; colPos++)
		{
			if (*(*board + rowPos * 9 + colPos) == -1) {
				boardComplete = FALSE;
				if (possibilities[rowPos][colPos]->size == 0)
					return boardComplete = ERORR;
			}
		}
	}
	return boardComplete;
}
