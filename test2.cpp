#include <iostream>
#include <vector>
#include <cstdlib>//rand()
#include <stdlib.h>//to clear the console
#include <windows.h>
using namespace std;

int ROW_COUNT = 6;
int COLUMN_COUNT = 7;

int PLAYER = 0;
int AI = 1;

int PLAYER_PIECE = 1;
int AI_PIECE = 2;

int EMPTY = 0;
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
	if (row < 6 && col < 7 && row != -1)
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
		if (board[r][col] == 0)
		{
			return r;
		}
	}
	//no place 
	return -1;
}

void print_board(int** board)
{
	
	//print the board flipped 
	for (int i = ROW_COUNT - 1; i >= 0; i--)
	{
		cout << "||";
		for (int j = 0; j < COLUMN_COUNT; j++)
		{
			cout  <<" "<< board[i][j] << " " << "||";
		}
		cout << endl;
	}
	cout << "------------------------------------------------" << endl;
}
/////////////////////////////////////////////////
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
		for (int r = 3; r < (ROW_COUNT); r++)
		{
			if (board[r][c] == piece && board[r - 1][c + 1] == piece && board[r - 2][c + 2] == piece && board[r - 3][c + 3] == piece)
				return true;
		}
	}
	return false;
}
/////////////////////////////////////////////////
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
	//////if 4 places is full then win
	if (count_Full == 4)
	{
		score += 100;
	}
	//////if 3 places is full and one empty 
	else if ((count_Full == 3) && (count_empty == 1))
	{
		score += 5;
	}

	//////if 2 places is full and 2 empty 
	else if ((count_Full == 2) && (count_empty == 2))
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
//Scoring Heuristic for our board. We evaluate the state of the whole board.
//The evaluation is based on 
//1) how many 2 in a row there are?
//2) how many 3 in a row there are? etc.
//3) how many pieces do you have in the center column?
//4) the opponent state does affect the score too. in order to prevent him from winning
//THE SCOR IS INDEPENDENT FROM WHICH PIECE WAS MOST RECENTLY DROPPPED
int score_position(int** board, int piece) {
	int score = 0;
	int center_count = 0;

	std::vector<int> center_array;

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
		std::vector<int> row_array;//stores 1 row
		for (int j = 0; j < COLUMN_COUNT; j++)
		{
			//select all col values for a certain row 
			row_array.push_back(board[r][j]);
		}
		for (int c = 0; c < COLUMN_COUNT - 3; c++)
		{ //find a window of size 1*4 we stop at COLUMN_COUNT-3 as if we continue the last 3 windows cant be 1*4
		  //we find this window to see if any player connect 4 if true he win   
			std::vector<int> window;
			for (int w = 0; w < WINDOW_LENGTH; w++)
			{
				window.push_back(row_array[c + w]);
			}
			score += evaluate_window(window, piece);
		}
	}
	//  Score Vertical	
	for (int j = 0; j < COLUMN_COUNT; j++)
	{
		std::vector<int> col_array;
		for (int r = 0; r < (ROW_COUNT); r++)
		{
			//select all col values for a certain row 
			col_array.push_back(board[r][j]);
		}
		for (int RR = 0; RR < ROW_COUNT - 3; RR++)
		{ //find a window of size 4*1 we stop at ROW_COUNT-3 as if we continue the last 3 windows cant be 4*1
		  //we find this window to see if any player connect 4 if true he win   
			std::vector<int> window;
			for (int w = 0; w < WINDOW_LENGTH; w++)
			{
				window.push_back(col_array[RR + w]);
			}
			score += evaluate_window(window, piece);
		}
	}
	// Score positive sloped diagonal
	for (int c = 0; c < COLUMN_COUNT - 3; c++)
	{
		for (int RR = 0; RR < ROW_COUNT - 3; RR++)
		{
			std::vector<int> window;
			for (int w = 0; w < WINDOW_LENGTH; w++)
			{
				window.push_back(board[RR + w][c + w]);
			}
			score += evaluate_window(window, piece);
		}
	}
	// Score negative sloped diagonal
	for (int RR = 0; RR < ROW_COUNT - 3; RR++)
	{
		for (int c = 0; c < COLUMN_COUNT - 3; c++)
		{
			std::vector<int> window;
			for (int w = 1; w < WINDOW_LENGTH; w++)
			{
				window.push_back(board[RR + 3 - w][c + w]);
			}
			score += evaluate_window(window, piece);
		}
	}
	return score;
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
	//cout << "This is just a copy of the board" << endl;
	//print_board(B);
}

vector<int> get_valid_locations(int** board)
{
	vector <int> valid_locations;
	for (int col = 0; col < (COLUMN_COUNT); col++)
	{
		if (is_valid_location(board, col) == true)
			valid_locations.push_back(col);
	}
	return valid_locations;
}
////////////////////////////////////
bool is_terminal_node(int** board)
{
	if (winning_move(board, PLAYER_PIECE) == 1 || winning_move(board, AI_PIECE) == 1 || get_valid_locations(board).size() == 0)
		return true;
	else
		return false;
}

int max2(int a, int b) { if (a > b) return a; else return b; }
int min2(int a, int b) { if (a < b) return a; else return b; }

