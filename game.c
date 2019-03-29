#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#define maxDepth 4
int nodes = 0;

//this struct represent for point
struct Point
{
	int x;
	int y;
};
// this struct store the action/move by player
struct Action 
{
	int row;
	int col;
	int value;
};
// this struct store game state
struct GameState
{
	int rows;
	int cols;
	struct Action lastAction;
	char grid[6][7];
};
// prototypes of functions
struct Action max(struct GameState *state, int depth);
struct Action min(struct GameState *state, int depth);

// this function finds the substring 
int contains(char* str, char* sub)
{
	char *ptr = strstr(str, sub);

	if (ptr != NULL) 
	{
		return 1;
	}
	return 0;
}
// this function validates the player move
int validateMove(struct GameState *state,int column)
{
	 //if condition to check the empty column by matching "-"
	 if(state->grid[0][column] == '-')
	    	return 1;
	    return 0;	   
}
// this function prints the board
void printBoard(int rows, int cols,GameState *state)
{
	printf("\n");
	int i=0;
	for(i=0;i<rows;i++)
    {
		int j=0;
        for(j=0;j<cols;j++)
        {
            printf("%c ",state->grid[i][j]);
        }
            printf("\n");
   }  
}
// this function generate random move
int getRandomMove(struct GameState *state,int lowerBound, int upperBound)
{
	int randomInt = (rand() % (upperBound - lowerBound)) + lowerBound;
	while(1)
	{
		if(validateMove(state,randomInt) == 0)// validate move
			randomInt = (rand() % (upperBound - lowerBound)) + lowerBound;	
		else break;
	}
	return randomInt;
}

// this function check if there is connect four horizontally 
int horizontalValidation(char* pair, struct GameState *state)
{
	char str[7];
	int i=0;
	for(i=0; i<6; i++)
	{
		int j=0;
		for(j=0;j<7;j++)
		{
			str[j] = state->grid[i][j];
		}
		if(contains(str,pair) == 1)
			return 1;
	}
	return 0;
}

// this function check if there is connect four vertically
int verticalValidation(char* pair, struct GameState *state)
{
	char str[6];
	int i=0;
	
	for(i=0; i<7; i++)
	{
		int j=0;
		for(j=0;j<6;j++)
		{
			str[j] = state->grid[j][i];
		}
		if(contains(str,pair) == 1)
			return 1;
	}
	return 0;
}


// this function validate winer
int goalState(char agent,struct GameState *state)
{
	int rows = 6;
	int cols = 7;
	int result = 0;
	char xpair[]="XXXX";
	char opair[]="OOOO";
	
	if(agent == 'X')
	{
		result = horizontalValidation(xpair,state);
		if(result == 0)
		{
			result = verticalValidation(xpair,state);
		}
	}
	else
	{
		result = horizontalValidation(opair,state);
		if(result == 0)
		{
			result = verticalValidation(opair,state);
		}
	}
	
	struct Point rightDiag[500];
	struct Point leftDiag[500];
	int rightDiagCount = 0;
	int leftDiagCount = 0;
		
	//validate diagonals
	if(result == 0)
	{	
		for(int j=0; j<cols-4+1; j++)
		{
			rightDiag[rightDiagCount].x = 0;
			rightDiag[rightDiagCount].y = j;
			rightDiagCount++;
		}
		for(int j=4-1; j<cols; j++)
		{
			leftDiag[leftDiagCount].x = 0;
			leftDiag[leftDiagCount].y = j;
			leftDiagCount++;
		}
		for(int i=1; i<rows-4+1; i++)
		{
			
			rightDiag[rightDiagCount].x = i;
			rightDiag[rightDiagCount].y = 0;
			rightDiagCount++;
			
			leftDiag[leftDiagCount].x = i;
			leftDiag[leftDiagCount].y = cols-1;
			leftDiagCount++;
		}
		
		//right
		for(int i=0;i<rightDiagCount;i++)
		{
			int k = 0;
			char d[rightDiagCount];
			int x = rightDiag[i].x;
			int y = rightDiag[i].y;
			while(1)
			{				
				if (x >= rows || y >= cols)
					break;
				d[k] = state->grid[x][y];
				k++;
				x+=1; y+=1;
			}
			if(agent == 'X')
			{
				if(contains(d,xpair) == 1)// validate for winner
					return 1;
			}
			else
			{
				if(contains(d,opair) == 1)// validate for winner
					return 1;
			}
		}
		
		//left
		for(int i=0;i<leftDiagCount;i++)
		{
			char d[leftDiagCount];
			int x = leftDiag[i].x; 
			int y = leftDiag[i].y;
			int k = 0;
			while(1)
			{
				if(y <0 || x >= rows || y>= cols)
					break;
				d[k] = state->grid[x][y];
				k++;
				x+=1; y-=1;
			}
			if(agent == 'X')
			{
				if(contains(d,xpair) == 1)
					return 1;
			}
			else
			{
				if(contains(d,opair) == 1)
					return 1;
			}
		}
	}
		
	return result;	
}

