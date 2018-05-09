# Sudoku
homework assignment to build a Sudoku game for the user .
the using of dynamic array is a must .
use the struct and main that given.
jump to [source code](https://github.com/dt170/Sudoku-/blob/master/ConsoleApplication1/ConsoleApplication1/Source.c)

# Struct
 ```
typedef struct _Array
{
	short *arr;
	unsigned short size;
} Array;
```
# main

```
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
```