vector<int> minimax(int** board, int depth, int alpha, int beta, int maximizingPlayer)
{
	//int col_score[2];
	vector <int> valid_locations = get_valid_locations(board);
	vector <int>col_score;
	vector <int>get_element;
	bool is_terminal = is_terminal_node(board);
	if (depth == 0 || is_terminal == 1)
	{
		if (is_terminal == true)
		{
			if (winning_move(board, AI_PIECE))
			{
				col_score.push_back(-1);
				col_score.push_back(10000000);
				return col_score;
			}
			else if (winning_move(board, PLAYER_PIECE))
			{
				col_score.push_back(-1);
				col_score.push_back(-10000000);
				return col_score;
			}
			else  // Game is over, no more valid moves
			{
				col_score.push_back(-1);
				col_score.push_back(0);
				return col_score;
			}
		}
		else // Depth is zero
		{
			col_score.push_back(-1);
			col_score.push_back(score_position(board, AI_PIECE));
			return col_score;
		}
	}
	int index = 0; int column = 0;
	if (maximizingPlayer)
	{
		int value = -1000000;
		//column = random.choice(valid_locations)
		//int column = valid_locations[3];
		int s = valid_locations.size();
		index = rand() % s; // pick a random index
		column = valid_locations[index];
		int** b_copy = create_board();
		for (int col = 0; col < s; col++)
		{
			int row = get_next_open_row(board, valid_locations[col]);
			if (row == -1)
			{
				continue;
			}
			copy(board, b_copy);
			drop_piece(b_copy, row, valid_locations[col], AI_PIECE);
			get_element = minimax(b_copy, depth - 1, alpha, beta, false);
			int new_score = get_element[1];
			if (new_score > value)
			{
				value = new_score;
				column = valid_locations[col];
			}
			alpha = max2(alpha, int(value));
			if (alpha >= beta)
				break;
		}
		//Free each sub-array
		for (int i = 0; i < ROW_COUNT; ++i) {
			delete[] b_copy[i];
		}
		//Free the array of pointers
		delete[] b_copy;
		col_score.push_back(column);
		col_score.push_back(int(value));
		return col_score;
	}
	else // Minimizing player
	{
		int value = 1000000;
		//column = random.choice(valid_locations)
		//int column = valid_locations[3];
		int s = valid_locations.size();
		index = rand() % s; // pick a random index
		column = valid_locations[index];
		int** b_copy = create_board();
		for (int col = 0; col < s; col++)
		{
			int row = get_next_open_row(board, valid_locations[col]);
			if (row == -1) { continue; }
			copy(board, b_copy);
			drop_piece(b_copy, row, valid_locations[col], PLAYER_PIECE);
			get_element = minimax(b_copy, depth - 1, alpha, beta, true);
			int new_score = get_element[1];

			if (new_score < int(value))
			{
				value = new_score;
				column = valid_locations[col];
			}
			beta = min2(beta, int(value));
			if (alpha >= beta)
				break;
		}
		//Free each sub-array
		for (int i = 0; i < ROW_COUNT; ++i) {
			delete[] b_copy[i];
		}
		//Free the array of pointers
		delete[] b_copy;
		col_score.push_back(column);
		col_score.push_back(int(value));
		return col_score;
	}
}

int main()
{
	int** board = create_board();
	print_board(board);
	bool game_over = false;
	int turn = 1;//make it random or ask the user who goes first
	int co = 0;
	while (game_over == false)
	{
		if (turn == PLAYER)
		{
			cout << "Please enter your column selection (1:7): ";

			cin >> co;
			system("CLS");
			if (is_valid_location(board, co-1) == 1)
			{
				int row = get_next_open_row(board, co-1);
				if (row == -1) { cout << "No empty slot in this column, Please choose another one" << endl; continue; }
				drop_piece(board, row, co-1, PLAYER_PIECE);
				if (winning_move(board, PLAYER_PIECE))
				{
					cout << "YOU WIN" << endl;
					game_over = true;
				}
				turn += 1;
				turn = turn % 2;

				print_board(board);
				//Sleep(2000);
			}
		}
		if ((turn == AI) && (game_over == false))
		{
			
			vector<int> col_score;
			col_score = minimax(board, 3, -1000000, 1000000, true);
			if (col_score[0] == -1) { continue; }
			//col_score[1] = minimax(board, 3, -int(INFINITY), int(INFINITY), true)[1];
			//col, minimax_score = minimax(board, 3, -(std::numeric_limits<double>::infinity()), (std::numeric_limits<double>::infinity()), true);
			
			/*cout << "please enter the column you selected to play (0:6): ";
			cin >> co;*/
			/*
			co = pick_best_move(board, AI_PIECE);*/
			if (is_valid_location(board, col_score[0]) == 1)
			{

				int row = get_next_open_row(board, col_score[0]);
				if (row == -1) { cout << "AI choose a column with no empty slot!!" << endl; continue; }
				drop_piece(board, row, col_score[0], AI_PIECE);
				system("CLS");
				if (winning_move(board, AI_PIECE))
				{
					cout << "AI WINS" << endl;
					game_over = true;
				}
				
				print_board(board);


				turn += 1;
				turn = turn % 2;
			}

		}

	}
	//Free each sub-array
	for (int i = 0; i < ROW_COUNT; ++i) {
		delete[] board[i];
	}
	//Free the array of pointers
	delete[] board;
	system("pause");
	return 0;
}