// initialize board and game state
void initBoard(int rows, int cols,struct GameState *state)
{
	state->rows = rows;
	state->cols = cols;
	state->lastAction.row = -1;
	state->lastAction.col = -1;
	state->lastAction.value = 0;
	int i=0;
	for(i=0;i<rows;i++)
	{
		int j=0;
		for(j=0;j<cols;j++)
		{
			state->grid[i][j]='-';
		}
	}
}

struct GameState* sucessorStateDoubleMode(char player, int row,int col,struct GameState *state)
{	
	struct GameState *newState;
	newState = state;
	newState->grid[row][col] = player;
	newState->lastAction.row = row;
	newState->lastAction.col = col;
	newState->lastAction.value = -1;
	return newState;
}

// this function generate successor state 
struct GameState* sucessorState(char player, int action,struct GameState *state)
{	
	int row;
	struct GameState *newState;
	for(row=0; row < state->rows && state->grid[row][action]!='X' && state->grid[row][action]!='O'; row++);
	newState = state;
	newState->grid[row-1][action] = player;
	newState->lastAction.row = row-1;
	newState->lastAction.col = action;
	newState->lastAction.value = -1;
	return newState;
}

/// search board to move
int canMove(int row, int col) 
{
	if ((row <= -1) || (col <= -1) || (row > 5) || (col > 6)) 
	{
		return 0;
	}
	return 1;
}

// validate given disk sequence in row
int check3InARow(char disk,struct GameState *state) {
	
	int times = 0;
	// validate sequence of three of given disk horizontally
	for (int i = 5; i >= 0; i--) 
	{
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i, j + 2)) {
				if (state->grid[i][j] == state->grid[i][j + 1]
						&& state->grid[i][j] == state->grid[i][j + 2]
						&& state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}
	// validate sequence of three of given disk vertically
	for (int i = 5; i >= 0; i--) {
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i - 2, j)) {
				if (state->grid[i][j] == state->grid[i - 1][j]
						&& state->grid[i][j] == state->grid[i - 2][j]
						&& state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}

	// validate sequence of three of given disk in diagonal 
	for (int i = 0; i < state->rows; i++) {
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i + 2, j + 2)) {
				if (state->grid[i][j] == state->grid[i + 1][j + 1]
						&& state->grid[i][j] == state->grid[i + 2][j + 2]
						&& state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}

	// validate sequence of three of given disk in diagonal 
	for (int i = 0; i < state->rows; i++) {
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i - 2, j + 2)) {
				if (state->grid[i][j] == state->grid[i - 1][j + 1]
						&& state->grid[i][j] == state->grid[i - 2][j + 2]
						&& state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}

	return times;
			
}

//validate given disk sequence in row
 int check2InARow(char disk,struct GameState *state) {
	
	int times = 0;
	
	// validate sequence of three of given disk horizontally
	for (int i = 5; i >= 0; i--) {
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i, j + 1)) {
				if (state->grid[i][j] == state->grid[i][j + 1]
						&&state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}

	// validate sequence of three of given disk vertically
	for (int i = 5; i >= 0; i--) {
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i - 1, j)) {
				if (state->grid[i][j] == state->grid[i - 1][j]
						&&state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}

	// validate sequence of three of given disk in diagonal 
	for (int i = 0; i < state->rows; i++) {
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i + 1, j + 1)) {
				if (state->grid[i][j] == state->grid[i + 1][j + 1]
						&&state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}

	// validate sequence of three of given disk in diagonal 
	for (int i = 0; i < state->rows; i++) {
		for (int j = 0; j < state->cols; j++) {
			if (canMove(i - 1, j + 1)) {
				if (state->grid[i][j] == state->grid[i - 1][j + 1]
						&& state->grid[i][j] == disk) {
					times++;
				}
			}
		}
	}

	return times;
			
}
// validate if game state is at goal
int reachAtGoal(struct GameState *state) 
{
	if (goalState('O',state) == 1)
		return 1;
	if (goalState('X',state) == 1)
		return 1;
	return 0;
}
// evaluate move 
int evaluate(struct GameState *state)
{	
	int XConnect = 0;
	int OConnect = 0;

	if(goalState('X',state) == 1) 
	{
		XConnect = XConnect + 100;
	}
	else if (goalState('O',state) == 1) 
	{
		OConnect = OConnect + 100;
	}
		
	XConnect  = XConnect + check3InARow('X',state)*10 + check2InARow('X',state);
    OConnect  = OConnect + check3InARow('O',state)*10 + check2InARow('O',state);
		
	return XConnect - OConnect;
}
// get valid row to move
int getAppropriateRow(int col,struct GameState *state) 
{
		int rowPos = -1;
		for (int row=0; row<state->rows; row++) 
		{
			if (state->grid[row][col] == '-') 
			{
				rowPos = row;
			}
		}
		return rowPos;
}

