// connect4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream> 
using namespace std; 
#include <math.h>
#include <list>
#include <vector>
int PLAYER = 0;
int AI = 1;

int ROW_COUNT = 6;
int COLUMN_COUNT = 7;

int EMPTY = 0;
int PLAYER_PIECE = 1;
int AI_PIECE = 2;

int WINDOW_LENGTH = 4;

int** create_board()
{
	//creat 2d array (matrix 2d ) intialized by 0
	int** board = 0;
	board = new int*[ROW_COUNT];
	for (int i = 0; i < ROW_COUNT; ++i) {
		board[i] = new int[COLUMN_COUNT];
		for (int j = 0; j < COLUMN_COUNT; ++j)
		{
			board[i][j] = 0;
		}
	}
	return board;
}


void drop_piece(int** board, int row, int col, int piece)
{
	// set the value of certain row and column by the number of the player 
	//as he put a piece in this place 
	board[row][col] = piece;
}

bool is_valid_location(int** board, int col)
{
	// check if there is a place in thic col to put a piece on it 
   //law fe mkan wa7d fady a22der a7ot 3ady 
	if (board[ROW_COUNT - 1][col] == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int get_next_open_row(int** board, int col)
{
	// loop through all the row which =0 in this col 
	// first row =0 we will return its index 
	for (int r = 0; r < (ROW_COUNT); r++)
	{
		if (board[r][col]== 0)
		{
			return r;
		}
	}
	//no place 
	return 9;
}

void print_board(int** board)
{
	//print the board flipped 
	for (int i = ROW_COUNT-1; i >= 0; i--)
	{

		for (int j = 0; j <COLUMN_COUNT; j++)
		{
			cout << "|" << board[i][j] << " " << "|";
		}
		cout << endl;
	}
}

/////////////////////////////////////////////////////////////

int evaluate_window(vector<int>window, int piece)
{
	int score = 0;
	// initialize enemy piece = player =1
	//if the piece sent to the function is =1 then ai is the enemy ai player =2
	int opp_piece = PLAYER_PIECE;
	if (piece == PLAYER_PIECE)
	{
		opp_piece = AI_PIECE;
	}

	int count_Full = 0;
	int count_empty = 0;
	int opp_count = 0;
		//////if 4 places is full then win      
		for (int i = 0; i < window.size(); i++)
		{
			if (window[i] == piece)
			{
				count_Full += 1;
			}
			else if (window[i] == EMPTY)
				{
				   count_empty += 1;
				}
		
			else if (window[i] == opp_piece)
			{
				opp_count += 1;
			}
        }
if (count_Full == 4)
{
	score += 100;
}
//////if 3 places is full and one empty 
if ((count_Full == 3) && (count_empty == 1))
{
	score += 5;
}

//////if 2 places is full and 2 empty 
if ((count_Full == 2) && (count_empty == 2))
{
	score += 2;
}
// if the enemy complete 3 he is about to win we decrease the score 
if ((opp_count == 3) && (count_empty == 1))
{
	score -= 4;
}

return score;
}

int score_position(int** board, int piece) {
	int score = 0;
	int center_count = 0;
	std::vector<int> row_array;
	std::vector<int> center_array;
	std::vector<int> col_array;
	std::vector<int> window;
	// Score center column
	//  from the middle the chance is big to win from different directions so we increase the score more than the others 
	for (int r = 0; r < (ROW_COUNT); r++)
	{
		center_array.push_back(board[r][COLUMN_COUNT / 2]);
	}
	for (int i = 0; i < center_array.size(); i++)
	{
		if (center_array[i] == piece)
		{
			center_count += 1;
		}
	}
	score += center_count * 3;

		// Score Horizontal
		for (int r = 0; r < (ROW_COUNT); r++)
		{
			for (int j = 0; j < COLUMN_COUNT; j++)
			{
				//select all col values for a certain row 
				row_array.push_back(board[r][j]);
			}
			for (int c = 0; c < COLUMN_COUNT - 3; c++)
			{ //find a window of size 1*4 we stop at COLUMN_COUNT-3 as if we continue the last 3 windows cant be 1*4
			  //we find this window to see if any player connect 4 if true he win   

				for (int w = 0; w < WINDOW_LENGTH; w++)
				{
					window.push_back(row_array[c+w]);
				}
				score += evaluate_window(window, piece);
			}
		}

	//  Score Vertical	
	for (int j = 0; j < COLUMN_COUNT; j++)
	{
		for (int r = 0; r < (ROW_COUNT); r++)
		{
			//select all col values for a certain row 
			col_array.push_back(board[r][j]);
		}
		for (int RR = 0; RR < ROW_COUNT - 3; RR++)
		{ //find a window of size 4*1 we stop at ROW_COUNT-3 as if we continue the last 3 windows cant be 4*1
		  //we find this window to see if any player connect 4 if true he win   
			
				for (int w = 0; w < WINDOW_LENGTH; w++)
				{
					window.push_back(row_array[RR+w]);
				}
				score += evaluate_window(window, piece);
		}
	}

	// Score posiive sloped diagonal
	for (int c = 0; c < COLUMN_COUNT - 3; c++)
	{
		for (int RR = 0; RR < ROW_COUNT - 3; RR++)
		{
			for (int w = 0; w < WINDOW_LENGTH; w++)
			{
				window.push_back(board[RR + w][c + w]);
			}
			score += evaluate_window(window, piece);
		}
	}


	for (int RR = 0; RR < ROW_COUNT - 3; RR++)
	{
		for (int c = 0; c < COLUMN_COUNT - 3; c++)
		{
			for (int w = 1; w < WINDOW_LENGTH; w++)
			{
				window.push_back(board[RR + 3 - w][c + w]);
			}
			score += evaluate_window(window, piece);
		}
	}
	return score;

}

