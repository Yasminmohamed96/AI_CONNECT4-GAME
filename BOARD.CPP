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

int max(int a, int b) { if (a > b) return a; else return b; }
int min(int a, int b) { if (a < b) return a; else return b; }

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

void copy(int** A, int**& B)
{
	for (int i = 0; i < ROW_COUNT; ++i) 
	{
		for (int j = 0; j < COLUMN_COUNT; ++j)
		{
			B[i][j] = A[i][j];
		}
	}
}

void drop_piece(int** board, int row, int col, int piece)
{
	if (row < 6 && col< 7)
	{
		// set the value of certain row and column by the number of the player 
		//as he put a piece in this place 
		board[row][col] = piece;
	}
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
			for (int w = 0; w < WINDOW_LENGTH; w++)
			{
				window.push_back(board[RR + 3 - w][c + w]);
			}
			score += evaluate_window(window, piece);
		}
	}
	return score;

}

//////////////////////////////
bool winning_move(int** board, int piece)
{
	// Check horizontal locations for win
	for (int c = 0; c < (COLUMN_COUNT - 3); c++)
	{
		for (int r = 0; r < (ROW_COUNT); r++)
		{
			if (board[r][c] == piece && board[r][c + 1] == piece && board[r][c + 2] == piece && board[r][c + 3] == piece)
				return true;
		}
	}

	// Check vertical locations for win
	for (int c = 0; c < (COLUMN_COUNT); c++)
	{
		for (int r = 0; r < (ROW_COUNT - 3); r++)
		{
			if (board[r][c] == piece && board[r + 1][c] == piece && board[r + 2][c] == piece && board[r + 3][c] == piece)
				return true;
		}
	}
	// Check positively sloped diaganols
	for (int c = 0; c < (COLUMN_COUNT - 3); c++)
	{
		for (int r = 0; r < (ROW_COUNT - 3); r++)
		{
			if (board[r][c] == piece && board[r + 1][c + 1] == piece && board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece)
				return true;
		}
	}
	// Check negatively sloped diaganols
	for (int c = 0; c < (COLUMN_COUNT - 3); c++)
	{
		for (int r=0; r<(3, ROW_COUNT);r++)
		{
			if (board[r][c] == piece && board[r - 1][c + 1] == piece && board[r - 2][c + 2] == piece && board[r - 3][c + 3] == piece)
				return true;
		}
	}
}

vector<int> get_valid_locations(int** board)
{
	vector <int> valid_locations;
	for (int col = 0; col < (COLUMN_COUNT); col++)
	{
		if (is_valid_location(board, col))
			valid_locations.push_back(col);
	}
	return valid_locations;
}

bool is_terminal_node(int** board)
{
	if (winning_move(board, PLAYER_PIECE) == 0 || winning_move(board, AI_PIECE) == 0 || get_valid_locations(board).size() == 0)
		return true;
	else
		return false;
}
///////////////////////////////////////
int* minimax(int** board, int depth, int alpha, int beta, int maximizingPlayer)
{
	int col_score[2];
	vector <int> valid_locations = get_valid_locations(board);
	bool is_terminal = is_terminal_node(board);
		if (depth == 0 || is_terminal)
		{
			if (is_terminal)
			{
				if (winning_move(board, AI_PIECE))
				{
					col_score[0] = -1;
					col_score[1] = 100000000000000;
					return col_score;
				}
				else if (winning_move(board, PLAYER_PIECE))
				{
					col_score[0] = -1;
					col_score[1] = -10000000000000;
					return col_score;
				}
				else  // Game is over, no more valid moves
				{
					col_score[0] = -1;
					col_score[1] = 0;
					return col_score;
				}
			}
			else // Depth is zero
			{
				col_score[0] = -1;
				col_score[1] = score_position(board, AI_PIECE);
				return col_score;
			}
		}
		if (maximizingPlayer)
		{
			float value = -INFINITY;
			//column = random.choice(valid_locations)
			int column = valid_locations[0];
			for (int col=0; col< valid_locations.size();col++)
			{
				int row = get_next_open_row(board, valid_locations[col]);
				int** b_copy = create_board();
				copy(board, b_copy);
				drop_piece(b_copy, row, col, AI_PIECE);
				int new_score = minimax(b_copy, depth - 1, alpha, beta, false)[1];
				if (new_score > value)
				{
					value = new_score;
					column = col;
				}
				alpha = max(alpha, int(value));
				if (alpha >= beta) 
					break;
			}
			col_score[0] = column;
			col_score[1] = int(value);
			return col_score;
		}
		else // Minimizing player
		{ 
			float value = INFINITY;
			//column = random.choice(valid_locations)
			int column = valid_locations[0];
			for (int col = 0; col < valid_locations.size(); col++)
			{
				int row = get_next_open_row(board, col);
				int** b_copy = create_board();
				copy(board, b_copy);
				drop_piece(b_copy, row, col, PLAYER_PIECE);
				int new_score = minimax(b_copy, depth - 1, alpha, beta, true)[1];
				if (new_score < int(value))
				{
					value = new_score;
					column = col;
				}
				beta = min(beta, int(value));
				if (alpha >= beta)
					break;
			}
			col_score[0] = column;
			col_score[1] = int(value);
			return col_score;
}