// generate moves for minmax
struct GameState makeMove(int col, char letter,struct GameState *state)
{
	//Action lastAction = new Action(getAppropriateRow(col), col);
	int rowAction = getAppropriateRow(col,state);
	struct GameState newState = *state;
	newState.grid[rowAction][col] = letter;
	newState.lastAction.row = rowAction;
	newState.lastAction.col = col;
	newState.lastAction.value = -1;
	return newState;
}

// validate col
int validateCol(int col,struct GameState *state) 
{
		if (state->grid[0][col] == '-')
			return 0;
		return 1;
}
// get valid move helper function for make move
struct GameState* getValidMove(char letter,struct GameState *state)
{	
	int index = 0;
	nodes = 0;
	struct GameState *children = (struct GameState*)malloc(1000*sizeof(struct GameState));
	for(int col=0; col< state->cols; col++) 
	{
		if(!validateCol(col,state)) 
		{
			struct GameState temp = makeMove(col,letter,state);
			//printf("\ndd%d",temp->lastAction.value);
			children[index] = temp;
			index++;
			nodes++;
		}
	}
	return children;
}
// remove disk from the board
struct GameState* removeDisk(char player,struct GameState *state) 
{
	int removed = 0;
	int ithCol = 0;
	for(int i=0;i<state->cols;i++)
	{
		if(state->grid[state->rows-1][i] == player)
		{
			state->grid[state->rows-1][i] = '-';// disk removed
			removed = 1;
			ithCol = i;
			break;
		}
	}
	
	if(removed == 1)// if disk removed below down above disks
	{
		for(int i=state->rows-1;i>0;i--)
		{
			state->grid[i][ithCol] = state->grid[i-1][ithCol];
		}
	}
	return state;
}


struct GameState sucessorStateMinMax(char player, int action,GameState *state)
{
	return makeMove(action,player,state);
}
// minmax algorithm
struct Action MiniMax(struct GameState *state,char disk)
{
        if (disk == 'X') 
        {
            return max(state, 0);
        }
        else 
        {
            return min(state, 0);
        }
}
// min function
struct Action min(struct GameState *state, int depth)
{
		if((reachAtGoal(state)) || (depth == maxDepth)) // base case
		{
			struct Action lastAction;
			lastAction.row = state->lastAction.row;
			lastAction.col = state->lastAction.col;
			lastAction.value = evaluate(state);
			return lastAction;
		}
		
    	struct GameState *childNodes = getValidMove('O',state);// get child nodes
		int childSize = nodes;
		struct Action minAction;
		minAction.row = -1;
		minAction.col = -1;
		minAction.value = 10000;
		int i=0;
		
		while(i< childSize)
		{
			struct GameState child = childNodes[i];
			//printf("\nchild%d",child.lastAction.value);
			struct Action action = max(&child, depth + 1);
			if(action.value <= minAction.value) 
			{
                if ((action.value == minAction.value)) 
                {
                        minAction.row = child.lastAction.row;
                        minAction.col = child.lastAction.col;
                        minAction.value = action.value;
                    
                }
                else 
				{
                        minAction.row = child.lastAction.row;
                        minAction.col = child.lastAction.col;
                        minAction.value = action.value;
                }
            }
			i++;
        }
        return minAction;
	}

//max function
struct Action max(struct GameState *state, int depth)
	{
        
		if((reachAtGoal(state)) || (depth == maxDepth))// base case
		{
			struct Action lastAction;
			lastAction.row = state->lastAction.row;
			lastAction.col = state->lastAction.col;
			lastAction.value = evaluate(state);
			return lastAction;
		}
    	struct GameState *childNodes = getValidMove('X',state);// get child nodes
		int childSize = nodes;
		struct Action maxAction;
		maxAction.row = -1;
		maxAction.col = -1;
		maxAction.value = -10000;
		int i=0;
		//for(int i =0; i<childNodes.size();i++)
		while(i < childSize)
		{
			struct GameState child = childNodes[i];
			//printf("\nchildm%d",child.lastAction.value);
			struct Action action = min(&child, depth + 1);
			if(action.value >= maxAction.value) // select max child node 
			{
                if ((action.value == maxAction.value)) 
                {
                        maxAction.row = child.lastAction.row;
                        maxAction.col = child.lastAction.col;
                        maxAction.value = action.value;
                }
                else 
				{
						maxAction.row = child.lastAction.row;
                        maxAction.col = child.lastAction.col;
                        maxAction.value = action.value;
                }
			}
			i++;
		}
		return maxAction;
	}

//max prunning
struct Action maxAlphaBeta(struct GameState *state, int depth, double a, double b)
	{

    	if((reachAtGoal(state)) || (depth == maxDepth))//base case
		{
			struct Action lastAction;
			lastAction.row = state->lastAction.row;
			lastAction.col = state->lastAction.col;
			lastAction.value = evaluate(state);
			return lastAction;
		}
		
		struct GameState *childNodes = getValidMove('X',state);// get child nodes
		int childSize = nodes;
		struct Action maxAction;
		maxAction.row = -1;
		maxAction.col = -1;
		maxAction.value = -10000;
		int i=0;

		
		for(int i =0; i<childSize;i++)
		{
			struct GameState child = childNodes[i];
			struct Action action = min(&child, depth + 1);
			if(action.value >= maxAction.value) // select max child
			{
                if ((action.value == maxAction.value)) 
                {
                        maxAction.row = child.lastAction.row;
                        maxAction.col = child.lastAction.col;
                        maxAction.value = action.value;                  
                }
                else 
				{
						maxAction.row = child.lastAction.row;
                        maxAction.col = child.lastAction.col;
                        maxAction.value = action.value;     
                }
			}
			
	        if (maxAction.value >= b)  return maxAction; //beta prune

	        // Update the alpha of the current max node.
	        a = (a > maxAction.value) ? a : maxAction.value;
		}
		
		return maxAction;
	}

//min prunning
struct Action minAlphaBeta(struct GameState *state, int depth, double a, double b) 
{

		if((reachAtGoal(state)) || (depth == maxDepth)) 
		{
			struct Action lastAction;
			lastAction.row = state->lastAction.row;
			lastAction.col = state->lastAction.col;
			lastAction.value = evaluate(state);
			return lastAction;
		}
		struct GameState *childNodes = getValidMove('O',state);// get child nodes
		int childSize = nodes;
		struct Action minAction;
		minAction.row = -1;
		minAction.col = -1;
		minAction.value = 10000;
		int i=0;
			
		for(int i =0; i<childSize;i++)
		{
			struct GameState child = childNodes[i];
			struct Action action = max(&child, depth + 1);
			if(action.value <= minAction.value) 
			{
                if ((action.value == minAction.value)) 
				{
                        minAction.row = child.lastAction.row;
                        minAction.col = child.lastAction.col;
                        minAction.value = action.value;
                    
                }
                else {
	                    minAction.row = child.lastAction.row;
                        minAction.col = child.lastAction.col;
                        minAction.value = action.value;
                }
            }
            if (minAction.value <= a) return minAction;// alpha prune

            // Update the beta of the current min node.
            b = (b < minAction.value) ? b : minAction.value;
        }
        return minAction;
	}
int main()
{
	srand(time(NULL));// seed for random number
	
	// declare variables 
	int depth = 4;
	int rows = 6;
	int cols = 7;
	int round = 1;
	int oScore = 0;
	int xScore = 0;
	int owins = 0;
	int xwins = 0;
	
	while(1)
	{
		// welcome message
	struct GameState state;
	printf("\n--------------------------");
	printf("\nRound: %d",round);
	printf("\nWelcome to Four Connect Game");
	printf("\nGame Depth: %d",depth);
	printf("\nGame Dimensions: %d,%d",rows,cols);
	printf("\n---------------------------\n");
	
	// user choice
	printf("\n1. MinMax vs Human");
	printf("\n2. MinMax vs Random (Disk Removal)");
	printf("\n3. MinMax vs Random (Without Disk Removal)");
	printf("\n4. Player vs Player");
	printf("\n5. Exit");
	int option;
	printf("\n Enter Option: ");
	scanf("%d",&option);
	
	initBoard(rows,cols,&state);
	
	if(option == 2 || option == 3)
	{
	// play game
	int i=0;
	int iterations = 0;
		while(1)
		{
			
			printf("\nPlayer X Turn's (Random)");
			int rm = getRandomMove(&state, 0, cols);
			state = *sucessorState('X',rm,&state);
			printBoard(rows,cols,&state);
			
			if(goalState('X',&state) ==  1)// check for winner
			{
				printf("\nPlayer X won");
				xwins++;
				xScore +=10;
				break;
			}
			
			if(iterations == 3 && option == 2)// after each 5 iterations the player O will able to remove its disk
			{
				iterations = 0;
				state = *removeDisk('O',&state);
				printf("\nGame State After Disk Removal\n");
				printBoard(rows,cols,&state);
				
			}
			
			printf("\nPlayer O Turn's (MinMax)");			
			struct Action action = MiniMax(&state,'O');				
			state = sucessorStateMinMax('O', action.col,&state);
			printBoard(rows,cols,&state);
			
			if(goalState('O',&state) == 1)// check for winner
			{
				printf("\nPlayer O won");
				owins++;
				oScore +=10;
				break;
			}
			iterations++;
			i++;
			
		}//end while
	}
	else if(option == 1)
	{
		// play game
		while(1)
		{
			
			printf("\nPlayer X Turn's (Human)");
			int c;
			printf("\nEnter (col) range(0-6): ");
			scanf("%d",&c);
			while(c > 6)
			{
				printf("Invalid col:");
				printf("\nEnter Col) range(0-6): ");
				scanf("%d",&c);
			}
						
			state = *sucessorState('X',c,&state);
			printBoard(rows,cols,&state);
			
			if(goalState('X',&state) ==  1)// check for winner
			{
				printf("\nPlayer X won");
				xwins++;
				xScore +=10;
				break;
			}
			
			printf("\nPlayer O Turn's (MinMax)");			
			struct Action action = MiniMax(&state,'O');				
			state = sucessorStateMinMax('O', action.col,&state);
			printBoard(rows,cols,&state);
			
			if(goalState('O',&state) == 1)// check for winner
			{
				printf("\nPlayer O won");
				owins++;
				oScore +=10;
				break;
			}
			
			
		}//end while
	}
	else if(option == 4)// player vs player mode
	{
		// play game
		while(1)
		{
			
			printf("\nPlayer X Turn's (Player-1)");
			int c,r;
			printf("\nEnter (row col) range(row: 0-5, col:0-6): ");
			scanf("%d %d",&r,&c);
			while(c > 6 || r > 5 || state.grid[r][c] != '-')
			{
				printf("Invalid col or row:");
				printf("\nEnter (row col) range(row: 0-5, col:0-6): ");
				scanf("%d %d",&r,&c);
			}
						
			state = *sucessorStateDoubleMode('X',r,c,&state);
			printBoard(rows,cols,&state);
			
			if(goalState('X',&state) ==  1)// check for winner
			{
				printf("\nPlayer X won");
				xwins++;
				xScore +=10;
				break;
			}
			
			printf("\nPlayer O Turn's (Player-2)");		
			printf("\nEnter (row col) range(row: 0-5, col:0-6): ");
			scanf("%d %d",&r,&c);
			while(c > 6 || r > 5|| state.grid[r][c] != '-')
			{
				printf("Invalid col or row:");
				printf("\nEnter (row col) range(row: 0-5, col:0-6): ");
				scanf("%d %d",&r,&c);
			}			
			state = *sucessorStateDoubleMode('O',r,c,&state);
			printBoard(rows,cols,&state);
			
			if(goalState('O',&state) == 1)// check for winner
			{
				printf("\nPlayer O won");
				owins++;
				oScore +=10;
				break;
			}
			
			
		}//end while
	}
	else if(option == 5)// exit
	{
		exit(0);
	}
	
	// print score 
	printf("\n---------------------------");
	printf("\nX Score: %d, O Score: %d",xScore,oScore);
	printf("\nWin Ration: X: %d, O: %d",xwins,owins);
	}
	return 0;
}